#include "man_encode.h"

#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>

#define RND 10
#define KEYS 10

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
		man_encode::~man_encode(){
			free(this->n_data);
		}
		unsigned char* man_encode::encodeFrame(unsigned char* data){
			if(this->lFrame != NULL){
				for(unsigned int x = 0; x < this->d_len ;x++){
					data[x] = (data[x]/RND)*RND;

					if(data[x] == this->lFrame[x]){ //&& this->frames != 0
						this->n_data[x] = '\xFF';
					} else {
						this->n_data[x] = data[x];
					}
				}
				free(this->lFrame);
			}else {
				this->lFrame = data;
				return data;
			}
			//this->frames = (this->frames >= KEYS ? 0 : ++this->frames);
			this->lFrame = data;
			return this->n_data;
		}

		void man_encode::dumpFrame(unsigned char* data,bool leaveOpen){
			FILE *pFile;
			fopen_s(&pFile,"test.data","ab");
			char* d = (char*)data;
			for(unsigned int i=0;i<this->w*this->h;i++){
				char dat[] = {d[2],d[1],d[0]};
				fwrite(&dat,3,1,pFile);
				d += 3;
			}
			fclose(pFile);
		}
	}
}