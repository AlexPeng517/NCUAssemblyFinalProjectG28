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
	void Brightness(unsigned char *im1, int w, int h, int val);
	void Contrast(unsigned char* im1, int w, int h, int val);//max:255
	void Blur(unsigned char* im1, int w, int h);
	void Clear(unsigned char* im1, int w, int h, int val);
	void Edge(unsigned char* im1, int w, int h);
	void invert(unsigned char* im1, int w, int h);
	// local C++ functions:
	int askForInteger();
	void showInt(int value, int width);
	int isEmpty(unsigned char* im1, int w, int h);
	void Grey(unsigned char* im1, unsigned char* im2, int w, int h);
	void flip(unsigned char* im1, unsigned char* im2, int w, int h);//他其實是rotate
	void shrink(unsigned char* im1, unsigned char* im2, int w, int h);
	void mirror(unsigned char* im1, unsigned char* im2, int w, int h);
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
	unsigned char* imageGrey1, * imageGrey2, * imageGrey3, *imagetmpR, * imagetmpG, * imagetmpB;
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
	imageGrey1 = (unsigned char*)calloc(width * height, 1);
	imageGrey2 = (unsigned char*)calloc(width * height, 1);
	imageGrey3 = (unsigned char*)calloc(width * height, 1);
	imagetmpR = (unsigned char*)calloc(width * height, 1);
	imagetmpG = (unsigned char*)calloc(width * height, 1);
	imagetmpB = (unsigned char*)calloc(width * height, 1);
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
			imageGrey1[i * width + j] = (red + blue + green) / 3;
			imageGrey2[i * width + j] = (red + blue + green) / 3;
			imageGrey3[i * width + j] = (red + blue + green) / 3;
			imagetmpR[i * width + j] = 0;
			imagetmpG[i * width + j] = 0;
			imagetmpB[i * width + j] = 0;
			// convert to grayscale
			//image1[i * width + j] = (blue + green + red) / 3;
		}
	fclose(input);

	//Contrast(imageR, 512, 512, 256);
	//Contrast(imageG, 512, 512, 256);
	//Contrast(imageB, 512, 512, 256);
	mirror(imageR, imagetmpR,512, 512);
	mirror(imageG, imagetmpG, 512, 512);
	mirror(imageB, imagetmpB, 512, 512);

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
				putc(imagetmpB[i * width + j], output);
				putc(imagetmpG[i * width + j], output);
				putc(imagetmpR[i * width + j], output);
			}
		}
		fclose(output);
	free(imageR);
	free(imageG);
	free(imageB);
	free(imageGrey1);
	free(imageGrey2);
	free(imageGrey3);
	free(imagetmpR);
	free(imagetmpG);
	free(imagetmpB);






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






