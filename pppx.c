 #include <stdio.h>
 #include <string.h>
 #include <pthread.h>
 #include <stdlib.h>
 #include <unistd.h>        
 #include <netdb.h>
 #include <netinet/in.h>
 #include "worker.h"
 #include "defs.h"
 #include "pppx.h" 
 #include "sock.h" 


 char command[BUFFER_RECV];


 static int sock_listen_command=0;

int   port_pppx; 
int   port_collecteur; 
char* ip_pppx;
char* ip_meuble;
int   port_meuble;
 void* listener(void* param)
 {
   printf("\r\nThread listener-command créé avec succés\r\n");
   
   int  clilen;
   char buffer[COMMAND_SIZE];
   struct sockaddr_in serv_addr, cli_addr;
   int n, pid;
   
   init_pppx(&ip_meuble,&port_meuble,&port_pppx);
   while(init_sock_server(&sock_listen_command,port_pppx)<0)
   {
   	printf("\r\nErreur création de socket d'écoute sur le port :%d\r\n",port_pppx);
	sleep(T_CON);  
   }
  
   
   /* Now start listening for the clients, here
    *       * process will go in sleep mode and will wait
    *             * for the incoming connection
    *                */
   
   printf("\r\nécoute les commandes sur le  port:%d\r\n",port_pppx);
   listen(sock_listen_command,MAX_CLIENTS_PPPX);
   clilen = sizeof(cli_addr);

   int nb_connect=0;
   int searchNode=0; /* recherche la prochaine commande et en déduit la réponse dynamiquement */
   while (true) 
   {
      printf("\r\nAttente de connexion\r\n");
      int newsockfd = accept(sock_listen_command, (struct sockaddr *) &cli_addr, &clilen);
      printf("\r\nNouvelle Connexion: %d\r\n",nb_connect);
		
      if (newsockfd < 0) {
         printf("\r\nErreur de connexion\r\n");
         exit(1);
      }

      bool clientDiscon=false;
      int searchNode=0;

      while(clientDiscon==false)
      {
      	doprocessing_pppx(newsockfd,&searchNode,&clientDiscon);
      }

      printf("\r\nfin de traîtement du client: %d\r\n",nb_connect++);
		
   } /* end of while */
 }


 void doprocessing_pppx (int sock,int* searchNode,bool* clientDiscon) {
   
   int n=0;
   printf("\r\nAttente d'une commande\r\n");
   memset(command, '0',BUFFER_RECV);
   n = read(sock,command,BUFFER_RECV);
   if (n==0) { 
	*clientDiscon=true;
	printf("\r\nErreur de lecture sur socket\r\n");
        return; 
   }
	
   command[n] = '\0';
   printf("\r\nCOLLECTEUR ===> '%s'\r\n",command);

   if (strcmp(command,RE_INIT)==0)
        return; 
	
   if (strcmp(command,"SIGNEDEVIE")==0)
        return; 


   //on construit une réponse dynamiquement en fonction de la commande
   //
   command[0] ='#';
   command[1] ='R';
   command[2] ='0';
   command[3] ='A';
   command[11]=';';
   command[12]='\0';
				
   n = send(sock,command,ACK_SIZE,0);
   if (n<0)
	*clientDiscon=true;
   else
        printf("\r\nPPPX <=== '%s'\r\n",command);
   // sleep(T_SEND);
				
 }	
 
 void  creat_threads()
 {

  int err = pthread_create(&(tid[LISTEN_COM]), NULL, &listener, NULL);
  if (err != 0)
     printf("\r\nErreur lors de la création du thread LISTENER :[%s]\r\n", strerror(err));
  
  //err = pthread_create(&(tid[SEND_ACK]), NULL, &sender, NULL);
  //if (err != 0)
  //   printf("\r\ncan't create thread SENDER :[%s]\r\n", strerror(err));

  if(pthread_join(tid[LISTEN_COM], NULL)) {
     printf("\r\nErreur lors de la synchro\r\n");
	return ;
  }
  
  //if(pthread_join(tid[SEND_ACK], NULL)) {
  //   printf("\r\nError joining thread SENDER\r\n");
  //	return ;
  
  }
