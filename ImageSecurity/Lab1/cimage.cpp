// CImage.cpp: implementation of the CImage class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <new.h>
#include "cimage.h"
#include "imgio.h"
#include "macro.h"


/*-----------------------------------------------------------------------------
|  Routine Name: Constructors
|       Purpose:
|         Input: type, width, height
|        Output:
|       Returns:
|
|    Written By: Christian REY
|          Date: 16/05/2001
| Modifications:
|----------------------------------------------------------------------------*/
CImage::CImage(){
	nb_comp=0;
	width=0;
	height=0;
	C[0] = NULL; 	
	C[1] = NULL; 	
	C[2] = NULL; 
}

CImage::CImage(char t, int w, int h){
	type = t;
	width = w;
	height = h;
	 
	if (type == GRAY_IMAGE) nb_comp = 1;
	else nb_comp = 3; 

	for (int i=0;i<nb_comp;i++) {
		C[i] = new short[width*height];
		memset(C[i],0,sizeof(short)*width*height);
	}
}

CImage::CImage(const CImage &anImage){
	type = anImage.type;
	width = anImage.width;
	height = anImage.height;
	nb_comp = anImage.nb_comp;

	for (int i=0;i<nb_comp;i++) {
		if (C[i]!=NULL) delete(C[i]);
      C[i] = new short[width*height];
		memcpy(C[i], anImage.C[i], sizeof(short)*width*height);
	}
}

int CImage::Create(int t, int w, int h){
	type = t;
	width =w;
	height = h;

	if (type == GRAY_IMAGE) nb_comp = 1;
	else nb_comp = 3; 

	for (int i=0;i<nb_comp;i++) {
		if (C[i]!=NULL) delete(C[i]);
      C[i] = new short[width*height];
		if (C[i]==NULL)
			return ERROR;
	}
    
	return SUCCESS;
}

int CImage::Copy(const CImage &anImage){
	type = anImage.type;
	width = anImage.width;
	height = anImage.height;
	nb_comp = anImage.nb_comp;

	for (int i=0;i<nb_comp;i++) {
		if (C[i]!=NULL) delete(C[i]);
      C[i] = new short[width*height];
		memcpy(C[i], anImage.C[i], sizeof(short)*width*height);
	}
 
	return SUCCESS;
}

/*-----------------------------------------------------------------------------
|  Routine Name: Destructor
|       Purpose:
|         Input:
|        Output:
|       Returns:
|
|    Written By: Christian REY
|          Date: 16/05/2001
| Modifications:
|----------------------------------------------------------------------------*/
CImage::~CImage(){
	for (int i=0;i<nb_comp;i++) 
		if (C[i]!=NULL){
			delete(C[i]);
			C[i] = NULL;
		}
}

/*-----------------------------------------------------------------------------
|  Routine Name: ReadPPMFile
|       Purpose: Read a RGB image (binary PPM format)
|         Input: path
|        Output: CImage (this)
|       Returns: statut
|
|    Written By: Christian REY
|          Date: 16/05/2001
| Modifications:
|----------------------------------------------------------------------------*/
int CImage::ReadPPMFile(char *path){
	int err;
	int nb_levels;

	type = RGB_IMAGE;
	nb_comp = 3;
	for (int c=0;c<nb_comp; c++)
		if (C[c]!=NULL) delete(C[c]);
	
	err = ReadPPM(path, &width, &height, &nb_levels, &C[0], &C[1], &C[2]);
	
	if (err==ERROR)
		return ERR_IO_FILE;
	else
		return SUCCESS;
}

/*-----------------------------------------------------------------------------
|  Routine Name: WritePPMFile
|       Purpose: Write a RGB image (binary PPM format)
|         Input: path
|        Output: none
|       Returns: statut
|
|    Written By: Christian REY
|          Date: 16/05/2001
| Modifications: 
|----------------------------------------------------------------------------*/
int CImage::WritePPMFile(char *path){
	int err;

	switch (type) {
		case RGB_IMAGE :
			err = WritePPM(path, width, height, 255, C[0], C[1], C[2]);
			break;
		case GRAY_IMAGE :
			err = WritePPM(path, width, height, 255, C[0], C[0], C[0]);
			break;
		default :
			return ERR_UNKNOWN_IMAGE_TYPE;
	}
	
	if (!err)
		return ERR_IO_FILE;
	else
		return SUCCESS;
}

