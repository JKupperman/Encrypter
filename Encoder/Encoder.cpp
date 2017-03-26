#include "Encoder.h"
#include "CImg.h"
#include <random>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
using namespace::std;
using namespace::cimg_library;

Encoder::Encoder()
{
	seed = 82377491;
	fileSize = 0;
}

Encoder::~Encoder()
{
	if (currentFile.is_open()) currentFile.close();
}

void Encoder::encode()
{
	getFile();
	if (!currentFile.is_open()) return;

	int dataSide = ceil(sqrt(fileSize/3));

	int borderWidth = 0;
	for (int i = seed; i > 0; i = i / 10) borderWidth++;
	int maskSide = dataSide + borderWidth;
	
CImg<unsigned char>* mask = createMask(seed, maskSide);
	CImg<unsigned char>* data = convertText(dataSide);

	CImg<unsigned char>* merged = merge(data, mask, dataSide, maskSide);

	delete data;
	delete mask;

	insertBorder(merged, borderWidth, maskSide);

	cout << "Enter destination: ";
	string fileName;
	cin >> fileName;
	fileName = "FileStore/" + fileName;
	merged->save(fileName.c_str());

	delete merged;
}

void Encoder::decode()
{

	cout << "Enter file name: ";
	string fileName;
	cin >> fileName;
	fileName = "FileStore/" + fileName;

	CImg<unsigned char>* encoded = new CImg<unsigned char>(fileName.c_str());

	int foundSeed = readSeed(encoded);
	CImg<unsigned char>* mask = createMask(foundSeed, encoded->width());
	CImg<unsigned char>* decoded = stripMask(encoded, mask, encoded->width());

	delete encoded;
	delete mask;


	unsigned char* pix;
	string outString = "";
	cimg_for(*decoded, pix, unsigned char)
	{
		if (*pix == 255) break;
		outString = outString + (char)*pix;
	}
	pix = nullptr;
	delete decoded;

	cout << "Enter file destination: ";
	string outputFile;
	cin >> outputFile;
	outputFile = "FileStore/" + outputFile;

	ofstream output(outputFile);
	if (output.is_open())
	{
		output << outString;
		output.close();
	}
	else cout << "File not saveable.";
}

void Encoder::getFile()
{
	if (currentFile.is_open())
	{
		currentFile.close();
		fileSize = 0;
	}

	cout << "Enter file name: ";
	cin >> fileName;
	fileName = "FileStore/" + fileName;

	currentFile.open(fileName);
	if (currentFile.is_open())
	{
		currentFile.seekg(0, std::ios::end);
		fileSize = currentFile.tellg();
		currentFile.seekg(0, std::ios::beg);
		cout << endl << "File opened successfully." << endl;
	}
	else
	{
		cout << endl << "File not found." << endl;
	}
}

void Encoder::setSeed()
{
	cout << "Enter seed: ";
	cin >> seed;
	cin.clear();
}

CImg<unsigned char>* Encoder::merge(CImg<unsigned char>* data, CImg<unsigned char>* mask, int dataSide, int maskSide)
{
	CImg<unsigned char>* pict = new CImg<unsigned char>(maskSide, maskSide, 1, 3, 0);
	unsigned char * pixel;
	int posit = 0;

	cimg_for(*pict, pixel, unsigned char)
	{
		*pixel = *(mask->data(posit));
		if (posit < fileSize) *pixel = (*pixel + *(data->data(posit))) % 256;
		posit++;
	}
	pixel = nullptr;

	return pict;
}

void Encoder::insertBorder(CImg<unsigned char>* preMod, int borderWidth, int maskSide)
{
	int seedVar = seed;
	int posit = 1;
	while (seedVar != 0)
	{
		*(preMod->data(maskSide*maskSide*3 - posit)) = 100 + (seedVar % 100);
		seedVar = seedVar / 100;
		posit ++;
	}
	*(preMod->data(maskSide*maskSide * 3 - posit)) = 205;
}

int Encoder::readSeed(CImg<unsigned char>* encrypted)
{
	int foundSeed = 0;
	int posit = 1;
	int fileLen = encrypted->width() * encrypted->width() * 3;
	while (*(encrypted->data(fileLen - posit)) != 205)
	{
		int multiplier = 1;
		for (int i = 1; i < posit; i++)
		{
			multiplier *= 100;
		}
		foundSeed += (*(encrypted->data(fileLen - posit)) - 100)  * multiplier;
		posit++;
	}

	return foundSeed;
}

CImg<unsigned char>* Encoder::stripMask(CImg<unsigned char>* encrypted, CImg<unsigned char>* mask, int side)
{
	CImg<unsigned char>* pict = new CImg<unsigned char>(side, side, 1, 3, 0);
	unsigned char* pixel;
	int posit = 0;

	cimg_for(*pict, pixel, unsigned char)
	{
		while (*(encrypted->data(posit)) - *(mask->data(posit)) == 0)
		{
			posit++;
		}
		*pixel = *(encrypted->data(posit)) - *(mask->data(posit));
		if (*pixel < 0) *pixel += 256;
		posit++;
	}
	pixel = nullptr;

	return pict;
}

CImg<unsigned char>* Encoder::createMask(long seedV, int side)
{
	srand(seedV);
	CImg<unsigned char>* pict = new CImg<unsigned char>(side, side, 1, 3, 0);
	
	unsigned char * pixel = new unsigned char;
	cimg_for(*pict, pixel, unsigned char)
	{
		*pixel = rand() % 256;
	}
	pixel = nullptr;

	return pict;
}

CImg<unsigned char>* Encoder::convertText(int side)
{
	CImg<unsigned char>* pict = new CImg<unsigned char>(side, side, 1, 3, 0);

	unsigned char * pixel;
	cimg_for(*pict, pixel, unsigned char)
	{
		if (!currentFile.eof())
		{
			*pixel = (currentFile.get() % 256);
			if (*pixel > 255) *pixel = 255;
		}
	}
	pixel = nullptr;

	return pict;
}
