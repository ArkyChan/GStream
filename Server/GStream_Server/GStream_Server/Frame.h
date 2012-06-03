#ifndef FRAME_H
#define FRAME_H

class Frame{
public:
	Frame(){
		this->data = NULL;
		this->w = 0;
		this->h = 0;
		this->bpp = 0;
	}
	~Frame(){
		delete data;
	}
	Frame(unsigned char* data,int width,int height,short bytesPerPixel){
		this->data = data;
		this->w = width;
		this->h = height;
		this->bpp = bytesPerPixel;
	}

	int getPixel(int x,int y){
		return this->data[x*y*this->bpp];
	}

	int getWidth(){
		return this->w;
	}

	int getHeight(){
		return this->h;
	}

	void setData(unsigned char* nData){
		delete this->data;
		this->data = nData;
	}

	unsigned char* getData(){
		return this->data;
	}
private:
	unsigned char* data;
	short bpp;
	int w,h;
};
#endif