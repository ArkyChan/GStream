#include "man_encode.h"

#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>

#define RND 20.0
#define KEYS 5

namespace Gstream {
	namespace encode {
		man_encode::man_encode(int w,int h,int bits){
			this->w = w;
			this->h = h;
			this->bits = bits;
			this->bpp = bits/8;
			this->d_len = w*h*this->bpp;
			this->n_data = (unsigned char*)malloc(this->d_len);
			this->lFrame = 0;
			this->frames = 0;
		}
		unsigned char* man_encode::encodeFrame(unsigned char* data){
			for(int x = 0; x < this->d_len ;x++){
				data[x] = (char)(int)((float)data[x]/RND)*RND;

				if(this->lFrame != NULL || (this->frames%5)!=0){
					if(data[x] == this->lFrame[x]){
						this->n_data[x] = '\x00';
					} else {
						this->n_data[x] = data[x];
					}
				}else {
					this->n_data[x] = data[x];
				}
			}
			this->frames++;
			this->lFrame = data;
			return this->n_data;
		}
		char man_encode::floorf_ASM(float a)	{
			int i;
			__asm {
				fld a
				fistp i
			}
			return (char)i;
		}
		void man_encode::dumpFrame(unsigned char* data){
			FILE *pFile;
			fopen_s(&pFile,"test.data","ab");
			char* d = (char*)data;
			for(int i=0;i<this->w*this->h;i++){
				char dat[] = {d[2],d[1],d[0]};
				fwrite(&dat,3,1,pFile);
				d += 3;
			}
			fclose(pFile);
		}
	}
}