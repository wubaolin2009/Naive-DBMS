/* Filename: search.cpp  */
/* Function:   realization of search.h  */
/* Author Wubaolin          */
/* vision  2.1.0  */
/* change list  :  In header Files  */  

#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h>
#include"mystring.h"
#include "createtable.h"
#include "importtable.h"
#include "search.h"
time_t start;
time_t over;
char* getwords(FILE* file,char* recieve)  // Analyses the string 
{
	int count=0;
	int i=fgetc(file);
	while(isspace(i))
	{
		i=fgetc(file);  //Igonore ' '
	}
	if(i==EOF) return NULL;
	if(i=='{') i=fgetc(file);

	while(i!='\0' && i!='\n' &&i!=' ' && i!= ',' &&i!='{')
	{
		recieve[count++]=i;
		i=fgetc(file);
		if(i=='}') {recieve[count]='\0'; return COLUMNEND;}
		if(i==-1) {recieve[count]='\0'; return LINEEND;}
		if(i=='\n') {recieve[count]=0;return LINEEND;}

	}
	recieve[count]='\0';
	return recieve;
}
int tablesearch(char* filename,char (*tableindex)[TABLE_NAME_MAX],Column** columnindex,Data*** database)
{
	FILE* file;
	int isnosearch=0;  //  Is there no restriction to the column name 
	int colname_print[COL_READ_MAX];  // Store the number of column to print   -1 is the end
	int colname_do[COL_READ_MAX];  // the order os Column to operte  -1 is the end
	char colnameprints[COL_READ_MAX][COLUMN_NAME_MAX];// recieve every columname to print 
	char colvalue[COL_READ_MAX][COLUMN_NAME_MAX];   //recieve evey column_value
	int colname_order;  //the order of column to sort by 
	int colnumber=0;
	int andor[AND_OR_MAX]; //  and or ARRAY   or=0 and=1    -1 is the end  
	int likeequal[LIKE_EQUAL_MAX]; //like  = Array   -1 is the end   0=like 1 = =  
//	char buffer[LINE_MAX];   //Buffer of file 
	int countavalible=0;    //Items that has been read 
	int tableoperate;    // The order of operting table  

	int countnumber=0;  //How Many columns to print 
	int numberofprint=0;  //how many lines to print   if 0  means all 
	int asc=ASC;
	if((file=fopen(filename,"r"))==NULL)
	{
		printf("Error in opening the file!\n");
		return 0;
	}
	start=over=(time_t)0;  //count the time
	start=clock();
	checkline1(file,colname_print,&numberofprint,colnameprints,&tableoperate ,tableindex,columnindex);
	if(database[tableoperate] == NULL )  {printf("The table hasn't been Initialized!\n"); return 0;}
	char *revalue=checkline2(file, andor, likeequal,colvalue,colname_do,tableoperate,columnindex);	
	char* command=revalue;
	if(command==NULL)
	{
		colname_order=0;
		asc=ASC;
	}
	else 
	{
		if(!mystrcmp(command,"order"))
		{
			char buffer[LINE_MAX];
			command=getwords(file,buffer); //by
			if(mystrcmp("by",buffer)) 
			{
				printf("You should follow order a by\n");
				return 0;
			}
			command=getwords(file,buffer); //column_name
			colname_order= columnnow(buffer,tableoperate,columnindex);
			if(colname_order == -1)
			{printf("Error in lcating the %s column\n",buffer);
			return 0;
			}
			command=getwords(file,buffer);  //ASC DESC or EOF
			if(command==NULL) asc=ASC;
			else if(!mystrcmp("ASC",buffer)) asc=ASC; 
			else asc=DESC;
		}
	}
	fclose(file);
	int *index=CreateIndex(tableoperate,colname_do,colname_order,likeequal,colvalue,andor,columnindex,database);            //  Create index of table to print 

	int _coltype= columnindex[tableoperate][colname_order].nametype;   //Style of data to sort by 
	int numberitems=0;  //How many lines to print 
	while( index[numberitems] != -1) numberitems++;
	shellsort(index ,numberitems,_coltype,asc,database[tableoperate][colname_order]);
	//Put the result to file  and return how many  has been searched
	int countsearch = fileprint("074090_select.txt",numberofprint,colname_print,database[tableoperate],columnindex[tableoperate],index); 
	free(index);   //Free the index 
	return countsearch;
}
int checkline1(FILE* file,int* colname_print,int* numberofprint,char (*colnameprints)[COLUMN_NAME_MAX],int* tableoperate ,char (*tableindex)[TABLE_NAME_MAX],Column** columnindex)
{

	char buffer[LINE_MAX];  //Buffer
	int isall=0;
	char* command=getwords(file,buffer);  //read keyword select 
	if(mystrcmp(command,"select")!=0) 
	{
		printf("The file should be started by select\n");
		return 0;
	}
	command=getwords(file,buffer);  //Judge the existance of TOP
	if(mystrcmp(command,"TOP")==0)
	{
		command=getwords(file,buffer);  //read the number after TOP
		*numberofprint=atoi(command);
		command=getwords(file,buffer); 
		if(!mystrcmp("*",command)) 
		{
			isall=1; //is all to print 
			command=getwords(file,buffer);//read from
			if(mystrcmp("from",buffer))
			{
				printf("you should write a from before your table name\n");
				return 0;
			}
			command=getwords(file,buffer); //read table name 
		}
		else    //read columname 
		{
			isall=0;
			int scount=0;
			while(mystrcmp(COLUMNEND,command))
			{
				mystrcpy(colnameprints[scount++],command);
				command=getwords(file,buffer);
			}
			mystrcpy(colnameprints[scount],buffer);
			colnameprints[scount+1][0]='\0';
			command=getwords(file,buffer);//read  keyword from
			if(mystrcmp("from",buffer)) //not from
			{
				printf("you should write a from before your table name\n");
				return 0;
			}
			command=getwords(file,buffer); //read table name
		}	
	}
	else if(mystrcmp(command,"*")==0) {
		numberofprint=0;
		isall=1;
		command=getwords(file,buffer);//read keyword from 
		if(mystrcmp("from",buffer))
		{
			printf("you should write a from before your table name\n");
			return 0;
		}
		command=getwords(file,buffer); //read table name 


	}
	else //read column 
	{
		numberofprint=0;
		isall=0;
		int scount=0;
		while(mystrcmp(COLUMNEND,command))
		{
			mystrcpy(colnameprints[scount++],command);
			command=getwords(file,buffer);
		}
		mystrcpy(colnameprints[scount],buffer);
		colnameprints[scount+1][0]=0;
		command=getwords(file,buffer);//read from
		if(mystrcmp("from",buffer))
		{
			printf("you should write a from before your table name\n");
			return 0;
		}
		command=getwords(file,buffer); //read table name


	}

	*tableoperate = tablenow(buffer,tableindex);  //decide the order of operting table
	if(isall) //if all are to print 
	{
		
		int icount=0; //count how many columns 
		while(columnindex[*tableoperate][icount].name[0]!='\0') icount++;

		for(int i=0;i<icount;i++)
		{
			colname_print[i]=i;

		}
		colname_print[icount]=-1;//END
	}
	else  //Not all is to print
	{
		int count_i=0;
		while(colnameprints[count_i][0]!=0)
		{
			colname_print[count_i++]= columnnow(colnameprints[count_i],*tableoperate ,columnindex);
		}
		colname_print[count_i]=-1;  //END


	}
	return 1;

}
int columnnow(char* name,int tableoperate , Column** columnindex) //return the order of the column with the name 
{
	int count=0;
	while(columnindex[tableoperate][count].name[0]!='\0')
	{
		if(mystrcmp(columnindex[tableoperate][count].name,name) == 0)
			return count;
		count++;
	}
	return -1;
}
char* checkline2(FILE* file,int* andor,int* likeequal,char (*colvalue)[COLUMN_NAME_MAX],int* colname_do,int tableoperate,Column** columnindex)
{
	//check the key word Where
	char buffer[LINE_MAX];
	char *command=getwords(file,buffer);  //read
	if(command!=NULL&&!mystrcmp(buffer,"where"))
	{
		int countcolname=0;
		int countlike=0;
		int countand=0;
		int countcolvalue=0; 
		command=getwords(file,buffer); //read the first columnname
		int isname=1;   //The string is a name or a column value
		while(mystrcmp(command,LINEEND)!=0 && mystrcmp(command,"order")!=0)  //没有读完一行
		{
			if(mystrcmp(command,"like")==0)
			{
				likeequal[countlike++]=0;
			}
			else if(mystrcmp(command,"=")==0)
			{
				likeequal[countlike++]=1;  
			}
			else if(mystrcmp(command,"AND")==0)
			{
				andor[countand++]=1;
			}
			else if(mystrcmp(command,"OR")==0)
			{
				andor[countand++]=0;
			}
			else if (isname)
			{
				colname_do[countcolname++]=  columnnow(command,tableoperate,columnindex);
				isname=0;
			}
			else //It's the value of column 
			{
				mystrcpy(colvalue[countcolvalue++],command);
				isname=1;
			}
			command=getwords(file,buffer);

		}
		likeequal[countlike]=-1;
		andor[countand]=-1;
		colname_do[countcolname]=-1;
		mystrcpy(colvalue[countcolvalue],buffer);
		colvalue[countcolvalue+1][0]='\0';
		if(mystrcmp(command,"order")) command=getwords(file,buffer);
		return command;
	}
	else 
	{
		likeequal[0]=-1;
		andor[0]=-1;
		colname_do[0]=-1;
		colvalue[0][0]='\0';
		return command;
	}

}
int *CreateIndex(int tableoperate,int* colname_do,int colname_order,int* likeequal,char(*colvalue)[COLUMN_NAME_MAX],int* andor,Column** columnindex,Data*** database)
{
	int lineall = columnindex[tableoperate][0].length;  //Number of line of a table
	int* head = (int*)malloc(sizeof(int) * (lineall+1));
	if(head== NULL) MEMROTY_ERROR
	
	if(colname_do[0]==-1) //select all
	{
		int i = 0;
		while(i<lineall) //create index without restriction 
		{
				head[i]=i;
				++i;
		}

		head[i] = -1;	
	return head;
	}
	else //select not all
	{
		int count_index = 0;
		int count_line =0;
		while( count_line < lineall)  //Not all line has been searched 
		{
			if( Istrue( colname_do,likeequal,colvalue,andor ,columnindex[tableoperate],database[tableoperate],count_line) )
				head[count_index++] = count_line;
			++count_line;
		}
		head[count_index] = -1;
		return head;
	}
}
int Istrue(int* columnname_do,int* likeequal,char(*colvalue)[COLUMN_NAME_MAX],int* andor,Column* columnindex,Data** table_now,int linenum)
{
	int operater[COL_READ_MAX];  //and or Opeater Array 
	basicresult(columnname_do,likeequal,colvalue,operater,columnindex,table_now,linenum);
	int stacknum[10];  //Number Stack 
	int stackop[10]; // Opetater Stack 
	int istacknum=-1;  //top of stack 
	int istackop=-1;   //top of stack 
	int num=0;   // exit of number stack  
	int op=0;    //exit of operater stack
	if(andor[0]==-1) //没有and or 
		return operater[0];
	stacknum[0]=operater[num++];
	istacknum=0;

	while(operater[num]!=-1&&andor[op]!=-1)  //There are any numbers
	{

		stacknum[++istacknum]=operater[num++];
		if(istackop==-1)  //The number is 0  push
		{	
			stackop[++istackop]=andor[op++];
			if(stackop[istackop] == 1)  //if it's AND  pop
			{
				stacknum[istacknum-1]=calculate(stacknum+istacknum-1,1);
				istacknum=istacknum-1;
				istackop=istackop-1;				
			}
		}

		else if(andor[op]!=-1&& andor[op]>=stackop[istackop]) //push operaters
		{
			stackop[istackop+1]=andor[op++];
			istackop++;
			//pop   number an  operaters
			stacknum[istacknum-1]=calculate(stacknum+istacknum-1,stackop[istackop]);
			istacknum=istacknum-1;
			istackop--;
		}
		else if(andor[op]==-1) stacknum[0]=calculate(stacknum,stackop[0]);
		else //pop operaters
		{
			//pop   number an  operaters
			stacknum[istacknum-1]=calculate(stacknum+istacknum-1,stackop[istackop]);
			istacknum=istacknum-1;
			istackop--;
		}
	}
	if(operater[num]==-1)
		if(istacknum==1) stacknum[0]=calculate(stacknum,stackop[0]);
	return stacknum[0];

}
void basicresult(int* colname_do,int* likeequal,char(*colvalue)[COLUMN_NAME_MAX],int* operater,Column* columnindex,Data** table_now,int linenum)
{
	int counti=0;   //Count of Column 
	while(colname_do[counti]!=-1)  //not arriver at the end of a table
	{
		int _type= columnindex[colname_do[counti]].nametype;  //Datastyle
		int col = colname_do[counti];  //current column number
		if(likeequal[counti]==0)// It's like
		{
			if(_type) operater[counti]=1;
			else{
				if(mystrstr(table_now[col][linenum].str,colvalue[counti])!=NULL)  //match the needs
					operater[counti]=1;
				else operater[counti]=0;
			}
		}
		else  // It's =
		{
			if(_type)
			{
				operater[counti]= ( (int)table_now[col][linenum].num == atoi(colvalue[counti]) )?1:0;
			}
			else
			{
				if(!mystrcmp(table_now[col][linenum].str,colvalue[counti])) operater[counti]=1;
				else operater[counti]=0;
			}
		}
		counti++;
	}
	operater[counti]=-1;
}
int fileprint(char* filename,int numberofprint,int *colname_print,Data** database,Column* columnindex,int *index)
{
	int count=0;
	int colcount=0;
	int i=0;  //Line Number
	int j=0;  //Column NUMber
	FILE* file=fopen(filename,"w+");
	if(file==NULL) 
	{
		printf("Error in Writing Files\n");
		return count;
	}
	over=clock();
	fprintf(file,"execute time: %d ms\nexecute result:\n",(int)(over-start));
	start=over=(time_t)0;
	if(numberofprint==0)  //all items are to put out
	{
		while(index[i] != -1)  //Not at the end of a table
		{
			while( colname_print[j] != -1)
			{
				int _type= columnindex[ colname_print[j] ].nametype;  //datastyle 1 is loat
				if(_type)  fprintf(file,"%.2f ",database[ colname_print[j] ][ index[i] ].num); //float
				else  fprintf(file,"%s ",database[ colname_print[j] ][ index[i] ].str);
				j++;
			}
			j=0;
			fputc('\n',file);  //print '\n'
			i++;	
		}
	}
	else //print not all lines
	{
		while( index[i] != -1 && i<numberofprint )  //not arrive the end of table 
		{
			while( colname_print[j] != -1)
			{
				int _type= columnindex[ colname_print[j] ].nametype;  //Data style
				if(_type)  fprintf(file,"%.2f ",database[ colname_print[j] ][ index[i] ].num); //float
				else  fprintf(file,"%s ",database[ colname_print[j] ][ index[i] ].str);
				j++;
			}
			j=0;
			fputc('\n',file);  //print 'n'
			i++;	
		}
	}
	fclose(file);
	return i;
}
int calculate(int* num,int op)
{
	if(op==0) return num[0]||num[1];
	else return num[0]&&num[1];
}

