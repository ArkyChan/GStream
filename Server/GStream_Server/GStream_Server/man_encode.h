#ifndef MAN_ENCODE_H
#define MAN_ENCODE_H

namespace Gstream {
	namespace encode {
		class man_encode {
		public:
			man_encode(int w,int h,int bits);
			unsigned char* encodeFrame(unsigned char* data);
			void dumpFrame(unsigned char* data);
		private:
			unsigned char* n_data;
			unsigned char* lFrame;
			unsigned int w,h,d_len,bits,bpp,frames;
			char man_encode::floorf_ASM(const float a);
		};
	}
}
#endif