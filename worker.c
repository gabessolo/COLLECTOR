 #include <stdio.h>
 #include <string.h>
 #include <pthread.h>
 #include <stdlib.h>
 #include <unistd.h>        
 #include <netdb.h>
 #include <netinet/in.h>
 #include "worker.h"
 #include "defs.h"

 #define LISTENER 0
 #define SENDER	  1

 pthread_t tid[2];

 static int   port_pass=0; 
 static char* ip_pass  =NULL;
 static struct list* list_commands=NULL;

 void* listener(void* param)
 {
   int sockfd, newsockfd, portno, clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int n, pid;
   
   /* First call to socket() function */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      printf("ERROR opening socket\r\n");
      exit(1);
   }
   
   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);
   
   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      printf("ERROR on binding\r\n");
      exit(1);
   }
  
   if (list_commands==NULL)
	list_commands=(struct list*)creer_list();
	
   if (list_commands==NULL) {
      printf("ERROR on creating list\r\n");
      exit(1);
   }
  
   struct list* _list=list_commands;   
   pthread_mutex_init(&(_list->mutex), NULL);
   pthread_cond_init(&(_list->condition), NULL);
 
   /* Now start listening for the clients, here
    *       * process will go in sleep mode and will wait
    *             * for the incoming connection
    *                */
   
   listen(sockfd,5);
   clilen = sizeof(cli_addr);
   
   while (1) {
      newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		
      if (newsockfd < 0) {
         printf("ERROR on accept\r\n");
         exit(1);
      }
      
      /* Create child process */
      pid = fork();
		
      if (pid < 0) {
         printf("ERROR on fork\r\n");
         exit(1);
      }
      
      if (pid == 0) {
         /* This is the client process */
         close(sockfd);
         doprocessing(newsockfd);
         exit(0);
      }
      else {
         close(newsockfd);
      }
		
   } /* end of while */
 }

/* The following code seqment shows a simple implementation of doprocessing function.*/

 void doprocessing (int sock) {
   int n=0;
   char buffer[256];
   bzero(buffer,256);
   n = read(sock,buffer,255);
   
   if (n < 0) {
      printf("ERROR reading from socket\r\n");
      exit(1);
   }
   
   printf("Here is the message: %s\r\n",buffer);
   
   struct noeud* node=(struct noeud*)creer_noeud();
   init_data(node,buffer);   
   struct list* _list=list_commands;
   if (_list!=NULL)
   {
   	pthread_mutex_lock(&(_list->mutex));

   	ajouter_noeud(_list,node);	  
	pthread_cond_signal(&(_list->condition));  

	pthread_mutex_unlock(&(_list->mutex));
   }

   n = write(sock,"I got your message",18);
   
   if (n < 0) {
      printf("ERROR writing to socket\r\n");
      exit(1);
   }
 }
	
 void* sender(void* param)
 {
   struct list* _list=list_commands;
   if (_list!=NULL)
   {
   	pthread_mutex_lock(&(_list->mutex));
	pthread_cond_wait(&(_list->condition),&(_list->mutex));
	pthread_mutex_unlock(&(_list->mutex));
   }

 }

 void  creat_threads(char* ip_passerelle,int port_passerelle)
 {
  
  if (init(ip_passerelle,port_passerelle)!=0)
  {
	strcpy(ip_passerelle,IP_PASS);
        port_passerelle= PORT_PASS;
  }  
 
  int err = pthread_create(&(tid[LISTENER]), NULL, &listener, NULL);
  if (err != 0)
     printf("can't create thread LISTENER :[%s]\r\n", strerror(err));
  else
     printf("Thread LISTENER created successfully\r\n");
  
  //sleep(2);

  err = pthread_create(&(tid[SENDER]), NULL, &sender, NULL);
  if (err != 0)
     printf("can't create thread SENDER :[%s]\r\n", strerror(err));
  else
     printf("Thread SENDER  created successfully\r\n");

	
  if(pthread_join(tid[LISTENER], NULL)) {
     printf("Error joining thread LISTENER\r\n");
	return ;
  }
  if(pthread_join(tid[SENDER], NULL)) {
     printf("Error joining thread SENDER\r\n");
	return ;
  }
 }
