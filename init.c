 #include <stdio.h>
 #include "init.h"
 #include <sys/time.h>
 #include "defs.h"

int  init_pppx(char** ip_meuble,int* port_meuble,int* port_pppx)
{
        if (ip_meuble!=NULL && *ip_meuble==NULL) 
	       *ip_meuble=(char*)malloc(20*sizeof(char));   
	else return 1;

        if (*ip_meuble!=NULL) 
		strcpy(*ip_meuble,IP_MEUBLE);
	else return 2;
	
	if (port_meuble!=NULL)  
		*port_meuble=PORT_MEUBLE;
	else return 3;
	
	if (port_pppx!=NULL)  
		*port_pppx=PORT_PPPX;
	else return 4;
        
        printf("\r\nport pppx  :%d\r\n",*port_pppx); 
        printf("\r\nport meuble:%d\r\n",*port_meuble); 
        printf("\r\nip   meuble:'%s'\r\n",*ip_meuble); 
	return 0;


}

 char* init_collecteur(char** ip_pppx,int* port_pppx,int* port_collecteur)
 {
        if (ip_pppx!=NULL && *ip_pppx==NULL) 
	       *ip_pppx=(char*)malloc(20*sizeof(char));   
	else return NULL;

        if (*ip_pppx!=NULL) 
		strcpy(*ip_pppx,IP_PPPX);
	else return NULL;

	if (port_pppx!=NULL)  
		*port_pppx=PORT_PPPX;
	else return NULL;
	
	if (port_collecteur!=NULL)  
		*port_collecteur=PORT_COLLECTEUR;
	else return NULL;
        
        char** ip_collecteur=NULL;
	ip_collecteur=(char**)malloc(sizeof(char));   
	*ip_collecteur=(char*)malloc(20*sizeof(char));   

	strcpy(*ip_collecteur,IP_COLLECTEUR);
	
	printf("\r\nport pppx  :%d\r\n",*port_pppx); 
        printf("\r\nport collecteur:%d\r\n",*port_collecteur); 
        printf("\r\nip   pppx:'%s'\r\n",*ip_pppx); 
        printf("\r\nip   collecteur:'%s'\r\n",*ip_collecteur); 
	
	return *ip_collecteur;
 }
