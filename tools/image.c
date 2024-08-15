// Task: Lab10 Digital Image Processing
// Programmer: Ziao Feng 2030026036
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "proto.h"
#define PI acos(-1)
/*
#include <cv.h>
#include <highgui.h>
#include <cvaux.h> //必须引此头文件
*/
/*
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#pragma comment(lib, "cv.lib")
#pragma comment(lib, "cxcore.lib")
#pragma comment(lib, "highgui.lib")
*/
// this function is to convert 16 bit image to 8 bit
Image8 *Convert_16_8(Image* image);
unsigned char *_16bit_convert_8bit(unsigned short *_16bit, unsigned int size);
unsigned char *_16bit_convert_8bit(unsigned short *_16bit, unsigned int size)
{
	size *= 2;
	unsigned char *_8bit = (unsigned char*)malloc(size*sizeof(unsigned char));
	memset(_8bit, 0x0, size);
	if (!_8bit) {
		
		return NULL;
	}

	/*判断是大端还是小端*/
	int big_endian = 1;
	unsigned short _s = 0x1122;
	unsigned char *_c = (unsigned char *)&_s;
	if (*_c == 0x22)
		big_endian = 0;

	for (unsigned int i = 0, j = 0; i < size; i++, i++, j++)
	{
		if (big_endian)
		{
			_8bit[i] = _16bit[j] >> 8;
			_8bit[i + 1] = _16bit[j] & 0xff;
		}
		else
		{
			_8bit[i] = _16bit[j] & 0xff;
			_8bit[i + 1] = _16bit[j] >> 8;
		}
	}
	return _8bit;
}

Image8 *thresholding_16_8(Image* image, int th);
Image8 *CreateNewImage8(Image8 * image, char *comment);
void SavePNMImage8(Image8 *, char *);
void SavePNMImage8(Image8 *temp_image, char *filename)
{
	int num, j;
	int size;
	FILE *fp;
	//char comment[100];


	printf("Saving Image %s\n", filename);
	fp = fopen(filename, "wb");// write in binary format
	if (!fp){
		printf("cannot open file for writing");
		exit(0);
	}

	//strcpy(comment,"#Created by Dr Mohamed N. Ahmed");

	if (temp_image->Type == GRAY){  // Gray (pgm)
		fprintf(fp, "P5\n");
		size = temp_image->Width * temp_image->Height;
	}
	else  if (temp_image->Type == COLOR){  // Color (ppm)
		fprintf(fp, "P6\n");
		size = temp_image->Width * temp_image->Height * 3;
	}

	for (j = 0; j<temp_image->num_comment_lines; j++)
		fprintf(fp, "%s\n", temp_image->comments[j]);

	fprintf(fp, "%d %d\n%d\n", temp_image->Width, temp_image->Height, 255);

	num = fwrite((void *)temp_image->data, 1, (size_t)size, fp);

	if (num != size){
		printf("cannot write image data to file");
		exit(0);
	}

	fclose(fp);
}

Image *removeLowerPartMask(Image*, double a, double b, double c);
Image *removeLowerPart(Image*,double a, double b, double c);
Image *thresholding_16(Image*, int);
int otsuThresholding1_16(Image*);
// void testshow(char*);

float map(float, float, float, float, float);// use to normalize data
float CalculateVariance(Image*);// use to calculate the variance of an image.
void SavePNMImage(Image *, char *);
int PlusDilationUnion1(int* visited, unsigned short *tempin, int Width, int Height, int row, int col, int index);
int PlusDilationUnion(int* visited, unsigned short *tempin, int Width, int Height, int row, int col, int index);
void CheckConnect1(Image*, char*);
void CheckConnect(Image*,char*);

int PlusDilationUnion2(int* visited, unsigned short *tempin, int Width, int Height, int row, int col, int index, unsigned short *tempout,unsigned short *tempout1);
Image *regionGrowing(Image*);
Image* movingAverage(Image* );
Image *PartitionOtsuThresholding(Image*);
int otsuThresholding1(Image*);// the grayScale should change to 256
int otsuThresholding(Image*);// the grayScale should change to 256

Image *GlobalThresholding(Image*, double t0);
Image *CannyEdgeDetection(Image*, double th1, double th2);// th1 bigger than th2, th1 is the upper limit,th2 is lower limit.
Image *LoGImage(Image*, int ksize, double sigma, double th);
double *generateGaussianTemplate(int ksize, double sigma);// ksize is filter size
double *GaussianLaplacianGenerator(int ksize, double sigma);
Image *GaussianImage(Image*, int ksize, double sigma);
Image *RobertGradientEdgeDetection(Image*);
Image *PrewittGradientEdgeDetection(Image*, double threshold, int type);// type = 1, vertical; type=2, horizontal; type=3, magnitude;

Image *SobelGradientEdgeDetection(Image*, double threshold, int type);// type = 1, vertical; type=2, horizontal; type=3, magnitude;


Image *BoundaryExtraction(Image*, int f, int* pos, int p_size);// attribute for erosion
Image *OpeningImage(Image*, int f1, int*, int p_size1, int f2, int*, int p_size2);// first maximum(XXX1) ,then minimum(XXX2)
Image *ClosingImage(Image*, int f1, int*, int p_size1, int f2, int*, int p_size2);// first minimum ,then maximum

Image *DilationImage(Image *, int f, int*,int p_size);// int* is the position index of the filter element we want, in different shape.
// for a 3*3 filter with + shape, the int* part attribute is { 1, 3, 4, 5, 7}
// for a 3*3 filter with "L" shape, the int* part attribute is {0, 3, 6, 7, 8}
// p_size is the number of elements in the int array
Image *ErosionImage(Image *, int f, int*, int p_size);// int* is the position index of the filter element we want, in different shape.
// for a 3*3 filter with + shape, the int* part attribute is { 1, 3, 4, 5, 7}
// for a 3*3 filter with "L" shape, the int* part attribute is {0, 3, 6, 7, 8}
// p_size is the number of elements in the int array

DFT *Filt_Notch_Lena(DFT*, int width, int height);
DFT *AddNoise(DFT*, float rad);
DFT *LNandGenerateDFT(Image*);// do ln() and DFT generate real and imag, 

DFT *Filt_GBF(DFT*, float, float);// DFT, cutoff frequency, bandwidth
DFT *Filt_BBF(DFT*, float, float, int facotr_n);// DFT, cutoff frequency, bandwidth, constant factor n
DFT *Filt_IBF(DFT*, float, float);// DFT, cutoff frequency, bandwidth
DFT *Filt_Homomorphic(DFT*, float, float, int, float);
DFT *Filt_IDHPF(DFT*, float, int);
DFT *Filt_BHPF(DFT*, float, int);
DFT *Filt_GHPF(DFT*, float, int);

DFT *Filt_IDLPF(DFT*, float, int);
DFT *Filt_BLPF(DFT*, float, int);
DFT *Filt_GLPF(DFT*, float, int);
Image *InverseMagnitudeDFT_norm_exp(DFT*, Image *);
Image *InverseMagnitudeDFT_no_norm(DFT*, Image *);// this function magnitude the real and imag part and get the restored image
Image *InverseMagnitudeDFT(DFT* , Image *);// this function magnitude the real and imag part and get the restored image with normed to 0-255
DFT *PhaseDFT_no_norm(DFT*, Image *);
Image *PhaseDFT(DFT* ,Image*);// do phase spectrum.
Image *MagnitudeDFT(DFT*,Image*);// do magnitude spectrum.
DFT *GenerateDFT(Image*);// the result is an DFT, containing two data.
DFT *GenerateInverseDFT(DFT* , Image*,int);// given real and imag, generate inverseDFT(return real and imag), int=0:use both magnitude & phase, int=1:use only phase, int=2:only magnitude
Image *SwapImage(Image *);
Image *ReadPNMImage(char *);
Image *CreateNewImage(Image *, char *comment);
Image *ReplaceValues(Image*, int value_a, int value_b);// this function remove the pixels with value_a by value_b
Image *AdaptiveMedianImage(Image *, int f);// f is S_max, the largest window size.
Image *AlphaMeanImage(Image *, int f,int d);
Image* NegativeImage(Image*);
Image *GeoAvgImage(Image *image, int f);
Image *AvgImage(Image *, int f);
Image *MedianImage(Image *, int f);
Image *TranslateImage(Image *, int dx, int dy);
Image *RotateImage(Image *, int angle);
Image *ShearVertivalImage(Image*, float v_para);
Image *ShearHorizontalImage(Image*, float h_para);

Image *LaplacianSharpenImage(Image*, int type);// type = 0, use type1 operator, type=1, use type2 operator

Image *SobelImage(Image*);
Image *thresholding(Image*, int);// int:the boundary threshold of pixel value.
Image *SharpenImage(Image*,Image*, float C);// this function add the mask and the original image, (mask, original, factor)
Image *GammaImage(Image*, float gama);
Image *GlobalHistogram(Image*);
Image *LocalHistogram(Image*);
Image *ShiftImage(Image*);
int TestReadImage(char *, char *);

/**
* @description:
* @param {float} x 要归一化的值
* @param {float} in_min 原数据最小值
* @param {float} in_max 原数据最大值
* @param {float} out_min 归一化后的最小值
* @param {float} out_max 归一化后的最大值
* @return {float} 归一化后的值
*/
float map(float x, float in_min, float in_max, float out_min, float out_max)
{
	//printf("%f %f %f\n", (float)((x - in_min) * (out_max - out_min)), (float)(in_max - in_min), (float)((x - in_min) * (out_max - out_min)) / (float)(in_max - in_min) + out_min);
	return (float)((x - in_min) * (out_max - out_min)) / (float)(in_max - in_min) + out_min;
}

int main(int argc, char **argv)
{
	TestReadImage("D://DIP//DIP//PGM_IMAGES//PGM_IMAGES//noisy_fingerprint.pgm", "D://DIP//DIP//Lab7//Results//lena_noise.pgm");
	//testshow("D://DIP//DIP//PGM_IMAGES//PGM_IMAGES//lena.pgm");
	return(0);
 }

int TestReadImage(char *filename, char *outfilename)
{
	Image *image;//open
	Image *image1;//close
	Image *outimage;
	Image *outimage1;
	Image *outimage2;
	Image8 *outimage8bit;
	DFT *outdft;
	DFT *outdft2;
	char* outfilename1;
	char* outfilename2;
	/*
	image = ReadPNMImage("E://OLD_F//fyp//match//result_open73_original//9_result.pgm");
	outimage8bit = Convert_16_8(image);
	//outimage8bit = thresholding_16_8(image, 33600);
	SavePNMImage8(outimage8bit, "E://OLD_F//fyp//match//result_open73_original_8bit//9_result_8bit.pgm");
	*/
	/* create background 16bit image for open */
	/*
	image = ReadPNMImage("E://OLD_F//fyp//match//TMJ-A_Open 0073_33600 16.pgm");
	outimage8bit = Convert_16_8(image);
	//outimage8bit = thresholding_16_8(image, 33600);
	SavePNMImage8(outimage8bit, "E://OLD_F//fyp//match//TMJ-A_Open 0073_33600 8bit_value.pgm");
	*/
	/* create background 16bit image for close */
	image = ReadPNMImage("E://OLD_F//fyp//match//TMJ-A_Closed_33500 0134 16.pgm");
	outimage8bit = Convert_16_8(image);
	//outimage8bit = thresholding_16_8(image, 33600);
	SavePNMImage8(outimage8bit, "E://OLD_F//fyp//match//TMJ-A_Closed_33500 0134 8bit_value.pgm");
	/*
	image = ReadPNMImage("F://fyp//pgm//TMJ-A_Open//TMJ-A_Open 0065.pgm");
	outimage = removeLowerPart(image, 0.002, 250, 270);
	outimage8bit = thresholding_16_8(outimage, 33600);
	SavePNMImage8(outimage8bit, "F://fyp//pgm//TMJ-A_Open_Res//TMJ-A_Open 0065_33600_8bit.pgm");
	*/
	/* preprocessing: generate thresholding and remove lower part for TMJ-A*/
	/*
	for (int i = 30; i <= 112; i++){
		char filename[200];
		sprintf(filename, "F://fyp//pgm//TMJ-A_Open//TMJ-A_Open %04d.pgm", i);
		printf("Processing file: %s\n", filename);
		image = ReadPNMImage(filename);
		outimage = removeLowerPart(image, 0.002, 250, 270);
		outimage8bit = thresholding_16_8(outimage, 33600);
		char filename1[200];
		sprintf(filename1, "F://fyp//pgm//TMJ-A_Open_Res_8bit//TMJ-A_Open %04d_33600.pgm", i);
		printf("Saving file: %s\n", filename1);
		SavePNMImage8(outimage8bit, filename1);

		sprintf(filename1, "F://fyp//pgm//TMJ-A_Open_Res_8bit_3D//TMJ-A_Open_33600 %04d.pgm", i);
		printf("Saving file: %s\n", filename1);
		SavePNMImage8(outimage8bit, filename1);
	}
	*/
	// 2023/9/14/18:43 add the below one multi-line comment
	//	before semister 1 the code was for canny Edge detection
	/*
	image = ReadPNMImage("F://fyp//pgm//TMJ-A_Closed//TMJ-A Closed - 0074.pgm");
	outimage = CannyEdgeDetection(image,32400,31768);
	//outimage = GaussianImage(image, 5, 1);
	SavePNMImage(outimage, "F://fyp//pgm//TMJ-A_Closed_test//TMJ-A Closed - 0074_canny.pgm");
	//outimage = removeLowerPart(image, 0.002, 250, 270);
	//outimage8bit = thresholding_16_8(outimage, 33600);
	//SavePNMImage8(outimage8bit, "F://fyp//pgm//TMJ-A_Open_Res//TMJ-A_Open 0065_33600_8bit.pgm");
	*/

	/* preprocessing: generate thresholding and remove lower part for TMJ-A Closed*/

	/*
	for (int i = 65; i <= 250; i++){
		char filename[200];
		sprintf(filename, "F://fyp//pgm//TMJ-A_Closed//TMJ-A Closed - %04d.pgm", i);
		printf("Processing file: %s\n", filename);
		image = ReadPNMImage(filename);
		outimage = removeLowerPart(image, 0.002, 250, 250);
		outimage8bit = thresholding_16_8(outimage, 33500);
		char filename1[200];
		sprintf(filename1, "F://fyp//pgm//TMJ-A_Closed_Res_8bit//TMJ-A_Closed_33500 %04d.pgm", i);
		printf("Saving file: %s\n", filename1);
		SavePNMImage8(outimage8bit, filename1);

		sprintf(filename1, "F://fyp//pgm//TMJ-A_Closed_Res_8bit_3D//TMJ-A_Closed_33500 %04d.pgm", i);
		printf("Saving file: %s\n", filename1);
		SavePNMImage8(outimage8bit, filename1);
	}
	*/
	/* from 65-250*/
	for (int i = 65; i <= 250; i++){

		char filename[200];
		sprintf(filename, "E://OLD_F//fyp//pgm//TMJ-A_Closed//TMJ-A Closed - %04d.pgm", i);
		printf("Processing file: %s\n", filename);
		image = ReadPNMImage(filename);
		outimage = removeLowerPart(image, 0.002, 250, 250);
		outimage = thresholding_16(outimage, 33100);
		char filename1[200];
		sprintf(filename1, "E://OLD_F//fyp//pgm//TMJ-A_Closed_Res_65-250//TMJ-A_Closed_33100 %04d.pgm", i);
		printf("Saving file: %s\n", filename1);
		SavePNMImage(outimage, filename1);
		// Try lower threshold(<33100)
		outimage = removeLowerPart(image, 0.002, 250, 250);
		outimage = thresholding_16(outimage, 32500);
		sprintf(filename1, "E://OLD_F//fyp//pgm//TMJ-A_Closed_Res_65-250//TMJ-A_Closed_32500 %04d.pgm", i);
		printf("Saving file: %s\n", filename1);
		SavePNMImage(outimage, filename1);

		outimage = removeLowerPart(image, 0.002, 250, 250);
		outimage = thresholding_16(outimage, 32700);
		sprintf(filename1, "E://OLD_F//fyp//pgm//TMJ-A_Closed_Res_65-250//TMJ-A_Closed_32700 %04d.pgm", i);
		printf("Saving file: %s\n", filename1);
		SavePNMImage(outimage, filename1);

		outimage = removeLowerPart(image, 0.002, 250, 250);
		outimage = thresholding_16(outimage, 32900);
		sprintf(filename1, "E://OLD_F//fyp//pgm//TMJ-A_Closed_Res_65-250//TMJ-A_Closed_32900 %04d.pgm", i);
		printf("Saving file: %s\n", filename1);
		SavePNMImage(outimage, filename1);

		outimage = removeLowerPart(image, 0.002, 250, 250);
		outimage = thresholding_16(outimage, 33000);
		sprintf(filename1, "E://OLD_F//fyp//pgm//TMJ-A_Closed_Res_65-250//TMJ-A_Closed_33000 %04d.pgm", i);
		printf("Saving file: %s\n", filename1);
		SavePNMImage(outimage, filename1);
		
		// Try higher threshold(>33100)
		/*
		outimage = removeLowerPart(image, 0.002, 250, 250);
		outimage = thresholding_16(outimage, 33300);
		sprintf(filename1, "E://OLD_F//fyp//pgm//TMJ-A_Closed_Res_65-250//TMJ-A_Closed_33300 %04d.pgm", i);
		printf("Saving file: %s\n", filename1);
		SavePNMImage(outimage, filename1);

		outimage = removeLowerPart(image, 0.002, 250, 250);
		outimage = thresholding_16(outimage, 33500);
		sprintf(filename1, "E://OLD_F//fyp//pgm//TMJ-A_Closed_Res_65-250//TMJ-A_Closed_33500 %04d.pgm", i);
		printf("Saving file: %s\n", filename1);
		SavePNMImage(outimage, filename1);

		outimage = removeLowerPart(image, 0.002, 250, 250);
		outimage = thresholding_16(outimage, 33700);
		sprintf(filename1, "E://OLD_F//fyp//pgm//TMJ-A_Closed_Res_65-250//TMJ-A_Closed_33700 %04d.pgm", i);
		printf("Saving file: %s\n", filename1);
		SavePNMImage(outimage, filename1);
		*/

	}
	

	/*
	outimage = removeLowerPart(image, 0.002, 250, 270);
	outimage8bit = thresholding_16_8(outimage, 33600);
	SavePNMImage8(outimage8bit, "F://fyp//pgm//TMJ-A_Open_Res_8bit//TMJ-A Open - 0065_33600_8bit.pgm");
	*/
	/* preprocessing: generate thresholding and remove lower part for TMJ-A OPEN*/
	/* try 4 threshold: 33100 33600 700 800*/
	/*
	for (int i = 30; i <= 112; i++){
		char filename[200];
		sprintf(filename, "E://OLD_F//fyp//pgm//TMJ-A_Open//TMJ-A_Open %04d.pgm", i);
		printf("Processing file: %s\n", filename);
		image = ReadPNMImage(filename);
		outimage = removeLowerPart(image, 0.002, 250, 270);
		outimage = thresholding_16(outimage, 33100);
		char filename1[200];
		sprintf(filename1, "E://OLD_F//fyp//pgm//TMJ-A_Open_Res//TMJ-A_Open %04d_33100.pgm", i);
		printf("Saving file: %s\n", filename1);
		SavePNMImage(outimage, filename1);

		outimage = removeLowerPart(image, 0.002, 250, 270);
		outimage = thresholding_16(outimage, 33600);
		sprintf(filename1, "E://OLD_F//fyp//pgm//TMJ-A_Open_Res//TMJ-A_Open %04d_33600.pgm", i);
		printf("Saving file: %s\n", filename1);
		SavePNMImage(outimage, filename1);

		outimage = removeLowerPart(image, 0.002, 250, 270);
		outimage = thresholding_16(outimage, 33700);
		sprintf(filename1, "E://OLD_F//fyp//pgm//TMJ-A_Open_Res//TMJ-A_Open %04d_33700.pgm", i);
		printf("Saving file: %s\n", filename1);
		SavePNMImage(outimage, filename1);

		outimage = removeLowerPart(image, 0.002, 250, 270);
		outimage = thresholding_16(outimage, 33800);
		sprintf(filename1, "E://OLD_F//fyp//pgm//TMJ-A_Open_Res//TMJ-A_Open %04d_33800.pgm", i);
		printf("Saving file: %s\n", filename1);
		SavePNMImage(outimage, filename1);
		
		outimage = removeLowerPart(image, 0.002, 250, 270);
		outimage = thresholding_16(outimage, 33600);
		sprintf(filename1, "E://OLD_F//fyp//pgm//TMJ-A_Open_Res_3D//TMJ-A_Open_33600 %04d.pgm", i);
		printf("Saving file: %s\n", filename1);
		SavePNMImage(outimage, filename1);
	}
	*/
	
	
		/*
	image = ReadPNMImage("F://fyp//pgm//TMJ-A_Open//TMJ-A_Open 0065.pgm");
	outimage = removeLowerPart(image, 0.002, 250, 270);
	outimage = thresholding_16(outimage, 33600);
	SavePNMImage(outimage, "F://fyp//pgm//TMJ-A_Open_Res//TMJ-A_Open 0065_33600.pgm");
	outimage = thresholding_16(outimage, 33700);
	SavePNMImage(outimage, "F://fyp//pgm//TMJ-A_Open_Res//TMJ-A_Open 0065_33700.pgm");
	*/
	/*
	// for testing you can change the filename and run those testing
	image = ReadPNMImage("F://fyp//pgm//TMJ-A_Open//TMJ-A_Open 0083.pgm");
	//printf("%d", otsuThresholding1_16(image));
	//outimage = removeLowerPartMask(image, 0.002, 250, 270);
	//outimage = thresholding_16(image, 34220);
	//outimage = thresholding_16(image, otsuThresholding1_16(image));
	outimage = removeLowerPart(image, 0.002, 250, 270);
	//outimage = thresholding_16(image, otsuThresholding1_16(outimage));
	outimage = thresholding_16(image, 33150);
	SavePNMImage(outimage, "F://fyp//pgm//TMJ-A_Open_Res//TMJ-A_Open 0083_33150.pgm");
	outimage = thresholding_16(image, 33100);
	SavePNMImage(outimage, "F://fyp//pgm//TMJ-A_Open_Res//TMJ-A_Open 0083_33100.pgm");
	outimage = thresholding_16(image, 33000);
	SavePNMImage(outimage, "F://fyp//pgm//TMJ-A_Open_Res//TMJ-A_Open 0083_33000.pgm");
	outimage = thresholding_16(image, 32900);
	SavePNMImage(outimage, "F://fyp//pgm//TMJ-A_Open_Res//TMJ-A_Open 0083_32900.pgm");
	outimage = thresholding_16(image, 32850);
	SavePNMImage(outimage, "F://fyp//pgm//TMJ-A_Open_Res//TMJ-A_Open 0083_32800.pgm");
	*/

	/*
	outimage = AvgImage(image, 5);
	SavePNMImage(outimage, "D://DIP//DIP//Lab10//Results//large_septagon_gaussian_noise_mean_0_std_50_added_avg5.pgm");
	*/
	return(0);
}
/*
void testshow(char * filename){
	IplImage *image = cvLoadImage(filename,-1);
	cvShowImage("TEST-IMAGE", image);
	cvWaitKey(0);
}
*/
Image8 *Convert_16_8(Image* image){
	unsigned short *tempin;
	unsigned char *tempout;
	int i, size;
	Image8 *outimage;

	outimage = CreateNewImage8(image, "#testing convert");
	tempin = image->data;
	tempout = outimage->data;
	if (image->Type == GRAY)   size = image->Width * image->Height * 2;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;
	/*
	unsigned short *data16 = (unsigned short *)image->data;
	int *data = (int *)malloc(sizeof(int) * size / 2); // allocate memory for int data
	for (i = 0; i < size / 2; i++) {
	unsigned short pixel = data16[i];
	int value = ((int)(pixel & 0xFF) << 8) | (int)(pixel >> 8); // shift low 8 bits to the left and combine with high 8 bits
	data[i] = value;
	}
	*/

	int *data = (int *)malloc(sizeof(int) * size / 2); // allocate memory for int data
	unsigned short *data16 = (unsigned short *)image->data;
	
	//*tempout = _16bit_convert_8bit(*(data16), sizeof(*data16)/sizeof(unsigned short));
	
	// try use norm function to map from 16bit to 8bit
	// get largest pixel value and smallest
	int max = (int)data16[0];
	int min = (int)data16[0];
	for (i = 0; i < size / 2; i++){
		if ((int)data[i] > max){
			max = (int)data[i];
			printf("bigger than max");
		}
		if ((int)data[i] < min){
			min = (int)data[i];
		}
	}
	printf("max:%d\nmin:%d\n",max,min);
	min = 0;
	// now we get max and min value of this 16bit image data
	// for each data16[i], norm it to (0,255)
	for (i = 0; i<size / 2; i++)
	{
		data[i] = (int)data16[i];
		float temp = data[i];
		*(tempout + i) = (int)(map(temp,(float)min,(float)max,0.0,255.0));

		//printf("%d\t", *tempout);
		//*tempout = *tempin;

		//tempin++;
		//data16++;
		//tempout++;
	}
	
	//printf("%d",max);
	return outimage;
}
Image8 *CreateNewImage8(Image * image, char *comment)// input 16bit image, output 8 bit.
{
	Image8 *outimage;
	int size, j;

	outimage = (Image8 *)malloc(sizeof(Image8));

	outimage->Type = image->Type;
	if (outimage->Type == GRAY)   size = image->Width * image->Height;
	else if (outimage->Type == COLOR) size = image->Width * image->Height * 3;

	outimage->Width = image->Width;
	outimage->Height = image->Height;
	outimage->num_comment_lines = image->num_comment_lines;

	/*--------------------------------------------------------*/
	/* Copy Comments for Original Image      */
	for (j = 0; j<outimage->num_comment_lines; j++)
		strcpy(outimage->comments[j], image->comments[j]);

	/*----------- Add New Comment  ---------------------------*/
	strcpy(outimage->comments[outimage->num_comment_lines], comment);
	outimage->num_comment_lines++;


	outimage->data = (unsigned char *)malloc(size);
	if (!outimage->data){
		printf("cannot allocate memory for new image");
		exit(0);
	}
	return(outimage);
}

