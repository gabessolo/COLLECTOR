 #include <stdio.h>
 #include "init.h"
 #include <sys/time.h>
 #include "defs.h"

 int init(char** ip_pppx,int* port_pppx)
 {

        if (ip_pppx==NULL) 
		ip_pppx=(char**)malloc(sizeof(char*));

        if (*ip_pppx==NULL) 
	       *ip_pppx=(char*)malloc(20*sizeof(char));   

        
        if (*ip_pppx==NULL) 
		strcpy(*ip_pppx,IP_PPPX);

	if (port_pppx!=NULL)    *port_pppx   =PORT_PPPX;
 
	return 0;
 }