/*-----------------------------------------------------------------------------
|  Routine Name: ReadPGMFile
|       Purpose: Read a RGB or a Gray image (binary PGM format)
|         Input: path
|        Output: CImage (this)
|       Returns: statut
|
|    Written By: Christian REY
|          Date: 16/05/2001
| Modifications:
|----------------------------------------------------------------------------*/
int CImage::ReadPGMFile(char *path){
	type = GRAY_IMAGE;
	nb_comp = 1;
	
	if (C[0]!=NULL) delete(C[0]);

	ReadPGM(path, &C[0], &width, &height);
	
	if (C[0]==NULL)
		return ERR_IO_FILE;
	
	return SUCCESS;
}

/*-----------------------------------------------------------------------------
|  Routine Name: WritePGMFile
|       Purpose: Write a RGB or a Gray image (binary PGM format)
|         Input: path
|        Output: none
|       Returns: statut
|
|    Written By: Christian REY
|          Date: 16/05/2001
| Modifications: 
|----------------------------------------------------------------------------*/
int CImage::WritePGMFile(char *path){
	if (type!=GRAY_IMAGE)
		return ERROR;
	
	WritePGM(path, C[0], width, height);
	
	return SUCCESS;
}


/*-----------------------------------------------------------------------------
|  Routine Name: BlockMean
|       Purpose: calculate the average intensity level value of a block in c
|         Input: c, posx, posy, bloc_width, block_height, min, max
|        Output: *value
|       Returns: status
|
|    Written By: Christian REY
|          Date: 29/06/2001
| Modifications: Hanna Johansson 16/05/2017
|----------------------------------------------------------------------------*/
int CImage::BlockMean(short *value, int c, int posx, int posy,
							 int block_width, int block_height){
	int sum = 0;

	if (C[c]==NULL)
		return ERROR;
	if (posx < 0 || posy < 0 || (posx + block_width-1) > width 
		|| (posy + block_height-1) > height)
		return ERROR;
	
	/* A COMPLETER, EXERCISE 3 */
	// Upper left corner of the block, similar as in exercise 2 (i+j*width)
	int up_left = posx + posy*width;

	// Loop over the block to calculate the sum of the intensity values
	for (int i=0; i<block_width; i++) {
		for (int j=0; j<block_height; j++) {
			sum = sum + C[c][up_left+i+j*width];
		}
	}

	// Compute average intensity level value for the block 
	(*value) = (short)(sum/(block_width*block_height));

	return SUCCESS;
}


/*-----------------------------------------------------------------------------
|  Routine Name: DrawBadBlock
|       Purpose: Dessine un carre
|         Input: posx, posy, bloc_width, block_height
|        Output: CImage (this)
|       Returns: status
|
|    Written By: Christian REY
|          Date: 29/06/2001
| Modifications:
|----------------------------------------------------------------------------*/
int CImage::DrawBadBlock(int posx, int posy, int block_width, int block_height){
	if (posx<0 || posy<0 || posx+block_width-1>width
		 ||posy+block_height-1>height)
		return ERROR;

	for (int c=0; c<nb_comp; c++) {
		if (C[c]==NULL)
			return ERROR;
		for (int j=0;j<block_height;j++)
			for (int i=0;i<block_width;i++){
				if (i==0 || j==0)
					C[c][i+posx+(j+posy)*width] = 255;
				else if (i==block_width-1 || j==block_height-1)
					C[c][i+posx+(j+posy)*width] = 0;
				else if (i==j || (block_width-1-i)==j) {
					if (c==0)
						C[c][i+posx+(j+posy)*width] = 255;
					else
						C[c][i+posx+(j+posy)*width] = 0;	
				}
				else
					C[c][i+posx+(j+posy)*width] = 240;
			}
	}
	
	return SUCCESS;
}

