#ifndef MAN_ENCODE_H
#define MAN_ENCODE_H

namespace Gstream {
	namespace encode {
		class man_encode {
		public:
			man_encode(int w,int h,int bits);
			~man_encode();
			unsigned char* encodeFrame(unsigned char* data);
			unsigned char* encodeFrameSmall(unsigned char* data);
			void dumpFrame(unsigned char* data,bool leaveOpen=false);
		private:
			unsigned char* n_data;
			unsigned char* lFrame;
			unsigned int w,h,d_len,bits,bpp,frames;
		};

		/*class man_encode_worker : public boost::thread{
		public:
			man_encode_worker();
			void work(unsigned char* in,unsigned char* out);
		};*/
	}
}
#endif