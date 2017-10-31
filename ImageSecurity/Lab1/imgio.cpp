/*******************  HeaderBegin  ***************************

NAME OF PROJECT         :       VIDEO ET IFS
NAME OF FILE            :       IMGio.c
OBJECT                  :       I/O for the short 
HISTORIC        :
Date :    11 janvier 1995      Author : JM SADOUL
Date :    14 avril   1995      Author : E POLIDORI
       routine i2a
   COMMENTS     :



********************  HeaderEnd   ****************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>


#include "imgio.h"
#define ERROR 0
#define SUCCESS 1

/******************************************************************/


int ReadPGM(char *name,short **retimage,int *width,int *height)
{
  unsigned position;
  unsigned imageSize,levels;
  int  magic,number,currentCHAR;
  
  FILE *imageID;

  char Debugchar[5];
  
  if((imageID=fopen(name,"rb"))==0) 
  {
		fprintf(stderr,"Impossible d'ouvrir le fichier %s\n",name);
    exit(1);
  }
   
  if (((magic=getc(imageID))!='P') || ((number=getc(imageID))!= '5'))
		fprintf(stderr,"Version inconnue de fichier PGM");
        
	if ((currentCHAR=getc(imageID))!='\n')
		fprintf(stderr,"fichier deteriore\n");

  while ((currentCHAR=getc(imageID))=='#')
	while ((currentCHAR=getc(imageID))!='\n') {} 

  fseek(imageID,ftell(imageID)-1,SEEK_SET);
  fscanf(imageID,"%s",Debugchar);
  *width=atoi(Debugchar);
  currentCHAR=getc(imageID);
  fscanf(imageID,"%s",Debugchar);
  *height=atoi(Debugchar);

  /*fread(height,sizeof(unsigned),1,imageID);*/
  imageSize=(*width)*(*height);
   
  fscanf(imageID,"%s",Debugchar);
  levels=atoi(Debugchar);

  if((currentCHAR=getc(imageID))!='\n')
                fprintf(stderr,"fichier deteriore\n");

  position=ftell(imageID);

  (*retimage)=(short *)(malloc(sizeof(short)*imageSize));

	for (unsigned int i=0; i<imageSize;i++) 
		(*retimage)[i] = (short)fgetc ( imageID );
  
	return SUCCESS;
}


/*-----------------------------------------------------------------------*/

void WritePGM(char *name,short *imageDATA,int width,int height)
{
  FILE *imageID;
        
  if((imageID=fopen(name,"wb"))==0)
  {
                fprintf(stderr,"Impossible d'ouvrir le fichier %s\n",name);
    exit(1);
  }

  fputs("P5\n",imageID);
  fputs("# Portable GrayMap file format\n",imageID);
  

  fprintf(imageID,"%u %u\n255\n",width,height);
  
	for (int i=0; i<width*height;i++) {
	 if (imageDATA[i]>255) fputc(255,imageID);
	 else if (imageDATA[i]<0) fputc(0,imageID);
	 else fputc(imageDATA[i],imageID);
	}
  fclose(imageID);
}


/*-----------------------------------------------------------------------*/

short * readRAW(char *name,int data_type,unsigned width,unsigned height)
{
        unsigned imageSize;
  unsigned loop;
  short *retimage, *p_retimage;
  unsigned char  *imageUC;

  FILE *imageID;
  
  if((imageID=fopen(name,"rb"))==0) {
                fprintf(stderr,"Impossible d'ouvrir le fichier %s\n",name);
    exit(1);
        }
   
  imageSize = width * height;
  
  if((retimage =(short*)malloc(sizeof(short)*imageSize))==NULL)
                fprintf(stderr,"Probleme d'allocation memoire\n");

  switch (data_type) 
  {
		case sizeof(unsigned char) :
			if((imageUC = (unsigned char *)malloc(sizeof(unsigned char)*imageSize))==NULL)
				fprintf(stderr,"Probleme d'allocation memoire\n");
          
      fread(imageUC,sizeof(unsigned char)*imageSize,1,imageID);
                        
      for(loop=0, p_retimage=retimage; loop < imageSize;loop++, p_retimage++, imageUC++){
				*p_retimage = (int)(*imageUC);
			}
    /*    free(imageUC);*/
			break;
                
     case sizeof(int) :
			fread(retimage,sizeof(int)*imageSize,1,imageID);
      break;
        }     
  return(retimage);
}

/*-----------------------------------------------------------------------*/

