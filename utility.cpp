#include "utility.h"

#define MAXRGB 255
#define MINRGB 0

std::string utility::intToString(int number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

int utility::checkValue(int value)
{
	if (value > MAXRGB)
		return MAXRGB;
	if (value < MINRGB)
		return MINRGB;
	return value;
}

/*-----------------------------------------------------------------------**/
void utility::addGrey(image &src, image &tgt, int value)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			tgt.setPixel(i,j,checkValue(src.getPixel(i,j)+value)); 
		}
}

/*-----------------------------------------------------------------------**/
void utility::binarize(image &src, image &tgt, int threshold)
{
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			if (src.getPixel(i,j) < threshold)
				tgt.setPixel(i,j,MINRGB);
			else
				tgt.setPixel(i,j,MAXRGB);
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::scale(image &src, image &tgt, float ratio)
{
	int rows = (int)((float)src.getNumberOfRows() * ratio);
	int cols  = (int)((float)src.getNumberOfColumns() * ratio);
	tgt.resize(rows, cols);
	for (int i=0; i<rows; i++)
	{
		for (int j=0; j<cols; j++)
		{	
			/* Map the pixel of new image back to original image */
			int i2 = (int)floor((float)i/ratio);
			int j2 = (int)floor((float)j/ratio);
			if (ratio == 2) {
				/* Directly copy the value */
				tgt.setPixel(i,j,checkValue(src.getPixel(i2,j2)));
			}

			if (ratio == 0.5) {
				/* Average the values of four pixels */
				int value = src.getPixel(i2,j2) + src.getPixel(i2,j2+1) + src.getPixel(i2+1,j2) + src.getPixel(i2+1,j2+1);
				tgt.setPixel(i,j,checkValue(value/4));
			}
		}
	}
}

//--------------------- New Function ----------------------------//
void utility::addThreshold(image &src, image &tgt, int threshold, int value){
	tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());
	for (int i=0; i<src.getNumberOfRows(); i++)
	{
		for (int j=0; j<src.getNumberOfColumns(); j++)
		{
			if (src.getPixel(i,j) >= threshold)
			{
				tgt.setPixel(i,j,checkValue(src.getPixel(i,j)+value));
			}
			else
			{
				tgt.setPixel(i,j,src.getPixel(i,j));
			}
			
		}
	}
}
//----------------------Helper Distance Function ----------------//
float utility::colorDistance(Color A, Color B){
	float R2 = pow(A.r - B.r,2);
	float G2 = pow(A.g - B.g,2);
	float B2 = pow(A.b - B.b,2);
	return sqrt(R2 + G2 + B2);
}
//--------------------- ROI Functions ----------------------------//
void utility::whiteOut(image &src, image &tgt, vector<Region> regions){
	//duplicates the source
	tgt.copyImage(src);
	for(auto roi : regions){
		for(int i = roi.i; i < roi.i + roi.ilen; i++){
			for(int j = roi.j; j < roi.j + roi.jlen; j++){
				//set to white
				tgt.setPixel(i,j,MAXRGB);
			}
		}
	}
}

void utility::thresholding(image &tgt, int t1, int t2, Region roi){
	for(int i = roi.i; i < roi.i + roi.ilen; i++){
		for(int j = roi.j; j < roi.j + roi.jlen; j++){
			int intensity = tgt.getPixel(i,j);
			if (intensity >= t1 && intensity < t2){
				//set to white
				tgt.setPixel(i,j,MAXRGB);
			}
			else{
				//set black
				tgt.setPixel(i,j,MINRGB);
			} 
		}
	}
	cout << t1 << t2 << endl;
}

void utility::colorBinarization(image &tgt, Color c, int tc, int dc, Region roi){
	for(int i = roi.i; i < roi.i + roi.ilen; i++){
		for(int j = roi.j; j < roi.j + roi.jlen; j++){
			//get color at point i,j
			Color pixelColor(tgt.getPixel(i,j,RED), tgt.getPixel(i,j,GREEN),tgt.getPixel(i,j,BLUE));
			//get distance from c
			float distance = colorDistance(c,pixelColor);
			//set to black if larger than threshold
			if(distance > tc){
				tgt.setPixel(i,j,RED,MINRGB);
				tgt.setPixel(i,j,GREEN,MINRGB);
				tgt.setPixel(i,j,BLUE,MINRGB);
			}
			//else, increase by dc
			else{
				tgt.setPixel(i,j,RED,checkValue(tgt.getPixel(i,j,RED) + dc));
				tgt.setPixel(i,j,GREEN,checkValue(tgt.getPixel(i,j,GREEN) + dc));
				tgt.setPixel(i,j,BLUE,checkValue(tgt.getPixel(i,j,BLUE) + dc));
			}
		}
	}
}
