#include "Encoder.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace::std;

#include "CImg.h"
using namespace cimg_library;

void main()
{
	
	Encoder encoder;
	bool cont = true;
	while (cont)
	{
		int opt;
		cout << endl << endl << "1 : Set seed \n2 : Encode file \n3 : Decode file\n4 : Quit" << endl << endl << "Select an option : ";
		cin >> opt;
		switch (opt)
		{
			case(1) : encoder.setSeed();
				break;
			case(2) : encoder.encode();
				break;
			case(3) : encoder.decode();
				break;
			case(4) : cont = false;
				break;
			default: break;
		}
	}
}