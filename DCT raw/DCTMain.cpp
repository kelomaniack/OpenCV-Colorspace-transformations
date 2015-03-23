//#include <stdio.h>
//
//#include "MyDCT.h"
//
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include "opencv2/imgproc/imgproc.hpp"
//#include <iostream>
//using namespace std;
//using namespace malakia;
//
//int main() {
//
//	MyDCT *dct = new MyDCT;
//
//	/*freopen("in512x512.png", "rb", stdin);
//	int r, c;
//	for (r = 0; r < N; r++)
//	for (c = 0; c < N; c++)
//		scanf("%c", &(dct->pic[r][c]));*/
//
//	//cv::Mat image;
//	//image = imread("C:\\ImageQualityPictures\\lena.bmp", CV_LOAD_IMAGE_COLOR);
//
//	//if (!image.data)                              // Check for invalid input
//	//{
//	//	cout << "Could not open or find the image" << std::endl;
//	//	return -1;
//	//}
//	//
//	//namedWindow("Display window", WINDOW_NORMAL);// Create a window for display.
//	//imshow("Display window", image);                   // Show our image inside it.
//
//	//waitKey(0);
//
//	dct->init();
//	dct->DCT("in512x512.png");
//	dct->quantization();
//	dct->IDCT();
//	dct->MSE();
//
//	system("pause");
//
//	return 0;
//}