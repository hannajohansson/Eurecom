// CImage.h: interface for the CImage class.
// Author: C. Rey 
#ifndef _cimage_h_
#define _cimage_h_
 
/* CImage defines */
#define PI										3.14159265359
#define GRAY_IMAGE								0
#define RGB_IMAGE								1
#define YUV_IMAGE								2


/* Error defines */
#define SUCCESS								 1
#define ERROR								 0
#define ERR_UNKNOWN_IMAGE_TYPE				-1
#define ERR_ICOMPATIBLE_IMAGE_SIZE			-2
#define ERR_MALLOC							-4
#define ERR_IO_FILE							-5


/* Class definition */
class CImage {
	char type;									// image type (GRAY, RGB, YUV)
	char nb_comp;								// number of componentes 	
	
	int width;									// image width  (in pixels)
	int height;									// image height (in pixels)	
	short *C[3];								// image data
		
public:
	/* Constructors / Destructor */
	CImage();
	CImage(char type,  int width, int height);
	CImage(const CImage &img);
	virtual ~CImage();

	int Create(int type, int width, int height);
	int Copy(const CImage &img);

	/* IO Methods */
	int ReadPPMFile(char *fileName);
	int WritePPMFile(char *fileName);
	int ReadPGMFile(char *fileName);
	int WritePGMFile(char *fileName);
	          
	/* Watermarking */
	int BlockMean(short *mean, int c, int posx, int posy, int block_width, int block_height);
	int DrawBadBlock(int posx, int posy, int block_width, int block_height);
	int DrawFlatBlock(int c, int posx, int posy, int block_width, int block_height, short g);
	
	unsigned long CRCBlock(int c, int posx, int posy, int block_width, int block_height, const unsigned long *crcTable);
	int InsertNoiseLSB();
	int ExtractNoiseLSB();
	int InsertCRCLSB();
	int ExtractCRCLSB();
	int InsertSelfEmbeddingLSB();
	int ExtractSelfEmbeddingLSB();
   
	friend void CRCTable(unsigned long **crcTable);
};

#endif
