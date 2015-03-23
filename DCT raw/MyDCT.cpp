#include "MyDCT.h"
#include <math.h>

using namespace malakia;


MyDCT::MyDCT(void){}
MyDCT::~MyDCT(void){}

void MyDCT::init() {
  int i, j;
  for (i = 0; i < 8; i++) {
    for (j = 0; j < 8; j++)
		COS[i][j] = cos((2 * i + 1) * j * acos(-1) / 16.0);
    if (i) C[i] = 1;
    else C[i] = 1 / sqrt(2);
  }
}

//Μπορώ να βάλω ως παραμέτρους
//char *fileName, int &width, int &height
//Στην main να κάνω πρώτα int LoadImage(char *fileName, int &width, int &height)
//και μετά να καλώ την DCT : DCT(fileName, width, height); ή DCT(fileName); Αφού θα έχει πάρει με αναφορά τα width και height
void MyDCT::DCT(char* filename) {
	
	//buf = NULL;

	/*gFreeImage img;
	if (!img.LoadImage(filename))
	{
		printf("load %s failed\n", filename);
		exit(0);
	}
	else
	{
		printf("img.width: %d\n", img.width);
		printf("img.height: %d\n", img.height);
		data = (unsigned char*)malloc(N * N * sizeof(unsigned char));
		data = img.getImageData(img.width, img.height);
		unsigned char(*pic)[N] = (unsigned char(*)[N])data;*/

	//}

	Mat image;
	image = imread(filename, CV_LOAD_IMAGE_UNCHANGED);
	//imshow("window", image);

	//waitKey(0);

	//if (!image.data)                              // Check for invalid input
	//	{
	//		cout << "Could not open or find the image" << std::endl;
	//		
	//	}
	//	
	//	namedWindow("Display window", WINDOW_NORMAL);// Create a window for display.
	//	imshow("Display window", image);                   // Show our image inside it.

	//	waitKey(0);
  //freopen("dct.raw", "wb", stdout);
  int r, c, i, j, x, y;
  for (r = 0; r < groups; r++)
  for (c = 0; c < groups; c++)
      for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++) {
          double sum = NULL;
          for (x = 0; x < 8; x++)
            for (y = 0; y < 8; y++)
				sum += (pic[r * 8 + x][c * 8 + y] - 128) * COS[x][i] * COS[y][j];
          sum *=(C[i] * C[j] * 0.25);
          dct[r * 8 + i][c * 8 + j] = sum;
      }
  /*for (r = 0; r < N; r++)
    for (c = 0; c < N; c++)
      putchar(dct[r][c]);*/

	/*FILE * f = fopen("dct.raw", "rb");
	if (!f)
	{
		printf("bad path : %s\n", "dct.raw");
		exit(0);
	}
	char pixels[512 * 512];
	fread(pixels, 512 * 512, 1, f);
	fclose(f);*/

	Mat A = Mat(512, 512, CV_32FC1, dct);

	try {
		imwrite("dct.png", A);

		//cvSaveImage("dct.bmp", &(IplImage(A)));
		exit(0);
	}
	catch (runtime_error& ex) {
		fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());

	}

	//////convert double[][] to unsigned char[][]
	//for (int i = 0; i < N; i++)
	//	for (int j = 0; j < N; j++)
	//		pic[i][j] = reinterpret_cast<unsigned char>(&dct[i][j]);

	////std::copy(std::begin(dct), std::end(dct), std::begin(pic));

	////convert unsigned char[][] to string
	//string bar;
	//bar = "";
	//for (int i = 0; i< N; i++)
	//{
	//	for (int j = 0; j < N; j++)
	//	{
	//		bar += pic[i][j];
	//	}
	//}

	////convert unsigned char[][] to const char* and then to char*
	//const char *writable = bar.c_str();
	//char* str = const_cast<char*>(writable);

	////convert finally char* to unsigned char*
	//vector<unsigned char> ustr(str, str + std::strlen(str));
	//unsigned char * ustrptr = ustr.data();
	//
	////unsigned char * writable = new unsigned char[bar.size() + 1];
	////copy(bar.begin(), bar.end(), writable);
	////writable[bar.size()] = '\0'; // don't forget the terminating 0


	/*if (!img.SaveImage("dct.png", ch, N, N)){
		printf("load %s failed\n", filename);
		exit(0);
	}
	else {
		printf("Image is saved to dct.png!");
		
	}*/
		
	/*Mat A = Mat(512,512, CV_32FC1, pic);

		try {
			imwrite("dct.png", A);
		}
		catch (runtime_error& ex) {
			fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
			
		}*/
	// don't forget to free the string after finished using it
	//delete[] writable;
}

void MyDCT::IDCT() {
  freopen("idct.raw", "wb", stdout);
  int r, c, i, j, x, y;
  for (r = 0; r < groups; r++)
  for (c = 0; c < groups; c++)
      for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++) {
          double sum = 0;
          for (x = 0; x < 8; x++)
            for (y = 0; y < 8; y++)
              sum += C[x] * C[y] * dct[r * 8 + x][c * 8 + y] * COS[i][x] * COS[j][y];
          sum *= 0.25;
          sum += 128;
          idct[r * 8 + i][c * 8 + j] = sum;
      }
  for (r = 0; r < N; r++)
    for (c = 0; c < N; c++)
      putchar(idct[r][c]);
}

void MyDCT::quantization() {
  int table[8][8] = {
    16, 11, 10, 16, 24, 40, 51, 61,
    12, 12, 14, 19, 26, 58, 60, 55,
    14, 13, 16, 24, 40, 57, 69, 56,
    14, 17, 22, 29, 51, 87, 80, 82,
    18, 22, 37, 56, 68, 109, 103, 77,
    24, 35, 55, 64, 81, 104, 113, 92,
    49, 64, 78, 87, 103, 121, 120, 101,
    72, 92, 95, 98, 112, 100, 103, 99
  };
  int r, c, i, j;
  for (r = 0; r < groups; r++)
  for (c = 0; c < groups; c++)
      for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++) {
          dct[r * 8 + i][c * 8 + j] = round(dct[r * 8 + i][c * 8 + j] / table[i][j]);
          dct[r * 8 + i][c * 8 + j] = dct[r * 8 + i][c * 8 + j] * table[i][j];
        }
}

void MyDCT::MSE() {
  freopen("sub.raw", "wb", stdout);
  double MSE = 0;
  int r, c;
  for (r = 0; r < N; r++)
  for (c = 0; c < N; c++) {
      MSE += (pic[r][c] - idct[r][c]) * (pic[r][c] - idct[r][c]);
      putchar(abs(pic[r][c] - idct[r][c]));
    }
	MSE /= (N * N);
  double PSNR = 10 * log10(255 * 255 / MSE);
  freopen("PSNR.txt", "w", stdout);
  printf("%.2lf\n", PSNR);
}