#ifndef GLCAP_H
#define GLCAP_H

#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <stdio.h>

class glCap
{
public:
	glCap();
	~glCap();

	bool LoadTGAFile(char *filename);
	void SaveTGAScreenShot(char *filename,int width,int height);
	void FreeImage();
	unsigned char * capScreen(int,int);
	void writeRaw(char* file, int width,int height,short,unsigned char* data);

	unsigned int ID;
	int imageWidth;
	int imageHeight;
	unsigned char *image;
protected:
	void GenerateTexture();
	bool LoadTGA(char *filename);
	bool WriteTGA(char *file, short int width, short int height, unsigned char* image=NULL);
	bool textureExist;
	int type;
};

#endif