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
	char *pch;

	vector<Region> regions;

	if ((fp = fopen(argv[1],"r")) == NULL) {
		fprintf(stderr, "Can't open file: %s\n", argv[1]);
		exit(1);
	}

	int numberOfRegions;

	while(fgets(str,MAXLEN,fp) != NULL) {
		pch = strtok(str, " ");
		src.read(pch);
		cout << pch << " ";
		
		pch = strtok(NULL, " ");
		strcpy(outfile, pch);
		cout << pch << " ";

		pch = strtok(NULL, " ");
		cout << pch << " ";

		pch = strtok(NULL, " ");
		numberOfRegions = atoi(pch);
		cout << numberOfRegions << endl;

		for (int i = 0; i < numberOfRegions; i++){
			int x, y, x0, y0;
			if (fgets(str,MAXLEN,fp) != NULL){
				pch = strtok(str, " ");
				x = atoi(pch);

				pch = strtok(NULL, " ");
				y = atoi(pch);

				pch = strtok(NULL, " ");
				x0 = atoi(pch);

				pch = strtok(NULL, " ");
				y0 = atoi(pch);
				regions.push_back(Region(x,y,x0,y0));
			}
		}		

		tgt.save(outfile);
		for (auto r : regions){
			r.print();
		}
		regions.clear();
	}
	fclose(fp);
	return 0;
}

