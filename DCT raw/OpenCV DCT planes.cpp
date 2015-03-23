//#include <iostream> 
//#include <opencv2/highgui/highgui.hpp> 
//using namespace std; 
//using namespace cv; 
//
//int main(int argc, char ** argv) { 
//	const char *fileName = "baboon.png"; 
//	if (argc > 1) { fileName = argv[1]; } 
//	
//	// 
//	// For your reference: Some image types of interest: 
//	// 
//	cout << "CV_8UC1 = " << CV_8UC1 << endl; 
//	cout << "CV_8UC3 = " << CV_8UC3 << endl; 
//	cout << "CV_32FC1 = " << CV_32FC1 << endl; 
//	cout << "CV_32FC3 = " << CV_32FC3 << endl; 
//	cout << "CV_64FC1 = " << CV_64FC1 << endl; 
//	cout << "CV_64FC3 = " << CV_64FC3 << endl; 
//	
//	Mat originalImage = imread(fileName, 1); 
//	
//	if (originalImage.empty() || (originalImage.data == NULL)) { 
//		cout << "Can't load image from " << fileName << "!" << endl; exit(EXIT_FAILURE); 
//	} 
//	
//	cout << "Working on image from " << fileName << endl; 
//	cout << "Original image type = " << originalImage.type() << endl; 
//	
//	// Split the image into its three planes 
//	vector<Mat> planes; 
//	split(originalImage, planes); 
//	
//	// Convert each plane to a type suitable for cv.dct(),
//	// and do the transform on each one. 
//	vector<Mat> outplanes(planes.size()); 
//	cout << " Individual input planes have type = " << planes[0].type() << endl; 
//	
//	for (size_t i = 0; i < planes.size(); i++) { 
//		planes[i].convertTo(planes[i], CV_32FC1); 
//		dct(planes[i], outplanes[i]); 
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
//	namedWindow("Original", CV_WINDOW_AUTOSIZE); 
//	
//	int x = 0; 
//	int y = 0; 
//	
//	moveWindow("Original", x, y); 
//	imshow("Original", originalImage); 
//	
//	// To display the DCT of the individual planes, uncomment the 
//	// following #define directive. 
//	////#define SHOW_DCT_PLANES 1 
//#ifdef SHOW_DCT_PLANES 
//	
//	x += 100; y += 100; 
//	namedWindow("DCT Image[0]", CV_WINDOW_AUTOSIZE); 
//	moveWindow("DCT Image[0]", x, y); 
//	imshow("DCT Image[0]", outplanes[0]); 
//	
//	x += 100;
//	y += 100; 
//	namedWindow("DCT Image[1]", CV_WINDOW_AUTOSIZE); 
//	moveWindow("DCT Image[1]", x, y); 
//	imshow("DCT Image[1]", outplanes[1]); 
//	
//	x += 100; 
//	y += 100; 
//	namedWindow("DCT Image[2]", CV_WINDOW_AUTOSIZE); 
//	moveWindow("DCT Image[2]", x, y); 
//	imshow("DCT Image[2]", outplanes[2]); 
//#endif 
//	
//	x += 100; 
//	y += 100; 
//	namedWindow("Merged DCT", CV_WINDOW_AUTOSIZE); 
//	moveWindow("Merged DCT", x, y); 
//	imshow("Merged DCT", merged); 
//	
//	// Start with the merged image and go the other way: 
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
//	namedWindow("Reconstituted Image", CV_WINDOW_AUTOSIZE); 
//	moveWindow("Reconstituted Image", x, y);
//	imshow("Reconstituted Image", remerged); 
//	waitKey(0); 
//	
//	destroyAllWindows(); 
//}