//#include <iostream> 
//#include <opencv2/highgui/highgui.hpp> 
//using namespace std;
//using namespace cv;
//
//int main()
//{
//	IplImage *input, *output, *b, *g, *r;
//	input = cvLoadImage("dolphin.jpg");
//
//	Mat myInput(input);
//	Mat img2;
//
//	myInput.convertTo(img2, CV_32FC1);
//
//	int width, height;
//	width = input->width;
//	height = input->height;
//	b = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
//	g = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
//	r = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
//	cvSplit(input, b, g, r, NULL);
//	cvNamedWindow("blue", CV_WINDOW_AUTOSIZE);
//	IplImage *b_dct, *g_dct, *r_dct;
//	b_dct = cvCreateImage(cvSize(width, height), 8, 1);
//	g_dct = cvCreateImage(cvSize(width, height), 8, 1);
//	r_dct = cvCreateImage(cvSize(width, height), 8, 1);
//	cvDCT(b, b_dct, 0);  // doubt??
//	cvShowImage("blue", b_dct);
//
//}