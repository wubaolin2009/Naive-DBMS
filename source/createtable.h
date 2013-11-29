/* Filename createtable.h */
/* Function Create table in the form of Union Array */
/* An multiple array as the whole index of table name as to locate the table you're oprating now*/
/* Author Wubaolin          */
/* Current vision 2.1.0   */
/* change list:  
2.0.0   These file has change the whole frame of the program  in order to make search opertate more effective
I 've change the form of saving data from linked table to Union Array  ,   And the things about table and column 
has been put in sth like Index  *
2.1.0   These part of function is not changed   */


/* Create database and function declarationg*/
#ifndef _CREATE_TABLE
#define _CREATE_TABLE
#include <stdio.h>
#include "datatype.h"
/* Read word from file,   once per deed  Not include \n ( tab */
/* file  pointer to FILE      buffer Where to write the word read to */
/*  If the word is key word or others return buffer,  If it arrives end Return NULL */
char* getword(FILE* file,char* buffer);

/* Judge how many column should be created in a file*/
/* file Pointer to FILE */
/* Return Column number */
int countcolumn(FILE* file);

/* Judge if any  ';'  appeared */
/* Name is the string that you want't to search */
/* Yes  return 1    esle 0 */
int is_include_dorm(char* name);

/* Locate the table withe the name of name */
/* name: table name   tableindex: Index of table*/
/* If the table has been created return -1    , Else return the accurate position of the table to create */
int iscreated(char* name,char (*tableindex)[TABLE_NAME_MAX]);

/* Create   DB								*/
/* filename :Comand file name     tableindex Index of searching a Table */  
/*columnindex Index fo searching a column    database The DB */
/* Succeed 1     fail  0   */
int createtable(char* filename,char (*tableindex)[TABLE_NAME_MAX],Column** columnindex,Data*** database);

/* Print the screen  */
void printscreen(void); 
#endif
