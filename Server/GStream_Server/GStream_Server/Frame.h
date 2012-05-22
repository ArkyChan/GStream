#ifndef FRAME_H
#define FRAME_H

struct Pixel{
	int x,y;
	DWORD col;

	bool operator !=(Pixel p){
		if(p.x != x && p.y != y && p.col != col)
			return true;
		return false;
	}
};
class Frame{
public:
	Frame(){
		data = new Pixel*[720];
		for(int i=0;i<720;i++){
			data[i] = new Pixel[1280];
		}
	}
	~Frame(){
		for(int i=0;i<720;i++){
			//delete[] &data[i];
		}
		
	}
	Pixel& operator()(int x,int y){
		return data[x][y];
	}
private:
	Pixel **data;
};
#endif