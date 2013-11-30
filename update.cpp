/* Filename: Update.cpp  */
/* Function:   realization of Update.h  */
/* Author Wubaolin          */
/* vision  2.1.0  */
/* change list  :  In header Files  */  
#include "mystring.h"
#include <stdlib.h>
#include "update.h"
#include "datatype.h"
#include "search.h"
#include "createtable.h"  //Locatetable
#include "time.h"
#include "importtable.h"
#include "datatype.h"
#include "delete.h"

extern time_t start,over;

void UpdateData(int* index,int coltype,int column2update,char* colupdatevalue,Data** table)
{
	int count = 0;  
	if(table==NULL) return;
	
	while( index[count] != -1)
	{
		if(coltype)  //float
		{
			table[column2update][ index[count] ].num = (float)atof(colupdatevalue);
		}
		else   //string
			mystrcpy( table[column2update][ index[count] ].str,colupdatevalue);
		count++;
	}
}
int Updatetable(char (*tableindex)[TABLE_NAME_MAX],Column** columnindex,Data*** database,char* filename)
{
	over=start=0;
	int tableopearate=0;  //order of table to operate 
	start=clock();  
	FILE* file = fopen(filename,"r");
	if(NULL == file)
	{
		printf("Error in opening the command file!\n");
		return 0;
	}
	char buffer[COLUMN_NAME_MAX];  //buffer to store command
	char* command=getwords(file,buffer);
	//Check Key word
	if(mystrcmp("update",command)) 
	{
		printf("The file should be included the keyword update!\n");
		return 0;
	}
	command=getwords(file,buffer);
	//Check table name 
	tableopearate = tablenow(buffer,tableindex);
	
	//Key word    set
	command=getwords(file,buffer);
	if(mystrcmp("set",buffer)) 
	{
		printf("The file should be included the keyword set!\n");
		return 0;
	}
	command=getwords(file,buffer);  //column name 
	int andor[AND_OR_MAX]; //and or ARRAY   or=0 and=1    -1 is the end  
	int likeequal[LIKE_EQUAL_MAX]; //like  = Array   -1 is the end   0=like 1 = =  
	char colvalue[COL_READ_MAX][COLUMN_NAME_MAX];   //recieve every column_value
	int colname_do[COL_READ_MAX];  //order of column to operate 
	char colupdatevalue[DATA_WORD_MAX];  //Store the value to update
	//int colname_print[COL_READ_MAX];  //Store the order of column to print
	int col_update;   //Order of operating column 
	//Reading the name to operate 
	col_update= columnnow(buffer,tableopearate,columnindex);  // Order of column to operate 
	//reading  =  
	command=getwords(file,buffer);
	if(mystrcmp(buffer,"="))
	{
		printf(" a = must follow the colname !\n");
		return 0;
	}
	command=getwords(file,buffer);  //Reading the values to write
	mystrcpy(colupdatevalue,buffer);  //Store the values to update
    checkline2(file,andor,likeequal,colvalue,colname_do,tableopearate,columnindex); //Reading the last line    
	fclose(file);  //close the file
	int* index = CreateIndex(tableopearate,colname_do,col_update,likeequal,colvalue,andor,columnindex,database) ; //Create seach index
	int _coltype= columnindex[tableopearate][col_update].nametype;   //datastyle
	UpdateData(index,_coltype,col_update,colupdatevalue,database[tableopearate]);
	if(database[tableopearate] == NULL) {free(index); return 0;}
	int count=getnumberofindex(index);
	fprintalltable(columnindex[tableopearate],database[tableopearate],tableopearate,"074090_update.txt");
	free(index);
	return count;
}

