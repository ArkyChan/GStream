// FFmpeg is C so we do this
extern "C"
{
#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif 

#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
#include <libswscale/swscale.h>
#include <libavformat\avformat.h>
}

// Note won't work for gcc if we ever swap over, you can make the make file then :p
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "swscale.lib")
#define INBUF_SIZE 4096

char filename[128] = "test.h264";
FILE *vidFile;

AVCodec *codec;
AVCodecContext *c= NULL;
int i, out_size, x, y, outbuf_size;
int had_output=0;
AVFrame *picture;
uint8_t *outbuf;
AVFrame* outpic;
int vWidth, vHeight;

// Just gonna try and encode it to a file to test
void vid_prep(int width, int height)
{
	vWidth = width;
	vHeight = height;

	// load the codecs, derp I keep forgeting this
	avcodec_register_all();

	/* find the mpeg1 video encoder */
	codec = avcodec_find_encoder(CODEC_ID_H264);
	if (!codec) {
		fprintf(stderr, "codec not found\n");
		exit(1);
	}

	c = avcodec_alloc_context3(codec);
	picture = avcodec_alloc_frame();

	/* put sample parameters */
	c->bit_rate = 200000;
	/* resolution must be a multiple of two */
	c->width = width;
	c->height = height;

	/* frames per second */
	AVRational avr;
	avr.den = 1;
	avr.num = 20;
	c->codec_id = CODEC_ID_H264;
	c->codec_type = AVMEDIA_TYPE_VIDEO; 
	c->time_base= avr;
	c->gop_size = 15; /* emit one intra frame every ten frames */
	c->max_b_frames=5;
	c->pix_fmt = PIX_FMT_YUV420P;

	//if(codec_id == CODEC_ID_H264)
	av_opt_set(c->priv_data, "preset", "slow", 0); // Not a great idea for us

	/* open it */
	if (avcodec_open2(c, codec, NULL) < 0) 
	{
		fprintf(stderr, "could not open codec\n");
		exit(1);
	}

	vidFile = fopen(filename, "wb");
	if (!vidFile) 
	{
		fprintf(stderr, "could not open %s\n", filename);
		exit(1);
	}

	/* alloc image and output buffer */
	outbuf_size = 100000 + 12*c->width*c->height;
	outbuf = (uint8_t*)av_malloc(outbuf_size);

	av_image_alloc(picture->data, picture->linesize, c->width, c->height, c->pix_fmt, 1);
}

// Write a frame
void vid_writeFrame(uint8_t* imgData,size_t size){
	outpic = avcodec_alloc_frame();
	int nbytes = avpicture_get_size(PIX_FMT_YUV420P, c->width, c->height);

	//create buffer for the output image
	uint8_t* outbuffer = (uint8_t*)av_malloc(nbytes);

	avpicture_fill((AVPicture*)picture, imgData, PIX_FMT_BGR24, c->width, c->height);
	avpicture_fill((AVPicture*)outpic, outbuffer, PIX_FMT_YUV420P, c->width, c->height);
	SwsContext* img_convert_ctx = sws_getContext(vWidth, vHeight, PIX_FMT_BGR24, vWidth, vHeight, PIX_FMT_YUV420P, SWS_FAST_BILINEAR, NULL, NULL, NULL);

	sws_scale(img_convert_ctx, picture->data, picture->linesize, 0, c->height, outpic->data, outpic->linesize);
	picture->width = 1280;
	outpic->width = 1280;
	outpic->height = 720;
	picture->height = 720;
	out_size = avcodec_encode_video(c, outbuf, outbuf_size, outpic);
	had_output |= out_size;
	if(out_size>0){
		printf("encoding frame %3d (size=%5d)\n", i, out_size);
		fwrite(outbuf, 1, out_size, vidFile);
	}

}

// Dummy vide data
void vid_writeDummyFrames()
{
	for(i=0;i<25;i++) 
	{
		fflush(stdout);

		for(y=0;y<c->height;y++) 
		{
			for(x=0;x<c->width;x++) 
			{
				picture->data[0][y * picture->linesize[0] + x] = x + y + i * 3;
			}
		}


		for(y=0;y<c->height/2;y++) 
		{
			for(x=0;x<c->width/2;x++) 
			{
				picture->data[1][y * picture->linesize[1] + x] = 128 + y + i * 2;
				picture->data[2][y * picture->linesize[2] + x] = 64 + x + i * 5;
			}
		}


		out_size = avcodec_encode_video(c, outbuf, outbuf_size, picture);
		had_output |= out_size;
		printf("encoding frame %3d (size=%5d)\n", i, out_size);
		fwrite(outbuf, 1, out_size, vidFile);
	}
}

// Wrap up the file
void vid_end()
{
	for(; out_size || !had_output; i++) 
	{
		fflush(stdout);

		out_size = avcodec_encode_video(c, outbuf, outbuf_size, NULL);
		had_output |= out_size;
		printf("write frame %3d (size=%5d)\n", i, out_size);
		fwrite(outbuf, 1, out_size, vidFile);
	}
	
	/* add sequence end code to have a real mpeg file */
	outbuf[0] = 0x00;
	outbuf[1] = 0x00;
	outbuf[2] = 0x01;
	outbuf[3] = 0xb7;
	fwrite(outbuf, 1, 4, vidFile);
	fclose(vidFile);
	free(outbuf);

	avcodec_close(c);
	av_free(c);
	av_free(picture->data[0]);
	av_free(picture);
	printf("\n");
}
