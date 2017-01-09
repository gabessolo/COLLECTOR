 #include <stdio.h>
 #include <string.h>
 #include <pthread.h>
 #include <stdlib.h>
 #include <unistd.h>        
 #include <netdb.h>
 #include <netinet/in.h>
 #include "worker.h"
 #include "defs.h"


 pthread_t tid[2];

 char command[SIZE_BUFFER_RECV];
 extern int  port_pppx; 

 static struct list* list_commands=NULL;
 static struct list* list_ack=NULL;
 static char ack[ACK_SIZE];

 void* listener(void* param)
 {
   printf("\r\nThread LISTENER<pppx>	created successfully\r\n");
   printf("\r\n port_pppx:%d\r\n",port_pppx);
   
   int sockfd, newsockfd, portno, clilen;
   char buffer[COMMAND_SIZE];
   struct sockaddr_in serv_addr, cli_addr;
   int n, pid;
   
   /* First call to socket() function */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      printf("\r\nERROR opening socket\r\n");
      exit(1);
   }
   
   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(port_pppx);
   
   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      printf("\r\nERROR on binding\r\n");
      exit(1);
   }
  
   if (list_commands==NULL)
	list_commands=(struct list*)creer_list();
	
   if (list_commands==NULL) {
      printf("\r\nERROR on creating list<COMMAND>\r\n");
      exit(1);
   }
  
   struct list* _list_c=list_commands;   
   init_list(_list_c);   
 
   if (list_ack==NULL)
	list_ack=(struct list*)creer_list();
	
   if (list_ack==NULL) {
      printf("\r\nERROR on creating list<ACK>\r\n");
      exit(1);
   }
  
   struct list* _list_a=list_ack;   
   init_list(_list_a);
   
   /* Now start listening for the clients, here
    *       * process will go in sleep mode and will wait
    *             * for the incoming connection
    *                */
   
   printf("\r\nThread Listener<pppx> en attente de commandes\r\n");
   listen(sockfd,5);
   clilen = sizeof(cli_addr);

   int nb_connect=1;
   int searchNode=0; /* recherche la prochaine commande et en déduit la réponse dynamiquement */
   while (true) 
   {
      newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
      printf("\r\nNew Connection <pppx>: %d\r\n",nb_connect++);
		
      if (newsockfd < 0) {
         printf("\r\nERROR on accept\r\n");
         exit(1);
      }
      bool clientDiscon=false;
      while(clientDiscon==false)
      {
      	doprocessing_pppx(newsockfd,&searchNode,&clientDiscon);
      }

      printf("\r\nfin proccessing<pppx>: %d\r\n",nb_connect);
		
   } /* end of while */
 }


 void doprocessing_pppx (int sock,int* searchNode,bool* clientDiscon) {
   int n=0;
   bzero(command,SIZE_BUFFER_RECV);
   
   printf("\r\nThread listener<pppx> Waiting for a New command\r\n");
   
   do {
    n = read(sock,command,COMMAND_SIZE);
    command[n]='\0';
    if (n==0) {*clientDiscon=true; return;}
	
    printf("\r\nThread listener<pppx> Receaved a New command from Collector %s[%d]\r\n",command,n);
    sleep(T_READ);
   }
   while(n!=(COMMAND_SIZE-1) && n!=(ACK_SIZE-1));

   printf("\r\nThread listener<pppx> New command receaved:%s \r\n",command);
   if (n < 0) {
      printf("\r\nERROR reading from socket\r\n");
      exit(1);
   }
   
   struct list* _list_c=list_commands;
   struct list* _list_a=list_ack;

   struct noeud* _noeud_command=(struct noeud*)creer_noeud();
   init_noeud(_noeud_command,_list_c->count);   
   init_command(_noeud_command,command,_list_c->count);   
   if (_list_c!=NULL)
   {
   	//pthread_mutex_lock(&(_list_c->mutex));
	//fill the list of command 
   	ajouter_noeud(_list_c,_noeud_command);	  
        printf("\r\nThread listener<pppx> add command :%s \r\n",command);
	//pthread_cond_signal(&(_list->condition));  
	//pthread_mutex_unlock(&(_list_c->mutex));
	sleep(T_AJ_CMD);
   }

   if (strcmp(RE_INIT,command)!=0)
   {
        //printf("\r\nThread listener<pppx> [0]\r\n");
	if (_noeud_command!=NULL){
        	//printf("\r\nThread listener<pppx> [1] \r\n");
		if (_noeud_command->data!=NULL)
		{
        	//	printf("\r\nThread listener<pppx> [2] \r\n");
			if (_noeud_command->data->commande!=NULL)
			{
			       //on construit une réponse dynamiquement en fonction de la commande
			       //et on la conserve dans la liste des ack ... en  réalité cela fait doublon
			       //
   			       struct noeud* _noeud_ack=(struct noeud*)creer_noeud();
                    	       init_noeud(_noeud_ack,_list_a->count);   
                    	       init_ack(_noeud_ack,"#0123456789;",_list_a->count);   

			      _noeud_ack->data->ack[0] ='#';
                              _noeud_ack->data->ack[1] ='R';
                              _noeud_ack->data->ack[2] ='0';
                              _noeud_ack->data->ack[3] ='A';
                              _noeud_ack->data->ack[4] =_noeud_command->data->commande[4];
                              _noeud_ack->data->ack[5] =_noeud_command->data->commande[5];
                              _noeud_ack->data->ack[6] =_noeud_command->data->commande[6];
                              _noeud_ack->data->ack[7] =_noeud_command->data->commande[7];
                              _noeud_ack->data->ack[8] =_noeud_command->data->commande[8];
                              _noeud_ack->data->ack[9] =_noeud_command->data->commande[9];
                              _noeud_ack->data->ack[10]=_noeud_command->data->commande[10];
                              _noeud_ack->data->ack[11]=';';
                              
			      //_noeud_ack->data->ack[12]='\0';
                    	      //init_ack(_noeud_ack,ack,_list_a->count);   
   			      //pthread_mutex_lock(&(_list_a->mutex));
   			       
                              // _noeud_ack->index=_list_a->count;
			       ajouter_noeud(_list_a,_noeud_ack);	  
				//sleep(T_AJ_CMD);
				
			       n = send(sock,_noeud_ack->data->ack/*"#R0A0011231;"*/,ACK_SIZE,0);
        		       if (n==(ACK_SIZE)) printf("\r\nThread listener<pppx> sent ACK <<<<<<<< %s >>>>>>>>>>> \r\n",_noeud_ack->data->ack);
			        	
			      // sleep(T_SEND);
			       //fill the list of command 
			       //pthread_cond_signal(&(_list->condition));  
			       //pthread_mutex_unlock(&(_list_a->mutex));
				
			}
		}
	}
   
   	if (n < 0) {
      		printf("\r\nERROR writing to socket\r\n");
      	exit(1);
   	}
   }
 }
	
 void* sender(void* param)
 {
   printf("\r\nThread SENDER<pppx>	 created successfully\r\n");
 }

 void  creat_threads(char* ip,int port)
 {

  //strcpy(ip_pass,ip);
  port_pppx=port; 
  
  int err = pthread_create(&(tid[LISTENER]), NULL, &listener, NULL);
  if (err != 0)
     printf("\r\ncan't create thread LISTENER :[%s]\r\n", strerror(err));
  
  //err = pthread_create(&(tid[SENDER]), NULL, &sender, NULL);
  //if (err != 0)
  //   printf("\r\ncan't create thread SENDER :[%s]\r\n", strerror(err));

  if(pthread_join(tid[LISTENER], NULL)) {
     printf("\r\nError joining thread LISTENER\r\n");
	return ;
  }
  //if(pthread_join(tid[SENDER], NULL)) {
  //   printf("\r\nError joining thread SENDER\r\n");
//	return ;
 // }
 }