Image8 *thresholding_16_8(Image* image, int th){// input 16bit image, output 8 bit.
	unsigned short *tempin;
	unsigned char *tempout;
	int i, size;
	Image8 *outimage;

	outimage = CreateNewImage8(image, "#testing threshold");
	tempin = image->data;
	tempout = outimage->data;
	if (image->Type == GRAY)   size = image->Width * image->Height * 2;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;
	/*
	unsigned short *data16 = (unsigned short *)image->data;
	int *data = (int *)malloc(sizeof(int) * size / 2); // allocate memory for int data
	for (i = 0; i < size / 2; i++) {
	unsigned short pixel = data16[i];
	int value = ((int)(pixel & 0xFF) << 8) | (int)(pixel >> 8); // shift low 8 bits to the left and combine with high 8 bits
	data[i] = value;
	}
	*/

	int *data = (int *)malloc(sizeof(int) * size / 2); // allocate memory for int data
	unsigned short *data16 = (unsigned short *)image->data;

	int max = 31000;

	for (i = 0; i<size / 2; i++)
	{
		data[i] = (int)data16[i];
		//printf("%d ", data[i]);
		if (i == 35057){
			//printf("value:%d", (int)(*tempin));
		}
		if (data[i]>max){
			max = data[i];
		}
		//printf("value: %d\t", (*tempin));

		*(tempout + i) = ((int)(*(data16 + i))>th) ? (unsigned char)(255) : (unsigned char)(0);

		//printf("%d\t", *tempout);
		//*tempout = *tempin;

		//tempin++;
		//data16++;
		//tempout++;
	}
	//printf("%d",max);
	return outimage;
}

Image *removeLowerPartMask(Image* image, double a, double b, double c){
	unsigned short *tempin, *tempout, *temp;
	int i, j, size;
	Image *outimage;

	outimage = CreateNewImage(image, "#testing threshold");
	tempin = image->data;
	temp = image->data;
	tempout = outimage->data;
	if (image->Type == GRAY)   size = image->Width * image->Height * 2;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;
	for (i = 0; i < image->Height; i++) {
		for (j = 0; j < image->Width; j++) {
			if (i >= (a * (j - b) * (j - b) + c)) {
				//temp[i * image->Width + j] = 31768;
				tempout[i * image->Width + j] = 0;
			}
			else {
				//temp[i * image->Width + j] = tempin[i * image->Width + j];
				tempout[i * image->Width + j] = 31768;
			}
			//*(tempout + i) = ((int)(*(data16 + i)) > 34220) ? (unsigned short)(65535) : (unsigned short)(0);
		}
	}
	for (i = 0; i < size / 2; i++)
	{
		if (i == 35057){
			//printf("value:%d", (int)(*tempin));
		}
		//*(tempout + i) = *(temp + i);

	}
	return outimage;
}
Image *removeLowerPart(Image* image, double a, double b, double c){
	unsigned short *tempin, *tempout, *temp;
	int i, j,size;
	Image *outimage;

	outimage = CreateNewImage(image, "#testing threshold");
	tempin = image->data;
	temp = image->data;
	tempout = outimage->data;
	if (image->Type == GRAY)   size = image->Width * image->Height * 2;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;
	for (i = 0; i < image->Height; i++) {
		for (j = 0; j < image->Width; j++) {
			if (i >= (a * (j - b) * (j - b) + c)) {
				temp[i * image->Width + j] = 31768;
				//tempout[i * image->Width + j] = 0;
			}
			else {
				temp[i * image->Width + j] = tempin[i * image->Width + j];
				//tempout[i * image->Width + j] = 31768;
			}
			//*(tempout + i) = ((int)(*(data16 + i)) > 34220) ? (unsigned short)(65535) : (unsigned short)(0);
		}
	}
	for (i = 0; i<size / 2; i++)
	{
		if (i == 35057){
			//printf("value:%d", (int)(*tempin));
		}
		*(tempout + i) = *(temp + i);

	}
	return outimage;
}
Image *thresholding_16(Image* image, int th){
	unsigned short *tempin, *tempout;
	int i, size;
	Image *outimage;

	outimage = CreateNewImage(image, "#testing threshold");
	tempin = image->data;
	tempout = outimage->data;
	if (image->Type == GRAY)   size = image->Width * image->Height*2;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;
	/*
	unsigned short *data16 = (unsigned short *)image->data;
	int *data = (int *)malloc(sizeof(int) * size / 2); // allocate memory for int data
	for (i = 0; i < size / 2; i++) {
		unsigned short pixel = data16[i];
		int value = ((int)(pixel & 0xFF) << 8) | (int)(pixel >> 8); // shift low 8 bits to the left and combine with high 8 bits
		data[i] = value;
	}
	*/

	int *data = (int *)malloc(sizeof(int) * size / 2); // allocate memory for int data
	unsigned short *data16 = (unsigned short *)image->data;

	int max = 31000;

	for (i = 0; i<size/2; i++)
	{
		data[i] = (int)data16[i];
		//printf("%d ", data[i]);
		if (i == 35057){
			//printf("value:%d", (int)(*tempin));
		}
		if (data[i]>max){
			max = data[i];
		}
		//printf("value: %d\t", (*tempin));

		*(tempout + i) = ((int)(*(data16 + i))>th) ? (unsigned short)(*(data16 + i)) : (unsigned short)(31768);
		
		//printf("%d\t", *tempout);
		//*tempout = *tempin;

		//tempin++;
		//data16++;
		//tempout++;
	}
	//printf("%d",max);
	return outimage;
}
int otsuThresholding1_16(Image* image){

	unsigned short *tempin;
	tempin = image->data;
	int i, j, size, threshold = 0;
	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;
	int Width = image->Width;
	int Height = image->Height;
	int pixelCount[65536];// here is the number of pixel values 2^n
	float pixelPro[65536];
	for (i = 0; i < 65536; i++){
		pixelCount[i] = 0;
		pixelPro[i] = 0;
	}
	for (i = 0; i < Height; i++)
	{
		for (j = 0; j < Width; j++)
		{
			pixelCount[(int)tempin[i * Width + j]]++;
		}
	}
	for (i = 0; i < 65536; i++)
	{
		pixelPro[i] = (float)pixelCount[i] / size;
	}
	float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
	for (i = 0; i < 65536; i++)
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
		for (j = 0; j < 65536; j++)
		{
			if (j <= i)   // background part
			{
				w0 += pixelPro[j];
				u0tmp += j * pixelPro[j];
			}
			else   // foreground part
			{
				w1 += pixelPro[j];
				u1tmp += j * pixelPro[j];
			}
		}
		u0 = u0tmp / w0;
		u1 = u1tmp / w1;
		u = u0tmp + u1tmp;
		//deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
		deltaTmp = w0 * w1 *pow(u0 - u1, 2);
		if (deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			threshold = i;
		}
	}
	printf("threshold16bit :%d", threshold);
	return threshold;
}



int PlusDilationUnion2(int* visited, unsigned short *tempin, int Width, int Height, int row, int col, int index, unsigned short *tempout,unsigned short *tempout1){
	int countNo = 1;
	int neighbour[8][2] = { { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 1, 0 }, { 1, 1 } };
	visited[row * Width + col] = index;
	for (int j = 0; j < 8; j++){
		int m = row + neighbour[j][0];
		int n = col + neighbour[j][1];
		int pos = m*Width + n;
		//printf("%d %d\t", m, n);
		if ((m >= 0) && (m<Width) && (n >= 0) && (n < Height) && (tempin[pos] == 0) && (visited[pos] == 0) && (row >= 0) && (col >= 0) && (tempout[pos] != 255)){
			tempout1[pos] = (unsigned short)(255);
			printf("m:%d n:%d\n",m,n);
			countNo += PlusDilationUnion2(visited, tempin, Width, Height, m, n, index,tempout,tempout1);

		}
	}
	return countNo;
}
/*
// check connect
int* visited = (int*)malloc(size*sizeof(int));
for (int i = 0; i < size; i++){
	visited[i] = 0;
}
int id = 0;
for (int i = 0; i < image->Width; i++) {
	for (int j = 0; j < image->Height; j++) {
		int pos = i * image->Width + j;
		if (tempseed[pos] == 255 && visited[pos] == 0) {
			id++;
			visited[pos] = id;
			//printf("i,j:%d %d\t", i, j);
			int countNo = PlusDilationUnion2(visited, tempseed, image->Width, image->Height, i, j, id, tempout, tempout1);
			printf(" %d  %d \n", id, countNo);
		}
	}
}
printf("haha");
*/
Image *regionGrowing(Image* image){
	Image *seedimage =thresholding(image, 254);
	unsigned short* tempseed, *tempin,*tempout,*tempout1;
	tempin = image->data;
	tempseed = seedimage->data;
	Image* outimage;
	outimage = CreateNewImage(image, "#testing Swap");
	tempout = outimage->data;
	Image* outimage1;
	outimage1 = CreateNewImage(image, "#testing Swap");
	tempout1 = outimage1->data;
	int i,size;
	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;
	for (i = 0; i < size; i++){
		*(tempout+i) = *(tempseed+i) - *(tempin+i);
	}
	for (i = 0; i<size; i++)
	{
		*(tempout+i) = ((int)(*(tempout+i))<68) ? (unsigned short)(0) : (unsigned short)(255);
		*(tempout1 + i) = *(tempseed + i);
	}
	for (i = 0; i < size; i++){
		if (*(tempseed + i) == 255){
			*(tempout1 + i) = 255;
		}
		else{
			*(tempout1 + i) = 0;
		}
	}
	int changes = 1;
	while (changes > 0){
		changes = 0;
		for (i = 0; i < size; i++){
			if (*(tempout + i) != 0 && *(tempout1 + i) != 255){
				if (i - image->Width >= 0 && *(tempout1 + i - image->Width) == 255){
					*(tempout1 + i) = 255;
					changes++;
				}
				if (i + image->Width < size && *(tempout1 + i + image->Width) == 255){
					*(tempout1 + i) = 255;
					changes++;
				}
				if (i%image->Width != 0 && *(tempout1 + i - 1) == 255){
					*(tempout1 + i) = 255;
					changes++;
				}
				if (i%image->Width != image->Width - 1 && *(tempout1 + i + 1) == 255){
					*(tempout1 + i) = 255;
					changes++;
				}
			}
		}
	}
	/*
	//remove edge
	for (i = 0; i < size; i++) {
		if (*(tempout1 + i) == 0) {
			if (i - image->Width < 0 || *(tempout1 + i - image->Width) == 0 || i + image->Width >= size || *(tempout1 + i + image->Width) == 0 || i % image->Width == 0 || *(tempout1 + i - 1) == 0 || i % image->Width == image->Width - 1 || *(tempout1 + i + 1) == 0) {
				*(tempout1 + i) = 255;
			}
		}
	}
	*/
	for (i = 0; i < size; i++) {
		if (*(tempout1 + i) == 0) {
			if (i - image->Width < 0 || *(tempout1 + i - image->Width) == 0 || i + image->Width >= size || *(tempout1 + i + image->Width) == 0 || i % image->Width == 0 || *(tempout1 + i - 1) == 0 || i % image->Width == image->Width - 1 || *(tempout1 + i + 1) == 0) {
				*(tempout1 + i) = 255;
			}
			else {
				*(tempout1 + i) = 0;
			}
		}
	}
	for (i = 0; i < size; i++) {
		if (*(tempout1 + i) == 0){
			*(tempout1 + i) = 255;
		}
		else{
			*(tempout1 + i) = *(tempseed+i);
		}
	}
	return outimage1;
}
Image* movingAverage(Image* image) {
	unsigned short* tempin, *tempout;
	int i, j, size, n = 1, sum = 0, k = 0;
	double b = 0.2;
	int width = image->Width, height = image->Height;
	unsigned short* T = (unsigned short*)malloc(width * height * sizeof(unsigned short));
	Image* outimage;
	outimage = CreateNewImage(image, "#testing Swap");
	tempin = image->data;
	tempout = outimage->data;

	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;

	for (i = 0; i < height; i++)
	{
		if (k == 0)
		{
			for (j = 0; j < width; j++) {
				sum += tempin[i * width + j];
				T[i * width + j] = b * (sum / n);

				n++;
				//printf("%d\n", n);
				if (tempin[i * width + j] > T[i * width + j]) {
					tempout[i * width + j] = 255;
				}
				else {
					tempout[i * width + j] = 0;
				}

			}
			k = 1;
		}
		else
		{
			for (j = width - 1; j >= 0; j--) {
				sum += tempin[i * width + j];
				T[i * width + j] = b * (sum / n);
				n++;
				//printf("%d\n", n);
				if (tempin[i * width + j] > T[i * width + j]) {
					tempout[i * width + j] = 255;
				}
				else {
					tempout[i * width + j] = 0;
				}
			}
			k = 0;
		}
	}
	return outimage;
}

