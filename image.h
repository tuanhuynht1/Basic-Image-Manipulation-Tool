#ifndef IMAGE_H
#define IMAGE_H
#include <iostream>
#include <stdio.h>
#include <stdlib.h> 
#include <vector>

using namespace std;

enum channel{RED, GREEN, BLUE, GREY=RED, GRAY=GREY};

struct imageData
{
   vector<int>  redChannel, greenChannel, blueChannel; 
   int numRows, numColumns; 
};

struct Region{
	int ilen, jlen, i, j;
	Region(int rows, int cols, int i_origin, int j_origin) : ilen(rows), jlen(cols), i(i_origin), j(j_origin) {}
};

struct Color{
   int r,g,b;
   Color(int R, int G, int B) : r(R), g(G), b(B) {}
};

class image 
{
private:
   imageData data;
   int getint(FILE *fp);
   friend class Utility;
   
public:
   image();
   image(image &img);
   image(int rows, int columns);
   ~image();
   
   void deleteImage();
   void copyImage(image &img);
   void resize (int numberOfRows, int numberOfColumns);
   void setNumberOfRows(int rows);
   void setNumberOfColumns(int columns);
   void setPixel(const int row, const int col, const int value);
   void setPixel(const int row, const int col, const int rgb, const int value);
   
   int getPixel(const int row, const int col);
   int getPixel(const int row, const int col, const int rgb);
   int getNumberOfRows();
   int getNumberOfColumns(); 
   vector<int>* getChannel(int rgb);
   
   bool setChannel(int rgb, vector<int> &channel);   
   bool save (char* file);
   bool save (const char* file);
   bool read (char* file);
   bool isInbounds (const int row, const int col);


};

#endif

