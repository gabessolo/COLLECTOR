 #include <stdio.h>
 #include <string.h>
 #include <pthread.h>
 #include <stdlib.h>
 #include <unistd.h>        
 #include <netdb.h>
 #include <netinet/in.h>
 #include "worker.h"
 #include "defs.h"

 char message[SIZE_BUFFER_SENT];  //20 messages max + message extinction
 char header_message[SIZE_BUFFER_SENT];

 pthread_t tid[2];

 char command[SIZE_BUFFER_RECV];
 char buffer[COMMAND_SIZE];
 
 extern int  port_weblogi; 
 extern int  port_pppx; 
 extern char* ip_pppx;

 static struct list* list_commands=NULL;
 static struct list* list_ack     =NULL;

 int sock_command_pppx=0;

 void* listener(void* param)
 {
   printf("\r\nThread LISTENER<collector>	created successfully\r\n");
   printf("\r\n ip_pppx:%s port_pppx:%d port_weblogi:%d  \r\n",ip_pppx,port_pppx,port_weblogi);
   
   int sockfd, newsockfd, portno, clilen;
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
   serv_addr.sin_port = htons(port_weblogi);
   
   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      printf("\r\nERROR on binding\r\n");
      exit(1);
   }
  
   if (list_commands==NULL)
	list_commands=(struct list*)creer_list();
	
   if (list_commands==NULL) {
      printf("\r\nERROR on creating list<commandes>\r\n");
      exit(1);
   }
  
   struct list* _list_c=list_commands;   
   init_list(_list_c);   
 
   if (list_ack==NULL)
	list_ack=(struct list*)creer_list();
	
   if (list_ack==NULL) {
      printf("\r\nERROR on creating list<ack>\r\n");
      exit(1);
   }
  
   struct list* _list_a=list_ack;   
   init_list(_list_a);
   
   /* Now start listening for the clients, here
    *       * process will go in sleep mode and will wait
    *             * for the incoming connection
    *                */
   
   printf("\r\nThread Listener<collector> en attente de commandes\r\n");
   listen(sockfd,5);
   clilen = sizeof(cli_addr);

   int nb_connect=1;
   int searchNode=0; //noeud de recherche du ACK
   while (true) 
   {
      newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
      printf("\r\nnb_connect<collector>: %d\r\n",nb_connect++);
		
      if (newsockfd < 0) {
         printf("\r\nERROR on accept\r\n");
         exit(1);
      }
      
      while(1)
      {
      	doprocessing_collector(newsockfd,newsockfd,&searchNode);
      }

      printf("\r\nfin proccessing<collector>: %d\r\n",nb_connect++);
		
   } /* end of while */
 }

 //reçoit les commandes de weblogi
 //envoi les ack de  pppx à weblogi
 void doprocessing_collector (int sock_command,int sock_ack_weblogi,int* searchNode /* prochain ACK*/) {

   int n=0;
   //bzero(command,SIZE_BUFFER_RECV);
   printf("\r\nThread listener<collector> waiting for a New command\r\n");
   memset(command, '0',COMMAND_SIZE);
   n = read(sock_command,command,COMMAND_SIZE);
   
   if (n < 0) {
      printf("\r\nERROR reading from socket\r\n");
      exit(1);
   }
   command[n]='\0';
   struct list* _list_c=list_commands; /* contient toutes les commandes en provenance de weblogi */
   struct list* _list_a=list_ack;      /* contient tous les ack en provenance du meuble via la passerelle */
   struct noeud* node=(struct noeud*)creer_noeud();
   init_command(node,command,_list_c->count);   
   if (_list_c!=NULL)
   {
   	pthread_mutex_lock(&(_list_c->mutex));
	//fill the list of command 
   	ajouter_noeud(_list_c,node);	  
   	printf("\r\nThread listener<collector>	add command :%s\r\n",command);
	//pthread_cond_signal(&(_list->condition));  
	pthread_mutex_unlock(&(_list_c->mutex));
	sleep(2);
   }

   if (strcmp(RE_INIT,command)!=0)
   {
   	//devrait être le retour de la passerelle stocké dans une  liste adaptée
	pthread_mutex_lock(&(_list_a->mutex));
	//pthread_cond_wait(&(_list->condition),&(_list->mutex));
   	printf("\r\nThread listener<collector>	TRY to extract  ACK no:%d\r\n",*searchNode);
	struct noeud* _noeud=extractMessage(_list_a,searchNode/* prochain ACK*/);
	pthread_mutex_unlock(&(_list_a->mutex));
	if (_noeud!=NULL)
	{
		if (_noeud->data!=NULL)
		{
			if (_noeud->data->ack!=NULL)
			{
				if (sock_ack_weblogi!=0)
				{
					n = send(sock_ack_weblogi,_noeud->data->ack,ACK_SIZE,0);
   					printf("\r\nThread listener<collector>	sent ACK[%d]====> %s <==== to WEBLOGI\r\n",*searchNode,_noeud->data->ack);
				}
			}
		}
	}
   	else { printf("\r\nThread listener<collector>  ACK[%d] DO NOT exist Try again in 1 sec \r\n",*searchNode); /*sleep(1);*/}
   
   	if (n < 0) {
      		printf("\r\nERROR writing to socket\r\n");
      	exit(1);
   	}
   }
	sleep(T_SEND);
 }

 // envoi les commandes à pppx
 // réceptonne les ACK de pppx
 // les empile	
 void* sender(void* param)
 {
  
  while (init_socket_pppx(&sock_command_pppx)!=0)
	sleep(1);

   printf("\r\nThread SENDER<collector>	 created successfully<sockfd_pppx:%d>\r\n",sock_command_pppx);
   struct list* _list_c=list_commands;
   struct list* _list_a=list_ack;
   int index=0; /* index prochaine commande à envoyer à pppx*/
   if (_list_c!=NULL && _list_a!=NULL)
   {
   	while(true)
	{
		pthread_mutex_lock(&(_list_c->mutex));
		//pthread_cond_wait(&(_list->condition),&(_list->mutex));
		struct noeud* _noeud=extractMessage(_list_c,&index /* prochaine commande */);
		pthread_mutex_unlock(&(_list_c->mutex));
		//printf("\r\nThread SENDER	runs \r\n");
		if (_noeud!=NULL) {
			//crée un message groupé
			//dialogue avec pppx
			if (_noeud->data!=NULL) 
			{
				printf("\r\n collector TRY to send to pppx:list[%d],command:%s\r\n",index,_noeud->data->commande);
				if (_noeud->data->commande!=NULL)
				{ 
					sendreceave(sock_command_pppx,_noeud->data->commande,_list_a);
					printf("\r\n collector sent command to pppx:list[%d]=%s\r\n",index,_noeud->data->commande);
				}
			}
		}
		sleep(1);
	}
   }

 }

 void  creat_threads(char* ip,int port)
 {

  //strcpy(ip_pass,ip);
  port_weblogi=port; 
  
  int err = pthread_create(&(tid[LISTENER]), NULL, &listener, NULL);
  if (err != 0)
     printf("\r\ncan't create thread LISTENER<collector> :[%s]\r\n", strerror(err));
  
  err = pthread_create(&(tid[SENDER]), NULL, &sender, NULL);
  if (err != 0)
     printf("\r\ncan't create thread SENDER<collector> :[%s]\r\n", strerror(err));

  if(pthread_join(tid[LISTENER], NULL)) {
     printf("\r\nError joining thread LISTENER<collector>\r\n");
	return ;
  }
  if(pthread_join(tid[SENDER], NULL)) {
     printf("\r\nError joining thread SENDER<collector>\r\n");
	return ;
  }
 }


