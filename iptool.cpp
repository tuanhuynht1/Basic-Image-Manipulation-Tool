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
#include <strings.h>
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

	vector<Region> regions;

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
		//---------------------------------------------------- WHITE OUT ------------------------//
		if(strncasecmp(op,"white",MAXLEN) == 0){
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
					regions.push_back(Region(rows,cols,i_origin,j_origin));
				}
			}
			utility::whiteOut(src,tgt,regions);
			regions.clear();
		}
		//---------------------------------------------------- DOUBLE THRESHOLDING ----------------//
		else if(strncasecmp(op,"thresholding",MAXLEN) == 0){
			int t1, t2;
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
					regions.push_back(Region(rows,cols,i_origin,j_origin));
					//parse threshold values
					pch = strtok(NULL, " ");
					t1 = atoi(pch);
					pch = strtok(NULL, " ");
					t2 = atoi(pch);
					cout << t1 << t2 << endl;
				}
			}
			//threshold function
			regions.clear();
		}

		else{
			cout << "Undefined function: [" << op <<"]?" << endl;
			break;
		}
		tgt.save(outfile);
	}
	fclose(fp);
	return 0;
}

