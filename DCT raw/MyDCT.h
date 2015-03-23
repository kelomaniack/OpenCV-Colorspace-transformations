#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h> 
#include <vector>
#include <boost/lexical_cast.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

#include "gFreeImage.h"

#define N 512
#define groups (int)(N / 8)

using namespace std;
using namespace cv;

namespace malakia{
	
	class MyDCT
	{
	public:
		MyDCT();
		~MyDCT(void);
	public:
		void init();
		void DCT(char* filename);
		void IDCT();
		void quantization();
		void MSE();


		gFreeImage img;

		double COS[8][8], C[8];
		
		double dct[N][N], idct[N][N];

		unsigned char pic[N][N];
		unsigned char *data;

		int width;
		int height;

	};
}