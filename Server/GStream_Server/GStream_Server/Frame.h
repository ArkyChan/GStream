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
		delete this->data;
	}
	Frame(unsigned char* data,int width,int height,short bytesPerPixel){
		this->data = data;
		this->w = width;
		this->h = height;
		this->bpp = bytesPerPixel;
	}

	DWORD getPixel(int x,int y){
		void* d = this->data;
		return (DWORD)(&d + (this->w*y*this->bpp) + (x*this->bpp));
	}

	int getWidth(){
		return this->w;
	}

	int getHeight(){
		return this->h;
	}

	void setData(unsigned char* nData,bool del=false){
		if(del){
			free(this->data);
		}
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