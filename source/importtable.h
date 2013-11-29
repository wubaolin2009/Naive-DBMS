/* Filename: import.h  */
/* Function:   Import a table  */
/* Author Wubaolin          */
/* vision  2.1.0  */
/* change list  :   
2.0.0  Change linked table  to Union Array   
*/
#ifndef _IMPORT_H 
#define _IMPORT_H
#include <stdio.h>
#include "datatype.h"


/*Reading Data             */
/* tablename:NameOfTable  filename:Filename Of Data     tableindex:Index of table name */
/*  columnindex: Index of Column   database: DB  */
/*  Return the count available  */
int ImportTable(char* tablename,char* filename,char (*tableindex)[TABLE_NAME_MAX],Column** columnindex,Data*** database );

/* Judge how many lines will be imported in a file */
/* file : Pointer to FILE */
/* Return Line number   */
int countdata(FILE* file);

/* Return the column number of the table you 're operating now*/
/* num:order of operating table        columnindex Index of Column */
/*  Return column number */
int countcol(int num,Column** columnindex);

/* Locate the table you 're doing now  */
/* name:NameOfTable    TableIndex IndexOfTable */
/* Return the order of the table with the name  */
int tablenow(char* name,char (*tableindex)[TABLE_NAME_MAX]);

/*Read words from  import.txt  */
/* file:Pointer to file    buffer where to write the word */
/* EOF return NULL    ','At the begining return EMPTYDATA 若遇到换行符 返回LINEEND*/
/* If '\n'At the beging  则返回 LINEEND */
char* getsource(FILE* file, char* buffer); 

#endif
