/* Filename: datatype.h  */
/* Function:   define WORD and some Data style */
/* Author Wubaolin          */
/* vision  2.1.0  */
/* change list  :   2.1.0  Insert some new defines as the return value of function getword */

#ifndef _DATA_TYPE_H
#define _DATA_TYPE_H
//Restriction of Database
#define TABLE_MAX 10 //Max tables that can be open in a program
#define LINENULL "linenull"  //when getsource   only get a '\n' at the begining 
#define TABLE_NAME_MAX 10  // The Max number of words in table name
#define COLUMN_NAME_MAX 10 // The Max number of words in column name 
#define DATA_WORD_MAX 100  //The Max number of word in per column 
#define LINE_MAX 80     // The Max number of word in every line 
#define COL_READ_MAX 10 //The Max number of column in a table
#define ASC 1  
#define DESC 0 
#define AND_OR_MAX 10 //The Max number of AND OR
#define LIKE_EQUAL_MAX 10 //The Max number of like =
#define LINEEND "lineend"   //getword to the end of a line 
#define COLUMNEND "colend"  //getword to the end of a column
#define  MEMROTY_ERROR {printf("Error in memory opertate!\n"); exit(1);}  //Memrory Error
/* Defines of Data */
typedef union data   //Data
{
	float num;
	char* str;
}Data;
typedef struct column  //Index of Column
{
	char name[COLUMN_NAME_MAX]; //The MAx number of column name
	int  nametype;  //Style name    "float" or "char
	int size;       //element size
	int can_empty;  // Can empty
	int length;  //  numbere of Line 
}Column;

#endif
