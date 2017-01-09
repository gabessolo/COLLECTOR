 #include <stdio.h>
 #include "init.h"
 #include <sys/time.h>

 int init(char** ip_pppx,int* port_weblogi,int* port_pppx)
 {
	char buffer[50];
        time_t now;

        if (ip_pppx==NULL) ip_pppx=(char**)malloc(sizeof(char));
        
        *ip_pppx=(char*)malloc(20*sizeof(char));   

        strcpy(*ip_pppx,"127.0.0.1");

	if (port_weblogi!=NULL) *port_weblogi=5000;
	if (port_pppx!=NULL)    *port_pppx   =9000;
 
        now=time(NULL);
        struct timeval* tv=localtime(&now); 

  	//sprintf(buffer,30,"%m-%d-%Y  %T.",localtime(tv));
        //printf("\r\nDemarrage Collecteur de Commandes:%s\r\n",buffer);
        //printf("\r\nInit OK\r\n");
  	//printf("%s%ld\r\n",buffer,tv.tv_usec);
	
	return 0;
 }
