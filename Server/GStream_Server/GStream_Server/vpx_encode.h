#ifndef VPX_ENCODE_H
#define VPX_ENCODE_H

#include "Logger.h"

#define VPX_CODEC_DISABLE_COMPAT 1
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"
#define interface (vpx_codec_vp8_cx())

extern "C" {
	#include <libswscale\swscale.h>
}

namespace Gstream {
	namespace encode {
		class vpx_encode {
		public:
			vpx_encode(uint32_t width,uint32_t height,uint8_t bits);
			unsigned char* encodeFrameRGB(unsigned char* inImg,uint32_t& lenght);
		private:
			unsigned char* imgBuffer;
			uint32_t w,h,frame_cnt;
			uint8_t bits;

			vpx_codec_ctx_t codec;
			vpx_codec_enc_cfg_t cfg;
			vpx_codec_err_t res;
			vpx_image_t out_img,in_img;
			vpx_codec_iter_t iter;
			const vpx_codec_cx_pkt_t* pkt;

			static void die_codec(vpx_codec_ctx_t *ctx, const char *s) {
				const char *detail = vpx_codec_error_detail(ctx);
				printf("%s: %s\n", s, vpx_codec_error(ctx));
				if(detail)
					printf("    %s\n",detail);
				exit(EXIT_FAILURE);
			}
		};
	}
}
#endif