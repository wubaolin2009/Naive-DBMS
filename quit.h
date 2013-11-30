/* Filename: quit.h  */
/* Function:   Free Memrory  */
/* Author Wubaolin          */
/* vision  2.1.0  */
/* change list  :   
2.0.0  Change linked table  to Union Array   
*/


#ifndef _QUIT_H
#define _QUIT_H
#include "datatype.h"
/* free memrory  */
/*  data:Tables to free   col: Index of columns to free     tableindex:Index of table*/
/* None Return Value */
void quit(Data*** data,Column** col,char (*tableindex)[TABLE_NAME_MAX]);
#endif
