#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "cimage.h"
#include "macro.h"
#include "defines.h"


/*--------------------------------------------------------------------------*/
void Usage(){
	printf(" tp [-i# original_image|-x# tested_image]\n");
	printf("  -i: protection mode\n");
	printf("  -x: checking mode\n");
	printf("   #: method (1-LSB, 2-CRC, 3-CRC+Self-Embedding)\n");
	printf("   image_file: binary PGM or PPM format\n");
	exit(0);
}

/*--------------------------------------------------------------------------*/
char *AddSuffix(char *aString, char *aSuffix, char *ext){
	char *tmp, *p_string;
     
	tmp = (char *)malloc(sizeof(char)*(strlen(aString)+strlen(aSuffix)+1));

	strcpy(tmp,aString);
	p_string = strstr(tmp,".");
	if (p_string!=NULL)
		p_string[0]=0;
	strcat(p_string,aSuffix);
	strcat(p_string,ext);

	return tmp;
}

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

void main(int argc,char **argv){	
	CImage myImage;
	char fileType, fileExt[5];
	char *inputFile, *outputFile=NULL;
	int method;


	/* Analyse de la ligne de commande */ 
	if (argc != 3) Usage();

	if (strncmp(argv[1],"-i1",3)==0)
		method = INSERT_LSB_MODE;
	else if (strncmp(argv[1],"-x1",3)==0)
		method = CHECK_LSB_MODE;
	else if (strncmp(argv[1],"-i2",3)==0)
		method = INSERT_CRC_MODE;
	else if (strncmp(argv[1],"-x2",3)==0)
		method = CHECK_CRC_MODE;
	else if (strncmp(argv[1],"-i3",3)==0)
		method = INSERT_SELFEMBEDDING_MODE;
	else if (strncmp(argv[1],"-x3",3)==0)
		method = CHECK_SELFEMBEDDING_MODE;
	else
		Usage();

	inputFile = argv[2];
	if (strstr(inputFile,".ppm")!=NULL) {
		fileType=PPM_IMAGE_FILE;
		strcpy(fileExt,".ppm");
	}
	else if (strstr(inputFile,".pgm")!=NULL) {
		fileType=PGM_IMAGE_FILE;
		strcpy(fileExt,".pgm");
	}
	else
		fileType=UNKNOWN_IMAGE_FILE;


	/* Open image file */
	switch (fileType) {
		case PPM_IMAGE_FILE: 
			printf(" .Reading original image file (PPM format)...\n");
         if (myImage.ReadPPMFile(inputFile)!=SUCCESS) {
				printf("  [ERROR] ReadPPMFile()\n");
				exit(0);
         }
         break;

		case PGM_IMAGE_FILE:
			printf(" .Reading original image file (PGM format)...\n");
			if (myImage.ReadPGMFile(inputFile)!=SUCCESS) {
				printf("  [ERROR] ReadPGMFile()\n");
				exit(0);
			}
			break;

		case UNKNOWN_IMAGE_FILE: 
			printf("  [ERROR] Unknown image format!\n");
			exit(0);
			break;
	}

	/* Watermarking operations */
	switch (method){
		case INSERT_LSB_MODE:
			printf(" .Insertion step (METHOD 1)...\n");
			if (myImage.InsertNoiseLSB()!=SUCCESS) {
				printf("  [ERROR] InsertNoiseLSB()\n");
				exit(0);
			}
			outputFile = AddSuffix(inputFile,"_LSB",fileExt);
			printf(" .Writing watermarked image file as %s...\n",outputFile);
	      break;
		
  /*-------------------------------------------------------------------*/
		case CHECK_LSB_MODE:
			printf(" .Checking step (METHOD 1)...\n");
			if (myImage.ExtractNoiseLSB()!=SUCCESS) {
				printf("  [ERROR] ExtractNoiseLSB()\n");
				exit(0);
			}
			outputFile = AddSuffix(inputFile,"_checked",fileExt);
			printf(" .Writing LSB plan as %s...\n",outputFile); 
			break;
		
  /*-------------------------------------------------------------------*/
		case INSERT_CRC_MODE:
			printf(" .Insertion step (METHOD 2)...\n");
			if (myImage.InsertCRCLSB()!=SUCCESS) {
				printf("  [ERROR] InsertCRCLSB()\n");
				exit(0);
			}
			outputFile = AddSuffix(inputFile,"_CRC",fileExt);
			printf(" .Writing watermarked image file as %s...\n",outputFile); 
			break;

  /*-------------------------------------------------------------------*/
		case CHECK_CRC_MODE:
			printf(" .Checking step (METHOD 2)...\n");
			if (myImage.ExtractCRCLSB()!=SUCCESS) {
				printf("  [ERROR] ExtractCRCLSB()\n");
				exit(0);
			}
			outputFile = AddSuffix(inputFile,"_checked",fileExt);
			printf(" .Writing checked image as %s...\n",outputFile);
			break;
		
	/*-------------------------------------------------------------------*/
		case INSERT_SELFEMBEDDING_MODE:
			printf(" .Insertion step (METHOD 3)...\n");
			if (myImage.InsertSelfEmbeddingLSB()!=SUCCESS) {
				printf("  [ERROR] InsertSelfEmbeddingLSB()\n");
				exit(0);
			}
			outputFile = AddSuffix(inputFile,"_Self",fileExt);
			printf(" .Writing watermarked image file as %s...\n",outputFile);
			break;
		
  /*-------------------------------------------------------------------*/
		case CHECK_SELFEMBEDDING_MODE:
			printf(" .Checking step (METHOD 3)...\n");
			if (myImage.ExtractSelfEmbeddingLSB()!=SUCCESS) {
				printf("  [ERROR] ExtractSelfEmbeddingLSB()\n");
				exit(0);
			}
			outputFile = AddSuffix(inputFile,"_checked",fileExt);
			printf(" .Writing checked image as %s...\n",outputFile);
			break;
		
  /*-------------------------------------------------------------------*/
		default:
			printf("  [ERROR] Unknown mode!\n");
			break;
	}


	/* Save output image */
	switch (fileType) {
		case PPM_IMAGE_FILE: 
			if (myImage.WritePPMFile(outputFile)!=SUCCESS) {
				printf("  [ERROR] WritePPMFile()\n");
				exit(0);
			}
			break;

		case PGM_IMAGE_FILE: 
			if (myImage.WritePGMFile(outputFile)!=SUCCESS) {
				printf("  [ERROR] WritePGMFile()\n");
				exit(0);
			}
			break;
	}
}

