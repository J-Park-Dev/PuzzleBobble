#include "Texture.h"

Texture::Texture(char const* filename) {
	bitmap32 = createBitMap(filename);
	imageWidth = FreeImage_GetWidth(bitmap32);
	imageHeight = FreeImage_GetHeight(bitmap32);
	textureData = FreeImage_GetBits(bitmap32);
}

Texture::~Texture() {
	//cout << "Texture destructor called~" << endl;
	FreeImage_Unload(bitmap);
	if(bitmap != bitmap32)
		FreeImage_Unload(bitmap32);
	//glDeleteTextures(1, &textureID);
}

FIBITMAP* Texture::createBitMap(char const* filename) {
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);

	if (format == -1) {
		cout << "Could not find image: " << filename << " - Aborting." << endl;
		exit(-1);
	}

	if (format == FIF_UNKNOWN) {
		cout << "Couldn't determine file format - attempting to get from file extension..." << endl;
		format = FreeImage_GetFIFFromFilename(filename);

		if (!FreeImage_FIFSupportsReading(format)) {
			cout << "Detected image format cannot be read!" << endl;
			exit(-1);
		}
	}

	bitmap = FreeImage_Load(format, filename);

	int bitsPerPixel = FreeImage_GetBPP(bitmap);

	if (bitsPerPixel == 32) {
		// cout << "Source image has " << bitsPerPixel << " bits per pixel. Skipping conversion." << endl;
		bitmap32 = bitmap;
	}
	else {
		// cout << "Source image has " << bitsPerPixel << " bits per pixel. Converting to 32-bit colour." << endl;
		bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
	}

	return bitmap32;
}

void Texture::drawBackWithTexturing() {
	glPushMatrix();
	glTranslatef(0.0, 0.0, -4.5);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


	// 11.20 by 지수
	// 이 부분은 한번만 실행되어야 메모리 누수가 생기지 않는다.
	// 11.27 by 지수
	// 이 부분을 주석처리 해도 결과에 변화가 없었다. textureID 의 값은 0이었다.
	// glBindTexture() 주석처리 했는데 문제가 없었다. 과연 textureID 는 필요한 것일까?
	/*static int count = 0;
	if (count == 0) {
		glGenTextures(1, &textureID);
		count++;
	}*/

	//glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureData);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	//glBindTexture(GL_TEXTURE_2D, textureID);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);		glVertex3f(-boundaryX, -boundaryY, 0.0);
	glTexCoord2f(0, 1);		glVertex3f(-boundaryX, boundaryY, 0.0);
	glTexCoord2f(1, 1);		glVertex3f(boundaryX, boundaryY, 0.0);
	glTexCoord2f(1, 0);		glVertex3f(boundaryX, -boundaryY, 0.0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glPopMatrix();
}