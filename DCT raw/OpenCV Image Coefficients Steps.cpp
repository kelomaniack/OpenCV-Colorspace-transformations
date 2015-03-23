#include <stdio.h>

#include "MyDCT.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

int main() {
	// Read image
	Mat img = imread("baboon.png", CV_LOAD_IMAGE_GRAYSCALE);

	// Make sure the both image dimensions are a multiple of 2
	Mat img2;
	int w = img.cols;
	int h = img.rows;
	int w2, h2;
	if (w % 2 == 0)
		w2 = w;
	else
		w2 = w + 1;
	if (h % 2 == 0)
		h2 = h;
	else
		h2 = h + 1;
	copyMakeBorder(img, img2, 0, h2 - h, 0, w2 - w, IPL_BORDER_REPLICATE);

	// Grayscale image is 8bits per pixel,
	// but dct() method wants float values!
	Mat img3 = Mat(img2.rows, img2.cols, CV_64F);
	img2.convertTo(img3, CV_64F);
	imwrite("C:\\tmp\\orig.png", img3);

	// Let's do the DCT now: image => frequencies
	Mat freq;
	dct(img3, freq);

	// Save a visualization of the DCT coefficients
	imwrite("C:\\tmp\\dct.png", freq);

	for (int Start = 100; Start > 0; Start -= 1)
	{
		// Set some frequencies to 0
		for (int y = Start; y < freq.rows; y++)
		{
			for (int x = Start; x < freq.cols; x++)
			{
				freq.at<double>(y, x) = 0.0;
			}
		}

		// Do inverse DCT: (some low) frequencies => image
		Mat dst;
		idct(freq, dst);

		// Show frame nr
		char txt[100];
		sprintf(txt, "%04d", Start);
		cv::putText(dst, txt, Point(10, 20),
			CV_FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(0, 0, 0));

		// Save visualization of reconstructed image where
		//we have thrown away some of the high frequencies
		char fname[500];
		sprintf(fname, "C:\\tmp\\idct_%04d.png", Start);
		imwrite(fname, dst);
	}
}