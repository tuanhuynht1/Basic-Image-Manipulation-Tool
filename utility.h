#ifndef UTILITY_H
#define UTILITY_H

#include "image.h"
#include <sstream>
#include <math.h>

class utility
{
	public:
		utility();
		virtual ~utility();
		static std::string intToString(int number);
		static int checkValue(int value);
		static void addGrey(image &src, image &tgt, int value);
		static void binarize(image &src, image &tgt, int threshold);
		static void scale(image &src, image &tgt, float ratio);
		//HW0 new function
		static void addThreshold(image &src, image &tgt, int threshold, int value);

		//HW1 test function
		static void whiteOut(image &src, image &tgt, vector<Region> regions);
		//HW1 Thresholding function
		static void thresholding(image &tgt, int t1, int t2, Region roi);
		//HW1 Color Binary function
		static void colorBinarization(image &tgt, Color c, int tc, int dc, Region roi);
		static float colorDistance(Color A, Color B);
		//HW1 smoothing helper
		static Region getSquareWindow(image &img, int ws, int i, int j);
		static Region get1DWindow(image &img, int ws, dimension dim, int i, int j);
		static int averageIntensity(image &img, Region window);
		//HW1 2D smoother
		static void twoDimensionalSmoothing(image &tgt, int ws, Region roi);
		//HW1 1D smoother 
		static void oneDimensionalSmoothing(image &tgt, int ws, Region roi);

};

#endif