Image *PartitionOtsuThresholding(Image* image){

	unsigned short *tempin,*tempout;
	tempin = image->data;
	int Width = image->Width;
	int Height = image->Height;
	int partWidth = Width / 3;
	int partHeight = Height / 2;
	int m, n, k,t,i,j,size;
	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;
	int threshold[7] = { 0 };// index = m+3*n
	int pixelCount[256];// here is the number of pixel values 2^n
	float pixelPro[256];
	for (m = 0; m < 3; m++){
		for (n = 0; n < 2; n++){
			for (i = 0; i < 256; i++){
				pixelCount[i] = 0;
				pixelPro[i] = 0;
			}
			// histogram of this part(m,n)
			i = n*partHeight;
			j = m*partWidth;
			printf("%d %d %d %d\n", m, n, j, i);
			for (k = 0; k < partHeight; k++){
				for (t = 0; t < partWidth; t++){
					pixelCount[(int)(*(tempin + Width*(i + k) + (j + t)))]++;
				}
			}
			
			for (i = 0; i < 256; i++)
			{
				pixelPro[i] = (float)pixelCount[i] / partWidth*partHeight;
				/*
				printf("%d:", i);
				for (j = 0; j < pixelPro[i]; j++)
					printf("*");
				printf("\n");
				*/
			}
			float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
			for (i = 0; i < 256; i++)
			{
				w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
				for (j = 0; j < 256; j++)
				{
					if (j <= i)   // background part
					{
						w0 += pixelPro[j];
						u0tmp += j * pixelPro[j];
					}
					else   // foreground part
					{
						w1 += pixelPro[j];
						u1tmp += j * pixelPro[j];
					}
				}
				u0 = u0tmp / w0;
				u1 = u1tmp / w1;
				u = u0tmp + u1tmp;
				//deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
				deltaTmp = w0 * w1 *pow(u0 - u1, 2);
				if (deltaTmp > deltaMax)
				{
					deltaMax = deltaTmp;
					threshold[m+3*n] = i;
				}
			}
			printf("%d\t", threshold[m + 3 * n]);
		}
	}
	Image *outimage;
	outimage = CreateNewImage(image, "#testing partition thresholding");
	tempout = outimage->data;
	for (m = 0; m < 3; m++){
		for (n = 0; n < 2; n++){
			i = n*partHeight;
			j = m*partWidth;
			for (k = 0; k < partHeight; k++){
				for (t = 0; t < partWidth; t++){
					//*(tempout + Width*(i + k) + (j + t)) = ((int)(*(tempin + Width*(i + k) + (j + t)))>threshold[m + 3 * n]) ? (unsigned short)(255) : (unsigned short)(0);
					*(tempout + Width*(i + k) + (j + t)) = threshold[m + 3 * n];
				}
			}
		}
	}
	return outimage;
}
int otsuThresholding1(Image* image){

	unsigned short *tempin;
	tempin = image->data;
	int i, j, size, threshold = 0;
	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;
	int Width = image->Width;
	int Height = image->Height;
	int pixelCount[256];// here is the number of pixel values 2^n
	float pixelPro[256];
	for (i = 0; i < 256; i++){
		pixelCount[i] = 0;
		pixelPro[i] = 0;
	}
	for (i = 0; i < Height; i++)
	{
		for (j = 0; j < Width; j++)
		{
			pixelCount[(int)tempin[i * Width + j]]++;
		}
	}
	for (i = 0; i < 256; i++)
	{
		pixelPro[i] = (float)pixelCount[i] / size;
	}
	float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
	for (i = 0; i < 256; i++)
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
		for (j = 0; j < 256; j++)
		{
			if (j <= i)   // background part
			{
				w0 += pixelPro[j];
				u0tmp += j * pixelPro[j];
			}
			else   // foreground part
			{
				w1 += pixelPro[j];
				u1tmp += j * pixelPro[j];
			}
		}
		u0 = u0tmp / w0;
		u1 = u1tmp / w1;
		u = u0tmp + u1tmp;
		//deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
		deltaTmp = w0 * w1 *pow(u0 - u1, 2);
		if (deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			threshold = i;
		}
	}
	printf("threshold1:%d", threshold);
	return threshold;
}
int otsuThresholding(Image* image){
	
	unsigned short *tempin;
	tempin = image->data;
	int i,j, size,threshold=0;
	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;
	int Width = image->Width;
	int Height = image->Height;
	int pixelCount[256];// here is the number of pixel values 2^n
	float pixelPro[256];
	for (i = 0; i < 256; i++){
		pixelCount[i] = 0;
		pixelPro[i] = 0;
	}
	for (i = 0; i < Height; i++)
	{
		for (j = 0; j < Width; j++)
		{
			pixelCount[(int)tempin[i * Width + j]]++;
		}
	}
	for (i = 0; i < 256; i++)
	{
		pixelPro[i] = (float)pixelCount[i] / size;
	}
	float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
	for (i = 0; i < 256; i++)
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
		for (j = 0; j < 256; j++)
		{
			if (j <= i)   // background part
			{
				w0 += pixelPro[j];
				u0tmp += j * pixelPro[j];
			}
			else   // foreground part
			{
				w1 += pixelPro[j];
				u1tmp += j * pixelPro[j];
			}
		}
		u0 = u0tmp / w0;
		u1 = u1tmp / w1;
		u = u0tmp + u1tmp;
		deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
		if (deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			threshold = i;
		}
	}
	printf("threshold:%d", threshold);
	return threshold;
}

Image *GlobalThresholding(Image* image, double t0){
	unsigned short *tempin, *tempout;
	int i, size;


	Image *outimage;

	outimage = CreateNewImage(image, "#testing global thresholding");
	tempin = image->data;
	tempout = outimage->data;


	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;

	int Width = image->Width;
	int Height = image->Height;

	int max = 0;
	max = *tempin;
	int min = 0;
	min = *tempin;
	
	for (i = 0; i<size; i++)
	{
		if (*(tempin+i) > max){
			max = *(tempin+i);
		}
		if (*(tempin+i) < min){
			min = *(tempin+i);
		}
	}
	int t = 0;
	t = (max + min) / 2;
	int mean1 = 0;
	int mean2 = 0;
	int cmean1 = 0;
	int cmean2 = 0;
	printf("ori%d", t);
	while (t>=t0){
		mean1 = 0;
		mean2 = 0;
		cmean1 = 0;
		cmean2 = 0;
		for (i = 0; i<size; i++)
		{
			if (*(tempin+i) >= t){
				mean1 += *(tempin+i);
				cmean1++;
			}
			if (*(tempin+i) < t){
				mean2 += *(tempin+i);
				cmean2++;
			}
		}
		
		mean1 = mean1/cmean1;
		mean2 = mean2/cmean2;
		t = (mean1 + mean2)/2;
		printf("%d", t);
		
	}
	printf("res%d", t);
	for (i = 0; i<size; i++)
	{
		*tempout = 0;
		if (*(tempin+i) > t){
			*tempout = *(tempin+i);
		}
		tempout++;
	}
	


	return(outimage);
}

Image *CannyEdgeDetection(Image* image, double th1, double th2){
	image = GaussianImage(image, 5, 1);
	// do sobel operator to calculate gradient
	unsigned short* tempin, *tempout;

	int i, j, k, t, v, size;
	Image* outimage;
	outimage = (Image*)malloc(sizeof(Image));
	int inputW = image->Width, inputH = image->Height;
	// get the result image size, size of canvas
	int outputW = inputW, outputH = inputH;
	outimage->Width = outputW;
	outimage->Height = outputH;
	outimage->Type = image->Type;

	if (outimage->Type == GRAY)
		size = outimage->Width * outimage->Height;
	else if (outimage->Type == COLOR)
		size = outimage->Width * outimage->Height * 3;

	outimage->data = (unsigned short*)malloc(size);
	tempin = image->data;
	tempout = outimage->data;


	int filter_size = 3 * 3;
	int SobX[9] = { -1, 2, -1, 0, 0, 0, 1, 2, 1 };// horizontal
	int SobY[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };// vertical 90 degree
	int Sob45[9] = { -2, -1, 0, -1, 0, 1, 0, 1, 2 };//45 degree
	int Sob135[9] = { 0, 1, 2, -1, 0, 1, -2, -1, 0 };// 135 degree
	int gx, gy,g45,g135;
	double result;
	double magnitude;
	// define the temp filter to save a f*f filter data
	unsigned short* tempfilter;

	tempfilter = (unsigned short*)malloc(3 * 3 * sizeof(unsigned short));
	double* tempangle;
	tempangle = (double*)malloc(size*sizeof(double));
	double* tempangle1;
	tempangle1 = (double*)malloc(size*sizeof(double));
	double* tempangle2;
	tempangle2 = (double*)malloc(size*sizeof(double));
	for (i = 1; i < outputH - 1; ++i){
		for (j = 1; j < outputW - 1; ++j){
			v = 0;
			result = 0;
			magnitude = 0;
			gx = 0;
			gy = 0;
			g45 = 0;
			g135 = 0;
			// put each original pixel into this 3*3 tempfilter
			// go over the 3*3 filter using two loops
			for (k = -1; k < 2; ++k){
				for (t = -1; t < 2; ++t){
					*(tempfilter + v) = *(tempin + inputW*(i + k) + (j + t));
					v++;
				}
			}
			// operate the sobelX operator to this tempfilter
			for (v = 0; v < 9; v++){
				gx = (int)(*(tempfilter + v))*(int)SobX[v] + gx;
			}
			// operate the sobelY operator to this tempfilter
			for (v = 0; v < 9; v++){
				gy = (int)(*(tempfilter + v))*(int)SobY[v] + gy;
			}
			for (v = 0; v < 9; v++){
				g45 = (int)(*(tempfilter + v))*(int)Sob45[v] + g45;
			}
			for (v = 0; v < 9; v++){
				g135 = (int)(*(tempfilter + v))*(int)Sob135[v] + g135;
			}
			//result
			magnitude = sqrt((double)(gx*gx) + (double)(gy*gy));
			
			result = atan2(gx, gy) / PI*2.0*90.0;// here the result is in degree unit
			if ((result>=-22.5&&result<22.5) || (result>=157.5&&result < -157.5)){
				// horizontal
				if (magnitude > g45 && magnitude > g135){
					tempout[inputW*i + j] = (int)(magnitude + 0.5);
				}
				else{
					tempout[inputW*i + j] = 0;
				}
			}
			else if ((result >= 22.5&&result<67.5) || (result<-112.5&&result >= -157.5)){
				//fall in 135 edge
				if (magnitude > gx && magnitude > gy){
					tempout[inputW*i + j] = (int)(magnitude + 0.5);
				}
				else{
					tempout[inputW*i + j] = 0;
				}
			}
			else if ((result >= 67.5&&result < 112.5) || (result >= -112.5&&result < -67.5)){
				// fall in vertical 
				if (magnitude > g45 && magnitude > g135){
					tempout[inputW*i + j] = (int)(magnitude + 0.5);
				}
				else{
					tempout[inputW*i + j] = 0;
				}
			}
			else{// -67.5 <= result < -22.5 || 112.5<= result < 157.5
				// fall in 45 edge
				if (magnitude > gx && magnitude > gy){
					tempout[inputW*i + j] = (int)(magnitude + 0.5);
				}
				else{
					tempout[inputW*i + j] = 0;
				}
			}	
		}
	}
	
	// here do double thresholding
	for (i = 1; i < outputH - 1; ++i){
		for (j = 1; j < outputW - 1; ++j){
			v = 0;
			if (tempout[inputW*i + j] < th2){
				tempout[inputW*i + j] = 0;
			}
			
			if (tempout[inputW*i + j] >= th2 && tempout[inputW*i + j] < th1){
				// check if this pixel neighbour only have one connected heavy edge pixel.
				for (k = -1; k < 2; ++k){
					for (t = -1; t < 2; ++t){
						
						if (tempout[inputW*(i + k) + j + t] >= th1){
							v++;// counter plus 1
							
						}
					}
				}
				if (v != 1){// if neighbours have more than 1 or no connected heavy pixels
					tempout[inputW*i + j] = 0;
					printf("%d\t", v);
				}
				else{
					printf("exist this kind of pixel:%d,%d\n",i,j);
				}
			}
			
			// another case is pixel value > th2, default not change, save it
		}
	}
	
	return (outimage);
}

Image *LoGImage(Image* image,  int ksize, double sigma, double th){
	unsigned short* tempin, *tempout;
	int i, j, k, t, v, size;
	Image* outimage;
	outimage = (Image*)malloc(sizeof(Image));
	int inputW = image->Width, inputH = image->Height;
	// get the result image size, size of canvas
	int outputW = inputW, outputH = inputH;
	outimage->Width = outputW;
	outimage->Height = outputH;
	outimage->Type = image->Type;

	if (outimage->Type == GRAY)
		size = outimage->Width * outimage->Height;
	else if (outimage->Type == COLOR)
		size = outimage->Width * outimage->Height * 3;

	outimage->data = (unsigned short*)malloc(size);
	tempin = image->data;
	tempout = outimage->data;


	int filter_size = ksize * ksize;
	double *window = GaussianLaplacianGenerator(ksize, sigma);
	int gx;
	double result;
	int weight = 0;
	for (i = 0; i < ksize*ksize; i++){
		weight += (window[i]);
		//printf("%f\n", (window[i]));
		//printf("%d\n", (int)(window[i]+0.1));
	}
	printf("%d\n", weight);
	// define the temp filter to save a f*f filter data
	unsigned short* tempfilter;

	tempfilter = (unsigned short*)malloc(ksize*ksize * sizeof(unsigned short));

	for (i = (ksize - 1) / 2; i < outputH - (ksize - 1) / 2; ++i){
		for (j = (ksize - 1) / 2; j < outputW - (ksize - 1) / 2; ++j){
			v = 0;
			result = 0;
			gx = 0;
			// put each original pixel into this 3*3 tempfilter
			// go over the 3*3 filter using two loops
			for (k = -(ksize - 1) / 2; k < (ksize + 1) / 2; ++k){
				for (t = -(ksize - 1) / 2; t < (ksize + 1) / 2; ++t){
					*(tempfilter + v) = *(tempin + inputW*(i + k) + (j + t));
					v++;
				}
			}
			// operate the sobelX operator to this tempfilter
			for (v = 0; v < ksize*ksize; v++){
				gx = (int)(*(tempfilter + v))*(int)window[v] + gx;
			}

			//result
			result = (double)(gx / weight);
			if (result > th){
				tempout[inputW*i + j] = (int)(result + 0.5);
			}
			else{
				tempout[inputW*i + j] = 0;
			}
			
		}
	}
	return (outimage);
}
double *GaussianLaplacianGenerator(int ksize, double sigma){
	// generate filter
	double *window = (double*)malloc(11 * 11 * sizeof(double));
	int center = ksize / 2; // 模板的中心位置，也就是坐标的原点
	double x2, y2;
	for (int i = 0; i < ksize; i++)
	{
		x2 = pow(i - center, 2);
		for (int j = 0; j < ksize; j++)
		{
			y2 = pow(j - center, 2);
			double g = ((x2 + y2) / (2 * sigma * sigma) - 1)* exp(-(x2 + y2) / (2 * sigma * sigma));
			g /= (PI * sigma* sigma* sigma* sigma);
			window[i*ksize+j] = g;
		}
	}
	double k = 1 / window[0]; // 将左上角的系数归一化为1
	for (int i = 0; i < ksize; i++)
	{
		for (int j = 0; j < ksize; j++)
		{
			window[i*ksize+j] *= k;
			//printf("%f\t", window[i*ksize+j]);
		}
	}
	return window;
}
double *generateGaussianTemplate( int ksize, double sigma){
	double *window = (double*)malloc(11 * 11 * sizeof(double));
	int center = ksize / 2; // 模板的中心位置，也就是坐标的原点
	double x2, y2;
	for (int i = 0; i < ksize; i++)
	{
		x2 = pow(i - center, 2);
		for (int j = 0; j < ksize; j++)
		{
			y2 = pow(j - center, 2);
			double g = exp(-(x2 + y2) / (2 * sigma * sigma));
			g /= 2 * PI * sigma;
			window[i*ksize+j] = g;
		}
	}
	double k = 1 / window[0]; // 将左上角的系数归一化为1
	for (int i = 0; i < ksize; i++)
	{
		for (int j = 0; j < ksize; j++)
		{
			window[i*ksize+j] *= k;
		}
	}
	return window;
}

Image *GaussianImage(Image* image, int ksize, double sigma){
	unsigned short* tempin, *tempout;

	int i, j, k, t, v, size;
	Image* outimage;
	outimage = (Image*)malloc(sizeof(Image));
	int inputW = image->Width, inputH = image->Height;
	// get the result image size, size of canvas
	int outputW = inputW, outputH = inputH;
	outimage->Width = outputW;
	outimage->Height = outputH;
	outimage->Type = image->Type;

	if (outimage->Type == GRAY)
		size = outimage->Width * outimage->Height;
	else if (outimage->Type == COLOR)
		size = outimage->Width * outimage->Height * 3;

	outimage->data = (unsigned short*)malloc(size*sizeof(unsigned short));
	tempin = image->data;
	tempout = outimage->data;


	int filter_size = ksize * ksize;
	double *window = generateGaussianTemplate(ksize, sigma);
	int gx;
	double result;
	int weight = 0;
	for (i = 0; i < ksize*ksize; i++){
		weight += (window[i]);
		//printf("%f\n", (window[i]));
		//printf("%d\n", (int)(window[i]+0.1));
	}
	printf("this is weight %d \n", weight);
	// define the temp filter to save a f*f filter data
	unsigned short* tempfilter;

	tempfilter = (unsigned short*)malloc(ksize*ksize * sizeof(unsigned short));

	for (i = (ksize - 1) / 2; i < outputH - (ksize - 1) / 2; ++i){
		for (j = (ksize - 1) / 2; j < outputW - (ksize - 1) / 2; ++j){
			//printf("%d.%d\t",i,j);
			v = 0;
			result = 0;
			gx = 0;
			// put each original pixel into this 3*3 tempfilter
			// go over the 3*3 filter using two loops
			for (k = -(ksize-1)/2; k < (ksize+1)/2; ++k){
				for (t = -(ksize - 1) / 2; t < (ksize + 1) / 2; ++t){
					*(tempfilter + v) = *(tempin + inputW*(i + k) + (j + t));
					v++;
				}
			}
			// operate the gaussian operator to this tempfilter
			for (v = 0; v < ksize*ksize; v++){
				gx = (int)(*(tempfilter + v))*(int)window[v] + gx;
			}
			
			//result
			result = (double)(gx/ weight);
			tempout[inputW*i + j] = (unsigned short)((int)(result + 0.5));
		}
	}
	//printf("this is test\n");
	return (outimage);
}
Image *RobertGradientEdgeDetection(Image* image){
	unsigned short* tempin, *tempout;

	int i, j, size;
	Image* outimage;
	outimage = (Image*)malloc(sizeof(Image));
	int inputW = image->Width, inputH = image->Height;
	// get the result image size, size of canvas
	int outputW = inputW, outputH = inputH;
	outimage->Width = outputW;
	outimage->Height = outputH;
	outimage->Type = image->Type;

	if (outimage->Type == GRAY)
		size = outimage->Width * outimage->Height;
	else if (outimage->Type == COLOR)
		size = outimage->Width * outimage->Height * 3;

	outimage->data = (unsigned short*)malloc(size);
	tempin = image->data;
	tempout = outimage->data;

	double result;
	
	for (i = 0; i < outputH - 1; ++i){// i is y
		for (j = 0; j < outputW - 1; ++j){// j is x
			result = 0;
			//result
			result = sqrt(pow((*(tempin + inputW*i + j) - (*(tempin + inputW*(i + 1) + j + 1))), 2) + pow((*(tempin + inputW*(i+1) + j) - (*(tempin + inputW*i + j + 1))), 2));
			tempout[inputW*i + j] = (int)(result + 0.5);
		}
	}
	return (outimage);
}
Image *PrewittGradientEdgeDetection(Image* image, double threshold, int type){// type = 1, vertical; type=2, horizontal; type=3, magnitude;
	unsigned short* tempin, *tempout;

	int i, j, k, t, v, size;
	Image* outimage;
	outimage = (Image*)malloc(sizeof(Image));
	int inputW = image->Width, inputH = image->Height;
	// get the result image size, size of canvas
	int outputW = inputW, outputH = inputH;
	outimage->Width = outputW;
	outimage->Height = outputH;
	outimage->Type = image->Type;

	if (outimage->Type == GRAY)
		size = outimage->Width * outimage->Height;
	else if (outimage->Type == COLOR)
		size = outimage->Width * outimage->Height * 3;

	outimage->data = (unsigned short*)malloc(size);
	tempin = image->data;
	tempout = outimage->data;


	int filter_size = 3 * 3;

	int PrewX[9] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
	int PrewY[9] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };

	//int PrewX[9] = { 0, 1, 1, -1, 0, 1, -1, -1, 0 };
	//int PrewY[9] = { -1, -1, 0, -1, 0, 1, 0, 1, 1 };

	int gx, gy;
	double result;
	// define the temp filter to save a f*f filter data
	unsigned short* tempfilter;

	tempfilter = (unsigned short*)malloc(3 * 3 * sizeof(unsigned short));

	for (i = 1; i < outputH - 1; ++i){
		for (j = 1; j < outputW - 1; ++j){
			v = 0;
			result = 0;
			gx = 0;
			gy = 0;
			// put each original pixel into this 3*3 tempfilter
			// go over the 3*3 filter using two loops
			for (k = -1; k < 2; ++k){
				for (t = -1; t < 2; ++t){
					*(tempfilter + v) = *(tempin + inputW*(i + k) + (j + t));
					v++;
				}
			}
			// operate the PrewX operator to this tempfilter
			for (v = 0; v < 9; v++){
				gx = (int)(*(tempfilter + v))*(int)PrewX[v] + gx;
			}
			// operate the PrewY operator to this tempfilter
			for (v = 0; v < 9; v++){
				gy = (int)(*(tempfilter + v))*(int)PrewY[v] + gy;
			}
			//result
			if (type == 1){
				if (gx > threshold){
					//printf("%d\t",gx);
					tempout[inputW*i + j] = (int)(gx + 0.5);
				}
				else{
					tempout[inputW*i + j] = 0;
				}
			}
			else if (type == 2){
				if (gy > threshold){
					tempout[inputW*i + j] = (int)(gy + 0.5);
				}
				else{
					tempout[inputW*i + j] = 0;
				}
			}
			else if (type == 3){
				result = sqrt((double)(gx*gx) + (double)(gy*gy));
				if (result > threshold){
					tempout[inputW*i + j] = (int)(result + 0.5);
				}
				else{
					tempout[inputW*i + j] = 0;
				}
			}
			else{
				tempout[inputW*i + j] = 0;
				printf("Wrong type!\n");
			}
		}
	}
	return (outimage);


}