/*-----------------------------------------------------------------------------
|  Routine Name: DrawFlatBlock
|       Purpose:
|         Input: posx, posy, bloc_width, block_height, gray level
|        Output: CImage (this)
|       Returns: status
|
|    Written By: Christian REY
|          Date: 29/06/2001
| Modifications:
|----------------------------------------------------------------------------*/
int CImage::DrawFlatBlock(int c, int posx, int posy, int block_width,
								  int block_height, short g){
	if (posx<0 || posy<0 || posx+block_width-1>width
		 || posy+block_height-1>height)
		return ERROR;

	if (C[c]==NULL)
		return ERROR;
  
	for (int j=0;j<block_height;j++)
		for (int i=0;i<block_width;i++)
			C[c][i+posx+(j+posy)*width] = g;
	  
	return SUCCESS;
}


/*-----------------------------------------------------------------------------
|  Routine Name: CRCBlock
|       Purpose: Calculate CRC value of a block
|         Input: c, posx, posy, block_width, block_height, *crcTable
|        Output: none
|       Returns: crc
|
|    Written By: Christian REY
|          Date: 02/05/2002
| Modifications:
|----------------------------------------------------------------------------*/
unsigned long CImage::CRCBlock(int c, int posx, int posy, int block_width,
										 int block_height, const unsigned long *crcTable){
	register unsigned long crc;
	int val;

	crc = 0xFFFFFFFF;

	for (int k=0; k<block_width; k++)
		for (int l=0; l<block_height; l++) {
			val = (int)C[c][posx+k + (posy+l)*width];
			SETBIT(val,0,0);
			crc = ((crc>>8) & 0x00FFFFFF) ^ (crcTable[ (crc^val) & 0xFF ]);
		}

	return (crc^0xFFFFFFFF);
}


/*-----------------------------------------------------------------------------
|  Routine Name: InsertNoiseLSB	
|       Purpose: Met les LSB des pixels de l'image a zero 
|         Input: CImage (this)
|        Output: CImage (this)
|       Returns: status
|
|    Written By: Christian REY
|          Date: 25/04/2002
| Modifications: Hanna Johansson 05/05/2017
|----------------------------------------------------------------------------*/
int CImage::InsertNoiseLSB(){
	int noise;
	srand(0);

	for (int c=0; c<nb_comp;c++) {
		if (C[c]==NULL)
			return ERROR;

		/* A COMPLETER, EXERCISE 1 */
		// Create random noise, 0 or 1 depending on if the random value is odd or even 
		noise = rand() % 2;

		// Loop over the image/all the pixels
		for (int p=0; p<height*width; p++) {
				// Access the pixel: C[c][p]. 
				// Position of the LSB = 0
				// Set the value in the LSB to noise
				SETBIT(C[c][p], 0, noise);
		}
	}

	return SUCCESS;
}

/*-----------------------------------------------------------------------------
|  Routine Name: ExtractNoiseLSB	
|       Purpose: 
|         Input: CImage (this)
|        Output: CImage (this)             
|       Returns: status
|
|    Written By: Christian REY
|          Date: 25/04/2002
| Modifications: Hanna Johansson 05/05/2017
|----------------------------------------------------------------------------*/
int CImage::ExtractNoiseLSB(){
	int noise;
	srand(0);

	for (int c=0; c<nb_comp;c++) {
		if (C[c]==NULL)
			return ERROR;	
     
		/* A COMPLETER, EXERCISE 2 */
		// Create random noise 
		noise = rand() % 2;

		// Loop over the image/all the pixels
		for (int p=0; p<height*width; p++) {
				// Access the pixel: C[c][p]. Position of the LSB = 0
				// Get the bit and compare it with the noise, 0 = equal and 1 = different 
				int bitvalue = GETBIT(C[c][p], 0) ^ noise;
				
				// If it is a match, set the pixel value to 0, otherwise to 255
				if (bitvalue == 0)
					C[c][p] = 0;
				else
					C[c][p] = 255;	
		}
	}      
  
	return SUCCESS;
}

