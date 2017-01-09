 #include <stdio.h>
 #include "init.h"
 #include <sys/time.h>

 int init(char** ip,int* port)
 {
	char buffer[50];
        time_t now;

        if (ip==NULL) ip=(char**)malloc(sizeof(char));
        
        *ip=(char*)malloc(20*sizeof(char));   

        strcpy(*ip,"192.168.38.102");

	*port=80;
 
        now=time(NULL);
        struct timeval* tv=localtime(&now); 

  	//sprintf(buffer,30,"%m-%d-%Y  %T.",localtime(tv));
        //printf("\r\nDemarrage Collecteur de Commandes:%s\r\n",buffer);
        //printf("\r\nInit OK\r\n");
  	//printf("%s%ld\r\n",buffer,tv.tv_usec);
	
	return 0;
 }
