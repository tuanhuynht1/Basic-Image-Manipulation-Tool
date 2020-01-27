/************************************************************
 *															*
 * This sample project include three functions:				*
 * 1. Add intensity for gray-level image.					*
 *    Input: source image, output image name, value			*
 *															*
 * 2. Image thresholding: pixels will become black if the	*
 *    intensity is below the threshold, and white if above	*
 *    or equal the threhold.								*
 *    Input: source image, output image name, threshold		*
 *															*
 * 3. Image scaling: reduction/expansion of 2 for 			*
 *    the width and length. This project uses averaging 	*
 *    technique for reduction and pixel replication			*
 *    technique for expansion.								*
 *    Input: source image, output image name, scale factor	*
 *															*
 ************************************************************/

#include "utility.h"
#include "image.h"
#include <vector>

using namespace std;

#define MAXLEN 256

int main (int argc, char** argv)
{
	image src, tgt;
	FILE *fp;
	char str[MAXLEN];
	char outfile[MAXLEN];

	//vector<Region> regions;

	if ((fp = fopen(argv[1],"r")) == NULL) {
		fprintf(stderr, "Can't open file: %s\n", argv[1]);
		exit(1);
	}

	vector<char*> argV;
	int rows, cols, i_origin, j_origin, numberOfRegions;
	char *op, *pch;

	while(fgets(str,MAXLEN,fp) != NULL) {

		argV = utility::parse(str,4);
		src.read(argV[0]);
		strcpy(outfile,argV[1]);
		op = argV[2];
		numberOfRegions = atoi(argV[3]);

		//---------------------------------------------------- DOUBLE THRESHOLDING ----------------//
		if(strncasecmp(op,"thresholding",MAXLEN) == 0){
			int t1, t2;
			//copy source image to target
			tgt.copyImage(src);
			for (int i = 0; i < numberOfRegions; i++){
				if (fgets(str,MAXLEN,fp) != NULL){
					argV = utility::parse(str,6);
					i_origin = atoi(argV[0]);
					j_origin = atoi(argV[1]);
					rows = atoi(argV[2]);
					cols = atoi(argV[3]);
					t1 = atoi(argV[4]);
					t2 = atoi(argV[5]);
					Region roi(rows,cols,i_origin,j_origin);
					utility::thresholding(tgt,t1,t2,roi);
				}
			}
		}
		//----------------------------------------------------- COLOR BINARIZATION -------------------//
		else if(strncasecmp(op,"bicolor",MAXLEN) == 0){
			int r, g, b, tc, dc;
			//copy source image to target
			tgt.copyImage(src);
			for (int i = 0; i < numberOfRegions; i++){
				if (fgets(str,MAXLEN,fp) != NULL){
					argV = utility::parse(str,9);
					i_origin = atoi(argV[0]);
					j_origin = atoi(argV[1]);
					rows = atoi(argV[2]);
					cols = atoi(argV[3]);
					r = atoi(argV[4]);
					g = atoi(argV[5]);
					b = atoi(argV[6]);
					tc = atoi(argV[7]);
					dc = atoi(argV[8]);
					Region roi(rows,cols,i_origin,j_origin);
					utility::colorBinarization(tgt,Color(r,g,b),tc,dc,roi);
				}
			}
		}
		//----------------------------------------------------- 2D SMOOTHING -------------------//
		else if(strncasecmp(op,"smooth2D",MAXLEN) == 0){
			int ws;
			//copy source image to target
			tgt.copyImage(src);
			for (int i = 0; i < numberOfRegions; i++){
				if (fgets(str,MAXLEN,fp) != NULL){
					argV = utility::parse(str,5);
					i_origin = atoi(argV[0]);
					j_origin = atoi(argV[1]);
					rows = atoi(argV[2]);
					cols = atoi(argV[3]);
					ws = atoi(argV[4]);
					Region roi(rows,cols,i_origin,j_origin);
					utility::twoDimensionalSmoothing(tgt,ws,roi);
				}
			}
		}
		else if(strncasecmp(op,"smooth1D",MAXLEN) == 0){
			int ws;
			//copy source image to target
			tgt.copyImage(src);
			for (int i = 0; i < numberOfRegions; i++){
				if (fgets(str,MAXLEN,fp) != NULL){
					argV = utility::parse(str,5);
					i_origin = atoi(argV[0]);
					j_origin = atoi(argV[1]);
					rows = atoi(argV[2]);
					cols = atoi(argV[3]);
					ws = atoi(argV[4]);
					Region roi(rows,cols,i_origin,j_origin);
					utility::oneDimensionalSmoothing(tgt,ws,roi);
				}
			}
		}
		//------------------------------------------------------------ INVALID -----------------//
		else{
			cout << "Undefined function: [" << op <<"]?" << endl;
			break;
		}
		tgt.save(outfile);
	}
	fclose(fp);

	
	// image input, output;
	// char filename[11] = "baboon.pgm";
	// input.read(filename);

	// output.copyImage(input);
	// utility::twoDimensionalSmoothing(output,9,Region(300,300,150,200));
	// output.save("testing3.pgm");

	// output.copyImage(input);
	// utility::oneDimensionalSmoothing(output,9,Region(300,300,150,200));
	// output.save("testing4.pgm");
	return 0;
}

