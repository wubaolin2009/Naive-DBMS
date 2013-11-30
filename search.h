/* Filename: search.h  */
/* Function:   serch a table  */
/* Author Wubaolin          */
/* vision  2.1.0  */
/* change list  :   
2.0.0  Change linked table  to Union Array  
2.1.0  Change sort  algorithm From Qsort to Shell sort now it supported 500,000 or more data to sort 
       recondition  Some Memrory leaks     and  Array beyond the boundary */ 
#ifndef _SEARCH_H
#define _SEARCH_H
#include <stdio.h>
#include "datatype.h"
/* Abstract an Analyse strings from the command file */
/* file Pointer to FIle    recieve Array To recieve words*/
/* Common words return recieve           '}'  return COLEND */
/* search a line return LINEEND   EOF  return NULL */
char* getwords(FILE* file,char* recieve); 

/*search the table           */
/* filename  CommandFileName    tableindex Index of Table */
/* column IndexOfColumn      database: DB   */
/* Return number of line that is searched   */
int tablesearch(char* filename,char (*tableindex)[TABLE_NAME_MAX],Column** columnindex,Data*** database);


/*Return the order of column with the name of name */
/* name:ColumName, tableoperte:orderOf operationg table    columnindex:Index of Column */
/* Return number of column */
int columnnow(char* name,int tableoperate , Column** columnindex);

/*   CheckThe line before where , And do some action to the arguments  */
/*  file pointerToFile    colname_print Array of orders of column to print  */
/* numberofprint:Number of line to print  0 means the whole   */
/*colnameprints ColumnNameToPrint   */
/* tableoperate :OrderOfOperationgTable   ,    tableindex   colunmindex */
/* Normal return 1    Error Return  0  */
int checkline1(FILE* file,int* colname_print,int* numberofprint,char (*colnameprints)[COLUMN_NAME_MAX],int* tableoperate ,char (*tableindex)[TABLE_NAME_MAX],Column** columnindex);

/*  Check The line with where, And do actions to the arguments  */
/*  file:Pointers to FILE   andor Arrays of ANDOR , likeequal  Array of LIKE and = */
/*  colvalue return the value after count,    colname_do :to store the column order that is restricted in some way */
/*   tableoperate OrderOfOpetatingTable ,  colunmnindex OrderOfOperationgColumn*/
char* checkline2(FILE* file,int* andor,int* likeequal,char (*colvalue)[COLUMN_NAME_MAX],int* colname_do,int tableoperate,Column** columnindex);

/* Create Index to print   */
/* tableoperate:OrderOfOpearationgTable    colname_do:Order of column to operate    colname_order: the order that you want to order by */
/* likeequal = AND ARRAY   colvalue columnValue to Match      andor:AND OR array    columnindex:IndexOfColumnIndex         database DB*/
/* Return the pointer to table   */
int *CreateIndex(int tableoperate,int* colname_do,int colname_order,int* likeequal,char(*colvalue)[COLUMN_NAME_MAX],int* andor,Column** columnindex,Data*** database);  

/* Judge the current matches the line as the select.txt said */
/* columnname_do OrderOfOpertiongColumn   likeequal = like Array          colvalue The condition that should be matched */
/* andor AND OR Array    columnindex ColumnIndex  table_now TableOpertiongNow            linenum The operating now linenumber */
/* 1Is Trun      0 Is False   */
int Istrue(int* columnname_do,int* likeequal,char(*colvalue)[COLUMN_NAME_MAX],int* andor,Column* columnindex,Data** table_now,int linenum);

/* basic result make the result of every statement to oprerater  as to judge with the AND OR  it's OK with the method of Stack */
/* columnname_do OrderOfOpearingColumn    likeequal = like'sArray  colvalue Conditions should be matched */
/* operater:Array to write     andor AND OR Array     columnindex IndexOfColumn   table_now TableOperting Now   linenum LineNumber Operationg now */
/*  None Return Value  */
void basicresult(int* colname_do,int* likeequal,char(*colvalue)[COLUMN_NAME_MAX],int* operater,Column* columnindex,Data** table_now,int linenum);

/* Write the result to file    */
/* filename NameOfWriting   , numberofprint:Item Number to print 0is all       colname_print ColumnOrder to print   , database DBtable */
/*  columnindex     index Index of Items to print  */
/*   Return the number of writing      */
int fileprint(char* filename,int numberofprint,int *colname_print,Data** database,Column* columnindex,int *index);

/*   Shell Sort   */
/*  index Index without Sort      num NumberOfSort    _coltype:  Data type , asc ASCorDESC   database Table */
/* None Value */
void shellsort(int* index,int num,int _coltype,int asc,Data* database);

/*  Calculate the last number in stack num 's value in the operater of op */
/*  num Stack  op Opearter */
/*  1True             0False */
int calculate(int* num,int op);


#endif