Image *SobelGradientEdgeDetection(Image* image, double threshold, int type){// type = 1, vertical; type=2, horizontal; type=3, magnitude;
	unsigned short* tempin, *tempout;

	int i, j, k, t, v, size;
	Image* outimage;
	outimage = (Image*)malloc(sizeof(Image));
	int inputW = image->Width, inputH = image->Height;
	// get the result image size, size of canvas
	int outputW = inputW, outputH = inputH;
	outimage->Width = outputW;
	outimage->Height = outputH;
	outimage->Type = image->Type;

	if (outimage->Type == GRAY)
		size = outimage->Width * outimage->Height;
	else if (outimage->Type == COLOR)
		size = outimage->Width * outimage->Height * 3;

	outimage->data = (unsigned short*)malloc(size);
	tempin = image->data;
	tempout = outimage->data;


	int filter_size = 3 * 3;
	
	int SobX[9] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };
	int SobY[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };

	//int SobX[9] = { -1, 2, -1, 0, 0, 0, 1, 2, 1 };
	//int SobY[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };

	int gx, gy;
	double result;
	// define the temp filter to save a f*f filter data
	unsigned short* tempfilter;

	tempfilter = (unsigned short*)malloc(3 * 3 * sizeof(unsigned short));

	for (i = 1; i < outputH - 1; ++i){
		for (j = 1; j < outputW - 1; ++j){
			v = 0;
			result = 0;
			gx = 0;
			gy = 0;
			// put each original pixel into this 3*3 tempfilter
			// go over the 3*3 filter using two loops
			for (k = -1; k < 2; ++k){
				for (t = -1; t < 2; ++t){
					*(tempfilter + v) = *(tempin + inputW*(i + k) + (j + t));
					v++;
				}
			}
			// operate the sobelX operator to this tempfilter
			for (v = 0; v < 9; v++){
				gx = (int)(*(tempfilter + v))*(int)SobX[v] + gx;
			}
			// operate the sobelY operator to this tempfilter
			for (v = 0; v < 9; v++){
				gy = (int)(*(tempfilter + v))*(int)SobY[v] + gy;
			}
			//result
			if (type == 1){
				if (gx > threshold){
					//printf("%d\t",gx);
					tempout[inputW*i + j] = (int)(gx + 0.5);
				}
				else{
					tempout[inputW*i + j] = 0;
				}	
			}
			else if (type == 2){
				if (gy > threshold){
					tempout[inputW*i + j] = (int)(gy + 0.5);
				}
				else{
					tempout[inputW*i + j] = 0;
				}
			}
			else if (type == 3){
				result = sqrt((double)(gx*gx) + (double)(gy*gy));
				if (result > threshold){
					tempout[inputW*i + j] = (int)(result + 0.5);
				}
				else{
					tempout[inputW*i + j] = 0;
				}	
			}
			else{
				tempout[inputW*i + j] = 0;
				printf("Wrong type!\n");
			}
		}
	}
	return (outimage);


}

void CheckConnect1(Image* image, char* file_path){
	FILE* w = fopen(file_path, "w");
	if (w == NULL) exit(0);
	unsigned short* tempin;
	int size;
	int inputW = image->Width, inputH = image->Height;
	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;

	tempin = image->data;
	int* visited = (int*)malloc(size*sizeof(int));
	for (int i = 0; i < size; i++){
		visited[i] = 0; 
		
	}
	int id = 0;
	//printf("value %d\t", tempin[180*inputW+311]);
	for (int i = 0; i < inputW; i++) {
		for (int j = 0; j < inputH; j++) {
			int pos = i * inputW + j;
			
			if (tempin[pos] == 255 && visited[pos] == 0) {
				id++;
				visited[pos] = id;
				//printf("i,j:%d %d\t", i, j);
				int countNo = PlusDilationUnion1(visited, tempin, inputW, inputH, i, j, id);
				printf(" %d  %d \n", id, countNo);
				fprintf(w, "%d  %d \n", id, countNo);
			}
		}
	}
	fclose(w);
}
int PlusDilationUnion1(int* visited, unsigned short *tempin, int Width, int Height, int row, int col, int index){
	int countNo = 1;
	int neighbour[8][2] = { { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, -1 }, { 0, 1 }, { 1, -1 }, { 1, 0 }, { 1, 1 }};
	visited[row * Width + col] = index;
	for (int j = 0; j <= 8; j++){
		int m = row + neighbour[j][0];
		int n = col + neighbour[j][1];
		int pos = m*Width + n;
		if (m == 0 && n == 1){
			//printf("%d %d pos\t", tempin[pos],visited[pos]);
		}
		//printf("%d %d\t", m, n);
		if ((m >= 0) && (m<Width) && (n >= 0) && (n < Height) && (tempin[pos] == 255) && (visited[pos] == 0)&&(row>=0)&&(col>=0)){
			//printf("YYY\t");
			countNo += PlusDilationUnion1(visited, tempin, Width, Height, m, n, index);
			
		}
	}

	return countNo;
}


int PlusDilationUnion(int* visited,unsigned short *tempin, int Width, int Height, int row,int col,int index){
	int countNo = 1;
	int neighbour[4][2] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };
	visited[row * Width + col] = index;
	for (int j = 0; j <= 3; j++){
		int m = row + neighbour[j][0];
		int n = col + neighbour[j][1];
		int pos = m*Width + n;
		//printf("%d %d", m,n);
		if ((m >= 0) && (m<Width) && (n >= 0) && (n < Height) && (tempin[pos] == 255) && (visited[pos] == 0)){
			countNo += PlusDilationUnion(visited, tempin, Width, Height, m,n,index);
		}
	}
	
	return countNo;
}

void CheckConnect(Image* image,char* file_path){
	FILE* w = fopen(file_path, "w");
	if (w == NULL) exit(0);
	unsigned short* tempin;
	int size;
	int inputW = image->Width, inputH = image->Height;
	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;
	
	tempin = image->data;
	int* visited = (int*)malloc(size*sizeof(int));
	for (int i = 0; i < size; i++){ visited[i] = 0; }
	int id = 0;
	for (int i = 0; i < inputW; i++) {
		for (int j = 0; j < inputH; j++) {
			int pos = i * inputW + j;
			if (tempin[pos] == 255 && visited[pos] == 0) {
				id++;
				visited[pos] = id;
				int countNo = PlusDilationUnion(visited, tempin, inputW, inputH, i, j, id);
				printf(" %d  %d \n", id, countNo);
				fprintf(w, "%d  %d \n", id, countNo);
			}
		}
	}
	fclose(w);
}

Image *BoundaryExtraction(Image* image, int f, int* pos, int p_size){
	unsigned short *tempin, *tempout, *tempEro;
	int i, size;

	Image* afterErosion;
	afterErosion = ErosionImage(image, f, pos, p_size);

	Image *outimage;

	outimage = CreateNewImage(image, "#testing Boundary Extraction");
	tempin = image->data;
	tempout = outimage->data;
	tempEro = afterErosion->data;


	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;

	int Width = image->Width;
	int Height = image->Height;


	for (i = 0; i<size; i++)
	{
		*tempout = *tempEro - *tempin;
		//printf("%c", *tempout);

		tempin++;
		tempout++;
		tempEro++;
	}


	return(outimage);

}
Image *ClosingImage(Image* image, int f1, int* pos1, int p_size1, int f2, int* pos2, int p_size2){// first maximum(XXX1) ,then minimum(XXX2)
	return ErosionImage(DilationImage(image, f1, pos1, p_size1), f2, pos2, p_size2);
}
Image *OpeningImage(Image* image, int f1, int* pos1, int p_size1, int f2, int* pos2, int p_size2){// first maximum(XXX1) ,then minimum(XXX2)
	return DilationImage(ErosionImage(image,f1,pos1,p_size1),f2,pos2, p_size2);
}
Image *ErosionImage(Image *image, int f, int* pos, int p_size){ // min value
	//printf("%d", pos[1]);
	unsigned short *tempin, *tempout;
	int i, j, k, w, size;
	int m = 1;
	int n = 0;
	int temp_for_targetShape = 0;
	unsigned short temp_exchange;
	Image *outimage;

	outimage = CreateNewImage(image, "#testing Erosion");
	tempin = image->data;
	tempout = outimage->data;


	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;

	int Width = image->Width;
	int Height = image->Height;

	int filter_size = f*f;
	// the smaller size is optional, doing avg filter would reduce the image size.
	/*
	// change outimage size
	outimage->Height = Height - f + 1;
	outimage->Width = Width - f + 1;
	*/
	// define the temp filter to save a f*f filter data
	unsigned short* tempfilter;
	tempfilter = (unsigned short*)malloc(f*f*sizeof(unsigned short));
	// move the center point to the center of first filter
	// move the original data location, change *tempin
	// the boundary remain the original pixel value
	for (i = 0; i < Width*(f - 1) / 2 + (f - 1) / 2; i++){
		*(tempout) = *(tempin);// the boundary ramain the same value
		tempin++;
		tempout++;
	}
	// data *tempin is now the center data for first filter, start loop
	for (j = Width*(f - 1) / 2 + (f - 1) / 2; j <= (Width*Height - 1) - (Width*(f - 1) / 2 + (f - 1) / 2); j++){
		if (m>Width - f + 1){// m and n is used to jump over the boundary data
			*(tempout) = *tempin;// the boundary ramain the same value
			n = n + 1;
		}
		else{
			for (k = 0; k < f*f; k++){
				//*(tempfilter + k) = *(tempin - (f - 1) / 2 - Width + (k%f) + ((int)(k / f))*Width);


				for (int poi = 0; poi < p_size; poi++){
					if (k == pos[poi]){
						*(tempfilter + temp_for_targetShape) = *(tempin - (f - 1) / 2 - Width + (k%f) + ((int)(k / f))*Width);
						temp_for_targetShape++;
					}
				}
			}
			temp_for_targetShape = 0;
			// use bubble sort to sort filter data
			for (int p = 0; p < p_size - 1; p++){
				for (int q = 0; q < p_size - 1 - p; q++){
					if (*(tempfilter + q) > *(tempfilter + q + 1)){
						temp_exchange = *(tempfilter + q);
						*(tempfilter + q) = *(tempfilter + q + 1);
						*(tempfilter + q + 1) = temp_exchange;
					}
				}
			}

			*tempout = *(tempfilter+p_size-1);// output the max data
		}
		tempout++;
		tempin++;
		m++;

		if (n == f - 1){
			m = 1;
			n = 0;
		}
	}
	// the boundary remain the original pixel value
	for (w = (Width*Height - 1) - (Width*(f - 1) / 2 + (f - 1) / 2) + 1; w <= (Width*Height - 1); w++){
		*(tempout) = *(tempin);// the boundary ramain the same value
		tempin++;
		tempout++;
	}
	return(outimage);
}
Image *DilationImage(Image *image, int f, int* pos, int p_size){ // min value
	//printf("%d", pos[1]);
	
	unsigned short *tempin, *tempout;
	int i, j, k, w, size;
	int m = 1;
	int n = 0;
	int temp_for_targetShape = 0;
	unsigned short temp_exchange;

	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;
	Image *outimage;
	outimage = (Image*)malloc(sizeof(Image));
	outimage = CreateNewImage(image, "#testing Dilation");
	tempin = image->data;
	outimage->data = (unsigned short*)malloc(size*sizeof(unsigned short));
	tempout = outimage->data;

	

	int Width = image->Width;
	int Height = image->Height;

	int filter_size = f*f;
	// define the temp filter to save a f*f filter data
	unsigned short* tempfilter;
	tempfilter = (unsigned short*)malloc(f*f*sizeof(unsigned short));
	// move the center point to the center of first filter
	// move the original data location, change *tempin
	// the boundary remain the original pixel value
	for (i = 0; i < Width*(f - 1) / 2 + (f - 1) / 2; i++){
		*(tempout) = *(tempin);// the boundary ramain the same value
		tempin++;
		tempout++;
	}
	
	// data *tempin is now the center data for first filter, start loop
	for (j = Width*(f - 1) / 2 + (f - 1) / 2; j <= (Width*Height - 1) - (Width*(f - 1) / 2 + (f - 1) / 2); j++){
		
		if (m>Width - f + 1){// m and n is used to jump over the boundary data
			*(tempout) = *tempin;// the boundary ramain the same value
			n = n + 1;
		}
		
		else{
			for (k = 0; k < f*f; k++){
				//*(tempfilter + k) = *(tempin - (f - 1) / 2 - Width + (k%f) + ((int)(k / f))*Width);
				for (int poi = 0; poi < p_size; poi++){
					if (k == pos[poi]){
						*(tempfilter + temp_for_targetShape) = *(tempin - (f - 1) / 2 - Width + (k%f) + ((int)(k / f))*Width);
						temp_for_targetShape++;
					}
				}
			}
			
			temp_for_targetShape = 0;
			// use bubble sort to sort filter data
			for (int p = 0; p < p_size - 1; p++){
				for (int q = 0; q < p_size - 1 - p; q++){
					if (*(tempfilter + q) > *(tempfilter + q + 1)){
						temp_exchange = *(tempfilter + q);
						*(tempfilter + q) = *(tempfilter + q + 1);
						*(tempfilter + q + 1) = temp_exchange;
					}
				}
			}

			*tempout = *(tempfilter);// output the median data
		}

		
		tempout++;
		tempin++;
		m++;

		if (n == f - 1){
			m = 1;
			n = 0;
		}
		//printf("%d\t", j);
	}
	printf("ending:%d\t", j);
	// the boundary remain the original pixel value ######## (Width*Height - 1) - (Width*(f - 1) / 2 + (f - 1) / 2) + 1
	for (w = j; w <= (Width*Height - 1); w++){
		*(tempout) = *(tempin);// the boundary ramain the same value
		tempin++;
		tempout++;
	}
	return(outimage);
}
DFT *Filt_Notch_Lena(DFT* inputdft, int width, int height){
	float* in_real = inputdft->data_real;
	float* in_imag = inputdft->data_imag;
	int i, j, size;
	int inputW = inputdft->Width;
	int inputH = inputdft->Height;
	float *tempout_img, *tempout_real;
	DFT *outdft;
	outdft = (DFT*)malloc(sizeof(DFT));
	outdft->Height = inputH;
	outdft->Width = inputW;
	size = inputW * inputH;
	//allocate memory for DFT daya arrays
	outdft->data_real = (float*)malloc(size * sizeof(float));
	outdft->data_imag = (float*)malloc(size * sizeof(float));
	tempout_real = outdft->data_real;
	tempout_img = outdft->data_imag;
	int in_centerX, in_centerY;
	in_centerX = inputW / 2;
	in_centerY = inputH / 2;
	//initialize
	for (int i = 0; i < inputH; i++) {
		for (int j = 0; j < inputW; j++) {
			int index = (i * inputW + j);
			*(tempout_real + index) = 0.0;
			*(tempout_img + index) = 0.0;
			//*(in_real + index) = 1.0;
		}
	}
	// add filter
	for (i = 0; i < inputW; i++){
		for (j = 0; j < inputH; j++){
			int index = (i * inputW + j);
			int out_i = i - in_centerX;
			int out_j = j - in_centerY;
			float dist = sqrt(out_i*out_i + out_j*out_j);
			*(tempout_real + index) = (float)(*(in_real + index));
			*(tempout_img + index) = (float)(*(in_imag + index));
			
			if ((j == width)&&(i<=height)){
				//printf("i;%d, j:%d\n", i, j);
				//printf("Real value ori: %f\n", *(in_real + 1 * inputW + 1));
				//printf("Imag value ori: %f\n", *(in_imag + 1 * inputW + 1));
				
				*(tempout_real + index) = (float)(*(in_real + 1 * inputW + 1));
				*(tempout_img + index) = (float)(*(in_imag + 1 * inputW + 1));
			}
			if ((j == width) && (i >= inputH - height)){
				*(tempout_real + index) = (float)(*(in_real + 1 * inputW + 1));
				*(tempout_img + index) = (float)(*(in_imag + 1 * inputW + 1));
			}
			//printf("real&imag center:%f ,%f", (*(in_real + 128 * inputW + 128), (*(in_imag + 128 * inputW + 128))));
			/*
			if (dist == rad){
			*(tempout_real + index) = (float)(*(in_real + 128*inputW+128));
			*(tempout_img + index) = (float)(*(in_imag + 128 * inputW + 128));
			}
			*/
		}
	}
	return outdft;
}
DFT *AddNoise(DFT* inputdft, float rad){
	float* in_real = inputdft->data_real;
	float* in_imag = inputdft->data_imag;
	int i, j, size;
	int inputW = inputdft->Width;
	int inputH = inputdft->Height;
	float *tempout_img, *tempout_real;
	DFT *outdft;
	outdft = (DFT*)malloc(sizeof(DFT));
	outdft->Height = inputH;
	outdft->Width = inputW;
	size = inputW * inputH;
	//allocate memory for DFT daya arrays
	outdft->data_real = (float*)malloc(size * sizeof(float));
	outdft->data_imag = (float*)malloc(size * sizeof(float));
	tempout_real = outdft->data_real;
	tempout_img = outdft->data_imag;
	int in_centerX, in_centerY;
	in_centerX = inputW / 2;
	in_centerY = inputH / 2;
	//initialize
	for (int i = 0; i < inputH; i++) {
		for (int j = 0; j < inputW; j++) {
			int index = (i * inputW + j);
			*(tempout_real + index) = 0.0;
			*(tempout_img + index) = 0.0;
			//*(in_real + index) = 1.0;
		}
	}
	// add noise
	for (i = 0; i < inputW; i++){
		for (j = 0; j < inputH; j++){
			int index = (i * inputW + j);
			int out_i = i - in_centerX;
			int out_j = j - in_centerY;
			float dist = sqrt(out_i*out_i + out_j*out_j);
			*(tempout_real + index) = (float)(*(in_real + index));
			*(tempout_img + index) = (float)(*(in_imag + index));
			
			if ((i == 128 && j == 8) || (i == 8&&j == 128)){
				*(tempout_real + index) = (float)(*(in_real + 128 * inputW + 128));
				*(tempout_img + index) = (float)(*(in_imag + 128 * inputW + 128));
			}
			//printf("real&imag center:%f ,%f", (*(in_real + 128 * inputW + 128), (*(in_imag + 128 * inputW + 128))));
			/*
			if (dist == rad){
				*(tempout_real + index) = (float)(*(in_real + 128*inputW+128));
				*(tempout_img + index) = (float)(*(in_imag + 128 * inputW + 128));
			}
			*/
		}
	}
	return outdft;
}
DFT *Filt_IBF(DFT* inputdft, float bandWidth, float cutoff){
	float half_band = bandWidth / 2.0;
	float* in_real = inputdft->data_real;
	float* in_imag = inputdft->data_imag;
	int i, j, size;
	int inputW = inputdft->Width;
	int inputH = inputdft->Height;
	float *tempout_img, *tempout_real;
	DFT *outdft;
	outdft = (DFT*)malloc(sizeof(DFT));
	outdft->Height = inputH;
	outdft->Width = inputW;
	size = inputW * inputH;
	//allocate memory for DFT daya arrays
	outdft->data_real = (float*)malloc(size * sizeof(float));
	outdft->data_imag = (float*)malloc(size * sizeof(float));
	tempout_real = outdft->data_real;
	tempout_img = outdft->data_imag;
	int in_centerX, in_centerY;
	in_centerX = inputW / 2;
	in_centerY = inputH / 2;
	//initialize
	for (int i = 0; i < inputH; i++) {
		for (int j = 0; j < inputW; j++) {
			int index = (i * inputW + j);
			*(tempout_real + index) = 0.0;
			*(tempout_img + index) = 0.0;
			//*(in_real + index) = 1.0;
		}
	}
	// filt
	for (i = 0; i < inputW; i++){
		for (j = 0; j < inputH; j++){
			int index = (i * inputW + j);
			int out_i = i - in_centerX;
			int out_j = j - in_centerY;
			float dist = sqrt(out_i*out_i + out_j*out_j);
			float H_filter;

			// this is IBF
			if (dist <= cutoff+half_band && (dist >= cutoff - half_band)){
				H_filter = 0.0;
			}
			else{
				H_filter = 1.0;
			}

			*(tempout_real + index) = (float)(H_filter * (*(in_real + index)));
			*(tempout_img + index) = (float)(H_filter * (*(in_imag + index)));
		}
	}
	return outdft;
}
DFT *Filt_BBF(DFT* inputdft, float bandWidth, float cutoff, int factor_n){
	float* in_real = inputdft->data_real;
	float* in_imag = inputdft->data_imag;
	int i, j, size;
	int inputW = inputdft->Width;
	int inputH = inputdft->Height;
	float *tempout_img, *tempout_real;
	DFT *outdft;
	outdft = (DFT*)malloc(sizeof(DFT));
	outdft->Height = inputH;
	outdft->Width = inputW;
	size = inputW * inputH;
	//allocate memory for DFT daya arrays
	outdft->data_real = (float*)malloc(size * sizeof(float));
	outdft->data_imag = (float*)malloc(size * sizeof(float));
	tempout_real = outdft->data_real;
	tempout_img = outdft->data_imag;
	int in_centerX, in_centerY;
	in_centerX = inputW / 2;
	in_centerY = inputH / 2;
	//initialize
	for (int i = 0; i < inputH; i++) {
		for (int j = 0; j < inputW; j++) {
			int index = (i * inputW + j);
			*(tempout_real + index) = 0.0;
			*(tempout_img + index) = 0.0;
			//*(in_real + index) = 1.0;
		}
	}
	// filt
	for (i = 0; i < inputW; i++){
		for (j = 0; j < inputH; j++){

			int index = (i * inputW + j);
			int out_i = i - in_centerX;
			int out_j = j - in_centerY;
			float dist = sqrt(out_i*out_i + out_j*out_j);
			float H_filter;
			// this is BBF
			H_filter = 1.0 / (1.0 + pow((dist*bandWidth / (dist*dist - cutoff*cutoff)), 2 * factor_n));
			if (dist == cutoff){
				H_filter = 1.0;
			}
			//printf("H_filter: %f\t", H_filter);
			*(tempout_real + index) = (float)(H_filter * (*(in_real + index)));
			*(tempout_img + index) = (float)(H_filter * (*(in_imag + index)));

		}
	}
	return outdft;

}
DFT *Filt_GBF(DFT* inputdft, float bandWidth, float cutoff){
	float* in_real = inputdft->data_real;
	float* in_imag = inputdft->data_imag;
	int i, j, size;
	int inputW = inputdft->Width;
	int inputH = inputdft->Height;
	float *tempout_img, *tempout_real;
	DFT *outdft;
	outdft = (DFT*)malloc(sizeof(DFT));
	outdft->Height = inputH;
	outdft->Width = inputW;
	size = inputW * inputH;
	//allocate memory for DFT daya arrays
	outdft->data_real = (float*)malloc(size * sizeof(float));
	outdft->data_imag = (float*)malloc(size * sizeof(float));
	tempout_real = outdft->data_real;
	tempout_img = outdft->data_imag;
	int in_centerX, in_centerY;
	in_centerX = inputW / 2;
	in_centerY = inputH / 2;
	//initialize
	for (int i = 0; i < inputH; i++) {
		for (int j = 0; j < inputW; j++) {
			int index = (i * inputW + j);
			*(tempout_real + index) = 0.0;
			*(tempout_img + index) = 0.0;
			//*(in_real + index) = 1.0;
		}
	}
	// filt
	for (i = 0; i < inputW; i++){
		for (j = 0; j < inputH; j++){

			int index = (i * inputW + j);
			int out_i = i - in_centerX;
			int out_j = j - in_centerY;
			float dist = sqrt(out_i*out_i + out_j*out_j);
			float H_filter;
			// this is GBF
			H_filter = pow(exp(1), -((dist*dist - cutoff*cutoff) / (dist*bandWidth)) * ((dist*dist - cutoff*cutoff) / (dist*bandWidth)));
			H_filter = 1.0 - H_filter;
			*(tempout_real + index) = (float)(H_filter * (*(in_real + index)));
			*(tempout_img + index) = (float)(H_filter * (*(in_imag + index)));

		}
	}
	return outdft;

}
DFT *Filt_Homomorphic(DFT* inputdft, float yH, float yL, int factor_c, float cutoff){
	// float temp_D = (float)(cutoff*cutoff / factor_c);
	float* in_real = inputdft->data_real;
	float* in_imag = inputdft->data_imag;
	int i, j, size;
	int inputW = inputdft->Width;
	int inputH = inputdft->Height;
	float *tempout_img, *tempout_real;
	DFT *outdft;
	outdft = (DFT*)malloc(sizeof(DFT));
	outdft->Height = inputH;
	outdft->Width = inputW;
	size = inputW * inputH;
	//allocate memory for DFT daya arrays
	outdft->data_real = (float*)malloc(size * sizeof(float));
	outdft->data_imag = (float*)malloc(size * sizeof(float));
	tempout_real = outdft->data_real;
	tempout_img = outdft->data_imag;
	int in_centerX, in_centerY;
	in_centerX = inputW / 2;
	in_centerY = inputH / 2;
	//initialize
	for (int i = 0; i < inputH; i++) {
		for (int j = 0; j < inputW; j++) {
			int index = (i * inputW + j);
			*(tempout_real + index) = 0.0;
			*(tempout_img + index) = 0.0;
			//*(in_real + index) = 1.0;
		}
	}
	// filt
	for (i = 0; i < inputW; i++){
		for (j = 0; j < inputH; j++){

			int index = (i * inputW + j);
			int out_i = i - in_centerX;
			int out_j = j - in_centerY;
			float dist = sqrt(out_i*out_i + out_j*out_j);
			float H_filter;
			// this is Homomorphic
			H_filter =(yH - yL)*(1.0 - exp((-1.0)*(factor_c)*(dist*dist)/(cutoff*cutoff))) + yL;
			*(tempout_real + index) = (float)(H_filter * (*(in_real + index)));
			*(tempout_img + index) = (float)(H_filter * (*(in_imag + index)));
		}
	}
	return outdft;

}
DFT *LNandGenerateDFT(Image* image){
	unsigned short* tempin;
	float *tempout_img, *tempout_real;
	int i, j, size;
	int inputW = image->Width;
	int inputH = image->Height;
	DFT *outdft;
	outdft = (DFT*)malloc(sizeof(DFT));
	outdft->Height = inputH;
	outdft->Width = inputW;
	int outputH, outputW;
	outputW = outdft->Width;
	outputH = outdft->Height;
	tempin = image->data;

	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;
	//allocate memory for DFT daya arrays
	outdft->data_real = (float*)malloc(size * sizeof(float));
	outdft->data_imag = (float*)malloc(size * sizeof(float));
	tempout_real = outdft->data_real;
	tempout_img = outdft->data_imag;
	//here I use *tempshift and *temprow to save temp image daya
	float *tempshift = (float*)malloc(size * sizeof(float));

	float *temprow = (float*)malloc(size * sizeof(float));
	float *temprow_i = (float*)malloc(size * sizeof(float));


	for (int i = 0; i < inputH; i++) {
		for (int j = 0; j < inputW; j++) {
			int index = (i * inputW + j);
			*(temprow + index) = 0.0;
			*(temprow_i + index) = 0.0;
			*(tempout_real + index) = 0.0;
			*(tempout_img + index) = 0.0;
		}
	}

	//shifting
	for (i = 0; i < inputH; i++){
		for (j = 0; j < inputW; j++){
			int index = (i * inputW + j);
			int factor = (i + j) % 2 == 0 ? 1 : (-1);
			if (*(tempin + index) == 0){
				*(tempshift + index) = 0;
			}
			else{
				*(tempshift + index) = (float)(factor * log(*(tempin + index)));
			}
			
		}
	}

	// do row-wise DFT
	for (int i = 0; i < inputH; i++) {
		for (int j = 0; j < inputW; j++) {
			int index = (i * inputW + j);
			for (int m = 0; m < inputH; m++) {
				float theta = (-2) *PI * i * m / inputH;
				float cosval = cos(theta);
				float sinval = sin(theta);
				int findex = (m * inputW + j);
				*(temprow + index) = *(temprow + index) + ((*(tempshift + findex)) * cosval);
				*(temprow_i + index) = *(temprow_i + index) + ((*(tempshift + findex)) * sinval);
			}

		}
	}

	// do column-wise DFT
	for (int i = 0; i < inputH; i++) {
		for (int j = 0; j < inputW; j++) {
			int index = (i * inputW + j);

			for (int n = 0; n < inputW; n++) {
				float theta = (-2) * PI * j * n / inputW;
				float cosval = cos(theta);
				float sinval = sin(theta);
				int rindex = (i * inputW + n);
				*(tempout_real + index) = *(tempout_real + index) + *(temprow + rindex)*cosval - *(temprow_i + rindex)*sinval;
				*(tempout_img + index) = *(tempout_img + index) + *(temprow + rindex)*sinval + *(temprow_i + rindex)*cosval;
			}
		}
	}
	return(outdft);
}

