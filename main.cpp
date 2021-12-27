// main.cpp

// Demonstrates function calls between a C++ program
// and an external assembly language module.
// Last update: 6/24/2005

#include <iostream>
#include <iomanip>
using namespace std;

extern "C" {
	// external ASM procedures:
	void DisplayTable();
	void SetTextOutColor(unsigned color);
	void Brightness(unsigned char* im1, int w, int h, int val);
	void Blur(unsigned char* im1, int w, int h);
	void Foo();
	// local C++ functions:
	int askForInteger();
	void showInt(int value, int width);

}

// program entry point
int main()
{

	int value = 0;
	int choice = 0;
	int i, j;
	char information[54];
	int width, height;
	unsigned char* image1, * image2;

	unsigned char* imageR, * imageG, * imageB;

	char outfilename[512];
	FILE* input, * output;



	input = fopen("lena.bmp", "rb");
	fread(information, 1, 54, input);
	width = *(int*)(information + 18);
	height = *(int*)(information + 22);
	//image1 = (unsigned char*)calloc(width * height, 1);
	imageR = (unsigned char*)calloc(width * height, 1);
	imageG = (unsigned char*)calloc(width * height, 1);
	imageB = (unsigned char*)calloc(width * height, 1);

	unsigned char blue, green, red;
	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++) {
			blue = getc(input);
			green = getc(input);
			red = getc(input);

			//read RGB as three different array
			imageR[i * width + j] = red;
			imageG[i * width + j] = green;
			imageB[i * width + j] = blue;

			// convert to grayscale
			//image1[i * width + j] = (blue + green + red) / 3;
		}
	fclose(input);

	Blur(imageR, 512, 512);
	Blur(imageG, 512, 512);
	Blur(imageB, 512, 512);
	Foo();

	//Save output image

	output = fopen("out.bmp", "wb");
	*(int*)(information + 18) = 512;
	*(int*)(information + 22) = 512;

	fwrite(information, 1, 54, output);
	for (int i = 0; i < 54; i++)
	{
		cout << information[i];
	}
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			// notice that we need to write as B,G,R order
			putc(imageB[i * width + j], output);
			putc(imageG[i * width + j], output);
			putc(imageR[i * width + j], output);
		}
	}
	fclose(output);
	free(imageR);
	free(imageG);
	free(imageB);






	//SetTextOutColor(0x1E);	// yellow on blue
	//DisplayTable();				// call ASM procedure












	return 0;
}

// Prompt the user for an integer. 

int askForInteger()
{
	int n;
	cout << "Enter an integer between 1 and 90,000: ";
	cin >> n;
	return n;
}

// Display a signed integer with a specified width.

void showInt(int value, int width)
{
	cout << setw(width) << value;
}