/*-----------------------------------------------------------------------------
|  Routine Name: InsertCRCLSB	
|       Purpose:
|         Input: 
|        Output:
|       Returns: status
|
|    Written By: Christian REY
|          Date: 25/04/2002
| Modifications: Hanna Johansson 12/05/2017
|----------------------------------------------------------------------------*/
int CImage::InsertCRCLSB(){
	int nb_xblocks = width/8;
	int nb_yblocks = height/8;
	unsigned long crc;
	unsigned long *crcTable;
	short bit;
	
	CRCTable(&crcTable);

	for (int c=0; c<nb_comp;c++) {
		// if the component is empty return ERROR
		if (C[c]==NULL)
			return ERROR;	

		// for each component which is not empty, loop over the number of blocks
		for (int i=0; i<nb_xblocks; i++)
			for (int j=0; j<nb_yblocks; j++) {

				/* A COMPLETER, EXERCISE 2 */
				// Compute the checksum value of block(i,j) using the function CRCBlock()
				// CRCBlock(int c, int posx, int posy, int block_width, int block_height, const unsigned long *crcTable)
				crc = CRCBlock(c, i*8, j*8, 8, 8, crcTable);

				// Upper left corner of the block. [ i*8 + j*8*width = (i+j*width)*8 = (posx+posy*width)*8 ]
				int upper_left= i*8 + j*8*width;

				// Insert the 32-bit checksum in the LSB of the 32 first pixels in block(i,j) = in the first 4 rows
				for (int r=0; r<4; r++) {
					for (int col=0; col<8; col++) {
						// Get the bit from the CRC value
						bit = GETBIT(crc, r*8+col);

						// Access the pixel: C[c][p]. Position of the LSB = 0. Set the value in the LSB to the CRC
						SETBIT(C[c][upper_left+(col+r*width)], 0, bit); 
					}
				} 
			} 
	}

	free(crcTable);
  
	return SUCCESS;
}

/*-----------------------------------------------------------------------------
|  Routine Name: ExtractCRCLSB	
|       Purpose:  
|         Input: 
|        Output:              
|       Returns: status
|
|    Written By: Christian REY
|          Date: 25/04/2002
| Modifications: Hanna Johansson 12/05/2017
|----------------------------------------------------------------------------*/
int CImage::ExtractCRCLSB(){
	int nb_xblocks = width/8;
	int nb_yblocks = height/8;
	unsigned long crc, xcrc;
	unsigned long *crcTable;
	short bit;

	CRCTable(&crcTable);
	
	for (int c=0; c<nb_comp;c++) {
		if (C[c]==NULL)
			return ERROR;	

		for (int i=0; i<nb_xblocks; i++)
			for (int j=0; j<nb_yblocks; j++) {	

				/* A COMPLETER, EXERCISE 2 */
				// Compute the checksum value of block(i,j) using the function CRCBlock()
				crc = CRCBlock(c, i*8, j*8, 8, 8, crcTable);

				// Upper left corner of the block
				int upper_left= i*8 + j*8*width;

				// Extract the checksum value hidden in the LSB of 32 first pixels of block(i,j)
				// Loop over the first 32 pixels in one block = the first 4 rows
				for (int r=0; r<4; r++) {
					for (int col=0; col<8; col++) {
						bit = GETBIT(C[c][upper_left+(col+r*width)], 0);
						SETBIT(xcrc, r*8+col, bit);
					}
				}

				// Compare the two checksum values and decide whether the block has been tampered or not
				if (xcrc != crc) {
					// DrawBadBlock(int posx, int posy, int block_width, int block_height)
					DrawBadBlock(i*8, j*8, 8, 8);
				}
				// Else (the block has not been tampered) --> do nothing

			}
	}

	free(crcTable);
  
	return SUCCESS;
}

