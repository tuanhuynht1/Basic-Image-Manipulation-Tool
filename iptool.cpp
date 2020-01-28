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
		
		//parse line into argument Vector for each operation
		argV = utility::parse(str,4);
		src.read(argV[0]);					//input file
		strcpy(outfile,argV[1]);			//output file
		op = argV[2];						//operation
		numberOfRegions = atoi(argV[3]);	//number of regions to operate on

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
		//----------------------------------------------------- 1D SMOOTHING -------------------//
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
	char f[] = "baboon.pgm";
	tgt.read(f);
	Region r(200,200,5,5);
	// cout << tgt.getPixel(5,5) << endl;
	// for(int i = r.i ; i < r.i + r.ilen; i++){
	// 	for(int j = r.j - 1; j < r.j + r.jlen + 1; j++){
	// 		cout << tgt.getPixel(r.i,j) << " ";
	// 	}
	// 	cout << endl;
	// }
	// cout << "---------" << endl;
	cout << "incremental" << endl;
	utility::incrementalSmoothing(tgt,9,r);
	tgt.save("testing5.pgm");

	return 0;
}

