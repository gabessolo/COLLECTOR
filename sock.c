#include "sock.h"

 int   init_sock_client(int* sockfd,const char* ip,int port,const char* serverName)
 {
    	struct sockaddr_in serv_addr; 

        if (sockfd==NULL)
	   return -1;	

    	if((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    	{
        	printf("\r\nErreur impossible de créer la socket\r\n");
        	return -2;
    	} 

   	memset(&serv_addr, '0', sizeof(serv_addr)); 

    	serv_addr.sin_family = AF_INET;
    	serv_addr.sin_port = htons(port); 

    	if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0)
    	{
        	printf("\r\ninet_pton error occured\r\n");
        	return -3;
    	} 
   
    	printf("\r\nTentative de connexion au Serveur <'%s'-ip:'%s'> <%d> <sockfd:%d> \r\n",serverName,ip,port,*sockfd);
    	if( connect(*sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    	{
    		printf("\n Erreur : Connexion echec sur le serveur:'%s'\n",serverName);
    		return -4;
    	} 

	return 0;
}

int init_sock_server(int* sockfd,int port)
{   
   struct sockaddr_in serv_addr;

   if (sockfd==NULL)
     return -1;

   *sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (*sockfd < 0) {
      printf("\r\nErreur création de socket\r\n");
      return -2;
   }
   
   bzero((char *) &serv_addr, sizeof(serv_addr));
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(port);
   
   if (bind(*sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      printf("\r\nErreur création de lien\r\n");
      return -3;
   }

   return 0;
}
 
