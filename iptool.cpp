/************************************************************
*	Basic Image Manipulations								*
*		Author: Tuan Huynh									*
*															*
* This project includes five functions:						*
* 															*
* Point Functions											*
* 		double threshold binarization						*
* 		binarization with color threshold					*
* 															*
* Localized Functions										*
* 		2D uniform smoothing								*
* 		1D uniform smoothing								*
* 		Incremental uniform smoothing						*
*															*
 ************************************************************/

#include "utility.h"
#include "image.h"
#include <vector>
#include <chrono>

using namespace std;

#define MAXLEN 256

int main (int argc, char** argv)
{
	image src, tgt;
	FILE *fp;
	char str[MAXLEN];
	char outfile[MAXLEN];

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
					//three region limit
					if(i > 2){
						continue;
					}

					argV = utility::parse(str,6);
					i_origin = atoi(argV[0]);
					j_origin = atoi(argV[1]);
					rows = atoi(argV[2]);
					cols = atoi(argV[3]);
					t1 = atoi(argV[4]);
					t2 = atoi(argV[5]);
					Region roi(rows,cols,i_origin,j_origin);

					//swap if lower bound is greater than upper bound
					if (t1 > t2){
						swap(t1,t2);
					}

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
					//three region limit
					if(i > 2){
						continue;
					}

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
					//three region limit
					if(i > 2){
						continue;
					}

					argV = utility::parse(str,5);
					i_origin = atoi(argV[0]);
					j_origin = atoi(argV[1]);
					rows = atoi(argV[2]);
					cols = atoi(argV[3]);
					ws = atoi(argV[4]);
					Region roi(rows,cols,i_origin,j_origin);

					auto started = chrono::high_resolution_clock::now();
					utility::twoDimensionalSmoothing(tgt,ws,roi);
					auto done = chrono::high_resolution_clock::now();
					cout << chrono::duration_cast<chrono::milliseconds>(done-started).count() << " ms (2D)" << endl;
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
					//three region limit
					if(i > 2){
						continue;
					}

					argV = utility::parse(str,5);
					i_origin = atoi(argV[0]);
					j_origin = atoi(argV[1]);
					rows = atoi(argV[2]);
					cols = atoi(argV[3]);
					ws = atoi(argV[4]);
					Region roi(rows,cols,i_origin,j_origin);

					auto started = chrono::high_resolution_clock::now();
					utility::oneDimensionalSmoothing(tgt,ws,roi);
					auto done = chrono::high_resolution_clock::now();
					cout << chrono::duration_cast<chrono::milliseconds>(done-started).count() << " ms (1D)" << endl;
				}
			}
		}
		//----------------------------------------------------- Incremental SMOOTHING ------------//
		else if(strncasecmp(op,"smooth++",MAXLEN) == 0){
			int ws;
			//copy source image to target
			tgt.copyImage(src);
			for (int i = 0; i < numberOfRegions; i++){
				if (fgets(str,MAXLEN,fp) != NULL){
					//three region limit
					if(i > 2){
						continue;
					}

					argV = utility::parse(str,5);
					i_origin = atoi(argV[0]);
					j_origin = atoi(argV[1]);
					rows = atoi(argV[2]);
					cols = atoi(argV[3]);
					ws = atoi(argV[4]);
					Region roi(rows,cols,i_origin,j_origin);

					auto started = chrono::high_resolution_clock::now();
					utility::incrementalSmoothing(tgt,ws,roi);
					auto done = chrono::high_resolution_clock::now();
					cout << chrono::duration_cast<chrono::milliseconds>(done-started).count() << " ms (inc)" << endl;
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

	return 0;
}

