/* Filename: quit.cpp  */
/* Function:   realization of  quit.h  */
/* Author Wubaolin          */
/* vision  2.1.0  */
/* change list  :  In header Files  */   
#include <malloc.h>
#include <stdio.h>
#include "mystring.h"
#include "quit.h"
#include "datatype.h"


void  quit(Data*** data,Column** col,char (*tableindex)[TABLE_NAME_MAX])
{
	int count_table=0;  //Table number
	int count_col =0; //Column number
	while(tableindex[count_table][0]!='\0')
	{
		if(data[count_table]== NULL)
		{   //If the table is NULL
			free(col[count_table++]);
		}
		count_table++;
	}
	if(data[count_table] == NULL)
	{
		free(col[count_table]);
		free(col);
		free(data);
		return;
	}
	while(col[count_table][count_col].name[0]!=0)
	{
		while(tableindex[count_table][0]!='\0')
		{
			int _coltype= col[count_table][count_col].nametype;   //Data style 
			if(_coltype)  //float
			{
				free(data[count_table][count_col]);
			}
			else
			{
				for(int i=0;i<col[count_table][count_col].length;i++)
				{
					char* temp= data[count_table][count_col][i].str;
					free(temp);  //free×Ö·û´®
				}
				
				free(data[count_table][count_col]);
			}
			count_col++;

		}
		free(col[count_table]);
		free(data[count_table]);
		count_col=0;
		count_table++;		
	}
	free(data);
	free(col);
}