/*-----------------------------------------------------------------------------
|  Routine Name: InsertSelfEmbeddingLSB	
|       Purpose: 
|         Input: 
|        Output:              
|       Returns: status
|
|    Written By: Christian REY
|          Date: 25/04/2002
| Modifications: Hanna Johansson 18/05/2017
|----------------------------------------------------------------------------*/
int CImage::InsertSelfEmbeddingLSB(){
	int nb_xblocks = width/8;
	int nb_yblocks = height/8;
	unsigned long crc;
	unsigned long *crcTable;
	short bit;
	short m[4];
	int ti,tj,k,l,posx,posy;
	
	CRCTable(&crcTable);
	//Each 8x8 block is divided into 4 sub-blocks of 4x4
	ti = 4;		//width of the sub-block
	tj = 4;		//height of the sub-block

	for (int c=0; c<nb_comp;c++) {
		if (C[c]==NULL)
			return ERROR;	

		//loop over the blocks
		for (int i=0; i<nb_xblocks; i++){
			for (int j=0; j<nb_yblocks; j++) {
	
				/* A COMPLETER, EXERCISE 3 */
				// Checksum value computation and embedding (as in exercise 2)
				// Compute the checksum value of block(i,j) using the function CRCBlock()
				crc = CRCBlock(c, i*8, j*8, 8, 8, crcTable);

				// Upper left corner of the block B(i,j)
				int upper_left= i*8 + j*8*width;

				// Insert the 32-bit checksum in the LSB of the 32 first pixels in block (i,j),
				// e.g. in the first 4 rows of the 8x8 block
				for (int w=0; w<8; w++) {
					for (int h=0; h<4; h++){
						bit = GETBIT(crc, h*8+w);
						SETBIT(C[c][upper_left + w + h*width], 0, bit);
					}
				}

				// Initialize m (the vector storing the 4 mean values for the 4 blocks)
				for(int block_nmbr=0; block_nmbr<4 ;block_nmbr++)
					m[block_nmbr] = 0;

				// Average gray level computation for the 4 sub-blocks
				for (int block=0; block<4; block++) {
					// Check the different cases (a switch could also be used)
					if(block == 0) {
						posx = i*8;
						posy = j*8;
						// Calculate the mean value for the currrent sub-block
						// BlockMean(short *value, int c, int posx, int posy, int block_width, int block_height)
						BlockMean(&m[block], c, posx, posy, 4, 4);	
					}
					else if(block == 1){
						posx = i*8 + 4;
						posy = j*8;
						BlockMean(&m[block], c, posx, posy, 4, 4);
					}						
					else if(block == 2){
						posx = i*8;
						posy = j*8 + 4;
						BlockMean(&m[block], c, posx, posy, 4, 4);
					}
					else if(block == 3){
						posx = i*8 + 4;
						posy = j*8 + 4;
						BlockMean(&m[block], c, posx, posy, 4, 4);
					}
				}

				// B(k,l) coordinates computation --> translation of the coordinates of block B(i,j)
				// Check so that the new coordinates are not outside of the image -> use modulus
				k = (i + 20) % nb_xblocks;
				l = (j + 20) % nb_yblocks;

				// Restoration bits embedding in LSB of block B(k,l)
				// Upper left corner of the LSB block B(k,l)
				int up_left= k*8 + l*8*width;

				// Insert the the 4 average intensity levels (the 8-bit mean values) of the sub-blocks 
				// in the LSB of the four 8 pixel vectors in block (k,l)
				for(int r=4; r<8; r++){
					for(int col=0; col<8; col++){
						bit = GETBIT(m[r-4], col);
						SETBIT(C[c][up_left + col + r*width], 0, bit);
					}
				}
			}
		}
	}

	free(crcTable);

	return SUCCESS;
}

