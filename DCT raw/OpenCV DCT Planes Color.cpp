//#include <stdio.h>
//
//#include "MyDCT.h"
//
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include "opencv2/imgproc/imgproc.hpp"
//
//int main() {
//
//	// Read image
//	Mat img = imread("IMG_1127.png", 1);
//
//	// Make sure the both image dimensions are a multiple of 2
//	Mat img2;
//	int w = img.cols;
//	int h = img.rows;
//	int w2, h2;
//	if (w % 2 == 0)
//		w2 = w;
//	else
//		w2 = w + 1;
//	if (h % 2 == 0)
//		h2 = h;
//	else
//		h2 = h + 1;
//	copyMakeBorder(img, img2, 0, h2 - h, 0, w2 - w, IPL_BORDER_REPLICATE);
//
//	// Split the image into its three planes 
//	vector<Mat> planes;
//	split(img, planes);
//
//	// Convert each plane to a type suitable for cv.dct(),
//	// and do the transform on each one. 
//	vector<Mat> outplanes(planes.size());
//	cout << " Individual input planes have type = " << planes[0].type() << endl;
//
//	for (size_t i = 0; i < planes.size(); i++) {
//		planes[i].convertTo(planes[i], CV_64F);
//		dct(planes[i], outplanes[i]);
//		
//	}
//
//	cout << " Individual DCT planes have type = " << outplanes[0].type() << endl;
//
//	// Now put the planes together into a single image Mat merged; 
//	Mat merged;
//	merge(outplanes, merged);
//	cout << "Merged DCT image has type = " << merged.type() << endl;
//
//	// 
//	// Show what we have so far 
//	// 
//	namedWindow("Original", CV_WINDOW_KEEPRATIO);
//
//	int x = 0;
//	int y = 0;
//
//	moveWindow("Original", x, y);
//	imshow("Original", img);
//	imwrite("C:\\tmp\\orig.png", img);
//	waitKey(0);
//
//#ifdef SHOW_DCT_PLANES 
//
//	x += 100; y += 100;
//	namedWindow("DCT Image[0]", CV_WINDOW_KEEPRATIO);
//	moveWindow("DCT Image[0]", x, y);
//	imshow("DCT Image[0]", outplanes[0]);
//
//	x += 100;
//	y += 100;
//	namedWindow("DCT Image[1]", CV_WINDOW_KEEPRATIO);
//	moveWindow("DCT Image[1]", x, y);
//	imshow("DCT Image[1]", outplanes[1]);
//
//	x += 100;
//	y += 100;
//	namedWindow("DCT Image[2]", CV_WINDOW_KEEPRATIO);
//	moveWindow("DCT Image[2]", x, y);
//	imshow("DCT Image[2]", outplanes[2]);
//#endif 
//
//	x += 100;
//	y += 100;
//	namedWindow("Merged DCT", CV_WINDOW_KEEPRATIO);
//	moveWindow("Merged DCT", x, y);
//	
//	imshow("Merged DCT", merged);
//	waitKey(0);
//
//	// Split into planes and do inverse DCT on each. 
//	split(merged, planes);
//
//	for (size_t i = 0; i < planes.size(); i++) {
//		idct(planes[i], outplanes[i]);
//		outplanes[i].convertTo(outplanes[i], CV_8UC1);
//	}
//
//	Mat remerged;
//	merge(outplanes, remerged);
//	cout << "Reconstituted image type = " << remerged.type() << endl;
//
//	x += 100;
//	y += 100;
//	namedWindow("Reconstituted Image", CV_WINDOW_KEEPRATIO);
//	moveWindow("Reconstituted Image", x, y);
//	imshow("Reconstituted Image", remerged);
//	imwrite("C:\\tmp\\IDCT.png", img);
//	waitKey(0);
//
//	destroyAllWindows();
//	
//}