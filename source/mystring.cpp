/* Filename: mystring.cpp  */
/* Function:   realization of mystring.h  */
/* Author Wubaolin          */
/* vision  2.1.0  */
/* change list  :  In header Files  */   

#include "mystring.h"

int mystrcmp(const char* s1,const char* s2)
{
	while( *s1!=0 && *s2!=0 && *(s1++) == *(s2++)  );
	if(*s1 == *s2 ) return 0;
	return ( *s1 > *s2 )?1:-1;
}

const char* mystrstr(char* buf,char* sub)    
{    
	char *bp;   
	char *sp;    
	if (!*sub)     
		return buf;   
	while (*buf)   
	{       
		bp = buf;  
		sp = sub;  
		do 
		{ 
			if (!*sp) return buf;     
		} while (*bp++ == *sp++);        
		buf += 1;    
	}    
	return 0;
}

char *mystrcpy(char* to,const char* from)   //strcopy 
{
	int i=0;
	while(from[i]!='\n' && from[i]!='\0')
	{
		to[i]=from[i];
		i++;
	}
	to[i]='\0';
	return to;
}