DFT *Filt_GHPF(DFT* inputdft, float cutoff, int factor_n){
	float* in_real = inputdft->data_real;
	float* in_imag = inputdft->data_imag;
	int i, j, size;
	int inputW = inputdft->Width;
	int inputH = inputdft->Height;
	float *tempout_img, *tempout_real;
	DFT *outdft;
	outdft = (DFT*)malloc(sizeof(DFT));
	outdft->Height = inputH;
	outdft->Width = inputW;
	size = inputW * inputH;
	//allocate memory for DFT daya arrays
	outdft->data_real = (float*)malloc(size * sizeof(float));
	outdft->data_imag = (float*)malloc(size * sizeof(float));
	tempout_real = outdft->data_real;
	tempout_img = outdft->data_imag;
	int in_centerX, in_centerY;
	in_centerX = inputW / 2;
	in_centerY = inputH / 2;
	//initialize
	for (int i = 0; i < inputH; i++) {
		for (int j = 0; j < inputW; j++) {
			int index = (i * inputW + j);
			*(tempout_real + index) = 0.0;
			*(tempout_img + index) = 0.0;
			//*(in_real + index) = 1.0;
		}
	}
	// filt
	for (i = 0; i < inputW; i++){
		for (j = 0; j < inputH; j++){

			int index = (i * inputW + j);
			int out_i = i - in_centerX;
			int out_j = j - in_centerY;
			float dist = sqrt(out_i*out_i + out_j*out_j);
			float H_filter;
			// this is GHPF
			H_filter = pow(exp(1), -((dist*dist) / (2.0*cutoff*cutoff)));
			H_filter = 1.0 - H_filter;
			*(tempout_real + index) = (float)(H_filter * (*(in_real + index)));
			*(tempout_img + index) = (float)(H_filter * (*(in_imag + index)));

		}
	}
	return outdft;
}
DFT *Filt_IDHPF(DFT* inputdft, float cutoff, int factor_n){
	float* in_real = inputdft->data_real;
	float* in_imag = inputdft->data_imag;
	int i, j, size;
	int inputW = inputdft->Width;
	int inputH = inputdft->Height;
	float *tempout_img, *tempout_real;
	DFT *outdft;
	outdft = (DFT*)malloc(sizeof(DFT));
	outdft->Height = inputH;
	outdft->Width = inputW;
	size = inputW * inputH;
	//allocate memory for DFT daya arrays
	outdft->data_real = (float*)malloc(size * sizeof(float));
	outdft->data_imag = (float*)malloc(size * sizeof(float));
	tempout_real = outdft->data_real;
	tempout_img = outdft->data_imag;
	int in_centerX, in_centerY;
	in_centerX = inputW / 2;
	in_centerY = inputH / 2;
	//initialize
	for (int i = 0; i < inputH; i++) {
		for (int j = 0; j < inputW; j++) {
			int index = (i * inputW + j);
			*(tempout_real + index) = 0.0;
			*(tempout_img + index) = 0.0;
			//*(in_real + index) = 1.0;
		}
	}
	// filt
	for (i = 0; i < inputW; i++){
		for (j = 0; j < inputH; j++){
			int index = (i * inputW + j);
			int out_i = i - in_centerX;
			int out_j = j - in_centerY;
			float dist = sqrt(out_i*out_i + out_j*out_j);
			float H_filter;
			
			// this is IDHPF
			if (dist <= cutoff){
				H_filter = 0.0;
			}
			else{
				H_filter = 1.0;
			}

			*(tempout_real + index) = (float)(H_filter * (*(in_real + index)));
			*(tempout_img + index) = (float)(H_filter * (*(in_imag + index)));
		}
	}
	return outdft;
}
DFT *Filt_BHPF(DFT* inputdft, float cutoff, int factor_n){
	float* in_real = inputdft->data_real;
	float* in_imag = inputdft->data_imag;
	int i, j, size;
	int inputW = inputdft->Width;
	int inputH = inputdft->Height;
	float *tempout_img, *tempout_real;
	DFT *outdft;
	outdft = (DFT*)malloc(sizeof(DFT));
	outdft->Height = inputH;
	outdft->Width = inputW;
	size = inputW * inputH;
	//allocate memory for DFT daya arrays
	outdft->data_real = (float*)malloc(size * sizeof(float));
	outdft->data_imag = (float*)malloc(size * sizeof(float));
	tempout_real = outdft->data_real;
	tempout_img = outdft->data_imag;
	int in_centerX, in_centerY;
	in_centerX = inputW / 2;
	in_centerY = inputH / 2;
	//initialize
	for (int i = 0; i < inputH; i++) {
		for (int j = 0; j < inputW; j++) {
			int index = (i * inputW + j);
			*(tempout_real + index) = 0.0;
			*(tempout_img + index) = 0.0;
			//*(in_real + index) = 1.0;
		}
	}
	// filt
	for (i = 0; i < inputW; i++){
		for (j = 0; j < inputH; j++){
			int index = (i * inputW + j);
			int out_i = i - in_centerX;
			int out_j = j - in_centerY;
			float dist = sqrt(out_i*out_i + out_j*out_j);
			float H_filter;
			// this is BHPF
			H_filter = 1 / (1 + pow((cutoff / dist), 2 * factor_n));


			*(tempout_real + index) = (float)(H_filter * (*(in_real + index)));
			*(tempout_img + index) = (float)(H_filter * (*(in_imag + index)));
		}
	}
	return outdft;
}
DFT *Filt_GLPF(DFT* inputdft, float cutoff, int factor_n){
	float* in_real = inputdft->data_real;
	float* in_imag = inputdft->data_imag;
	int i, j, size;
	int inputW = inputdft->Width;
	int inputH = inputdft->Height;
	float *tempout_img, *tempout_real;
	DFT *outdft;
	outdft = (DFT*)malloc(sizeof(DFT));
	outdft->Height = inputH;
	outdft->Width = inputW;
	size = inputW * inputH;
	//allocate memory for DFT daya arrays
	outdft->data_real = (float*)malloc(size * sizeof(float));
	outdft->data_imag = (float*)malloc(size * sizeof(float));
	tempout_real = outdft->data_real;
	tempout_img = outdft->data_imag;
	int in_centerX, in_centerY;
	in_centerX = inputW / 2;
	in_centerY = inputH / 2;
	//initialize
	for (int i = 0; i < inputH; i++) {
		for (int j = 0; j < inputW; j++) {
			int index = (i * inputW + j);
			*(tempout_real + index) = 0.0;
			*(tempout_img + index) = 0.0;
			//*(in_real + index) = 1.0;
		}
	}
	// filt
	for (i = 0; i < inputW; i++){
		for (j = 0; j < inputH; j++){
			
			int index = (i * inputW + j);
			int out_i = i - in_centerX;
			int out_j = j - in_centerY;
			float dist = sqrt(out_i*out_i + out_j*out_j);
			float H_filter;
			// this is GLPF
			H_filter = pow(exp(1), -((dist*dist) / (2.0*cutoff*cutoff)));
			*(tempout_real + index) = (float)(H_filter * (*(in_real + index)));
			*(tempout_img + index) = (float)(H_filter * (*(in_imag + index)));
		
		}
	}
	return outdft;
}
DFT *Filt_BLPF(DFT* inputdft, float cutoff, int factor_n){
	float* in_real = inputdft->data_real;
	float* in_imag = inputdft->data_imag;
	int i, j, size;
	int inputW = inputdft->Width;
	int inputH = inputdft->Height;
	float *tempout_img, *tempout_real;
	DFT *outdft;
	outdft = (DFT*)malloc(sizeof(DFT));
	outdft->Height = inputH;
	outdft->Width = inputW;
	size = inputW * inputH;
	//allocate memory for DFT daya arrays
	outdft->data_real = (float*)malloc(size * sizeof(float));
	outdft->data_imag = (float*)malloc(size * sizeof(float));
	tempout_real = outdft->data_real;
	tempout_img = outdft->data_imag;
	int in_centerX, in_centerY;
	in_centerX = inputW / 2;
	in_centerY = inputH / 2;
	//initialize
	for (int i = 0; i < inputH; i++) {
		for (int j = 0; j < inputW; j++) {
			int index = (i * inputW + j);
			*(tempout_real + index) = 0.0;
			*(tempout_img + index) = 0.0;
			//*(in_real + index) = 1.0;
		}
	}
	// filt
	for (i = 0; i < inputW; i++){
		for (j = 0; j < inputH; j++){
			int index = (i * inputW + j);
			int out_i = i - in_centerX;
			int out_j = j - in_centerY;
			float dist = sqrt(out_i*out_i + out_j*out_j);
			float H_filter;
			// this is BLPF
			H_filter = 1 / (1 + pow((dist / cutoff), 2 * factor_n));
			

			*(tempout_real + index) = (float)(H_filter * (*(in_real + index)));
			*(tempout_img + index) = (float)(H_filter * (*(in_imag + index)));
		}
	}
	return outdft;
}
/**
* @description:
* @param {DFT*} inputdft The input real and imaginary image after DFT
* @param {float} cutoff The input cutoff for filtering
* @param {int} factor_n The powered factor,filter order n
*/
DFT *Filt_IDLPF(DFT* inputdft, float cutoff, int factor_n){
	float* in_real = inputdft->data_real;
	float* in_imag = inputdft->data_imag;
	int i, j, size;
	int inputW = inputdft->Width;
	int inputH = inputdft->Height;
	float *tempout_img, *tempout_real;
	DFT *outdft;
	outdft = (DFT*)malloc(sizeof(DFT));
	outdft->Height = inputH;
	outdft->Width = inputW;
	size = inputW * inputH;
	//allocate memory for DFT daya arrays
	outdft->data_real = (float*)malloc(size * sizeof(float));
	outdft->data_imag = (float*)malloc(size * sizeof(float));
	tempout_real = outdft->data_real;
	tempout_img = outdft->data_imag;
	int in_centerX, in_centerY;
	in_centerX = inputW / 2;
	in_centerY = inputH / 2;
	//initialize
	for (int i = 0; i < inputH; i++) {
		for (int j = 0; j < inputW; j++) {
			int index = (i * inputW + j);
			*(tempout_real + index) = 0.0;
			*(tempout_img + index) = 0.0;
			//*(in_real + index) = 1.0;
		}
	}
	// filt
	for (i = 0; i < inputW; i++){
		for (j = 0; j < inputH; j++){
			int index = (i * inputW + j);
			int out_i = i - in_centerX;
			int out_j = j - in_centerY;
			float dist = sqrt(out_i*out_i + out_j*out_j);
			float H_filter;
			// this is BLPF
			// H_filter = 1 / (1 + pow((dist / cutoff), 2 * factor_n));
			// this is IDLPF
			if (dist <= cutoff){
				H_filter = 1.0;
			}
			else{
				H_filter = 0.0;
			}

			*(tempout_real + index) = (float)(H_filter * (*(in_real + index)));
			*(tempout_img + index) = (float)(H_filter * (*(in_imag + index)));
		}
	}
	return outdft;
}
Image *InverseMagnitudeDFT_norm_exp(DFT* inputdft, Image *image){
	Image* outimage;
	outimage = CreateNewImage(image, "Test magnitude");
	float* in_real = inputdft->data_real;
	float* in_imag = inputdft->data_imag;
	unsigned short *tempout;
	int i, j;
	outimage->Height = inputdft->Height;
	outimage->Width = inputdft->Width;
	outimage->data = (unsigned short*)malloc(outimage->Height*outimage->Width*sizeof(unsigned short));
	outimage->Type = GRAY;
	tempout = outimage->data;
	int inputW = inputdft->Width;
	int inputH = inputdft->Height;
	float* temp = (float*)malloc(outimage->Height*outimage->Width*sizeof(float));
	float max = 0.0;
	float min = 0.0;
	for (i = 0; i < inputH; i++){
		for (j = 0; j < inputW; j++){
			int index = (i * inputW + j);


			*(temp + index) = exp(sqrt(*(in_real + index)*(*(in_real + index)) + *(in_imag + index)*(*(in_imag + index))));


			//printf("%f\t", *(temp + index));

			// do log2 to norm
			//*(temp + index) = log2f(*(temp + index));
			//printf("%f\t", *(temp + index));
			if (*(temp + index) == log2f(0)){
				*(temp + index) = 0.0;
			}
			if (*(temp + index) >= max){
				max = *(temp + index);
			}
			if (i <= 10 && j <= 10){
				printf("%d,%d,%f,%f,%f\n", i, j, *(in_real + index), *(in_imag + index), *(temp + index));
			}
			if (i == 0 && j == 0){
				//printf("%f,%f,%f\t", *(in_real + index), *(in_imag + index), *(temp + index));
				min = *(temp + index);// initialize min value
				//printf("%f\t", *(temp + index));
			}
			if (*(temp + index) <= min){
				min = *(temp + index);

			}
			if (i == 128 && j == 128){
				//printf("%f\t", *(temp + index));
				printf("#################################");
				printf("%d,%d\t\n", i, j);
				printf("%f,%f,%f\n", *(in_real + index), *(in_imag + index), *(temp + index));
				//*(temp + index) = 800000.0;
				//max = 800000.0;
			}
		}
	}
	printf("max:%f,min:%f\n", max, min);

	for (i = 0; i < inputH; i++){
		for (j = 0; j < inputW; j++){
			int index = (i * inputW + j);
			//printf("%f\t", *(temp + index));
			*(temp + index) = map(*(temp + index), min, max, 0.0, 255.0);
			//printf("%f\t", *(temp + index));
			//printf("%f\t%d\t%d", map(*(temp + index), min, max, 0, 255), i, j);

			*(tempout + index) = (unsigned short)(*(temp + index));
			//printf("%d\t", *(tempout + index));
		}
	}
	return outimage;
}

