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

 char buffer[SIZE_BUFFER_RECV];
 extern int  port_weblogi; 
 extern int  port_pppx; 
 extern char* ip_pppx;

 static struct list* list_commands=NULL;

 void* listener(void* param)
 {
   printf("\r\nThread LISTENER<collector>	created successfully\r\n");
   printf("\r\n ip_pppx:%s port_pppx:%d port_weblogi:%d  \r\n",ip_pppx,port_pppx,port_weblogi);
   
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
   serv_addr.sin_port = htons(port_weblogi);
   
   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      printf("\r\nERROR on binding\r\n");
      exit(1);
   }
  
   if (list_commands==NULL)
	list_commands=(struct list*)creer_list();
	
   if (list_commands==NULL) {
      printf("\r\nERROR on creating list\r\n");
      exit(1);
   }
  
   struct list* _list=list_commands;   
   init_list(_list);   
 
   /* Now start listening for the clients, here
    *       * process will go in sleep mode and will wait
    *             * for the incoming connection
    *                */
   
   printf("\r\nThread Listener<collector> en attente de commandes\r\n");
   listen(sockfd,5);
   clilen = sizeof(cli_addr);

   int nb_connect=0;
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
      	doprocessing(newsockfd);
      }

      printf("\r\nfin proccessing<collector>: %d\r\n",nb_connect);
		
   } /* end of while */
 }


 void doprocessing (int sock) {
   int n=0;
   bzero(buffer,SIZE_BUFFER_RECV);
   n = read(sock,buffer,SIZE_BUFFER_RECV);
   
   if (n < 0) {
      printf("\r\nERROR reading from socket\r\n");
      exit(1);
   }
   
   struct list* _list=list_commands;
   struct noeud* node=(struct noeud*)creer_noeud();
   init_data(node,buffer,_list->count);   
   if (_list!=NULL)
   {
   	pthread_mutex_lock(&(_list->mutex));
	//fill the list of command 
   	ajouter_noeud(_list,node);	  
	//pthread_cond_signal(&(_list->condition));  
	pthread_mutex_unlock(&(_list->mutex));
	sleep(2);
   }

   if (strcmp("#E00007A13A000000001ZZ;",buffer)!=0)
   {
   	//devrait être le retour de la passerelle stocké dans une  liste adaptée
	n = write(sock,"#R0A0010000;",12);
   
   	if (n < 0) {
      		printf("\r\nERROR writing to socket\r\n");
      	exit(1);
   	}
   }
 }
	
 void* sender(void* param)
 {
  
  int sockfd=0;
  while (init_socket_pppx(&sockfd)!=0)
	sleep(1);

   printf("\r\nThread SENDER<collector>	 created successfully<sockfd_pppx:%d>\r\n",sockfd);
   struct list* _list=list_commands;
   int index=0;
   if (_list!=NULL)
   {
   	while(true)
	{
		pthread_mutex_lock(&(_list->mutex));
		//pthread_cond_wait(&(_list->condition),&(_list->mutex));
		struct noeud* _noeud=extractMessage(_list,index);
		pthread_mutex_unlock(&(_list->mutex));
		//printf("\r\nThread SENDER	runs \r\n");
		if (_noeud!=NULL) {
			//crée un message groupé
			//dialogue avec pppx
			if (_noeud->data!=NULL) 
				if (_noeud->data->commande!=NULL)
				{ 
					sendreceave(sockfd,_noeud->data->commande);
					printf("\r\n collector sent to pppx:list[%d]=%s\r\n",index,_noeud->data->commande);
				}
			index++;
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


int sendreceave(int sockfd,char* message)
{
   	int n = 0;
    	char recvBuff[1024];

        if ((send(sockfd,message,strlen(message) ,0))== -1) {
            printf("\r\nFailure Sending Message\n");
    	    return 1;
	}
    	else {
         printf("\r\nMessage being<collector to pppx>  sent: %s\n",message);
    	}
	if (strcmp(message,"#E00007A13A000000001ZZ;")!=0)
	{	
    		memset(recvBuff, '0',sizeof(recvBuff));
	    	n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
       		recvBuff[n] = 0;
	        printf("\r\nMessage Receaved from pppx:%s\r\n",recvBuff);
                //le message reçu doit être empilé sur une liste spécifique et retourné par doprocessing à weblogi  
    		if(n < 0)
    		{
        		printf("\n Read error \n");
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

