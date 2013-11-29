/* Filename: Import.cpp  */
/* Function:   realization of Import.h  */
/* Author Wubaolin          */
/* vision  2.1.0  */
/* change list  :  In header Files  */  
#include "mystring.h"
#include<malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include<ctype.h>
#include "createtable.h"
#include "datatype.h"
#include "importtable.h"

int countdata(FILE* file)    //Numbers of data to import
{
	int count=0;
	char buffer[20];
	char* command=buffer;
	command = getsource(file,buffer);
	while(command!=NULL)
	{
		if(mystrcmp(command,"lineend")==0)
			count++;
		command=getsource(file,buffer);
	}
	return count+1;
}
int countcol(int num,Column** columnindex) //return column count 
{
	int count = 0;
	int i=0;
	while(columnindex[num][count].name[0] != '\0')
		count++;
	return count;
}
char* getsource(FILE* file, char* buffer) //read word once one
{
	char temp;
	int i=0;
	temp=fgetc(file);
	if(temp == '\n'||temp==' ') return LINENULL;
	while(isspace(temp))
	{
		temp=fgetc(file);
	}
	
	if(temp==EOF) return NULL;
	if(temp== ',') return ",";
	else buffer[i++]=temp;
	temp=fgetc(file);
	while((!isspace(temp))&&temp!=EOF)
	{
		if(temp==','){ buffer[i]='\0'; return buffer;}
		else
		{
			buffer[i++]=temp;
			temp=fgetc(file);
		}
	}
	if(temp=='\n') {return LINEEND; buffer[i]='\0';}
	
	buffer[i]='\0';
	return buffer;
}

int tablenow(char* name,char (*tableindex)[TABLE_NAME_MAX])
{
	int i=0;
	int count=0;
	while(count<TABLE_MAX)
	{
		if( mystrcmp(name,tableindex[count]) == 0) return count;
		count++;
	}
	return -1;

}
int ImportTable(char* tablename,char* filename,char (*tableindex)[TABLE_NAME_MAX],Column** columnindex,Data*** database )
{
	FILE* file;
	if( (file=fopen(filename,"r+"))==0)
	{
		printf("Can't open files!\n");
		return NULL;
	}
	//Initialze the table
	int tableoperte = tablenow(tablename,tableindex);  //which table to operate
	if(tableoperte == -1 )
	{
		printf("Cant't locate the table with the name of %s!\n",tablename); return 0;
	}
	/* Create Table   */
	//Judge the number of datas  
	int _countdata = countdata(file);
	rewind(file);
	//Judge the number of column 
	int _countcol = countcol(tableoperte,columnindex);
	//the number of datas to write to column 
	for(int i=0;i<_countcol;i++)
	{
		columnindex[tableoperte][i].length = _countdata;
	}
	if(countdata <= 0 ) {printf("FIle Error!\n"); return 0;}
	if (  (database[tableoperte] = (data**) malloc(sizeof(data*) * _countcol ) )  == NULL )
		MEMROTY_ERROR;
	for(int o=0;o<_countcol;o++)  //Initializing every data
	{
		if ( (database[tableoperte][o] = (data*) malloc(sizeof(data) * _countdata)) == NULL )
			MEMROTY_ERROR;
	}
	char buffer[10]={0};  //Buffer of command
		
	/*Data Import    */
	int number_column=0;  //Column Number
	int numberline=0;  //Line Number
	char *command=buffer;
	while(file)
	{
		command=getsource(file,buffer);
		if(command==NULL) break;
		int type = columnindex[tableoperte][number_column].nametype;     //data style 
		int size = columnindex[tableoperte][number_column].size;   //size of data

		if( (mystrcmp(",",command)==0) && columnindex[tableoperte][number_column].can_empty==1 ) 
		{
			if(type) database[tableoperte][number_column][numberline].num=0;
			else 
			{
				database[tableoperte][number_column][numberline].str = (char*)malloc(sizeof(char)*size);
				if(database[tableoperte][number_column][numberline].str == NULL) MEMROTY_ERROR
				mystrcpy(database[tableoperte][number_column][numberline].str,"0");
			}
			number_column++;
			if(number_column == _countcol) {number_column=0; numberline++;}			
			//Judge a line is over or not
		}
		
		else if( (mystrcmp(LINENULL,command)==0) && columnindex[tableoperte][number_column].can_empty==1 ) 
		{
			while(number_column<_countcol)
			{
				type = columnindex[tableoperte][number_column].nametype;
				if(type) database[tableoperte][number_column][numberline].num=0;
				else 
				{
					database[tableoperte][number_column][numberline].str = (char*)malloc(sizeof(char)*size);
					if(database[tableoperte][number_column][numberline].str == NULL) MEMROTY_ERROR
					mystrcpy(database[tableoperte][number_column][numberline].str,"0");
				}
				number_column++;
			}
			number_column=0; 
			numberline++;	
		}
		
		else if( (mystrcmp(",",command)==0) && columnindex[tableoperte][number_column].can_empty==1 ) 
		{
			if(type) database[tableoperte][number_column][numberline].num=0;
			else 
			{
				database[tableoperte][number_column][numberline].str = (char*)malloc(sizeof(char)*size);
				if(database[tableoperte][number_column][numberline].str == NULL) MEMROTY_ERROR
				mystrcpy(database[tableoperte][number_column][numberline].str,"0");
			}
			number_column++;
			if(number_column == _countcol) {number_column=0; numberline++;}			
			//Judge a line is over or not
		}
		
		else if( mystrcmp(LINENULL,command)==0 && columnindex[tableoperte][number_column].can_empty==0 ) 
		{
			while(number_column<_countcol)
			{
				type = columnindex[tableoperte][number_column].nametype;
				if(type) database[tableoperte][number_column][numberline].num=0;
				else 
				{
					database[tableoperte][number_column][numberline].str = (char*)malloc(sizeof(char)*size);
					if(database[tableoperte][number_column][numberline].str == NULL) MEMROTY_ERROR
					mystrcpy(database[tableoperte][number_column][numberline].str,"ERROR");
				}
				number_column++;
			}
			number_column=0; 
			numberline++;	
		}
			
		else
		{
			if(type) database[tableoperte][number_column][numberline].num=(float)atof(buffer);
			else 
			{
				database[tableoperte][number_column][numberline].str = (char*)malloc(sizeof(char)*size);
				if(database[tableoperte][number_column][numberline].str == NULL) MEMROTY_ERROR
					mystrcpy(database[tableoperte][number_column][numberline].str,buffer);
			}
			number_column++;
			if(mystrcmp(command,LINEEND)==0)
			{
				while(number_column<_countcol)
				{
				if(type) database[tableoperte][number_column][numberline].num=0;
				else 
				{
					database[tableoperte][number_column][numberline].str = (char*)malloc(sizeof(char)*size);
					if(database[tableoperte][number_column][numberline].str == NULL) MEMROTY_ERROR
					mystrcpy(database[tableoperte][number_column][numberline].str,"0");
				}
				number_column++;
				}
			}
			
			if(number_column == _countcol) {number_column=0; numberline++;}			
			//Judge a line is over or not
		}
			
	}
	fclose(file);
	return _countdata;
}

