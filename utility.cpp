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
//--------------------- Point Functions ----------------------------//
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

//----------------------------------------------------------- Localized Functions ------------------------//
Region utility::getSquareWindow(image &img, int ws, int i, int j){
	int offset = ws / 2;
	int imax = img.getNumberOfRows();
	int jmax = img.getNumberOfColumns();
	if(i + offset >= imax || i - offset < 0 || j + offset >= jmax || j - offset < 0){
		return Region(1,1,i,j);
	}
	else{
		return Region(ws, ws, i - offset, j - offset);
	}
}

Region utility::get1DWindow(image &img, int ws, dimension dim, int i, int j){
	int max, offset = ws / 2;
	switch (dim){
		case ROW:
			max = img.getNumberOfColumns();
			if (j + offset < max && j - offset >= 0){
				return Region(1, ws,i,j - offset);
			}
		case COL:
			max = img.getNumberOfRows();
			if (i + offset < max && i - offset >= 0){
				return Region(ws, 1,i - offset, j);
			}
	}
	return Region(1,1,i,j);
}

int utility::averageIntensity(image &img, Region window){
	int pixels = 0, sum = 0;
	for (int i = window.i; i < window.i + window.ilen; i++){
		for(int j = window.j; j < window.j + window.jlen; j++){
			pixels++;
			sum += img.getPixel(i,j);
		}
	}
	return sum / pixels;
}

void utility::twoDimensionalSmoothing(image &tgt, int ws, Region roi){
	image original(tgt);
	for(int i = roi.i; i < roi.i + roi.ilen; i++){
		for(int j = roi.j; j < roi.j + roi.jlen; j++){
			Region window = getSquareWindow(tgt,ws,i,j);
			tgt.setPixel(i,j,averageIntensity(original,window));
		}
	}
}

void utility::oneDimensionalSmoothing(image &tgt, int ws, Region roi){
	image original(tgt);
	for(int i = roi.i; i < roi.i + roi.ilen; i++){
		for(int j = roi.j; j < roi.j + roi.jlen; j++){
			Region row = get1DWindow(tgt,ws,ROW,i,j);
			tgt.setPixel(i,j,averageIntensity(original,row));
		}
	}
	original.copyImage(tgt);
	for(int i = roi.i; i < roi.i + roi.ilen; i++){
		for(int j = roi.j; j < roi.j + roi.jlen; j++){
			Region col = get1DWindow(tgt,ws,COL,i,j);
			tgt.setPixel(i,j,averageIntensity(original,col));
		}
	}
}

vector<char*> utility::parse(char* str, int argC){
	vector<char*> arguments;
	char *pch;
	pch = strtok(str, " ");
	arguments.push_back(pch);
	for(int i = 1; i < argC; i++){
		pch = strtok(NULL, " ");
		arguments.push_back(pch);
	}
	return arguments;
}
