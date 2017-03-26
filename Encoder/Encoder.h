#ifndef ENCODER_H
#define ENCODER_H

#include <fstream>
#include "CImg.h"
using namespace::std;
using namespace::cimg_library;


class Encoder
{
public:
	Encoder();
	~Encoder();
	
	void encode();
	void decode();
	void getFile();
	void setSeed();

	CImg<unsigned char>* createMask(long seedV, int side);
	CImg<unsigned char>* convertText(int side);
	CImg<unsigned char>* merge(CImg<unsigned char>* data, CImg<unsigned char>* mask, int dataSide, int maskSide);
	void insertBorder(CImg<unsigned char>* preMod, int borderWidth, int maskSide);
	int readSeed(CImg<unsigned char>* encrypted);
	CImg<unsigned char>* stripMask(CImg<unsigned char>* encrypted, CImg<unsigned char>* mask, int side);


private:
	long seed;
	ifstream currentFile;
	string fileName;
	int fileSize;
};

#endif