Image *InverseMagnitudeDFT_no_norm(DFT* inputdft, Image *image){
	Image* outimage;
	outimage = CreateNewImage(image, "Test magnitude");
	float* in_real = inputdft->data_real;
	float* in_imag = inputdft->data_imag;
	unsigned short *tempout;
	int i, j;
	outimage->Height = inputdft->Height;
	outimage->Width = inputdft->Width;
	outimage->data = (unsigned short*)malloc(outimage->Height*outimage->Width*sizeof(unsigned short));
	outimage->Type = GRAY;
	tempout = outimage->data;
	int inputW = inputdft->Width;
	int inputH = inputdft->Height;
	float* temp = (float*)malloc(outimage->Height*outimage->Width*sizeof(float));
	float max = 0.0;
	float min = 0.0;
	for (i = 0; i < inputH; i++){
		for (j = 0; j < inputW; j++){
			int index = (i * inputW + j);


			*(temp + index) = sqrt(*(in_real + index)*(*(in_real + index)) + *(in_imag + index)*(*(in_imag + index)));


			//printf("%f\t", *(temp + index));

			// do log2 to norm
			//*(temp + index) = log2f(*(temp + index));
			//printf("%f\t", *(temp + index));
			if (*(temp + index) == log2f(0)){
				*(temp + index) = 0.0;
			}
			if (*(temp + index) >= max){
				max = *(temp + index);
			}
			if (i <= 10 && j <= 10){
				printf("%d,%d,%f,%f,%f\n", i, j, *(in_real + index), *(in_imag + index), *(temp + index));
			}
			if (i == 0 && j == 0){
				//printf("%f,%f,%f\t", *(in_real + index), *(in_imag + index), *(temp + index));
				min = *(temp + index);// initialize min value
				//printf("%f\t", *(temp + index));
			}
			if (*(temp + index) <= min){
				min = *(temp + index);

			}
			if (i == 128 && j == 128){
				//printf("%f\t", *(temp + index));
				printf("#################################");
				printf("%d,%d\t\n", i, j);
				printf("%f,%f,%f\n", *(in_real + index), *(in_imag + index), *(temp + index));
				//*(temp + index) = 800000.0;
				//max = 800000.0;
			}
		}
	}
	printf("max:%f,min:%f\n", max, min);

	for (i = 0; i < inputH; i++){
		for (j = 0; j < inputW; j++){
			int index = (i * inputW + j);
			//printf("%f\t", *(temp + index));
			//*(temp + index) = map(*(temp + index), min, max, 0.0, 255.0);
			//printf("%f\t", *(temp + index));
			//printf("%f\t%d\t%d", map(*(temp + index), min, max, 0, 255), i, j);

			*(tempout + index) = (unsigned short)(*(temp + index));
			//printf("%d\t", *(tempout + index));
		}
	}
	return outimage;
}
Image *InverseMagnitudeDFT(DFT* inputdft, Image *image){
	Image* outimage;
	outimage = CreateNewImage(image, "Test magnitude");
	float* in_real = inputdft->data_real;
	float* in_imag = inputdft->data_imag;
	unsigned short *tempout;
	int i, j;
	outimage->Height = inputdft->Height;
	outimage->Width = inputdft->Width;
	outimage->data = (unsigned short*)malloc(outimage->Height*outimage->Width*sizeof(unsigned short));
	outimage->Type = GRAY;
	tempout = outimage->data;
	int inputW = inputdft->Width;
	int inputH = inputdft->Height;
	float* temp = (float*)malloc(outimage->Height*outimage->Width*sizeof(float));
	float max = 0.0;
	float min = 0.0;
	for (i = 0; i < inputH; i++){
		for (j = 0; j < inputW; j++){
			int index = (i * inputW + j);


			*(temp + index) = sqrt(*(in_real + index)*(*(in_real + index)) + *(in_imag + index)*(*(in_imag + index)));
			
			
			//printf("%f\t", *(temp + index));

			// do log2 to norm
			//*(temp + index) = log2f(*(temp + index));
			//printf("%f\t", *(temp + index));
			if (*(temp + index) == log2f(0)){
				*(temp + index) = 0.0;
			}
			if (*(temp + index) >= max){
				max = *(temp + index);
			}
			if (i <= 10 && j <= 10){
				printf("%d,%d,%f,%f,%f\n", i, j, *(in_real + index), *(in_imag + index), *(temp + index));
			}
			if (i == 0 && j == 0){
				//printf("%f,%f,%f\t", *(in_real + index), *(in_imag + index), *(temp + index));
				min = *(temp + index);// initialize min value
				//printf("%f\t", *(temp + index));
			}
			if (*(temp + index) <= min){
				min = *(temp + index);

			}
			if (i == 128 && j == 128){
				//printf("%f\t", *(temp + index));
				printf("#################################");
				printf("%d,%d\t\n", i, j);
				printf("%f,%f,%f\n", *(in_real + index), *(in_imag + index), *(temp + index));
				//*(temp + index) = 800000.0;
				//max = 800000.0;
			}
		}
	}
	printf("max:%f,min:%f\n", max, min);

	for (i = 0; i < inputH; i++){
		for (j = 0; j < inputW; j++){
			int index = (i * inputW + j);
			//printf("%f\t", *(temp + index));
			*(temp + index) = map(*(temp + index), min, max, 0.0, 255.0);
			//printf("%f\t", *(temp + index));
			//printf("%f\t%d\t%d", map(*(temp + index), min, max, 0, 255), i, j);

			*(tempout + index) = (unsigned short)(*(temp + index));
			//printf("%d\t", *(tempout + index));
		}
	}
	return outimage;
}

DFT *GenerateInverseDFT(DFT* inputdft, Image *image,int type){
	float* in_real = inputdft->data_real;
	float* in_imag = inputdft->data_imag;
	int i, j, size;
	int inputW = inputdft->Width;
	int inputH = inputdft->Height;
	float* temp = (float*)malloc(inputH*inputW*sizeof(float));

	float *tempout_img, *tempout_real;
	DFT *outdft;
	outdft = (DFT*)malloc(sizeof(DFT));
	outdft->Height = inputH;
	outdft->Width = inputW;
	int outputH, outputW;
	outputW = outdft->Width;
	outputH = outdft->Height;
	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;
	//allocate memory for DFT daya arrays
	outdft->data_real = (float*)malloc(size * sizeof(float));
	outdft->data_imag = (float*)malloc(size * sizeof(float));
	tempout_real = outdft->data_real;
	tempout_img = outdft->data_imag;
	//here I use *tempshift and *temprow to save temp image daya
	float *tempshift = (float*)malloc(size * sizeof(float));
	float *temprow = (float*)malloc(size * sizeof(float));
	float *temprow_i = (float*)malloc(size * sizeof(float));
	for (int i = 0; i < inputH; i++) {
		for (int j = 0; j < inputW; j++) {
			int index = (i * inputW + j);
			*(temprow + index) = 0.0;
			*(temprow_i + index) = 0.0;
			*(tempout_real + index) = 0.0;
			*(tempout_img + index) = 0.0;
			//*(in_real + index) = 1.0;
		}
	}
	if (type == 0){
		printf("doing restoration: original image\n");
	}
	if (type == 1){
		//using only phase
		printf("doing restoration: phase spectrum\n");
		for (i = 0; i < inputH; i++){
			for (j = 0; j < inputW; j++){
				int index = (i * inputW + j);
				float mag = sqrt(*(in_imag + index)*(*(in_imag + index)) + *(in_real + index)*(*(in_real + index)));
				*(in_imag + index) = (float)((*(in_imag + index)) / mag);
				*(in_real + index) = (float)((*(in_real + index)) / mag);
			}
		}
	}
	if (type == 2){
		//using only magnitude
		for (i = 0; i < inputH; i++){
			for (j = 0; j < inputW; j++){
				int index = (i * inputW + j);
				//printf("%f\t", *(in_real + index));
				int factor = (i + j) % 2 == 0 ? 1 : (-1);
				float mag = sqrt(*(in_imag + index)*(*(in_imag + index)) + *(in_real + index)*(*(in_real + index)));
				*(in_imag + index) = (float)(0.0);
				*(in_real + index) = (float)(mag*factor);
				//printf("%f %f %f\t", *(in_imag + index), *(in_real + index), mag);
			}
		}
	}
	
	// do row-wise DFT
	for (int i = 0; i < inputH; i++) {
		for (int j = 0; j < inputW; j++) {
			int index = (i * inputW + j);
			for (int m = 0; m < inputH; m++) {
				float theta = (2) * PI * i * m / inputH;
				float cosval = cos(theta);
				float sinval = sin(theta);
				int findex = (m * inputW + j);
				
				*(temprow + index) = *(temprow + index) + ((*(in_real + findex)) * cosval) - ((*(in_imag + findex)) * sinval);
				*(temprow_i + index) = *(temprow_i + index) + ((*(in_real + findex)) * sinval) + ((*(in_imag + findex)) * cosval);
			}
		}
	}
	// do column-wise DFT
	for (int i = 0; i < inputH; i++) {
		for (int j = 0; j < inputW; j++) {
			int index = (i * inputW + j);
			for (int n = 0; n < inputW; n++) {
				float theta = (2) * PI * j * n / inputW;
				float cosval = cos(theta);
				float sinval = sin(theta);
				int rindex = (i * inputW + n);
				*(tempout_real + index) = *(tempout_real + index) + *(temprow + rindex)*cosval - *(temprow_i + rindex)*sinval;
				*(tempout_img + index) = *(tempout_img + index) + *(temprow + rindex)*sinval + *(temprow_i + rindex)*cosval;
			}
			*(tempout_real + index) = *(tempout_real + index) / (float)(outputW*outputH);
			*(tempout_img + index) = *(tempout_img + index) / (float)(outputW*outputH);
		}
	}
	return(outdft);
}
Image *PhaseDFT(DFT* inputdft, Image *image){
	Image* outimage;
	outimage = CreateNewImage(image, "Test magnitude");
	float* in_real = inputdft->data_real;
	float* in_imag = inputdft->data_imag;
	unsigned short *tempout;
	int i, j;
	outimage->Height = inputdft->Height;
	outimage->Width = inputdft->Width;
	outimage->data = (unsigned short*)malloc(outimage->Height*outimage->Width*sizeof(unsigned short));
	outimage->Type = GRAY;
	tempout = outimage->data;
	int inputW = inputdft->Width;
	int inputH = inputdft->Height;
	float* temp = (float*)malloc(outimage->Height*outimage->Width*sizeof(float));
	float max = 0.0;
	float min = 0.0;
	for (i = 0; i < inputH; i++){
		for (j = 0; j < inputW; j++){
			//printf("i:%d,j:%d\t", i, j);

			int index = (i * inputW + j);
			*(temp + index) = atan2f(*(in_imag + index), *(in_real + index));
			//*(temp + index) = log2f(*(temp + index));
			
			//printf("%f\t", *(temp + index));
			if (*(temp + index) >= max){
				max = *(temp + index);
			}
			if (*(temp + index) == log2f(0)){
				*(temp + index) = 0.0;
			}
			if (i == 0 && j == 0){
				//printf("%f,%f,%f\t", *(in_real + index), *(in_imag + index), *(temp + index));
				min = *(temp + index);// initialize min value
				//printf("%f\t", *(temp + index));
			}
			if (*(temp + index) <= min){
				min = *(temp + index);

			}
			if (i <= 10 && j <= 10){
				printf("%d,%d,%f,%f,%f\n", i, j, *(in_real + index), *(in_imag + index), *(temp + index));
			}
			if (i == 128 && j == 128){
				//printf("%f\t", *(temp + index));
				printf("%d,%d\t\n", i, j);
				printf("%f,%f,%f\n", *(in_real + index), *(in_imag + index), *(temp + index));
				//*(temp + index) = 800000.0;
				//max = 800000.0;
			}
		}
	}
	printf("max:%f,min:%f\n", max, min);

	for (i = 0; i < inputH; i++){
		for (j = 0; j < inputW; j++){
			int index = (i * inputW + j);
			//printf("%f\t", *(temp + index));
			*(temp + index) = map(*(temp + index), min, max, 0.0, 255.0);
			//printf("%f\t", *(temp + index));
			//printf("%f\t%d\t%d", map(*(temp + index), min, max, 0, 255), i, j);

			*(tempout + index) = (unsigned short)(*(temp + index));
			//printf("%d\t", *(tempout + index));
		}
	}
	return outimage;
}
Image *MagnitudeDFT(DFT* inputdft,Image *image){
	Image* outimage;
	outimage = CreateNewImage(image, "Test magnitude");
	float* in_real = inputdft->data_real;
	float* in_imag = inputdft->data_imag;
	unsigned short *tempout;
	int i, j;
	outimage->Height = inputdft->Height;
	outimage->Width = inputdft->Width;
	outimage->data = (unsigned short*)malloc(outimage->Height*outimage->Width*sizeof(unsigned short));
	outimage->Type = GRAY;
	tempout = outimage->data;
	int inputW = inputdft->Width;
	int inputH = inputdft->Height;
	float* temp = (float*)malloc(outimage->Height*outimage->Width*sizeof(float));
	float max = 0.0;
	float min = 0.0;
	for (i = 0; i < inputH; i++){
		for (j = 0; j < inputW; j++){
			//printf("i:%d,j:%d\t", i, j);
			
			int index = (i * inputW + j);
			*(temp+index) = sqrt(*(in_real + index)*(*(in_real + index)) + *(in_imag + index)*(*(in_imag + index)))/256.0;
			
			
			if (i <= 10 && j <= 10){
				printf("%d,%d,%f,%f,%f\n",i,j, *(in_real + index), *(in_imag + index), *(temp + index));
			}
			//printf("%f\t", *(temp + index));
			
			// do log2 to norm
			*(temp + index) = log2f(*(temp + index));
			//printf("%f\t", *(temp + index));
			if (*(temp + index) == log2f(0)){
				*(temp + index) = 0.0;
			}
			if (*(temp + index) >= max){
				max = *(temp + index);
			}

			if (i==0&&j==0){
				//printf("%f,%f,%f\t", *(in_real + index), *(in_imag + index), *(temp + index));
				min = *(temp + index);// initialize min value
				//printf("%f\t", *(temp + index));
			}
			if (*(temp + index) <= min){
				min = *(temp + index);
				
			}
			if (i==128&&j==128){
				//printf("%f\t", *(temp + index));
				printf("#################################");
				printf("%d,%d\t\n", i, j);
				printf("%f,%f,%f\n", *(in_real + index), *(in_imag + index), *(temp + index));
				//*(temp + index) = 800000.0;
				//max = 800000.0;
			}
		}
	}
	printf("max:%f,min:%f\n", max, min);

	for (i = 0; i < inputH; i++){
		for (j = 0; j < inputW; j++){
			int index = (i * inputW + j);
			//printf("%f\t", *(temp + index));
			*(temp + index) = map(*(temp + index), min, max, 0.0, 255.0);
			//printf("%f\t", *(temp + index));
			//printf("%f\t%d\t%d", map(*(temp + index), min, max, 0, 255), i, j);
			
			*(tempout + index) = (unsigned short)(*(temp + index));
			//printf("%d\t", *(tempout + index));
		}
	}
	return outimage;
}
Image *ShiftImage(Image* image){
	unsigned short *tempin,*tempout;
	int i,j, size;
	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;
	Image *outimage;
	outimage = CreateNewImage(image, "#testing Swap");
	tempin = image->data;
	outimage->data = (unsigned short*)malloc(size*sizeof(unsigned short));
	tempout = outimage->data;
	int inputW = image->Width;
	int inputH = image->Height;
	
	for (i = 0; i < inputH; i++){
		for (j = 0; j < inputW; j++){
			int factor = (i + j) % 2 == 0 ? 1 : (-1);
			int index = (i * inputW + j);
			/*
			int x = (i + j) % 2 == 0 ? i : -i;
			int y = (i + j) % 2 == 0 ? j : -j;
			printf("x:%d,y:%d,value:%d\t", x, y, *(tempin + x*inputW + y));
			*/
			*(tempout + index) = factor*(*(tempin + i*inputW + j));

			//printf("i:%d,j:%d\t", i, j);
		}
	}
	return outimage;
}
DFT *GenerateDFT(Image* image){
	unsigned short* tempin;
	float *tempout_img, *tempout_real;
	int i, j, size;
	int inputW = image->Width;
	int inputH = image->Height;
	DFT *outdft;
	outdft = (DFT*)malloc(sizeof(DFT));
	outdft->Height = inputH;
	outdft->Width = inputW;
	int outputH, outputW;
	outputW = outdft->Width;
	outputH = outdft->Height;
	tempin = image->data;

	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;
	//allocate memory for DFT daya arrays
	outdft->data_real = (float*)malloc(size * sizeof(float));
	outdft->data_imag = (float*)malloc(size * sizeof(float));
	tempout_real = outdft->data_real;
	tempout_img = outdft->data_imag;
	//here I use *tempshift and *temprow to save temp image daya
	float *tempshift = (float*)malloc(size * sizeof(float));

	float *temprow = (float*)malloc(size * sizeof(float));
	float *temprow_i = (float*)malloc(size * sizeof(float));
	

	for (int i = 0; i < inputH; i++) {
		for (int j = 0; j < inputW; j++) {
			int index = (i * inputW + j);
			*(temprow + index) = 0.0;
			*(temprow_i + index) = 0.0;
			*(tempout_real + index) = 0.0;
			*(tempout_img + index) = 0.0;
		}
	}

	//shifting
	for (i = 0; i < inputH; i++){
		for (j = 0; j < inputW; j++){
			int index = (i * inputW + j);
			int factor = (i + j) % 2 == 0 ? 1 : (-1);
			*(tempshift + index) = (float)(factor*(*(tempin + i*inputW + j)));
		}
	}

	// do row-wise DFT
	for (int i = 0; i < inputH; i++) {
		for (int j = 0; j < inputW; j++) {
			int index = (i * inputW + j);
			for (int m = 0; m < inputH; m++) {
				float theta = (-2) *PI * i * m / inputH;
				float cosval = cos(theta);
				float sinval = sin(theta);
				int findex = (m * inputW + j);
				*(temprow + index) = *(temprow + index)+((*(tempshift + findex)) * cosval);
				*(temprow_i + index) = *(temprow_i + index)+ ((*(tempshift + findex)) * sinval);
			}
			
		}
	}

	// do column-wise DFT
	for (int i = 0; i < inputH; i++) {
		for (int j = 0; j < inputW; j++) {
			int index = (i * inputW + j);
			
			for (int n = 0; n < inputW; n++) {
				float theta = (-2) * PI * j * n / inputW;
				float cosval = cos(theta);
				float sinval = sin(theta);
				int rindex = (i * inputW + n);
				*(tempout_real + index) = *(tempout_real + index)+ *(temprow + rindex)*cosval - *(temprow_i + rindex)*sinval;
				*(tempout_img + index) = *(tempout_img + index)+ *(temprow + rindex)*sinval + *(temprow_i + rindex)*cosval;
			}
		}
	}
	return(outdft);
}

