﻿#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string>
#include <regex>
#include <msclr\marshal_cppstd.h>
#pragma once



struct BMP {
	int width;
	int height;
	unsigned char* imageR;
	unsigned char* imageG;
	unsigned char* imageB;
};




extern "C" {
	// external ASM procedures:
	//void SetTextOutColor(unsigned color);
	void Exposure(unsigned char* im1, int w, int h, int val);
	void Contrast(unsigned char* im1, int w, int h, int val);//max:255
	void Blur(unsigned char* im1, int w, int h);
	void Clear(unsigned char* im1, int w, int h, int val);
	void Edge(unsigned char* im1, int w, int h);
	void Invert(unsigned char* im1, int w, int h);
	void Grey(unsigned char* im1, unsigned char* im2, int w, int h);
	void Transpose(unsigned char* im1, unsigned char* im2, int w, int h);
	void rotate(unsigned char* im1, unsigned char* im2, int w, int h);
	void shrink(unsigned char* im1, unsigned char* im2, int w, int h);
	void ColorTemperature(unsigned char* im1, int w, int h, int val);
	void mirrorHori(unsigned char* im1, unsigned char* im2, int w, int h);
	void mirrorVert(unsigned char* im1, unsigned char* im2, int w, int h);
	void shift(unsigned char* im1, unsigned char* im2, int w, int h, int a, int b);
	// local C++ functions:
	//int isEmpty(unsigned char* im1, int w, int h);
	
}



BMP readBMP(std::string filename,int option) {

	BMP loadedBMP;
	char information[54];
	int width, height;
	unsigned char* imageR, * imageG, * imageB;

	FILE* input;

	input = fopen(filename.c_str(), "rb");
	fread(information, 1, 54, input);
	width = *(int*)(information + 18);
	height = *(int*)(information + 22);
	//image1 = (unsigned char*)calloc(width * height, 1);
	imageR = (unsigned char*)calloc(width * height, 1);
	imageG = (unsigned char*)calloc(width * height, 1);
	imageB = (unsigned char*)calloc(width * height, 1);
	switch (option) {
	case 1: {
		unsigned char blue, green, red;
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++) {
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
		
		break;
	}
	case 2: {
		unsigned char blue, green, red;
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++) {
				blue = getc(input);
				green = getc(input);
				red = getc(input);

				//read RGB as three different array
				imageR[i * width + j] = (blue + green + red) / 3;
				imageG[i * width + j] = (blue + green + red) / 3;
				imageB[i * width + j] = (blue + green + red) / 3;

				// convert to grayscale
				//image1[i * width + j] = (blue + green + red) / 3;
			}
		break;
	}
	case 3:{
		unsigned char blue, green, red;
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++) {
				blue = getc(input);
				green = getc(input);
				red = getc(input);

				//read RGB as three different array
				imageR[i * width + j] = 0;
				imageG[i * width + j] = 0;
				imageB[i * width + j] = 0;

				// convert to grayscale
				//image1[i * width + j] = (blue + green + red) / 3;
			}
		break;
		
	}

	}
	
	fclose(input);

	loadedBMP.width = width;
	loadedBMP.height = height;
	loadedBMP.imageR = imageR;
	loadedBMP.imageG = imageG;
	loadedBMP.imageB = imageB;
	
	return loadedBMP;
}

void saveBMP(std::string outputFileName, BMP outputBMP, std::string metadataSource) {

	//Save output image
	FILE* output, *metadataSourceFile;
	

	char metadata[54];
	metadataSourceFile = fopen(metadataSource.c_str(), "rb");
	fread(metadata, 1, 54, metadataSourceFile);

	output = fopen(outputFileName.c_str(), "wb");
	*(int*)(metadata + 18) = outputBMP.width;
	*(int*)(metadata + 22) = outputBMP.height;

	fwrite(metadata, 1, 54, output);
	
	for (int i = 0; i < outputBMP.width; i++) {
		for (int j = 0; j < outputBMP.height; j++) {
			// notice that we need to write as B,G,R order
			putc(outputBMP.imageB[i * outputBMP.width + j], output);
			putc(outputBMP.imageG[i * outputBMP.width + j], output);
			putc(outputBMP.imageR[i * outputBMP.width + j], output);
		}
	}
	fclose(output);

}


namespace TEST {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace std;

