/* Filename: update.h  */
/* Function:   updata a table  */
/* Author Wubaolin          */
/* vision  2.1.0  */
/* change list  :   
2.0.0  Change linked table  to Union Array   */
#ifndef _UPDATE_H
#define _UPDATE_H

#include "datatype.h"

/* Updatetable :UpdataDB */
/* tableindex:TableIndex    columnindex:ColumnIndex   database: DB   filename:Command Filename */
/* The number of update items */
int Updatetable(char (*tableindex)[TABLE_NAME_MAX],Column** columnindex,Data*** database,char* filename);


/* Make index    to    table  as to update it  */
/* index:Index to update   coltype:styletype ,column2update :columns that will be updated*/
/* colupdatevalue: value to write       table:Table to operate  */
/* None Return Value */
void UpdateData(int* index,int coltype,int column2update,char* colupdatevalue,Data** table);

#endif




