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
		//HW0
		static void addThreshold(image &src, image &tgt, int threshold, int value);
		//HW1 functions of ROI
		static void whiteOut(image &src, image &tgt, vector<Region> regions);
};

#endif

