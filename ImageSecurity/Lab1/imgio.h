#ifdef __cplusplus
extern "C" {  // only need to export C interface if
              // used by C++ source code
#endif

int ReadPGM(char *,short **,int *,int *);
void WritePGM(char *,short *,int ,int);
short * readRAW(char *,int ,unsigned ,unsigned);
void writeRAW(char *,short *, int , unsigned ,unsigned);

int ReadPPM(char *, int *, int *, int *, short **, short **, short **);
int ppmb_read_data(FILE *, int, int, short *,short *, short *);
int ppmb_read_header(FILE *, int *, int *, int *); 
int WritePPM(char *, int, int, int, short *, short *, short *);
int ppmb_write_data (FILE *, int, int, short *, short *, short *);
int ppmb_write_header (FILE *, int, int, int);
 
#ifdef __cplusplus
}
#endif
