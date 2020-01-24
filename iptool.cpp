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
#include <string.h>
#include <vector>

using namespace std;

#define MAXLEN 256

int main (int argc, char** argv)
{
	image src, tgt;
	FILE *fp;
	char str[MAXLEN];
	char outfile[MAXLEN];
	char *op;
	char *pch;

	//vector<Region> regions;

	if ((fp = fopen(argv[1],"r")) == NULL) {
		fprintf(stderr, "Can't open file: %s\n", argv[1]);
		exit(1);
	}

	int rows, cols, i_origin, j_origin, numberOfRegions;

	while(fgets(str,MAXLEN,fp) != NULL) {
		//parse source
		pch = strtok(str, " ");
		src.read(pch);
		cout << pch << " ";
		//parse target
		pch = strtok(NULL, " ");
		strcpy(outfile, pch);
		cout << pch << " ";
		//parse operation
		pch = strtok(NULL, " ");
		op = pch;
		cout << pch << " ";
		//parse number of regions
		pch = strtok(NULL, " ");
		numberOfRegions = atoi(pch);
		cout << numberOfRegions << endl;

		//---------------------------------------------------- DOUBLE THRESHOLDING ----------------//
		if(strncasecmp(op,"thresholding",MAXLEN) == 0){
			int t1, t2;
			//copy source image to target
			tgt.copyImage(src);
			for (int i = 0; i < numberOfRegions; i++){
				if (fgets(str,MAXLEN,fp) != NULL){
					//parse region attributes
					pch = strtok(str, " ");
					i_origin = atoi(pch);
					pch = strtok(NULL, " ");
					j_origin = atoi(pch);
					pch = strtok(NULL, " ");
					rows = atoi(pch);
					pch = strtok(NULL, " ");
					cols = atoi(pch);
					//initialize region
					Region roi(rows,cols,i_origin,j_origin);
					//parse threshold values
					pch = strtok(NULL, " ");
					t1 = atoi(pch);
					pch = strtok(NULL, " ");
					t2 = atoi(pch);
					//call operation on region
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
					//parse region attributes
					pch = strtok(str, " ");
					i_origin = atoi(pch);
					pch = strtok(NULL, " ");
					j_origin = atoi(pch);
					pch = strtok(NULL, " ");
					rows = atoi(pch);
					pch = strtok(NULL, " ");
					cols = atoi(pch);
					//initialize region
					Region roi(rows,cols,i_origin,j_origin);
					//parse 3 channels of desired color
					pch = strtok(NULL, " ");
					r = atoi(pch);
					pch = strtok(NULL, " ");
					g = atoi(pch);
					pch = strtok(NULL, " ");
					b = atoi(pch);
					//parse TC and DC
					pch = strtok(NULL, " ");
					tc = atoi(pch);
					pch = strtok(NULL, " ");
					dc = atoi(pch);
					//call operation on region
					cout << r << g << b << tc << dc << endl;
					utility::colorBinarization(tgt,Color(r,g,b),tc,dc,roi);
				}
			}
		}
		//----------------------------------------------------- WHITE OUT ------------------------//
		// else if(strncasecmp(op,"white",MAXLEN) == 0){
		// 	for (int i = 0; i < numberOfRegions; i++){
		// 		if (fgets(str,MAXLEN,fp) != NULL){
		// 			//parse region attributes
		// 			pch = strtok(str, " ");
		// 			i_origin = atoi(pch);
		// 			pch = strtok(NULL, " ");
		// 			j_origin = atoi(pch);
		// 			pch = strtok(NULL, " ");
		// 			rows = atoi(pch);
		// 			pch = strtok(NULL, " ");
		// 			cols = atoi(pch);
		// 			regions.push_back(Region(rows,cols,i_origin,j_origin));
		// 		}
		// 	}
		// 	utility::whiteOut(src,tgt,regions);
		// }
		//-------------------------------------------------------- COPY IMAGE ------------------//
		else if(strncasecmp(op,"copy",MAXLEN) == 0){
			tgt.copyImage(src);
		}
		//------------------------------------------------------------ INVALID -----------------//
		else{
			cout << "Undefined function: [" << op <<"]?" << endl;
			break;
		}
		tgt.save(outfile);
		// regions.clear();
	}
	fclose(fp);

	return 0;
}

