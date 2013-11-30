/* Filename: createtable.cpp  */
/* Function:   realization of createtable.h  */
/* Author Wubaolin          */
/* vision  2.1.0  */
/* change list  :  In header Files  */   
#include <stdio.h>
#include "mystring.h"
#include <malloc.h>
#include<stdlib.h>
#include <ctype.h>
#include "createtable.h"


void printscreen(void)   
{
	printf("         *****************************************************************   \n");
	printf("         * T i n y    I n - M e m D a t a B a s e     v e r s i o n 0.1  *   \n");
	printf("         *                                   CopyRight Reserved          *   \n");
	printf("         *****************************************************************  \n");
}

int is_include_dorm(char* name)  
{

	int i=0;
	while(name[i]!='\0')
	{
		if(name[i++]==';') return 1;
	}
	return 0;
}
char* getword(FILE* file,char* buffer)  //Reading words
{
	char temp;
	int i=0;
	temp=fgetc(file);
	while(isspace(temp)) temp=fgetc(file);
	if(temp==')') return NULL;
	while(temp=='(' || isspace(temp))
	{
		temp=fgetc(file);
	}
	while(temp!='(' &&temp!=' ' && temp!='\n')
	{
		buffer[i++]=temp;
		temp=fgetc(file);
	}
	buffer[i]='\0';
	return buffer;
}
int iscreated(char* name,char (*tableindex)[TABLE_NAME_MAX])  //Judge it has been created?
{
	for(int i=0;i<TABLE_MAX;i++)
	{
		if(tableindex[i][0]==0) return i;
		if(mystrcmp(name,tableindex[i])==0) return -1;
	}
	return -1;
}
int countcolumn(FILE* file)    //Column Number to create
{
	int c=0;
	int count=0;
	while((c=fgetc(file)) != EOF) 
		if(c==';') count++;
	return count;
}
int createtable(char* filename,char (*tableindex)[TABLE_NAME_MAX],Column** columnindex,Data*** database)  //Create DB
{
	
	char *fname=filename;  //Filename
	char buffer[20];  // Recieve every word
	char *command=buffer;
	FILE* file;  // pointer to FILE that you want to open 
	if((file=fopen(filename,"r+"))  ==  NULL)
	{
		printf("Error in reading filen!\n");
		return NULL;
	}
	
	int _countcol = countcolumn(file);
	/*do the input file*/
	rewind(file);
	command=getword(file,buffer);
	/*Judge Key word*/
	if(mystrcmp(command,"create")!=0 && mystrcmp(command,"CREATE")!=0) 
	{
		printf("Where is create!\n");
		return 0;
	}
	command=getword(file,buffer);
	if(mystrcmp(command,"table")!=0 && mystrcmp(command,"TABLE")!=0 )  
	{
		printf("Where is table!\n");
		return 0;
	}

	/*Reading table name */
	command=getword(file,buffer);
    int tableoperate = iscreated(command,tableindex); //Which table to operate
	if(tableoperate == -1)                 //The table has been created
	{
		printf("The table with the name of %s has been already created!\n",command);
		return 0;
	}
	mystrcpy(tableindex[tableoperate],command);  //Reading   table name
	command=getword(file,buffer);
	/*Reding  every column */
	int countcol=0;  // Column Count
	// Initialize every column 

	if( (columnindex[tableoperate] =(Column*) malloc (sizeof(Column)*(_countcol+1)) ) == NULL )
	{
		printf("Error in allocating memrory!\n");
		exit(1);
	}

	while(command!=NULL)  //Not at the EOF
	{
		mystrcpy(columnindex[tableoperate][countcol].name,command); //Reding Column Name
		command=getword(file,buffer);  //Style of reading 
		if(mystrcmp(command,"float")==0)
		columnindex[tableoperate][countcol].nametype=1;
		else columnindex[tableoperate][countcol].nametype=0;
		command=getword(file,buffer);  //Size
		columnindex[tableoperate][countcol].size=atoi(command);
		command=getword(file,buffer);  // NOT NULL  or  NULL
		if(mystrcmp(command,"NOT")==0)columnindex[tableoperate][countcol].can_empty=0;
		else columnindex[tableoperate][countcol].can_empty=1;
		//Ignore a line
		while(!is_include_dorm(command)) command=getword(file,buffer);
		countcol++;
		if( (command=getword(file,buffer)) ==NULL)   //Arrive at EOF
		{
			break;
		}
	}
	columnindex[tableoperate][countcol].name[0]='\0';
	fclose(file);
	return 1;
}
