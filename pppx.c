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

// extern int  port_pppx; 
// extern int  port_meuble; 
// extern char*  ip_meuble; 

 static struct list* list_commands=NULL;
 static char ack[ACK_SIZE];
 static int sock_listen_command=0;

int   port_pppx; 
int   port_collecteur; 
char* ip_pppx;
char* ip_meuble;
int   port_meuble;
 void* listener(void* param)
 {
   printf("\r\nThread listener créé avec succés\r\n");
   
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
  
   if (list_commands==NULL)
	list_commands=(struct list*)creer_list();
	
   if (list_commands==NULL) {
      printf("\r\nErreur à la création d'une liste de commandes\r\n");
      exit(1);
   }
  
   struct list* _list_c=list_commands;   
   init_list(_list_c);   
 
   
   /* Now start listening for the clients, here
    *       * process will go in sleep mode and will wait
    *             * for the incoming connection
    *                */
   
   printf("\r\nécoute les commandes sur le  port:%d\r\n",port_pppx);
   listen(sock_listen_command,1);
   clilen = sizeof(cli_addr);

   int nb_connect=0;
   int searchNode=0; /* recherche la prochaine commande et en déduit la réponse dynamiquement */
   while (true) 
   {
      printf("\r\nAttente de commandes\r\n");
      int newsockfd = accept(sock_listen_command, (struct sockaddr *) &cli_addr, &clilen);
      printf("\r\nNouvelle Connexion: %d\r\n",nb_connect++);
		
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

      printf("\r\nfin de traîtement du client: %d\r\n",nb_connect);
		
   } /* end of while */
 }


 void doprocessing_pppx (int sock,int* searchNode,bool* clientDiscon) {
   int n=0;
   bzero(command,BUFFER_RECV);
   
   struct list* _list_c=list_commands;

   if (_list_c!=NULL)
   {
   	pthread_mutex_lock(&(_list_c->mutex));
	//get the next command 
	struct noeud* _noeud=extractMessage(_list_c,searchNode/* prochaine commande */);
        printf("\r\nGet command:%s \r\n",command);
	pthread_mutex_unlock(&(_list_c->mutex));
	//sleep(T_AJ_CMD);

	if (_noeud!=NULL)
	{
	    if (_noeud->data!=NULL)
	    {
        	//printf("\r\nThread listener<pppx> [1] \r\n");
		if (_noeud->data->commande!=NULL)
		{
			       //on construit une réponse dynamiquement en fonction de la commande
			       //

			      _noeud->data->ack[0] ='#';
                              _noeud->data->ack[1] ='R';
                              _noeud->data->ack[2] ='0';
                              _noeud->data->ack[3] ='A';
                              _noeud->data->ack[4] =_noeud->data->commande[4];
                              _noeud->data->ack[5] =_noeud->data->commande[5];
                              _noeud->data->ack[6] =_noeud->data->commande[6];
                              _noeud->data->ack[7] =_noeud->data->commande[7];
                              _noeud->data->ack[8] =_noeud->data->commande[8];
                              _noeud->data->ack[9] =_noeud->data->commande[9];
                              _noeud->data->ack[10]=_noeud->data->commande[10];
                              _noeud->data->ack[11]=';';
                              
			      _noeud->data->ack[12]='\0';
				
			       n = send(sock,_noeud->data->ack,ACK_SIZE,0);
        		       if (n<0)
				*clientDiscon=true;
 			       else
				printf("\r\nThread listener à envoyé un ACK <<< %s >>> \r\n",_noeud->data->ack);
			        	
			      // sleep(T_SEND);
				
		}
	    }
        } 
   }
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