/*-----------------------------------------------------------------------------
|  Routine Name: ExtractSelfEmbeddingLSB	
|       Purpose: 
|         Input: 
|        Output:              
|       Returns: status
|
|    Written By: Christian REY
|          Date: 25/04/2002
| Modifications: Hanna Johansson 19/05/2017
|----------------------------------------------------------------------------*/
int CImage::ExtractSelfEmbeddingLSB(){
	int nb_xblocks = width/8;
	int nb_yblocks = height/8;
	unsigned long crc, xcrc=0;
	unsigned long *crcTable;
	short bit;
	short m[4];
	int ti,tj,k,l,posx,posy;

	CRCTable(&crcTable);
	ti = 4;
	tj = 4;
	
	for (int c=0; c<nb_comp;c++) {
		if (C[c]==NULL)
			return ERROR;	

		for (int i=0; i<nb_xblocks; i++){
			for (int j=0; j<nb_yblocks; j++) {

				/* A COMPLETER, EXERCISE 3 */
				// Compute the checksum value of block(i,j) using the function CRCBlock()
				crc = CRCBlock(c, i*8, j*8, 8, 8, crcTable);

				// Upper left corner of the block
				int upper_left= i*8 + j*8*width;

				// Check the checksum value
				// Extract the checksum value hidden in the LSB of 32 first pixels of block(i,j)
				// Loop over the first 32 pixels in one block = the first 4 rows
				for (int r=0; r<4; r++) {
					for (int col=0; col<8; col++) {
						bit = GETBIT(C[c][upper_left + col + r*width], 0);
						SETBIT(xcrc, r*8+col, bit);
					}
				}

				/*----------------------------------------------------------------------------------------
				  Compare the two checksum values and decide whether the block has been tampered or not.
				  If this is not a match --> 
					1. Do B(k,l) coordinates computation 
					2. Initialize m (the vector storing the 4 mean values for the 4 blocks)
					3. Extract the restoration bits hidden in the LSB of block B(k,l)
					4. Switch between the 4 sub-blocks to get their coordinates posx and posy
					5. Reconstruct the tampered sub-blocks by using DrawFlatBlock 
				-----------------------------------------------------------------------------------------*/
				if (xcrc != crc) {
					// B(k,l) coordinates computation --> translation of the coordinates of block B(i,j)
					k = (i + 20) % nb_xblocks;
					l = (j + 20) % nb_yblocks;

					// Initialize m 
					for (int block_nmbr=0; block_nmbr<4 ;block_nmbr++)
						m[block_nmbr] = 0;

					// Upper left corner of the LSB Block B(k,l)
					int up_left= k*8 + l*8*width;

					// Extract the restoration bits hidden in the LSB of the four 8 pixel vectors in block B(k,l)
					// and save it in m
					for(int row=4; row<8; row++){
						for(int column=0; column<8; column++){
							int rest_bit = GETBIT(C[c][up_left + column + row*width], 0);
							SETBIT(m[row-4], column, rest_bit);
						}
					}

					// Reconstruct the 4 tampered sub-blocks
					for (int block=0; block<4; block++) {
						// Check the different cases (a switch could also be used)
						if(block == 0) {
							posx = i*8;
							posy = j*8;
							// DrawFlatBlock(int c, int posx, int posy, int block_width, int block_height, short g)
							DrawFlatBlock(c, posx, posy, 4, 4, m[block]);
						}
						else if(block == 1){
							posx = i*8 + 4;
							posy = j*8;
							DrawFlatBlock(c, posx, posy, 4, 4, m[block]);
						}
						else if(block == 2){
							posx = i*8;
							posy = j*8 + 4;
							DrawFlatBlock(c, posx, posy, 4, 4, m[block]);
						}
						else if(block == 3){
							posx = i*8 + 4;
							posy = j*8 + 4;
							DrawFlatBlock(c, posx, posy, 4, 4, m[block]);
						}
					}
					// else (the block is not tampered) --> do nothing
				}
			}
		}
	}

	free(crcTable);

	return SUCCESS;
}


/**************************************************************/
/*			     FONCTIONS A AMIES                               */
/**************************************************************/
void CRCTable(unsigned long **crcTable){
	unsigned long	crc, poly;
	int i, j;
  
	(*crcTable) = (unsigned long *)malloc(sizeof(unsigned long)*256);
	poly = 0xEDB88320L;
	
	for (i=0; i<256; i++) {
		crc = i;
		for (j=8; j>0; j--) {
			if (crc&1) 
				crc = (crc >> 1) ^ poly;
			else 
				crc >>= 1;
		}
		(*crcTable)[i] = crc;
	}
}
