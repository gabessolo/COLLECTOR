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

 #define CLOG_MAIN
 #include "clog.h"
 
 const int MY_LOGGER = 0; /* Unique identifier for logger */

 char command[BUFFER_RECV];

 static int sock_listen_command=0;

 int   port_pppx; 
 int   port_collecteur; 
 char* ip_pppx;
 char* ip_meuble;
 int   port_meuble;
 void* listener(void* param)
 {
    int r=0;
    /* Initialize the logger */
    r = clog_init_path(MY_LOGGER, "pppx_log.txt");
    if (r != 0) {
        fprintf(stderr, "Logger initialization failed.\n");
        return NULL;
    }
    /* Set minimum log level to info (default: debug) */
   clog_set_level(MY_LOGGER, CLOG_INFO);
   //clog_info(CLOG(MY_LOGGER),"Thread listener-command créé avec succés");
   
   int  clilen;
   char buffer[COMMAND_SIZE];
   struct sockaddr_in  cli_addr;
   
   init_pppx(&ip_meuble,&port_meuble,&port_pppx);
   while(init_sock_server(&sock_listen_command,port_pppx)<0)
   {
   	fprintf(stderr,"Erreur création de socket d'écoute sur le port :%d",port_pppx);
	sleep(T_CON);  
   }
  
   
   /* Now start listening for the clients, here
    *       * process will go in sleep mode and will wait
    *             * for the incoming connection
    *                */
   
   //clog_info(CLOG(MY_LOGGER),"écoute les commandes sur le  port:%d",port_pppx);
   listen(sock_listen_command,MAX_CLIENTS_PPPX);
   clilen = sizeof(cli_addr);

   int nb_connect=0;
   int searchNode=0; /* recherche la prochaine commande et en déduit la réponse dynamiquement */
   while (true) 
   {
      //clog_info(CLOG(MY_LOGGER),"Attente de connexion");
      int newsockfd = accept(sock_listen_command, (struct sockaddr *) &cli_addr, &clilen);
      //clog_info(CLOG(MY_LOGGER),"Nouvelle Connexion: %d",nb_connect);
		
      if (newsockfd < 0) {
         fprintf(stderr,"Erreur de connexion");
         return NULL;
      }

      bool clientDiscon=false;
      int searchNode=0;

      while(clientDiscon==false)
      {
      	doprocessing_pppx(newsockfd,&searchNode,&clientDiscon);
      }

      //clog_info(CLOG(MY_LOGGER),"fin de traîtement du client: %d",nb_connect++);
		
   } /* end of while */
   clog_free(MY_LOGGER);
 }


 void doprocessing_pppx (int sock,int* searchNode,bool* clientDiscon) {
   
   int n=0;
   //clog_info(CLOG(MY_LOGGER),"Attente d'une commande");
   memset(command, '0',BUFFER_RECV);
   n = read(sock,command,BUFFER_RECV);
   if (n==0) { 
	*clientDiscon=true;
	fprintf(stderr,"Erreur de lecture sur socket");
        return; 
   }
	
   command[n] = '\0';
   clog_info(CLOG(MY_LOGGER),"C ==> '%s'==> P",command);

   if (strcmp(command,RE_INIT)==0)
        return; 
	
   if (strcmp(command,"SIGNEDEVIE")==0)
        return; 


   //on construit une réponse dynamiquement en fonction de la commande
   //
   if (command[0]=='#')
   {
   	command[0] ='#';
   	command[1] ='R';
   	command[2] ='0';
   	command[3] ='0';
   	command[7]='1';
   	command[8]='2';
   	command[9]='3';
   	command[10]='1';
   	command[11]=';';
   	command[12]='\0';
   
   }else if (command[4]=='#')
   {
   	command[0] ='#';
   	command[1] ='R';
   	command[2] ='0';
   	command[3] ='0';
   	
        command[4] =command[8];
   	command[5] =command[9];
   	command[6] =command[10];
   	
        command[7]='1';
   	command[8]='2';
   	command[9]='3';
   	command[10]='1';
   	command[11]=';';
   	command[12]='\0';
   } 	
  
   do {
   n = send(sock,command,ACK_SIZE,0);
   sleep(T_SEND);
   }
   while(n<ACK_SIZE && n>0);
   
   if (n<=0)
	*clientDiscon=true;
   else
        clog_info(CLOG(MY_LOGGER),"P ==> '%s' ==> C",command);
  
   //sleep(T_SEND);
   command[3] ='A';

   n = send(sock,command,ACK_SIZE,0);
   if (n<0)
	*clientDiscon=true;
   else
        clog_info(CLOG(MY_LOGGER),"P ==> '%s' ==> C",command);
				
 }	
 
 void  creat_threads()
 {

  int err = pthread_create(&(tid[LISTEN_COM]), NULL, &listener, NULL);
  if (err != 0)
     fprintf(stderr,"Erreur lors de la création du thread LISTENER :[%s]", strerror(err));
  
  //err = pthread_create(&(tid[SEND_ACK]), NULL, &sender, NULL);
  //if (err != 0)
  //   printf("\r\ncan't create thread SENDER :[%s]\r\n", strerror(err));

  if(pthread_join(tid[LISTEN_COM], NULL)) {
     fprintf(stderr,"Erreur lors de la synchro");
	return ;
  }
  
  //if(pthread_join(tid[SEND_ACK], NULL)) {
  //   printf("\r\nError joining thread SENDER\r\n");
  //	return ;
  
  }