void shellsort(int* index,int num,int _coltype,int asc,Data* database)
{
	int k,j,i;
	float t;
	char tf[20];
	k=num/2;
	if(_coltype == 1 && asc == 1)
	{
		while(k>0)
		{
			for(j=k; j<=num-1;j++)
			{
				t= database[index[j]].num;
				i=j-k;
				int indextemp = index[j];
				while( (i>=0) && ( database[index[i] ].num > t))
				{
					index[i+k] = index[i] ;
					i=i-k;
				}
				index[i+k] =indextemp;
			}
			k=k/2;
		}
		return;

	}
	else if(_coltype == 1 && asc == 0)
	{
		while(k>0)
		{
			for(j=k; j<=num-1;j++)
			{
				t= database[index[j]].num;
				i=j-k;
				int indextemp = index[j];
				while( (i>=0) && ( database[index[i] ].num < t))
				{
					index[i+k] = index[i] ;
					i=i-k;
				}
				index[i+k] = indextemp;
			}
			k=k/2;
		}
		return;

	}
	else if(_coltype == 0 && asc == 1)
	{
		while(k>0)
		{
			for(j=k; j<=num-1;j++)
			{
				mystrcpy(tf,database[ index[j] ].str);
				i=j-k;
				int indextemp = index[j];
				while( (i>=0) && ( mystrcmp(database[index[i] ].str, tf)>0) )
				{
					index[i+k] = index[i] ;
					i=i-k;
				}
				index[i+k] = indextemp;
			}
			k=k/2;
		}
		return;

	}
	else if(_coltype == 0 && asc == 0)
	{
		while(k>0)
		{
			for(j=k; j<=num-1;j++)
			{
				mystrcpy(tf,database[ index[j] ].str);
				i=j-k;
				int indextemp = index[j];
				while( (i>=0) && ( mystrcmp(database[index[i] ].str, tf)<0) )
				{
					index[i+k] = index[i] ;
					i=i-k;
				}
				index[i+k] = indextemp;
			}
			k=k/2;
		}
		return;
	}
}
