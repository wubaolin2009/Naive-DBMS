/* Filename: mystring.h  */
/* Function:   The replacement of library mystring.h */
/* Author Wubaolin          */
/* vision  2.1.0  */
/* change list  :   NONE */

#ifndef _MY_STRING_H
#define _MY_STRING_H

/* mystrcmp   The same as the lib function mystrcmp */
int mystrcmp(const char* s1,const char* s2);

/*Stringcopy   not copy \n */
/* to:Destination  from:Source */
/* Return to */
char* mystrcpy(char* to,const char* from);  

/* mystrstr   The same as the lib function mystrstr */
const char* mystrstr(char* buf,char* sub); 

#endif
