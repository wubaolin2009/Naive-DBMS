/* Filename: delete.cpp  */
/* Function:   realization of delete.h  */
/* Author Wubaolin          */
/* vision  2.1.0  */
/* change list  :  In header Files  */   
#include "mystring.h"
#include<malloc.h>
#include <stdlib.h>
#include "datatype.h"
#include "delete.h"
#include"search.h"
#include "time.h"
#include "importtable.h"

extern time_t start,over;

void free_a_table(Column** columnindex,int numberoftable,Data** table2free)
{
	int i=0;  //count
	
	while( columnindex[numberoftable][i].name[0] !='\0')  //Not all columns have been deleted  
	{
		int _coltype= columnindex[numberoftable][i].nametype;   //Data Style to sort
		if(_coltype)
		free(table2free[i]);
		else //Free String 
		{
			for(int j=0;j<columnindex[numberoftable][0].length;j++)
			{
				free(table2free[i][j].str);
			}
			free(table2free[i]);
		}
		i++;
	}
	free(table2free);
}
int Deletetable(char* filename,char (*tableindex)[TABLE_NAME_MAX],Column** columnindex,Data*** database)
{
	over=start=0;
	start=clock();
	FILE* file = fopen(filename,"r+");
	if(NULL == file)
	{
		printf("Error in opening the command file!\n");
		return 0;
	}
	char buffer[COLUMN_NAME_MAX];
	char* command=getwords(file,buffer);
	//Check Key word Delete
	if(mystrcmp("delete",command)) 
	{
		printf("The file should be included the keyword update!\n");
		return 0;
	}
	command=getwords(file,buffer); //Reading  from
	if(mystrcmp("from",buffer)) 
	{
		printf("The file should be included the keyword from!\n");
		return 0;
	}
	//Reading table name 
	command=getwords(file,buffer);
	int tableoperate = tablenow(buffer,tableindex);  //The order of opertating table
	if(tableoperate==-1) 
	{
		printf("Can't Locate the table!\n");
		return 0;
	}

	int andor[AND_OR_MAX]; //  and or ARRAY   or=0 and=1    -1 is the end  
	int likeequal[LIKE_EQUAL_MAX]; //like  = Array   -1 is the end   0=like 1 = =  
	char colvalue[COL_READ_MAX][COLUMN_NAME_MAX];   //Recieve every column value
	int colname_do[COL_READ_MAX];  //Order of column to delete  
	int col_delete=0;
	checkline2(file,andor,likeequal,colvalue,colname_do,tableoperate,columnindex); //Reading the last line
	fclose(file);  //close file
	int* index = CreateIndex(tableoperate,colname_do,0,likeequal,colvalue,andor,columnindex,database) ; //Create Searching Index
	int loriginal = columnindex[tableoperate][0].length;  //Reading data length of the original one
	Data** head = deleteatable(columnindex,database,tableoperate,index);
	int count=fprintalltable(columnindex[tableoperate],head,tableoperate,"074090_delete.txt");  //write to the file
	free(index);
	return loriginal-count;
}
Data** deleteatable(Column** columnindex,Data*** table,int tableoperate,int* index)
{
	int old_line =0;
	int old_column = 0;
	int index_line =0;
	int new_line = 0;
	int new_table_line =0;
	int new_table_col =0;
	int newline = columnindex[tableoperate]->length - getnumberofindex(index) ;
	if(newline == 0) 
	{
		free_a_table(columnindex,tableoperate,table[tableoperate]);
		table[tableoperate] = NULL;
		return NULL;  //If there are none element Return NULL
	}
	int col = getnumberofcol(columnindex[tableoperate]); //Return the number of operationg column now 
	Data** head = (Data**)malloc(sizeof(Data*) * col) ;  //The new table after delete
	
	for(int i=0;i<col ;i++)   //Initialze the new table 
	{
		head[i] = (Data*)malloc(sizeof(Data) * newline);
	}
	//Reading datas
	int lineall = columnindex[tableoperate]->length;  //Length of original table
	while(old_line<lineall) //Not all lines are opertated completely
	{
		if( old_line != index[index_line])   //Ignore the line to delete  to  rebuild the new table
		{
			while(columnindex[tableoperate][new_table_col].name[0]!='\0')  //Not all column are done  in a lin  
			{
				int _coltype= columnindex[tableoperate][new_table_col].nametype;   //check the style 
				if(_coltype) //float
				{
					head[new_table_col][new_table_line].num = table[tableoperate][old_column][old_line].num;  //copy  float
				}
				else
				{
					head[new_table_col][new_table_line].str = (char*)malloc( sizeof(char)*columnindex[tableoperate][new_table_col].size);
					if(head[new_table_col][new_table_line].str == NULL ) MEMROTY_ERROR
					mystrcpy( head[new_table_col][new_table_line].str,table[tableoperate][old_column][old_line].str);//copy string 
				}
				new_table_col++;
				old_column++;
			}
			new_table_line++;
			new_table_col=0;
			old_line++;
			old_column=0;
		}
		else   //Don't let the element to delete  to write to the new table
		{
			index_line++;
			old_line++;
		}
	}
	free_a_table(columnindex,tableoperate,table[tableoperate]);
	table[tableoperate] = head;
	int numtowrite=0;//Lengthof column to write
	while(columnindex[tableoperate][numtowrite].name[0] != '\0') columnindex[tableoperate][numtowrite++].length = newline;
	return head;
	
}

int getnumberofcol(Column* column2get)
{
	int _count=0;
	while(column2get[_count].name[0] != '\0') _count++;
	return _count;
}

int getnumberofindex(int* index)
{
	int _count=0;
	while(index[_count] != -1) _count++;
	return _count;
}

int fprintalltable(Column* colunmindex, Data** table ,int tableoperate,char* filename)
{
	int countline=0;  //Line Number
	int countcol =0; //Column Number
	FILE* fp = fopen(filename,"w+");
	if(fp == NULL) { printf("Error in writing files\n"); return 0;}
	over = clock();
	fprintf(fp,"Excute Time: %d ms\nThe results are(Ater delete):\n",(int)(over-start));
	over = start =0;
	if(table==NULL){ fclose(fp); return 0;} 
	while(countline<colunmindex->length)//Not all lines are operated completely 
	{
		while(colunmindex[countcol].name[0] != '\0')  //Not all columns are operatede completely
		{
			int _coltype= colunmindex[countcol].nametype;   //data style
			if(_coltype)  //float
			{
				 fprintf(fp,"%.2f ",table[countcol][countline].num);

			}
			else fprintf(fp,"%s ",table[countcol][countline].str);
			countcol++;
		}
		countcol=0;
		countline++;
		fputc('\n',fp);
	}
	fclose(fp);	

	return countline;
}

