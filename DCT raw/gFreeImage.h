#pragma once //prevent repeated references

#include "FreeImage.h"
#include "stdlib.h"
#include <stdio.h>


class gFreeImage
{
public:
	gFreeImage(void);
	~gFreeImage(void);
public:
    FIBITMAP *bitmap;
	unsigned char *imageData;
	unsigned char *imageData4;
	FREE_IMAGE_FORMAT imagetype;
	int width;
	int height;

	int LoadImage(char *filename);
	int LoadImageGrey(char *filename);
	int SaveImage(char *filename);
	bool gFreeImage::SaveImage(char *fileName, unsigned char *buffer, int width, int height);
	int SaveImageGrey(char *filename);
    unsigned char * getImageData(int& w, int& h);
	unsigned char * getImageDataGrey(int& w, int& h);
};
