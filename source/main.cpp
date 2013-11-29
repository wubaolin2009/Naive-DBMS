/* Filename: main.cpp  */
/* Function:   Excution of program  */
/* Author Wubaolin          */
/* vision  2.1.0  */
#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <stdlib.h>
#include "datatype.h"
#include "createtable.h"
#include "importtable.h"
#include "search.h"
#include "update.h"
#include "quit.h"
#include "help.h"
#include "delete.h"

//#include <crtdbg.h>
//#include "mystring.h"

int main(void)
{
	//#ifdef _DEBUG
	//_CrtSetDbgFlag(_crtDbgFlag|_CRTDBG_LEAK_CHECK_DF);
	//char* pDumMemoryLeak = new char[20];	
	//::mystrcpy(pDumMemoryLeak,"Dum Leak");
	//#endif
	int i=1;
	char buffer[LINE_MAX];
	char* command=buffer;  // check  command
	
	char tableindex[TABLE_MAX][TABLE_NAME_MAX]={0};  //Index of Table		
	Data*** database = (Data***)malloc(sizeof(Data**) * TABLE_MAX);  //Data 
	if(database == NULL) {printf("error in creating Table\n"); exit(1);}
	int j1=0;
	for(j1=0;j1<TABLE_MAX;j1++) database[j1] = NULL;  //Initialize every table to NULL
	Column** columnindex= (Column**) malloc( sizeof(Column*) * TABLE_MAX) ;  //Index of Column 
	if(database == NULL) {printf("error in creating Column\n"); exit(1);}
	int j2=0;
	for(j2=0;j2<TABLE_MAX;j2++) columnindex[j2] = NULL;  //Initialize every column to NULL
	printscreen();   //print screen 
	while(i)
	{
		printf("DB1=>");
		gets(buffer);
		command=buffer;
		while(isspace(*command)){command++;}//ignore ' '
		if(*(command)++!='-')
		{
			printf("command should be started by - \n");
			continue;
		}
		if(*command == 'c')  //create database
		{
			command++;
			while(isspace((*command))){command++;}//ignore ' '
			if(createtable(command,tableindex,columnindex,database) == 0)
				printf("Error in Initializing the table and create it!\n");
			else printf("Succeed in Initializing the table!\n");
			continue;
		}
		if(*command=='i')  // Import DB
		{
			command++;
			while(isspace((*command))){command++;}//ignore ' '
			int i=0;
			char tablename[TABLE_NAME_MAX];
			char filename[20];
			while(*command!=' ')
			{
				tablename[i++]=*(command++);
			}
			tablename[i]='\0';
			while(isspace((*command))){command++;}//ignore ' '
			i=0;
			while(*command!='\0')
			{
				filename[i++]=*(command++);
			}
			filename[i]='\0';
			printf("%d items has been imported\n", ImportTable(tablename,filename,tableindex,columnindex,database));
			continue;
		}
		if(*command=='s')
		{
			command++;
			while(isspace((*command))){command++;}///ignore ' '
			printf("------%d Items has been searched !--------------\n",tablesearch(command,tableindex,columnindex,database) );
			continue;
		}
		if(*command == 'u')  //ignore ' '
		{
			command++;
			while(isspace((*command))){command++;}///ignore ' '
			printf("-------%d Items has been updated!---------------\n",Updatetable(tableindex,columnindex,database,command));
			printf("The result is printed to the File 074090_update.txt \n");
			continue;
		}
		if(*command == 'd')  //delete DB
		{
			command++;
			while(isspace((*command))){command++;}///ignore ' '
			printf("-------%d Items has been Deleted!---------------\n",Deletetable(command,tableindex,columnindex,database) );
			printf("The result is printed to the File 074090_delete \n");
			continue;
		}
		if(*command == 'h')  //help
		{
			help();
			continue;
		}
		if(*command == 'q')  //Quit free Memrory
		{
			quit(database,columnindex,tableindex);
			break;
		}
		else
		{
			puts("Commands can't ben supported!\n");
		}

	}
	printf("\nThank you for using!");
	return 0;
}
