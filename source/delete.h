/* Filename: delete.h  */
/* Function:   Delete a table  */
/* Author Wubaolin          */
/* vision  2.1.0  */
/* change list  :   
2.0.0  Change linked table delete to Union Array delete  , Make delete at original array  
2.1.0  Create new table and free the original   to  make a better stability */

#ifndef _DELETE_TABLE_H
#define _DELETE_TABLE_H

#include "datatype.h"


/* free A table  */
/*　columnindex:Index of Column　numberoftable:order of a table　   table2free The pointer of table that you want to free　*/
/* None Return value　　*/
void free_a_table(Column** columnindex,int numberoftable,Data** table2free);

/* Delete a Table  return The new table pointer */
/*  columnindex: Index of column    table:DB       tableoperate: The order of operting table   index:Index to sort */
/* Return th pointer to new table , It it's NULL , return NULL */
Data** deleteatable(Column** columindex,Data*** table,int tableoperate,int* index);

/* Get the number of Column   */
/* column2get The order of table you want to  locate the column at */
/* Return COlumn number */
int getnumberofcol(Column* column2get);


/* Get the number of the Index element available */
/* index:IndexName   */
/* Return the number that is available  */
int getnumberofindex(int* index);

/*Delete table     */
/* filename: Command file  tableindex:Index of Table     columnindex:Index of column   database:DB   */
/* Return the number of delete */
int Deletetable(char* filename,char (*tableindex)[TABLE_NAME_MAX],Column** columnindex,Data*** database);

/* Write the table after delete  */
/* columnindex:Index of column   table:Talbe To Write   tableoperate:Table order that is operationg now     filename: Filename that you want to  write to*/
/* Return the number of written   */
int fprintalltable(Column* colunmindex, Data** table ,int tableoperate,char* filename);
#endif