void writeRAW(char *name,short *imageDATA, int data_type, unsigned width,\
              unsigned height)
{
        FILE *imageID;
  unsigned loop;
  unsigned char * p_imageDATA;
//      unsigned char data_tmp;
        
  if((imageID=fopen(name,"wb"))==0) {
                fprintf(stderr,"Impossible d'ouvrir le fichier %s\n",name);
    exit(1);
  }

  p_imageDATA = (unsigned char *)malloc(sizeof(unsigned char)*width*height);
  switch (data_type) {
                case sizeof(unsigned char) :
                         for(loop=0; loop < (width*height);loop++) 
       {
                                 if (imageDATA[loop] > 255)
                                         p_imageDATA[loop] = 255;
         else if (imageDATA[loop] < 0)
           p_imageDATA[loop] = 0;
         else
           p_imageDATA[loop] = (unsigned char)imageDATA[loop];
       }
       
                         fwrite(p_imageDATA,sizeof(unsigned char)*width*height,1,imageID);
       break;
       
                case sizeof(int) :
                         fwrite(imageDATA,sizeof(int)*width*height,1,imageID);
       break;
        }
  fclose(imageID);
}

/******************************************************************************/

int ReadPPM (char *filein_name, int *xsize, int *ysize, int *maxrgb,
        short **rarray, short **garray, short  **barray ) 
{

  FILE *filein;
  int   numbytes;
  int   result;

  filein = fopen ( filein_name, "rb" );

  if ( filein == NULL ) {
    printf ( "\n" );
    printf ( "PPMB_READ: Fatal error!\n" );
    printf ( "  Cannot open the input file %s.\n", filein_name );
    return ERROR;
  }

  /* Read the header.*/
  result = ppmb_read_header ( filein, xsize, ysize, maxrgb );

  if ( result == ERROR ) {
    printf ( "\n" );
    printf ( "PPMB_READ: Fatal error!\n" );
    printf ( "  PPMB_READ_HEADER failed.\n" );
    return ERROR;
  }

/* Allocate storage for the data. */
  numbytes = (*xsize)*(*ysize)*sizeof (short );

  //*rarray = (short *)malloc(numbytes);
	*rarray = new short[numbytes];
		
  if ( *rarray == NULL ) {
    printf ( "\n" );
    printf ( "PPMB_READ: Fatal error!\n" );
    printf ( "  Unable to allocate memory for data.\n" );
    printf ( "  Seeking %d bytes.\n", numbytes );
    return ERROR;
  }

  //*garray = (short *)malloc(numbytes);
	*garray = new short[numbytes];
  if ( *garray == NULL ) {
    printf ( "\n" );
    printf ( "PPMB_READ: Fatal error!\n" );
    printf ( "  Unable to allocate memory for data.\n" );
    printf ( "  Seeking %d bytes.\n", numbytes );
    return ERROR;
  }

  //*barray = (short *)malloc(numbytes);
	*barray = new short[numbytes];
  if (*barray == NULL) {
    printf ( "\n" );
    printf ( "PPMB_READ: Fatal error!\n" );
    printf ( "  Unable to allocate memory for data.\n" );
    printf ( "  Seeking %d bytes.\n", numbytes );
    return ERROR;
  }

/* Read the data. */
  result = ppmb_read_data ( filein, *xsize, *ysize, *rarray, *garray, *barray );

  if ( result == ERROR ) {
    printf ( "\n" );
    printf ( "PPMB_READ: Fatal error!\n" );
    printf ( "  PPMB_READ_DATA failed.\n" );
    return ERROR;
  }

/* Close the file. */
  fclose ( filein );

  return SUCCESS;
}

/******************************************************************************/

int ppmb_read_data ( FILE *filein, int xsize, int ysize, short  *rarray, 
        short  *garray, short  *barray ) 
{
  int   i;
  int   int_val;
  short   *indexb;
  short   *indexg;
  short   *indexr;
  int   j;
  int   k;
  int   numval;

  indexr = rarray;
  indexg = garray;
  indexb = barray;
  numval = 0;

  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {

      for ( k = 0; k < 3; k++ ) {

        int_val = fgetc ( filein );

        if ( int_val == EOF ) {
          printf ( "\n" );
          printf ( "PPMB_READ_DATA: Failed reading data byte %d.\n", numval );
          return ERROR;
        }
        else {
          if ( k == 0 ) {
            *indexr = int_val;
            indexr = indexr + 1;
          }
          else if ( k == 1 ) {
            *indexg = int_val;
            indexg = indexg + 1;
          }
          else if ( k == 2 ) {
            *indexb = int_val;
            indexb = indexb + 1;
          }
        }
        numval = numval + 1;
      }
    }
  }
  return SUCCESS;
}
/******************************************************************************/