	/// <summary>
	/// MyForm 的摘要
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO:  在此加入建構函式程式碼
			//
		}

	protected:
		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^ inputTB;
	protected:

	protected:

	protected:
	private: System::Windows::Forms::Label^ Label1;

	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::TextBox^ outputTB;
	private: System::Windows::Forms::Button^ exitBT;


	private: System::Windows::Forms::Button^ submitBT;
	private: System::Windows::Forms::ComboBox^ appCB;

	private: System::Windows::Forms::OpenFileDialog^ openFileDialog1;
	private: System::Windows::Forms::Button^ openBT;
	private: System::Windows::Forms::Button^ saveBT;

	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::FolderBrowserDialog^ folderBrowserDialog1;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::TrackBar^ argumentTR;

	private: System::Windows::Forms::Label^ nopeLB;
	private: System::Windows::Forms::Label^ argumentLB;
	private: System::Windows::Forms::ComboBox^ argumentCB;
	private: System::Windows::Forms::TextBox^ argumentHoTB;
	private: System::Windows::Forms::TextBox^ argumentVeTB;
	private: System::Windows::Forms::Label^ argumentHoLB;
	private: System::Windows::Forms::Label^ argumentVeLB;













	private:
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
		/// 這個方法的內容。
		/// </summary>
		void InitializeComponent(void)
		{
			this->inputTB = (gcnew System::Windows::Forms::TextBox());
			this->Label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->outputTB = (gcnew System::Windows::Forms::TextBox());
			this->exitBT = (gcnew System::Windows::Forms::Button());
			this->submitBT = (gcnew System::Windows::Forms::Button());
			this->appCB = (gcnew System::Windows::Forms::ComboBox());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->openBT = (gcnew System::Windows::Forms::Button());
			this->saveBT = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->folderBrowserDialog1 = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->argumentTR = (gcnew System::Windows::Forms::TrackBar());
			this->nopeLB = (gcnew System::Windows::Forms::Label());
			this->argumentLB = (gcnew System::Windows::Forms::Label());
			this->argumentCB = (gcnew System::Windows::Forms::ComboBox());
			this->argumentHoTB = (gcnew System::Windows::Forms::TextBox());
			this->argumentVeTB = (gcnew System::Windows::Forms::TextBox());
			this->argumentHoLB = (gcnew System::Windows::Forms::Label());
			this->argumentVeLB = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->argumentTR))->BeginInit();
			this->SuspendLayout();
			// 
			// inputTB
			// 
			this->inputTB->Font = (gcnew System::Drawing::Font(L"新細明體", 26, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->inputTB->Location = System::Drawing::Point(276, 43);
			this->inputTB->Name = L"inputTB";
			this->inputTB->Size = System::Drawing::Size(492, 70);
			this->inputTB->TabIndex = 0;
			this->inputTB->Text = L"Please enter";
			// 
			// Label1
			// 
			this->Label1->AutoSize = true;
			this->Label1->BackColor = System::Drawing::Color::Transparent;
			this->Label1->Font = (gcnew System::Drawing::Font(L"新細明體", 26, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->Label1->Location = System::Drawing::Point(35, 47);
			this->Label1->Name = L"Label1";
			this->Label1->Size = System::Drawing::Size(215, 52);
			this->Label1->TabIndex = 1;
			this->Label1->Text = L"IntputFile";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"新細明體", 26, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->label2->Location = System::Drawing::Point(65, 369);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(157, 52);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Output";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"新細明體", 26, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->label3->Location = System::Drawing::Point(12, 148);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(253, 52);
			this->label3->TabIndex = 4;
			this->label3->Text = L"Application\r\n";
			// 
			// outputTB
			// 
			this->outputTB->Font = (gcnew System::Drawing::Font(L"新細明體", 26, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->outputTB->Location = System::Drawing::Point(276, 314);
			this->outputTB->Name = L"outputTB";
			this->outputTB->Size = System::Drawing::Size(492, 70);
			this->outputTB->TabIndex = 5;
			this->outputTB->Text = L"Folder Name";
			// 
			// exitBT
			// 
			this->exitBT->Font = (gcnew System::Drawing::Font(L"新細明體", 26, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->exitBT->Location = System::Drawing::Point(522, 517);
			this->exitBT->Name = L"exitBT";
			this->exitBT->Size = System::Drawing::Size(186, 75);
			this->exitBT->TabIndex = 6;
			this->exitBT->Text = L"Exit";
			this->exitBT->UseVisualStyleBackColor = true;
			this->exitBT->Click += gcnew System::EventHandler(this, &MyForm::exitBT_Click);
			// 
			// submitBT
			// 
			this->submitBT->Font = (gcnew System::Drawing::Font(L"新細明體", 26, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->submitBT->Location = System::Drawing::Point(161, 517);
			this->submitBT->Name = L"submitBT";
			this->submitBT->Size = System::Drawing::Size(186, 75);
			this->submitBT->TabIndex = 6;
			this->submitBT->Text = L"Submit";
			this->submitBT->UseVisualStyleBackColor = true;
			this->submitBT->Click += gcnew System::EventHandler(this, &MyForm::submitBT_Click_1);
			// 
			// appCB
			// 
			this->appCB->Font = (gcnew System::Drawing::Font(L"新細明體", 26, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->appCB->FormattingEnabled = true;
			this->appCB->Items->AddRange(gcnew cli::array< System::Object^  >(13) {
				L"Exposure", L"Contrast", L"Blur", L"Clear", L"Edge",
					L"Invert", L"Grey", L"Transpose", L"Rotate", L"Shrink", L"Color Temperature", L"Mirror", L"Shift"
			});
			this->appCB->Location = System::Drawing::Point(276, 145);
			this->appCB->Name = L"appCB";
			this->appCB->Size = System::Drawing::Size(492, 60);
			this->appCB->TabIndex = 8;
			this->appCB->Text = L"Please choose";
			this->appCB->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::appCB_SelectedIndexChanged);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// openBT
			// 
			this->openBT->Font = (gcnew System::Drawing::Font(L"新細明體", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->openBT->Location = System::Drawing::Point(799, 44);
			this->openBT->Name = L"openBT";
			this->openBT->Size = System::Drawing::Size(75, 69);
			this->openBT->TabIndex = 10;
			this->openBT->Text = L"...";
			this->openBT->UseVisualStyleBackColor = true;
			this->openBT->Click += gcnew System::EventHandler(this, &MyForm::openBT_Click);
			// 
			// saveBT
			// 
			this->saveBT->Font = (gcnew System::Drawing::Font(L"新細明體", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->saveBT->Location = System::Drawing::Point(799, 315);
			this->saveBT->Name = L"saveBT";
			this->saveBT->Size = System::Drawing::Size(75, 69);
			this->saveBT->TabIndex = 10;
			this->saveBT->Text = L"...";
			this->saveBT->UseVisualStyleBackColor = true;
			this->saveBT->Click += gcnew System::EventHandler(this, &MyForm::saveBT_Click);
			// 
			// textBox1
			// 
			this->textBox1->Font = (gcnew System::Drawing::Font(L"新細明體", 26, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->textBox1->Location = System::Drawing::Point(276, 416);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(492, 70);
			this->textBox1->TabIndex = 5;
			this->textBox1->Text = L"File Name(.bmp)";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"新細明體", 26, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->label4->Location = System::Drawing::Point(31, 229);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(219, 52);
			this->label4->TabIndex = 11;
			this->label4->Text = L"Argument";
			// 
			// argumentTR
			// 
			this->argumentTR->Location = System::Drawing::Point(353, 230);
			this->argumentTR->Maximum = 255;
			this->argumentTR->Name = L"argumentTR";
			this->argumentTR->Size = System::Drawing::Size(415, 69);
			this->argumentTR->TabIndex = 12;
			this->argumentTR->Visible = false;
			this->argumentTR->Scroll += gcnew System::EventHandler(this, &MyForm::argumentTR_Scroll);
			// 
			// nopeLB
			// 
			this->nopeLB->AutoSize = true;
			this->nopeLB->Font = (gcnew System::Drawing::Font(L"新細明體", 28, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->nopeLB->Location = System::Drawing::Point(442, 232);
			this->nopeLB->Name = L"nopeLB";
			this->nopeLB->Size = System::Drawing::Size(137, 56);
			this->nopeLB->TabIndex = 13;
			this->nopeLB->Text = L"Nope";
			// 
			// argumentLB
			// 
			this->argumentLB->AutoSize = true;
			this->argumentLB->Font = (gcnew System::Drawing::Font(L"新細明體", 22, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->argumentLB->Location = System::Drawing::Point(268, 233);
			this->argumentLB->Name = L"argumentLB";
			this->argumentLB->Size = System::Drawing::Size(40, 44);
			this->argumentLB->TabIndex = 14;
			this->argumentLB->Text = L"0";
			this->argumentLB->Visible = false;
			// 
			// argumentCB
			// 
			this->argumentCB->Font = (gcnew System::Drawing::Font(L"新細明體", 26, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->argumentCB->FormattingEnabled = true;
			this->argumentCB->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"Horizontal", L"Vertical" });
			this->argumentCB->Location = System::Drawing::Point(276, 228);
			this->argumentCB->Name = L"argumentCB";
			this->argumentCB->Size = System::Drawing::Size(492, 60);
			this->argumentCB->TabIndex = 16;
			this->argumentCB->Text = L"Please choose";
			this->argumentCB->Visible = false;
			// 
			// argumentHoTB
			// 
			this->argumentHoTB->Font = (gcnew System::Drawing::Font(L"新細明體", 26, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->argumentHoTB->Location = System::Drawing::Point(391, 225);
			this->argumentHoTB->Name = L"argumentHoTB";
			this->argumentHoTB->Size = System::Drawing::Size(126, 70);
			this->argumentHoTB->TabIndex = 17;
			this->argumentHoTB->Text = L"0";
			this->argumentHoTB->Visible = false;
			// 
			// argumentVeTB
			// 
			this->argumentVeTB->Font = (gcnew System::Drawing::Font(L"新細明體", 26, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->argumentVeTB->Location = System::Drawing::Point(642, 226);
			this->argumentVeTB->Name = L"argumentVeTB";
			this->argumentVeTB->Size = System::Drawing::Size(126, 70);
			this->argumentVeTB->TabIndex = 18;
			this->argumentVeTB->Text = L"0";
			this->argumentVeTB->Visible = false;
			// 
			// argumentHoLB
			// 
			this->argumentHoLB->AutoSize = true;
			this->argumentHoLB->Font = (gcnew System::Drawing::Font(L"新細明體", 26, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->argumentHoLB->Location = System::Drawing::Point(274, 232);
			this->argumentHoLB->Name = L"argumentHoLB";
			this->argumentHoLB->Size = System::Drawing::Size(111, 52);
			this->argumentHoLB->TabIndex = 19;
			this->argumentHoLB->Text = L"Hori";
			this->argumentHoLB->Visible = false;
			// 
			// argumentVeLB
			// 
			this->argumentVeLB->AutoSize = true;
			this->argumentVeLB->Font = (gcnew System::Drawing::Font(L"新細明體", 26, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->argumentVeLB->Location = System::Drawing::Point(525, 233);
			this->argumentVeLB->Name = L"argumentVeLB";
			this->argumentVeLB->Size = System::Drawing::Size(109, 52);
			this->argumentVeLB->TabIndex = 20;
			this->argumentVeLB->Text = L"Vert";
			this->argumentVeLB->Visible = false;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(9, 18);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(891, 638);
			this->Controls->Add(this->argumentVeLB);
			this->Controls->Add(this->argumentHoLB);
			this->Controls->Add(this->argumentVeTB);
			this->Controls->Add(this->argumentHoTB);
			this->Controls->Add(this->nopeLB);
			this->Controls->Add(this->argumentTR);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->saveBT);
			this->Controls->Add(this->openBT);
			this->Controls->Add(this->appCB);
			this->Controls->Add(this->submitBT);
			this->Controls->Add(this->exitBT);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->outputTB);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->Label1);
			this->Controls->Add(this->inputTB);
			this->Controls->Add(this->argumentLB);
			this->Controls->Add(this->argumentCB);
			this->Name = L"MyForm";
			this->Text = L"ImageProcessing";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->argumentTR))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
	






#pragma endregion
private: System::Void submitBT_Click_1(System::Object^ sender, System::EventArgs^ e) {











	BMP image,image2;
	
	String^ inputFile_str = inputTB->Text;
	/*
	char* inputFile = new char();
	int n = inputFile_str->Length;
	for (int i = 0; i < n; i++) {
		inputFile[i] = inputFile_str[i];
	}
	*/
	std::string inputFile = msclr::interop::marshal_as<std::string>(inputFile_str);
	//image = readBMP("D:\\Sophomore\\AssemblyLang\\FinalProject\\ASMFinalProject\\lena.bmp", 2);
	inputFile = std::regex_replace(inputFile, std::regex(R"(\\)"), R"(\\)");
	String^ o = appCB->Text; // option

	



	if (o == "Grey") {
		image =  readBMP(inputFile,1);
		image2 = readBMP(inputFile,2);
	}
	else if (o == "Transpose" || o == "Shrink" || o == "Mirror" || o == "Rotate" || o == "Shift") {
		image = readBMP(inputFile, 3);
		image2 = readBMP(inputFile, 1);
	}
	else {
		image = readBMP(inputFile, 1);
	}
	int width = image.width;
	int height = image.height;
	unsigned char* imageR, *imageG, *imageB, *image2R, *image2G, *image2B;
	imageR = (unsigned char*)calloc(width * height, 1);
	imageG = (unsigned char*)calloc(width * height, 1);
	imageB = (unsigned char*)calloc(width * height, 1);
	image2R = (unsigned char*)calloc(width * height, 1);
	image2G = (unsigned char*)calloc(width * height, 1);
	image2B = (unsigned char*)calloc(width * height, 1);
	imageR = image.imageR;
	imageG = image.imageG;
	imageB = image.imageB;
	image2R = image2.imageR;
	image2G = image2.imageG;
	image2B = image2.imageB;
	//Application options
	if (o == "Exposure") {
		int val = argumentTR->Value;
		Exposure(imageR, width, height, val);
		Exposure(imageG, width, height, val);
		Exposure(imageB, width, height, val);
	}
	else if (o == "Blur") {
		Blur(imageR, width, height);
		Blur(imageG, width, height);
		Blur(imageB, width, height);
	}
	else if (o == "Contrast") {
		int val = argumentTR->Value;
		Contrast(imageR, width, height, val);
		Contrast(imageG, width, height, val);
		Contrast(imageB, width, height, val);
	}
	else if (o == "Clear") {
		int val = argumentTR->Value;
		Clear(imageR, width, height, val);
		Clear(imageG, width, height, val);
		Clear(imageB, width, height, val);

	}
	else if (o == "Edge") {
		Edge(imageR, width, height);
		Edge(imageG, width, height);
		Edge(imageB, width, height);
	}
	else if (o == "Invert") {
		Invert(imageR, width, height);
		Invert(imageG, width, height);
		Invert(imageB, width, height);
	}
	else if (o == "Grey") {
		Grey(imageR, image2R, width, height);
		Grey(imageG, image2G, width, height);
		Grey(imageB, image2B, width, height);
	}
	else if (o == "Transpose") {
		Transpose(image2R, imageR, width, height);
		Transpose(image2G, imageG, width, height);
		Transpose(image2B, imageB, width, height);
	}
	else if (o == "Rotate") {
		String^ angle = argumentCB->Text;
		if (angle == "90") {
			rotate(image2R, imageR, width, height);
			rotate(image2G, imageG, width, height);
			rotate(image2B, imageB, width, height);
		}
		else if (angle == "180") {
			rotate(image2R, imageR, width, height);
			rotate(image2G, imageG, width, height);
			rotate(image2B, imageB, width, height);
			*image2R = *imageR;
			*image2G = *imageG;
			*image2B = *imageB;
			rotate(image2B, imageB, width, height);
			rotate(image2R, imageR, width, height);
			rotate(image2G, imageG, width, height);
		}
		else if (angle == "270") {
			rotate(image2R, imageR, width, height);
			rotate(image2G, imageG, width, height);
			rotate(image2B, imageB, width, height);
			rotate(imageR, image2R, width, height);
			rotate(imageG, image2G, width, height);
			rotate(imageB, image2B, width, height);
			rotate(image2R, imageR, width, height);
			rotate(image2G, imageG, width, height);
			rotate(image2B, imageB, width, height);
		}
	}
	else if (o == "Shrink") {
		shrink(image2R, imageR, width, height);
		shrink(image2G, imageG, width, height);
		shrink(image2B, imageB, width, height);
	}
	else if (o == "Color Temperature") {
		int val = argumentTR->Value;
		ColorTemperature(imageB, width, height, val);
	}
	else if (o == "Mirror") {
		String^ method = argumentCB->Text;
		if (method == "Horizontal") {
			mirrorHori(image2R, imageR, width, height);
			mirrorHori(image2G, imageG, width, height);
			mirrorHori(image2B, imageB, width, height);
		}
		else if (method == "Vertical") {
			mirrorVert(image2R, imageR, width, height);
			mirrorVert(image2G, imageG, width, height);
			mirrorVert(image2B, imageB, width, height);
		}
	}
	else if (o == "Shift") {
		int hori = System::Convert::ToInt32(argumentHoTB->Text);
		int vert = System::Convert::ToInt32(argumentVeTB->Text);

		shift(image2R, imageR, width, height, vert, hori);
		shift(image2G, imageG, width, height, vert, hori);
		shift(image2B, imageB, width, height, vert, hori);
	}




	//Save output image
	String^ outputFolder = outputTB->Text;
	String^ outputFilename = textBox1->Text;
	String^ outputFile_str = outputFolder + "\\" + outputFilename;
	std::string outputFile = msclr::interop::marshal_as<std::string>(outputFile_str);
	outputFile = std::regex_replace(outputFile, std::regex(R"(\\)"), R"(\\)");
	saveBMP(outputFile, image, inputFile);
	
	
}










private: System::Void exitBT_Click(System::Object^ sender, System::EventArgs^ e) {
	exit(0);
}
private: System::Void openBT_Click(System::Object^ sender, System::EventArgs^ e) {
	openFileDialog1->Title = "Insert an Image";
	openFileDialog1->FileName = "";
	openFileDialog1->Filter = "Bitmap Image (.bmp)|*.bmp";
	openFileDialog1->ShowDialog();
	inputTB->Text = openFileDialog1->FileName;
}
private: System::Void saveBT_Click(System::Object^ sender, System::EventArgs^ e) {
	folderBrowserDialog1->ShowDialog();
	outputTB->Text = folderBrowserDialog1->SelectedPath;
}
private: System::Void appCB_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	String^ o = appCB->Text;
	//argument display type : trackBar, Nope, ComboBox, TextBox
	if (o == "Exposure" || o == "Contrast" || o == "Clear" || o == "Color Temperature") {
		argumentLB->Visible = true;
		argumentTR->Visible = true;
		nopeLB->Visible = false;
		argumentCB->Visible = false;
		argumentHoLB->Visible = false;
		argumentHoTB->Visible = false;
		argumentVeLB->Visible = false;
		argumentVeTB->Visible = false;
	}
	else if (o == "Blur" || o == "Edge" || o == "Invert" || o == "Grey" || o == "Shrink" || o == "Transpose") {
		argumentLB->Visible = false;
		argumentTR->Visible = false;
		nopeLB->Visible = true;
		argumentCB->Visible = false;
		argumentHoLB->Visible = false;
		argumentHoTB->Visible = false;
		argumentVeLB->Visible = false;
		argumentVeTB->Visible = false;
	}
	else if (o == "Mirror" || o == "Rotate") {
		argumentLB->Visible = false;
		argumentTR->Visible = false;
		nopeLB->Visible = false;
		argumentCB->Visible = true;
		argumentHoLB->Visible = false;
		argumentHoTB->Visible = false;
		argumentVeLB->Visible = false;
		argumentVeTB->Visible = false;
	}
	else if (o == "Shift") {
		argumentLB->Visible = false;
		argumentTR->Visible = false;
		nopeLB->Visible = false;
		argumentCB->Visible = false;
		argumentHoLB->Visible = true;
		argumentHoTB->Visible = true;
		argumentVeLB->Visible = true;
		argumentVeTB->Visible = true;

	}
	//TrackBar
	if (o == "Exposure" || o == "Contrast" || o == "Clear") {
		argumentTR->Maximum = 255;
		argumentTR->Minimum = 0;
		argumentTR->Value = 0;
		argumentTR->LargeChange = 5;
		argumentTR->SmallChange = 1;
		argumentTR->TickFrequency = 1;
	}
	else if (o == "Color Temperature") {
		argumentTR->Maximum = 255;
		argumentTR->Minimum = -255;
		argumentTR->Value = 0;
		argumentTR->LargeChange = 5;
		argumentTR->SmallChange = 1;
		argumentTR->TickFrequency = 1;
	}
	//ComboBox
	if (o == "Mirror") {
		argumentCB->Text = "Horizontal";
		argumentCB->Items->Clear();
		argumentCB->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"Horizontal", L"Vertical" });
	}
	else if (o == "Rotate") {
		argumentCB->Text = "90";
		argumentCB->Items->Clear();
		argumentCB->Items->AddRange(gcnew cli::array< System::Object^  >(3) { L"90", L"180", L"270" });
	}
	//reset argumentLB
	argumentLB->Text = "0";
}
private: System::Void argumentTR_Scroll(System::Object^ sender, System::EventArgs^ e) {
	argumentLB->Text = System::Convert::ToString(argumentTR->Value);
}
};
}