int sendreceave(int sockfd,char* command,struct list* _list /* liste des ack */)
{
   	int n = 0;
    	char recvBuff[ACK_SIZE];

        if ((send(sockfd,command,strlen(command) ,0))== -1) {
            printf("\r\nFailure Sending Message\n");
    	    return 1;
	}
    	else {
         printf("\r\ncommand being sent <collector to pppx> : %s\n",command);
    	}
        sleep(T_SEND);	
	if (strcmp(command,RE_INIT)!=0)
	{	
    		memset(recvBuff, '0',ACK_SIZE);
	    	n = read(sockfd, recvBuff, ACK_SIZE-1);
       		recvBuff[n] = 0;
	        printf("\r\n ACK Receaved from pppx ==========> %s <==========\r\n",recvBuff);
                //le message reçu doit être empilé sur une liste spécifique et retourné par doprocessing à weblogi  
    		if(n < 0)
    		{
        		printf("\n Read error \n");
    		}
   
   		struct noeud* node=(struct noeud*)creer_noeud();
   		init_ack(node,recvBuff,_list->count);   
   		if (_list!=NULL)
   		{
   			pthread_mutex_lock(&(_list->mutex));
			//fill the list of command 
   			ajouter_noeud((_list),node);	  
			//pthread_cond_signal(&(_list->condition));  
			pthread_mutex_unlock(&(_list->mutex));
			sleep(T_AJ_CMD);
   		}
 	}	
	return 0;
 }

 int   init_socket_pppx(int* sockfd)
 {
   	int n = 0;
    	char recvBuff[1024];
    	struct sockaddr_in serv_addr; 

    	if((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    	{
        	printf("\n Error : Could not create socket \n");
        	return 1;
    	} 

   	memset(&serv_addr, '0', sizeof(serv_addr)); 

    	serv_addr.sin_family = AF_INET;
    	serv_addr.sin_port = htons(port_pppx); 

    	if(inet_pton(AF_INET, ip_pppx, &serv_addr.sin_addr)<=0)
    	{
        	printf("\n inet_pton error occured\n");
        	return 2;
    	} 
   
    	printf("\r\nTry to connect on Server <%s><%d><sockfd:%d>\r\n",ip_pppx,port_pppx,*sockfd);
    	if( connect(*sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    	{
    		printf("\n Error : Connect Failed \n");
    		return 3;
    	} 

	return 0;
   }

