#include "vpx_encode.h"

namespace Gstream {
	namespace encode {
		vpx_encode::vpx_encode(uint32_t width,uint32_t height,uint8_t bits){
			this->w = width;
			this->h = height;
			this->bits = bits;
			this->imgBuffer = (unsigned char*)malloc(this->w*this->h*this->bits/8);

			//start up VPX
			vpx_img_alloc(&this->out_img, VPX_IMG_FMT_I420, this->w, this->h, 1);
			this->res = vpx_codec_enc_config_default(interface, &this->cfg, 0);
			this->cfg.rc_target_bitrate = this->w * this->h * this->cfg.rc_target_bitrate / this->cfg.g_w / this->cfg.g_h;
			this->cfg.g_w = this->w;
			this->cfg.g_h = this->h;
			this->iter = NULL;
			this->frame_cnt = 0;

			if(vpx_codec_enc_init(&this->codec, interface, &this->cfg, 0)){
				_LOG("VPX failed to initialize",_ERROR);
				die_codec(&this->codec, "Failed to initialize encoder");
			}
		}

		unsigned char* vpx_encode::encodeFrameRGB(unsigned char* in,uint32_t& size){
			struct SwsContext* convert_context = sws_getContext(this->w,this->h,PIX_FMT_BGR24,this->w,this->h,PIX_FMT_YUV420P,SWS_FAST_BILINEAR,NULL,NULL,NULL);

			vpx_img_wrap(&this->in_img, VPX_IMG_FMT_RGB24, this->w, this->h, 0,in);
			int output_slice_h = sws_scale(convert_context,this->in_img.planes,this->in_img.stride,0,this->h,this->out_img.planes,this->out_img.stride);

			if(vpx_codec_encode(&this->codec, &this->out_img, this->frame_cnt, 1,0, VPX_DL_REALTIME)) {
				die_codec(&codec, "Failed to encode frame");
			}

			this->pkt = vpx_codec_get_cx_data(&this->codec, &this->iter);

			if(this->pkt->kind==VPX_CODEC_CX_FRAME_PKT){
				size = this->pkt->data.frame.sz;
				if(size > this->w*this->h*this->bits/8){
					_LOG("BUFFER LARGER THAN RAW",_WARN);
				}
				this->imgBuffer = (unsigned char*)this->pkt->data.frame.buf;
				size = this->pkt->data.frame.sz;
				this->frame_cnt++;
			}
			return this->imgBuffer;
		}
	}
}