Image *thresholding(Image* image, int th){
	unsigned short *tempin, *tempout;
	int i, size;
	Image *outimage;

	outimage = CreateNewImage(image, "#testing threshold");
	tempin = image->data;
	tempout = outimage->data;
	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;
	for (i = 0; i<size; i++)
	{
		
		*tempout = ((int)(*tempin)>th) ? (unsigned short)(255) : (unsigned short)(0);

		//printf("%c", *tempout);
		tempin++;
		tempout++;
	}
	return outimage;
}
Image *LocalHistogram(Image* image){
	unsigned short* tempin, *tempout;
	int i, j,m,n,v,k,t,size;
	Image* outimage;

	outimage = CreateNewImage(image, "#testing Histogram");
	tempin = image->data;
	tempout = outimage->data;
	int inputW = image->Width;
	int inputH = image->Height;
	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;

	
	int temp = 0;

	// define the temp filter to save a f*f filter data
	unsigned short* tempfilter;
	tempfilter = (unsigned short*)malloc(3 * 3 * sizeof(unsigned short));

	for (i = 1; i < inputH - 1; ++i){
		for (j = 1; j < inputW - 1; ++j){
			int num[256] = { 0 };
			int sum[256] = { 0 };
			int cdf[256] = { 0 };
			temp = 0;
			v = 0;
			// put each original pixel into this 3*3 tempfilter
			// go over the 3*3 filter using two loops
			for (m = -1; m < 2; ++m){
				for (n = -1; n < 2; ++n){
					*(tempfilter + v) = *(tempin + inputW*(i + m) + (j + n));
					v++;
				}
			}
			for (v = 0; v < 3 * 3; v++)
			{
				temp = (int)(*(tempfilter + v));
				num[temp]++;
			}
			int temp_sum = 0;
			for (t = 0; t < 256; t++){
				temp_sum = 0;
				for (k = 0; k <= t; k++){
					temp_sum = temp_sum + num[k];
				}
				sum[t] = temp_sum;
				cdf[t] = round((double)((double)(sum[t]) / ((double)(3*3))*(double)(256 - 1)));
				//printf("%d,%d\t", t, cdf[t]);	
			}
			
			//printf("%d %d\t", (int)(*(tempfilter + 4)), cdf[(int)(*(tempfilter + 4))]);
			tempout[inputW*i + j] = (unsigned short)(cdf[(int)(*(tempfilter + 4))]);
		}
	}

	
	return(outimage);
}
Image *GlobalHistogram(Image* image){
	unsigned short* tempin, *tempout;
	int i,j, size;
	Image* outimage;

	outimage = CreateNewImage(image, "#testing Histogram");
	tempin = image->data;
	tempout = outimage->data;
	int inputW = image->Width;
	int inputH = image->Height;
	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;

	int num[256] = { 0 };
	int sum[256] = { 0 };
	int cdf[256] = { 0 };
	int temp = 0;
	
	for (i = 0; i<size; i++)
	{
		temp = (int)(*(tempin + i));
		num[temp]++;
	}
	int temp_sum=0;
	for (i = 0; i < 256; i++){
		
		temp_sum = 0;
		for (j = 0; j <= i; j++){
			temp_sum = temp_sum + num[j];
		}
		sum[i] = temp_sum;
		cdf[i] = round((double)((double)(sum[i]) / ((double)(inputH*inputW))*(double)(256 - 1)));
	}
	
	for (i = 0; i < size; i++){
		*(tempout + i) = (unsigned short)(cdf[(int)(*(tempin + i))]);
	}
	return(outimage);
}
float CalculateVariance(Image* image){
	unsigned short* tempin;
	int i, size;
	tempin = image->data;
	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;
	float avg = 0.0;
	for (i = 0; i < size; i++){
		avg = avg + *(tempin + i);
	}
	avg = (float)(avg / (float)size);
	float variance = 0.0;
	for (i = 0; i < size; i++){
		variance = variance+pow((fabs((float)(*(tempin + i))-avg)),2.0);
	}
	variance = (float)(variance / (float)size);
	return variance;
}
Image *GammaImage(Image* image, float gama){
	unsigned short* tempin, *tempout;
	int i, size;
	Image* outimage;

	outimage = CreateNewImage(image, "#testing Gamma");
	tempin = image->data;
	tempout = outimage->data;

	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;

	float coef = (float)(gama);

	float result = 0.0;
	for (i = 0; i<size; i++)
	{
		result = pow((float)((*tempin + 0.5) / 256), coef);
		//printf("%f\t", result);
		*tempout = result * 256 - 0.5;
		
		//*tempout = log((*tempin)/gama);
		
		tempin++;
		tempout++;
		result = 0.0;
	}

	return(outimage);

}
Image *SobelImage(Image* image){
	unsigned short* tempin, *tempout;
	
	int i, j, k, t, v, size;
	Image* outimage;
	outimage = (Image*)malloc(sizeof(Image));
	int inputW = image->Width, inputH = image->Height;
	// get the result image size, size of canvas
	int outputW = inputW, outputH = inputH;
	outimage->Width = outputW;
	outimage->Height = outputH;
	outimage->Type = image->Type;

	if (outimage->Type == GRAY)
		size = outimage->Width * outimage->Height;
	else if (outimage->Type == COLOR)
		size = outimage->Width * outimage->Height * 3;

	outimage->data = (unsigned short*)malloc(size);
	tempin = image->data;
	tempout = outimage->data;


	int filter_size = 3 * 3;
	int SobX[9] = { -1, 2, -1, 0, 0, 0, 1, 2, 1 };
	int SobY[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
	int gx, gy;
	double result;
	// define the temp filter to save a f*f filter data
	unsigned short* tempfilter;
	
	tempfilter = (unsigned short*)malloc(3 * 3 * sizeof(unsigned short));

	for (i = 1; i < outputH - 1; ++i){
		for (j = 1; j < outputW - 1; ++j){
			v = 0;
			result = 0;
			gx = 0;
			gy = 0;
			// put each original pixel into this 3*3 tempfilter
			// go over the 3*3 filter using two loops
			for (k = -1; k < 2; ++k){
				for (t = -1; t < 2; ++t){
					*(tempfilter + v) = *(tempin + inputW*(i + k) + (j + t));
					v++;
				}
			}
			// operate the sobelX operator to this tempfilter
			for (v = 0; v < 9; v++){
				gx = (int)(*(tempfilter + v))*(int)SobX[v] + gx;
			}
			// operate the sobelY operator to this tempfilter
			for (v = 0; v < 9; v++){
				gy = (int)(*(tempfilter + v))*(int)SobY[v] + gy;
			}
			//result
			result = sqrt((double)(gx*gx) + (double)(gy*gy));
			tempout[inputW*i + j] = (int)(result+0.5);
		}
	}
	return (outimage);
}
Image *LaplacianSharpenImage(Image* image, int type){
	// here i update my algorithm to use two para to present a pixel's location
	// different from my avg and midian filter
	float C = 0.01; // C is a factor to make boundary more obvious
	unsigned short* tempin, *tempout;
	int i, j, k,t,v,size;
	Image* outimage;
	outimage = (Image*)malloc(sizeof(Image));
	int inputW = image->Width, inputH = image->Height;
	// get the result image size, size of canvas
	int outputW = inputW, outputH = inputH;
	outimage->Width = outputW;
	outimage->Height = outputH;
	outimage->Type = image->Type;

	if (outimage->Type == GRAY)
		size = outimage->Width * outimage->Height;
	else if (outimage->Type == COLOR)
		size = outimage->Width * outimage->Height * 3;

	outimage->data = (unsigned short*)malloc(size*sizeof(unsigned short));
	tempin = image->data;
	tempout = outimage->data;


	int filter_size = 3*3;
	int lap[9];
	if (type == 0){
		lap[0] = 0;
		lap[1] = 1;
		lap[2] = 0;
		lap[3] = 1;
		lap[4] = -4;
		lap[5] = 1;
		lap[6] = 0;
		lap[7] = 1;
		lap[8] = 0;
	}
	else{
		lap[0] = 1;
		lap[1] = 1;
		lap[2] = 1;
		lap[3] = 1;
		lap[4] = -8;
		lap[5] = 1;
		lap[6] = 1;
		lap[7] = 1;
		lap[8] = 1;
	}
	// define the temp filter to save a f*f filter data
	unsigned short* tempfilter;
	int result = 0;
	tempfilter = (unsigned short*)malloc(3*3*sizeof(unsigned short));

	for (i = 1; i < outputH - 1; ++i){
		for (j = 1; j < outputW - 1; ++j){
			v = 0;
			result = 0;
			// put each original pixel into this 3*3 tempfilter
			// go over the 3*3 filter using two loops
			for (k = -1; k < 2; ++k){
				for (t = -1; t < 2; ++t){
					*(tempfilter + v) = *(tempin+inputW*(i+k)+(j+t));
					v++;
				}
			}
			// operate the laplacian operator to this tempfilter
			for (v = 0; v < 9; v++){
				result = (int)(*(tempfilter + v))*(int)lap[v]+ result;
			}
			//result
			//result = *(tempfilter + 4) + fabs(result)* C;
			tempout[inputW*i + j] = abs(result);

		}
	}
	return (outimage);
}

Image *SharpenImage(Image* mask, Image* image, float C){
	unsigned short *tempin, *tempout, *tempmask;
	int i, j,k,size;
	i = 0;

	Image *outimage;

	outimage = CreateNewImage(image, "#testing Sharpen");
	tempin = image->data;
	tempmask = mask->data;
	tempout = outimage->data;


	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;

	int Width = image->Width;
	int Height = image->Height;

	
	for (i = 0; i<size; i++)
	{
		*tempout = *tempin + C*( *tempmask);
		if (*tempout >= 255){
			//printf("%d", i);
			//printf("b:%d\t", *tempout);
			*tempout = (unsigned short)(255);
			//printf("a:%d\t", *tempout);
		}
		tempin++;
		tempmask++;
		tempout++;
	}
	
	/*
	float max = *tempin+C*(*tempmask);
	float min = max;
	for(int j =0;j<size;j++){
		float tempvalue = 0.0;
		tempvalue =(float) ((*tempin+j)+C*(*tempmask+j));
		printf("%f\t", tempvalue);
		if(tempvalue>= max){max = tempvalue;}
		if(tempvalue<=min){min = tempvalue;}
		
	}
	printf("max:%f min:%f\n",max,min);
	for (int k=0;k<size;k++){
		
		*(tempout+k) = (unsigned short)(map((float)((*tempin+k)+C*(*tempmask+k)),min,max,0.0,255.0));
		
	}
	*/
	
	

	return(outimage);
}
Image *ReplaceValues(Image* image, int value_a, int value_b){
	unsigned short* tempin, *tempout;
	int i, size;
	Image* outimage;

	outimage = CreateNewImage(image, "#testing replace");
	tempin = image->data;
	tempout = outimage->data;

	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;


	for (i = 0; i<size; i++)
	{
		
		*tempout = *tempin;
		if ((int)(*tempout) == value_a){
			*tempout = (unsigned short)(value_b);
		}
		tempin++;
		tempout++;
	}

	return(outimage);
}
Image* NegativeImage(Image* image){
	unsigned short* tempin, *tempout;
	int i, size;
	Image* outimage;

	outimage = CreateNewImage(image, "#testing Nega");
	tempin = image->data;
	tempout = outimage->data;

	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;


	for (i = 0; i<size; i++)
	{
		*tempout = 255 - (*tempin);

		tempin++;
		tempout++;
	}

	return(outimage);
}
Image *ShearHorizontalImage(Image* image, float h_para){
	unsigned short* tempin, *tempout;
	int i, j, size;

	Image* outimage;
	outimage = (Image*)malloc(sizeof(Image));

	int inputW = image->Width, inputH = image->Height;
	// get the result image size, size of canvas
	int outputW = (int)(inputW*(float)(1.0 + fabs(h_para)) + 0.5), outputH = inputH;

	outimage->Width = outputW;
	outimage->Height = outputH;
	outimage->Type = image->Type;

	if (outimage->Type == GRAY)
		size = outimage->Width * outimage->Height;
	else if (outimage->Type == COLOR)
		size = outimage->Width * outimage->Height * 3;

	outimage->data = (unsigned short*)malloc(size);
	tempin = image->data;
	tempout = outimage->data;

	int in_centerX, out_centerX, in_centerY, out_centerY;
	in_centerX = inputW / 2;
	in_centerY = inputH / 2;
	out_centerX = outputW / 2;
	out_centerY = outputH / 2;

	int in_i, in_j, out_i, out_j;

	// first set default canvas background as black color
	for (i = 0; i<outputW*outputH; i++)
	{
		*(tempout + i) = 0;
	}

	//go over the out image
	for (i = 0; i < outputW; i++){
		for (j = 0; j < outputH; j++){
			out_i = i - out_centerX;
			out_j = j - out_centerY;
			in_i = (int)(out_i + h_para*out_j) + in_centerX;
			in_j = (int)(out_j) + in_centerY;

			if (in_i >= 0 && in_i < inputW && in_j >= 0 && in_j < inputH){
				*(tempout + j*outputW + i) = *(tempin + in_j*inputW + in_i);
			}
		}
	}
	return(outimage);


}
Image *ShearVertivalImage(Image* image, float v_para){
	unsigned short* tempin, *tempout;
	int i, j, size;

	Image* outimage;
	outimage = (Image*)malloc(sizeof(Image));

	int inputW = image->Width, inputH = image->Height;
	// get the result image size, size of canvas
	int outputW = inputW, outputH = (int)(inputH*(float)(1.0 + fabs(v_para))+0.5);
	
	outimage->Width = outputW;
	outimage->Height = outputH;
	outimage->Type = image->Type;

	if (outimage->Type == GRAY)
		size = outimage->Width * outimage->Height;
	else if (outimage->Type == COLOR)
		size = outimage->Width * outimage->Height * 3;

	outimage->data = (unsigned short*)malloc(size);
	tempin = image->data;
	tempout = outimage->data;

	int in_centerX, out_centerX, in_centerY, out_centerY;
	in_centerX = inputW / 2;
	in_centerY = inputH / 2;
	out_centerX = outputW / 2;
	out_centerY = outputH / 2;

	int in_i, in_j, out_i, out_j;

	// first set default canvas background as black color
	for (i = 0; i<outputW*outputH; i++)
	{
		*(tempout + i) = 0;
	}

	//go over the out image
	for (i = 0; i < outputW; i++){
		for (j = 0; j < outputH; j++){
			out_i = i - out_centerX;
			out_j = j - out_centerY;
			in_i = (int)(out_i)+in_centerX;
			in_j = (int)(out_j + v_para*out_i) + in_centerY;

			if (in_i >= 0 && in_i < inputW && in_j >= 0 && in_j < inputH){
				*(tempout + j*outputW + i) = *(tempin + in_j*inputW + in_i);
			}
		}
	}
	return(outimage);


}
Image *RotateImage(Image *image, int angle){
	unsigned short* tempin, *tempout;
	int i, j, size;

	Image* outimage;
	outimage = (Image*)malloc(sizeof(Image));

	int inputW = image->Width, inputH = image->Height;
	// get the result image size, size of canvas
	int outputW = inputW, outputH = inputH;

	outimage->Width = outputW;
	outimage->Height = outputH;
	outimage->Type = image->Type;

	if (outimage->Type == GRAY)
		size = outimage->Width * outimage->Height;
	else if (outimage->Type == COLOR)
		size = outimage->Width * outimage->Height * 3;

	outimage->data = (unsigned short*)malloc(size);
	tempin = image->data;
	tempout = outimage->data;
	
	double Angle;
	Angle = 1.0 * PI * angle / 180;

	int in_centerX, out_centerX, in_centerY, out_centerY;
	in_centerX = inputW / 2;
	in_centerY = inputH / 2;
	out_centerX = outputW / 2;
	out_centerY = outputH / 2;

	int in_i, in_j, out_i, out_j;
	
	// first set default canvas background as black color
	for (i = 0; i<outputW*outputH; i++)
	{
		*(tempout + i) = 0;
	}
	
	//go over the out image
	for (i = 0; i < inputW; i++){
		for (j = 0; j < inputH; j++){
			out_i = i - out_centerX;
			out_j = j - out_centerY;
			in_i = (int)((cos((double)Angle)*out_i - sin((double)Angle)*out_j)-0.5)+ in_centerX;
			in_j = (int)((sin((double)Angle)*out_i + cos((double)Angle)*out_j)-0.5)+ in_centerY;

			if (in_i >= 0 && in_i < inputW && in_j >= 0 && in_j < inputH){
				*(tempout + j*outputW + i) = *(tempin + in_j*inputW + in_i);
			}
		}
	}
	return(outimage);

}
Image *TranslateImage(Image *image, int dx, int dy){// dx>0:move right, dy>0:move down
	unsigned short* tempin, *tempout;
	int i,j,k,m,n, size;
	Image* outimage;
	outimage = (Image*)malloc(sizeof(Image));

	int inputW = image->Width, inputH = image->Height;
	// get the result image size, size of canvas
	int outputW = inputW + abs(dx), outputH = inputH + abs(dy);

	outimage->Width = outputW;
	outimage->Height = outputH;
	outimage->Type = image->Type;

	if (outimage->Type == GRAY)
		size = outimage->Width * outimage->Height;
	else if (outimage->Type == COLOR)
		size = outimage->Width * outimage->Height * 3;

	outimage->data = (unsigned short*)malloc(5*size);// if given exactly size of the output image, when dx>0 and dy>0, it will oversize this array.
	tempin = image->data;
	tempout = outimage->data;
	// first set default canvas background as black color
	for (i = 0; i<outputW*outputH; i++)
	{
		*(tempout+i) = 0;
	}
	// move tempout to the destination position
	for (m = 1; m <= outputW*max(0, dy) + max(0, dx); m++){
		tempout++;
	}
	
	// go over the original image
	for (j = 0; j <= inputH - 1; j++){// j is original image row number
		for (k = 0; k <= inputW - 1; k++){// k is original image col number
			*tempout = *(tempin+inputW*j + k);
			tempout++;
		
		}
		
		// jump over the black part
		for (n = 1; n <= abs(dx); n++){
			tempout++;
		}
	}
	return(outimage);
}
Image *AdaptiveMedianImage(Image *image, int f){
	unsigned short *tempin, *tempout;
	int i, j, k, w, size;
	int m = 1;
	int n = 0;
	unsigned short temp_exchange;
	Image *outimage;

	outimage = CreateNewImage(image, "#testing Swap");
	tempin = image->data;
	tempout = outimage->data;


	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;

	int Width = image->Width;
	int Height = image->Height;

	int filter_size = f*f;
	// the smaller size is optional, doing avg filter would reduce the image size.
	/*
	// change outimage size
	outimage->Height = Height - f + 1;
	outimage->Width = Width - f + 1;
	*/
	
	// move the center point to the center of first filter
	// move the original data location, change *tempin
	// the boundary remain the original pixel value
	for (i = 0; i < Width*(f - 1) / 2 + (f - 1) / 2; i++){
		*(tempout) = *(tempin);// the boundary ramain the same value
		tempin++;
		tempout++;
	}
	// data *tempin is now the center data for first filter, start loop
	for (j = Width*(f - 1) / 2 + (f - 1) / 2; j <= (Width*Height - 1) - (Width*(f - 1) / 2 + (f - 1) / 2); j++){
		if (m>Width - f + 1){// m and n is used to jump over the boundary data
			*(tempout) = *tempin;// the boundary ramain the same value
			n = n + 1;
		}
		else{
			
			for (int temp_windowSize = 3; temp_windowSize <= f; temp_windowSize++){
				// define the temp filter to save a f*f filter data
				unsigned short* tempfilter;
				tempfilter = (unsigned short*)malloc(f*f*sizeof(unsigned short));
				for (k = 0; k < temp_windowSize*temp_windowSize; k++){
					*(tempfilter + k) = *(tempin - (temp_windowSize - 1) / 2 - Width + (k%temp_windowSize) + ((int)(k / temp_windowSize))*Width);
				}

				// use bubble sort to sort filter data
				for (int p = 0; p < temp_windowSize*temp_windowSize - 1; p++){
					for (int q = 0; q < temp_windowSize*temp_windowSize - 1 - p; q++){
						if (*(tempfilter + q) > *(tempfilter + q + 1)){
							temp_exchange = *(tempfilter + q);
							*(tempfilter + q) = *(tempfilter + q + 1);
							*(tempfilter + q + 1) = temp_exchange;
						}
					}
				}
				// do checking if go to stage B
				// if median > min and median < max: goto B
				if (*(tempfilter + (temp_windowSize*temp_windowSize - 1) / 2) > *(tempfilter) && *(tempfilter + (temp_windowSize*temp_windowSize - 1) / 2) < *(tempfilter + (temp_windowSize*temp_windowSize - 1))){
					// stage B
					// if Zxy > min and Zxy < max: remain same value of (x,y)
					if (*tempin > *(tempfilter) && *tempin < *(tempfilter + (temp_windowSize*temp_windowSize - 1))){
						*(tempout) = *tempin;
					}
					else{
						*(tempout) = *(tempfilter + (temp_windowSize*temp_windowSize - 1) / 2);
					}
				}
				if (temp_windowSize == f){
					*(tempout) = *(tempfilter + (temp_windowSize*temp_windowSize - 1) / 2); 
				}


			}
			
		
		}
		tempout++;
		tempin++;
		m++;

		if (n == f - 1){
			m = 1;
			n = 0;
		}
	}
	// the boundary remain the original pixel value
	for (w = (Width*Height - 1) - (Width*(f - 1) / 2 + (f - 1) / 2) + 1; w <= (Width*Height - 1); w++){
		*(tempout) = *(tempin);// the boundary ramain the same value
		tempin++;
		tempout++;
	}
	return(outimage);
}
Image *AlphaMeanImage(Image *image, int f,int d){
	unsigned short *tempin, *tempout;
	int i, j, k, w, size;
	int m = 1;
	int n = 0;
	int index_low = (d / 2);
	int index_high = (f*f) - (d / 2);
	unsigned short temp_exchange;
	Image *outimage;

	outimage = CreateNewImage(image, "#testing Swap");
	tempin = image->data;
	tempout = outimage->data;


	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;

	int Width = image->Width;
	int Height = image->Height;

	int filter_size = f*f;
	// the smaller size is optional, doing avg filter would reduce the image size.
	/*
	// change outimage size
	outimage->Height = Height - f + 1;
	outimage->Width = Width - f + 1;
	*/
	// define the temp filter to save a f*f filter data
	unsigned short* tempfilter;
	tempfilter = (unsigned short*)malloc(f*f*sizeof(unsigned short));
	// move the center point to the center of first filter
	// move the original data location, change *tempin
	// the boundary remain the original pixel value
	for (i = 0; i < Width*(f - 1) / 2 + (f - 1) / 2; i++){
		*(tempout) = *(tempin);// the boundary ramain the same value
		tempin++;
		tempout++;
	}
	// data *tempin is now the center data for first filter, start loop
	for (j = Width*(f - 1) / 2 + (f - 1) / 2; j <= (Width*Height - 1) - (Width*(f - 1) / 2 + (f - 1) / 2); j++){
		if (m>Width - f + 1){// m and n is used to jump over the boundary data
			*(tempout) = *tempin;// the boundary ramain the same value
			n = n + 1;
		}
		else{
			for (k = 0; k < f*f; k++){
				*(tempfilter + k) = *(tempin - (f - 1) / 2 - Width + (k%f) + ((int)(k / f))*Width);
			}

			// use bubble sort to sort filter data
			for (int p = 0; p < f*f - 1; p++){
				for (int q = 0; q < f*f - 1 - p; q++){
					if (*(tempfilter + q) > *(tempfilter + q + 1)){
						temp_exchange = *(tempfilter + q);
						*(tempfilter + q) = *(tempfilter + q + 1);
						*(tempfilter + q + 1) = temp_exchange;
					}
				}
			}
			int temp_mean = 0;
			for (int it = index_low; it < index_high; it++){
				temp_mean = temp_mean + (*(tempfilter + it));
			}
			*tempout = (1.0/(f*f - d))*temp_mean;// output the mean
		}
		tempout++;
		tempin++;
		m++;

		if (n == f - 1){
			m = 1;
			n = 0;
		}
	}
	// the boundary remain the original pixel value
	for (w = (Width*Height - 1) - (Width*(f - 1) / 2 + (f - 1) / 2) + 1; w <= (Width*Height - 1); w++){
		*(tempout) = *(tempin);// the boundary ramain the same value
		tempin++;
		tempout++;
	}
	return(outimage);

}

Image *GeoAvgImage(Image *image, int f){
	unsigned short *tempin, *tempout;
	int i, j, k, p, size;
	int m = 1;
	int n = 0;
	Image *outimage;
	outimage = CreateNewImage(image, "#testing Swap");
	tempin = image->data;
	tempout = outimage->data;
	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;
	int Width = image->Width;
	int Height = image->Height;
	int filter_size = f*f;
	
	// define the temp filter to save a f*f filter data
	unsigned short* tempfilter;
	tempfilter = (unsigned short*)malloc(f*f*sizeof(unsigned short));
	// move the center point to the center of first filter
	// move the original data location, change *tempin
	// the boundary remain the original pixel value
	for (i = 0; i < Width*(f - 1) / 2 + (f - 1) / 2; i++){
		*(tempout) = *(tempin);// the boundary ramain the same value
		tempin++;
		tempout++;
	}
	// data *tempin is now the center data for first filter, start loop
	for (j = Width*(f - 1) / 2 + (f - 1) / 2; j <= (Width*Height - 1) - (Width*(f - 1) / 2 + (f - 1) / 2); j++){
		if (m>Width - f + 1){// m and n is used to jump over the boundary data
			*(tempout) = *tempin;// the boundary ramain the same value
			n = n + 1;
		}
		else{
			double tempavg = 1.0; //this is average output for an array
			for (k = 0; k < f*f; k++){
				*(tempfilter + k) = *(tempin - (f - 1) / 2 - Width + (k%f) + ((int)(k / f))*Width);
				tempavg = tempavg * pow(*(tempfilter + k), (1.0 / (f*f)));
			}
			//printf("RES: %f\t", tempavg);
			//tempavg = (int)(pow(tempavg , 1.0/(f*f))); //use for geometric avg filter
			*tempout = (unsigned short)(tempavg+0.5);
			//printf("%d\n", (int)(*tempout));
		}
		tempout++;
		tempin++;
		m++;
		if (n == f - 1){
			m = 1;
			n = 0;
		}
	}
	// the boundary remain the original pixel value
	for (p = (Width*Height - 1) - (Width*(f - 1) / 2 + (f - 1) / 2) + 1; p <= (Width*Height - 1); p++){
		*(tempout) = *(tempin);// the boundary ramain the same value
		tempin++;
		tempout++;
	}
	return(outimage);
}

Image *AvgImage(Image *image, int f){
	unsigned short *tempin, *tempout;
	int i, j, k,p, size;
	int m = 1;
	int n = 0;

	Image *outimage;

	outimage = CreateNewImage(image, "#testing Swap");
	tempin = image->data;
	tempout = outimage->data;


	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;

	int Width = image->Width;
	int Height = image->Height;

	int filter_size = f*f;
	// the smaller size is optional, doing avg filter would reduce the image size.
	/*
	// change outimage size
	outimage->Height = Height - f + 1;
	outimage->Width = Width - f + 1;
	*/
	// define the temp filter to save a f*f filter data
	unsigned short* tempfilter;
	tempfilter = (unsigned short*)malloc(f*f*sizeof(unsigned short));
	// move the center point to the center of first filter
	// move the original data location, change *tempin
	// the boundary remain the original pixel value
	for (i = 0; i < Width*(f - 1) / 2 + (f - 1) / 2; i++){
		*(tempout) = *(tempin);// the boundary ramain the same value
		tempin++;
		tempout++;
	}
	// data *tempin is now the center data for first filter, start loop
	for (j = Width*(f - 1) / 2 + (f - 1) / 2; j <= (Width*Height - 1) - (Width*(f - 1) / 2 + (f - 1) / 2); j++){
		if (m>Width - f + 1){// m and n is used to jump over the boundary data
			*(tempout) = *tempin;// the boundary ramain the same value
			n = n + 1;
		}
		else{
			int tempavg = 0; //this is average output for an array
			for (k = 0; k < f*f; k++){
				*(tempfilter+k) = *(tempin - (f - 1) / 2 - Width + (k%f) + ((int)(k / f))*Width);
				tempavg = tempavg + *(tempfilter + k);
			}
			tempavg = tempavg / (f*f); //use for avg filter
			*tempout = tempavg;
			
		}
		tempout++;
		tempin++;
		m++;
		if (n == f - 1){
			m = 1;
			n = 0;
		}
	}
	// the boundary remain the original pixel value
	for (p = (Width*Height - 1) - (Width*(f - 1) / 2 + (f - 1) / 2) + 1; p <= (Width*Height - 1); p++){
		*(tempout) = *(tempin);// the boundary ramain the same value
		tempin++;
		tempout++;
	}
	return(outimage);

}

Image *MedianImage(Image *image, int f){
	unsigned short *tempin, *tempout;
	int i, j, k, w, size;
	int m = 1;
	int n = 0;
	unsigned short temp_exchange;
	Image *outimage;

	outimage = CreateNewImage(image, "#testing Swap");
	tempin = image->data;
	tempout = outimage->data;


	if (image->Type == GRAY)   size = image->Width * image->Height;
	else if (image->Type == COLOR) size = image->Width * image->Height * 3;

	int Width = image->Width;
	int Height = image->Height;

	int filter_size = f*f;
	// the smaller size is optional, doing avg filter would reduce the image size.
	/*
	// change outimage size
	outimage->Height = Height - f + 1;
	outimage->Width = Width - f + 1;
	*/
	// define the temp filter to save a f*f filter data
	unsigned short* tempfilter;
	tempfilter = (unsigned short*)malloc(f*f*sizeof(unsigned short));
	// move the center point to the center of first filter
	// move the original data location, change *tempin
	// the boundary remain the original pixel value
	for (i = 0; i < Width*(f - 1) / 2 + (f - 1) / 2; i++){
		*(tempout) = *(tempin);// the boundary ramain the same value
		tempin++;
		tempout++;
	}
	// data *tempin is now the center data for first filter, start loop
	for (j = Width*(f - 1) / 2 + (f - 1) / 2; j <= (Width*Height - 1) - (Width*(f - 1) / 2 + (f - 1) / 2); j++){
		if (m>Width - f + 1){// m and n is used to jump over the boundary data
			*(tempout) = *tempin;// the boundary ramain the same value
			n = n + 1;
		}
		else{
			for (k = 0; k < f*f; k++){
				*(tempfilter + k) = *(tempin - (f - 1) / 2 - Width + (k%f) + ((int)(k / f))*Width);
			}
			
			// use bubble sort to sort filter data
			for (int p = 0; p < f*f-1; p++){
				for (int q = 0; q < f*f-1 - p; q++){
					if (*(tempfilter+q) > *(tempfilter+q + 1)){
						temp_exchange = *(tempfilter+q);
						*(tempfilter+q) = *(tempfilter+q + 1);
						*(tempfilter+q + 1) = temp_exchange;
					}
				}
			}

			*tempout = *(tempfilter+(f*f-1)/2);// output the median data
		}
		tempout++;
		tempin++;
		m++;

		if (n == f - 1){
			m = 1;
			n = 0;
		}
	}
	// the boundary remain the original pixel value
	for (w = (Width*Height - 1) - (Width*(f - 1) / 2 + (f - 1) / 2) + 1; w <= (Width*Height - 1); w++){
		*(tempout) = *(tempin);// the boundary ramain the same value
		tempin++;
		tempout++;
	}
	return(outimage);

}
// change SwapImage to do processing
Image *SwapImage(Image *image)
{
	unsigned short *tempin, *tempout;
	int i, size;


	Image *outimage;

	 outimage=CreateNewImage(image,"#testing Swap");
	 tempin=image->data;
	 tempout=outimage->data;


	 if(image->Type==GRAY)   size  = image->Width * image->Height;
	 else if(image->Type==COLOR) size  = image->Width * image->Height * 3;

	 int Width = image->Width;
	 int Height = image->Height;


	 for (i = 0; i<size; i++)
	 {
		 *tempout = *tempin;
		 printf("%c", *tempout);

		 tempin++;
		 tempout++;
	 }


	 return(outimage);

}

/*******************************************************************************/
//Read PPM image and return an image pointer                                   
/**************************************************************************/
Image *ReadPNMImage(char *filename)
{
  char ch;
  int  maxval, Width, Height;
  int size, num,j;
  FILE *fp;
  Image *image;
  int num_comment_lines=0;


  image=(Image *)malloc(sizeof(Image));

  if((fp=fopen(filename,"rb"))==NULL){ // read rb, 二进制图像， read r ，字符串
    printf("Cannot open %s\n", filename);
    exit(0);
  }

  printf("Loading %s ...",filename);

  if (fscanf(fp, "P%c\n", &ch) != 1) {
    printf("File is not in ppm/pgm raw format; cannot read\n");
    exit(0);
  }
  if( ch != '6' && ch !='5') {
    printf("File is not in ppm/pgm raw format; cannot read\n");
    exit(0);
  }

  if(ch == '5')image->Type=GRAY;  // Gray (pgm)
  else if(ch=='6')image->Type=COLOR;  //Color (ppm)
  /* skip comments */
  ch = getc(fp);
  j=0;
  while (ch == '#')
    {
    image->comments[num_comment_lines][j]=ch;
    j++;
      do {
        ch = getc(fp);
        image->comments[num_comment_lines][j]=ch;
        j++;
      } while (ch != '\n');     /* read to the end of the line */
        image->comments[num_comment_lines][j-1]='\0';
        j=0;
        num_comment_lines++;
      ch = getc(fp);            /* thanks, Elliot */
    }

  if (!isdigit((int)ch)){
    printf("Cannot read header information from ppm file");
    exit(0);
  }

  ungetc(ch, fp);               /* put that digit back */

  /* read the width, height, and maximum value for a pixel */
  fscanf(fp, "%d%d%d\n", &Width, &Height, &maxval);

  /*
  if (maxval != 255){
    printf("image is not true-color (24 bit); read failed");
    exit(0);
  }
  */

  if(image->Type==GRAY)
        size          = Width * Height*2;
  else  if(image->Type==COLOR)
        size          = Width * Height *3;
  image->data   = (unsigned short *) malloc(size);
  image->Width  = Width;
  image->Height = Height;
  image->num_comment_lines= num_comment_lines;

  if (!image->data){
    printf("cannot allocate memory for new image");
    exit(0);
  }
  int i;
  unsigned short *data16 = (unsigned short *)image->data; // store data as unsigned short
  for (i = 0; i < size; i += 2) {
	  unsigned char c1 = fgetc(fp);
	  unsigned char c2 = fgetc(fp);
	  data16[i / 2] = ((unsigned short)c1) << 8 | c2; // combine two bytes to form an unsigned short value
  }
  //num = fread((void *) image->data, 1, (size_t) size, fp);
  //printf("Complete reading of %d  %d bytes \n", num, size);
  /*
  if (num != size){
    printf("cannot read image data from file");
    exit(0);
  }
  */
  //for(j=0;j<image->num_comment_lines;j++){
  //      printf("%s\n",image->comments[j]);
  //      }

  fclose(fp);

  /*-----  Debug  ------*/

  if(image->Type==GRAY)printf("..Image Type PGM\n");
  else printf("..Image Type PPM Color\n");
  /*
  printf("Width %d\n", Width);
  printf("Height %d\n",Height);
  printf("Size of image %d bytes\n",size);
  printf("maxvalue %d\n", maxval);
  */
  return(image);
}

void SavePNMImage(Image *temp_image, char *filename)
{
  int num,j;
  int size ;
  FILE *fp;
  //char comment[100];


  printf("Saving Image %s\n", filename);
  fp=fopen(filename, "wb");// write in binary format
  if (!fp){
    printf("cannot open file for writing");
    exit(0);
  }

  //strcpy(comment,"#Created by Dr Mohamed N. Ahmed");

  if(temp_image->Type==GRAY){  // Gray (pgm)
        fprintf(fp,"P5\n");
        size = temp_image->Width * temp_image->Height*2;
  }
  else  if(temp_image->Type==COLOR){  // Color (ppm)
        fprintf(fp,"P6\n");
        size = temp_image->Width * temp_image->Height*3;
  }

  for(j=0;j<temp_image->num_comment_lines;j++)
        fprintf(fp,"%s\n",temp_image->comments[j]);

  fprintf(fp, "%d %d\n%d\n", temp_image->Width, temp_image->Height, 65535);
  int i;
  unsigned short *data16 = (unsigned short *)temp_image->data;
  for (i = 0; i < size / 2; i++) {
	  unsigned short pixel = data16[i];
	  unsigned char c1 = (pixel >> 8) & 0xFF; // get the upper 8 bits of the 16-bit value
	  unsigned char c2 = pixel & 0xFF; // get the lower 8 bits of the 16-bit value
	  fputc(c1, fp);
	  fputc(c2, fp);
  }
  /*
  num = fwrite((void *) temp_image->data, 2, (size_t) (size/2), fp);

  if (num != size/2){
    printf("cannot write image data to file");
    exit(0);
  }
  */
  fclose(fp);
}

/*************************************************************************/
/*Create a New Image with same dimensions as input image                 */
/*************************************************************************/

Image *CreateNewImage(Image * image, char *comment)
{
        Image *outimage;
        int size,j;

        outimage=(Image *)malloc(sizeof(Image));

        outimage->Type  =image->Type;
        if(outimage->Type==GRAY)   size  = image->Width * image->Height*2;
        else if(outimage->Type==COLOR) size  = image->Width * image->Height * 3;

        outimage->Width =image->Width;
        outimage->Height=image->Height;
        outimage->num_comment_lines=image->num_comment_lines;

        /*--------------------------------------------------------*/
        /* Copy Comments for Original Image      */
        for(j=0;j<outimage->num_comment_lines;j++)
                strcpy(outimage->comments[j],image->comments[j]);

        /*----------- Add New Comment  ---------------------------*/
        strcpy(outimage->comments[outimage->num_comment_lines],comment);
        outimage->num_comment_lines++;


        outimage->data   = (unsigned short *) malloc(size);
        if (!outimage->data){
                printf("cannot allocate memory for new image");
                exit(0);
        }
        return(outimage);
}