int ppmb_read_header ( FILE *filein, int *xsize, int *ysize, int *maxrgb ) 
{
  int   c_val;
  int   count;
  int   flag;
  int   nchar;
  int   state;
  char  string[80];

  state = 0;
  nchar = 0;

  for ( ;; ) {

    c_val = fgetc ( filein );

    if ( c_val == EOF ) {
      return ERROR;
    }

    if (c_val == '#')
    {
                        while (c_val !='\n') c_val = fgetc ( filein );
      c_val = fgetc ( filein );
    }

/*  If not whitespace, add the character to the current string.*/

    flag = isspace ( c_val );

    if ( !flag ) {
      string[nchar] = c_val;
      nchar = nchar + 1;
    }

/* See if we have finished an old item, or begun a new one. */
    if ( state == 0 ) {
      if ( !flag ) {
        state = 1;
      }
      else {
        return ERROR;
      }
    }
    else if ( state == 1 ) {
      if ( flag ) {
        string[nchar] = 0;
        nchar = nchar + 1;
        if ( strcmp ( string, "P6" ) != 0 && strcmp ( string, "p6" ) != 0 ) {
          printf ( "\n" );
          printf ( "PPMB_READ_HEADER: Fatal error.\n" );
          printf ( "  Bad magic number = %s.\n", string );
          return ERROR;
        }
        nchar = 0;
        state = 2;
      }
    }
    else if ( state == 2 ) {
      if ( !flag ) {
        state = 3;
      }
    }
    else if ( state == 3 ) {
      if ( flag ) {
        string[nchar] = 0;
        nchar = nchar + 1;
        count = sscanf ( string, "%d", xsize );
        if ( count == EOF ) {
          return ERROR;
        }
        nchar = 0;
        state = 4;
      }
    }
    else if ( state == 4 ) {
      if ( !flag ) {
        state = 5;
      }
    }
    else if ( state == 5 ) {
      if ( flag ) {
        string[nchar] = 0;
        nchar = nchar + 1;
        count = sscanf ( string, "%d", ysize );
        if ( count == EOF ) {
          return ERROR;
        }
        nchar = 0;
        state = 6;
      }
    }
    else if ( state == 6 ) {
      if ( !flag ) {
        state = 7;
      }
    }
    else if ( state == 7 ) {
      if ( flag ) {
        string[nchar] = 0;
        nchar = nchar + 1;
        count = sscanf ( string, "%d", maxrgb );
        if ( count == EOF ) {
          return ERROR;
        }
        nchar = 0;
        return SUCCESS;
      }
    }
  }
}


/******************************************************************************/

int WritePPM ( char *fileout_name, int xsize, int ysize, int maxrgb,
        short  *rarray, short  *garray, short  *barray ) 
{

  FILE *fileout;
  int   result;

/* Open the output file.*/
  fileout = fopen ( fileout_name, "wb" );

  if ( fileout == NULL ) {
    printf ( "\n" );
    printf ( "PPMB_WRITE: Fatal error!\n" );
    printf ( "  Cannot open the output file %s.\n", fileout_name );
    return ERROR;
  }

/* Write the header. */
  result = ppmb_write_header ( fileout, xsize, ysize, maxrgb );

  if ( result == ERROR ) {
    printf ( "\n" );
    printf ( "PPMB_WRITE: Fatal error!\n" );
    printf ( "  PPMB_WRITE_HEADER failed.\n" );
    return ERROR;
  }

/* Write the data. */
  result = ppmb_write_data ( fileout, xsize, ysize, rarray, garray, barray );

  if ( result == ERROR ) {
    printf ( "\n" );
    printf ( "PPMB_WRITE: Fatal error!\n" );
    printf ( "  PPMB_WRITE_DATA failed.\n" );
    return ERROR;
  }

/* Close the file. */
  fclose ( fileout );

  return SUCCESS;
}

/******************************************************************************/

int ppmb_write_data ( FILE *fileout, int xsize, int ysize, short  *rarray, short  *garray, short  *barray ) 
{
  int  i;
  short  *indexb;
  short  *indexg;
  short  *indexr;
  int  j;

  indexr = rarray;
  indexg = garray;
  indexb = barray;

  for ( j = 0; j < ysize; j++ ) {
    for ( i = 0; i < xsize; i++ ) {
      if (*indexr>255) fputc(255,fileout);
                        else if (*indexr<0) fputc(0,fileout);
      else fputc ( *indexr, fileout );
      
      if (*indexg>255) fputc(255,fileout);
      else if (*indexg<0) fputc(0,fileout);
      else fputc ( *indexg, fileout );
      
      if (*indexb>255) fputc(255,fileout);
      else if (*indexb<0) fputc(0,fileout);
      else fputc ( *indexb, fileout );
      
      indexr = indexr + 1;
      indexg = indexg + 1;
      indexb = indexb + 1;
    }
  }
  return SUCCESS;
}

/******************************************************************************/

int ppmb_write_header ( FILE *fileout, int xsize, int ysize, int maxrgb ) 
{

  fprintf ( fileout, "P6\n%d %d\n%d\n", xsize, ysize, maxrgb );
        return SUCCESS;
}
