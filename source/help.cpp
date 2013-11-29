/* Filename: help.cpp  */
/* Function:   realization of help.h  */
/* Author Wubaolin          */
/* vision  2.1.0  */
/* change list  :  In header Files  */  
#include <stdio.h>
#include "help.h"

void help()
{
	puts("\nThe commands are as follows:\n");
	puts("Create table: \n-c filename :It will create a table if it hasn't been created!:");
	puts("\nImport table: \n-i Tablename filename : It will import the datas to the Table");
	puts("\nSearch table: \n-s filename : It will search the table as you wish");
	puts("\nUpdate table: \n-u filename : It will update the table as you wish");
	puts("\nDelete table: \n-d filename : It will delete the table as you wish");
	puts("\nHelp:-h");
	puts("\nQuit -q \n Quit ,Free memory!\n");
}
