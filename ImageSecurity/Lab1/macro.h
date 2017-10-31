#define ABS(val) ( ((val)<0) ? (-(val)) : (val) )
#define CARRE(val) ( (val) * (val) )
#define MAX(val1,val2) ( ( (val1)<(val2) ) ? (val2) : (val1) )
#define RoundInt(val)  ( ((val)>0) ? ((short int)((val)+0.5)) : ((short int)((val)-0.5)) )


/*****************************************************************************/
/*	      		     MACROS A COMPLETER                                       */
/*****************************************************************************/
#define SETBIT(pixel,pos,bit)  ( (bit) ? ( (pixel) |= (1<<(pos)) ) : ( (pixel) &= ~(1<<(pos)) ) ) 
#define GETBIT(pixel,pos)  ( ( (pixel) & (1<<(pos)) ) >> (pos) ) 

