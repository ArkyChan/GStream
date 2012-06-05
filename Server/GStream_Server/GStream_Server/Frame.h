#ifndef FRAME_H
#define FRAME_H

class Frame{
public:
	//Default constucter
	Frame() {
		this->data = NULL;
		this->w = 0;
		this->h = 0;
		this->bpp = 0;
	}
	~Frame() {
		delete this->data;
	}
	Frame(unsigned char* data,int width,int height,short bytesPerPixel) {
		this->data = data;
		this->w = width;
		this->h = height;
		this->bpp = bytesPerPixel;
	}

	DWORD getPixel(int x,int y) {
		void* d = this->data;
		return (DWORD)(&d + (this->w*y*this->bpp) + (x*this->bpp));
	}

	int getWidth() {
		return this->w;
	}

	int getHeight() {
		return this->h;
	}

	void setData(unsigned char*  nData,bool del=false) {
		if(del){
			free(this->data);
		}
		this->data = nData;
	}

	unsigned char* getData() {
		return this->data;
	}

	// Gets the data as a single dim int array (rgb is stored in the int)
	int* getRGBInt()
	{
		char* d = (char*)data;
		int* buff = new int[w*h];

		for(int i=0; i<w*h; i++)
		{
			buff[i] = d[0] << 16 | d[1] << 8 | d[2];
			d += 3;
		}

		return buff;
	}

	void writeRaw(char* file) {
		FILE *pFile;
		fopen_s(&pFile,file,"wb");
		char* d = (char*)data;
		for(int i=0;i<w*h;i++){
			char dat[] = {d[2],d[1],d[0]};
			fwrite(&dat,3,1,pFile);
			d += 3;
		}
		fclose(pFile);
	}

	/*unsigned char* getYUV() {
		size_t image_size = this->w * this->h;
		size_t upos = image_size;
		size_t vpos = upos + upos / 4;
		size_t i = 0;

		for( size_t line = 0; line < this->h; ++line )
		{
			if( !(line % 2) )
			{
				for( size_t x = 0; x < this->w; x += 2 )
				{
					uint8_t r = this->data[3 * i];
					uint8_t g = this->data[3 * i + 1];
					uint8_t b = this->data[3 * i + 2];

					destination[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;

					destination[upos++] = ((-38*r + -74*g + 112*b) >> 8) + 128;
					destination[vpos++] = ((112*r + -94*g + -18*b) >> 8) + 128;

					r = this->data[3 * i];
					g = this->data[3 * i + 1];
					b = this->data[3 * i + 2];

					destination[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;
				}
			}
			else
			{
				for( size_t x = 0; x < this->w; x += 1 )
				{
					uint8_t r = this->data[3 * i];
					uint8_t g = this->data[3 * i + 1];
					uint8_t b = this->data[3 * i + 2];

					destination[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;
				}
			}
		}
	}*/
private:
	unsigned char* data;
	short bpp;
	int w,h;
};
#endif