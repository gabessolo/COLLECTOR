 #include <stdio.h>
 #include "init.h"

 int init(char** ip_passerelle,int* port_passerelle)
 {
	if (ip_passerelle==NULL || port_passerelle==NULL)   	
		return 	1;

	strcpy(*ip_passerelle,"127.0.0.1");
   	*port_passerelle=39000;

	return 0;
 }
