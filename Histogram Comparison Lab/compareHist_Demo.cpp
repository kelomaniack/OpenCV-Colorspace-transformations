/**
* @file compareHist_Demo.cpp
* @brief Sample code to use the function compareHist for the L*u*v colorspace
* @author OpenCV team
*/

//#include "opencv2/imgcodecs.hpp"
	#include "opencv2/highgui/highgui.hpp"
	#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

#include <fstream>

using namespace std;
using namespace cv;


/**
* @function main
*/
int main(int argc, char** argv)
{
	Mat src_base, Lab_base;
	Mat src_test1, Lab_test1;
	Mat Lab_half_down;

	/// Load three images with different environment settings
	if (argc < 3)
	{
		printf("** Error. Usage: ./compareHist_Demo <image_settings0> <image_setting1>\n");
		return -1;
	}

	src_base = imread(argv[1], 1);
	src_test1 = imread(argv[2], 1);

	/// Convert to HSV
	cvtColor(src_base, Lab_base, COLOR_BGR2Lab);
	cvtColor(src_test1, Lab_test1, COLOR_BGR2Lab);
	
	Lab_half_down = Lab_base(Range(Lab_base.rows / 2, Lab_base.rows - 1), Range(0, Lab_base.cols - 1));

	/// Using 50 bins for hue and 60 for saturation
	int h_bins = 50; int s_bins = 60;
	int histSize[] = { h_bins, s_bins };

	// L varies from 0 to 100, u from -83.185 to 98.254 and v from 107.863 to 94.482
	float L_ranges[] = { 0, 100 };
	float u_ranges[] = { 0,255 };
	float v_ranges[] = { 0,255 };

	const float* ranges[] = { L_ranges, u_ranges, v_ranges };

	// Use the o-th and 1-st channels
	int channels[] = { 0, 1, 2};


	/// Histograms
	MatND hist_base;
	MatND hist_half_down;
	MatND hist_test1;

	/// Calculate the histograms for the L*u*v images
	calcHist(&Lab_base, 1, channels, Mat(), hist_base, 2, histSize, ranges, true, false);
	normalize(hist_base, hist_base, 0, 1, NORM_MINMAX, -1, Mat());

	calcHist(&Lab_half_down, 1, channels, Mat(), hist_half_down, 2, histSize, ranges, true, false);
	normalize(hist_half_down, hist_half_down, 0, 1, NORM_MINMAX, -1, Mat());

	calcHist(&Lab_test1, 1, channels, Mat(), hist_test1, 2, histSize, ranges, true, false);
	normalize(hist_test1, hist_test1, 0, 1, NORM_MINMAX, -1, Mat());

	ofstream myfile;
	myfile.open("example.txt");
	

	/// Apply the histogram comparison methods
	for (int i = 0; i < 4; i++)
	{
		int compare_method = i;
		double base_base = compareHist(hist_base, hist_base, compare_method);
		double base_half = compareHist(hist_base, hist_half_down, compare_method);
		double base_test1 = compareHist(hist_base, hist_test1, compare_method);

		printf(" Method [%d] Base-Base, Base-Half, Base-Test(1): %f, %f, %f\n", i, base_base, base_half, base_test1);
		myfile << " Method [" << i << "] Perfect, Base-Half, Base-Test(1), Base-Test(2) : " << "\t" << base_base << ",\t\t" << base_half << ",\t" << base_test1 << ",\t" << endl;
		
	}

	myfile.close();
	printf("Done \n");

	system("PAUSE");

	return 0;
}