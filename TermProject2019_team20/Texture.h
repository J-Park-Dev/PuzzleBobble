#pragma once
#include <iostream>
#include <GL/glut.h>
#include <FreeImage.h>
#include "Control.h"

using namespace std;

class Texture {
private:
	FIBITMAP* bitmap;
	FIBITMAP* bitmap32;
	GLuint textureID;
	GLubyte* textureData;
	int imageWidth, imageHeight;
public:
	Texture(char const* filename);
	~Texture();
	FIBITMAP* createBitMap(char const* filename);
	void drawBackWithTexturing();
};