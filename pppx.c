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

 char buffer[SIZE_BUFFER_RECV];
 extern int  port_pppx; 

 static struct list* list_commands=NULL;

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
      printf("\r\nERROR on creating list\r\n");
      exit(1);
   }
  
   struct list* _list=list_commands;   
   init_list(_list);   
 
   /* Now start listening for the clients, here
    *       * process will go in sleep mode and will wait
    *             * for the incoming connection
    *                */
   
   printf("\r\nThread Listener<pppx> en attente de commandes\r\n");
   listen(sockfd,5);
   clilen = sizeof(cli_addr);

   int nb_connect=1;
   while (true) 
   {
      newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
      printf("\r\nnb_connect<pppx>: %d\r\n",nb_connect++);
		
      if (newsockfd < 0) {
         printf("\r\nERROR on accept\r\n");
         exit(1);
      }
      
      while(1)
      {
      	doprocessing(newsockfd);
      }

      printf("\r\nfin proccessing<pppx>: %d\r\n",nb_connect);
		
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
   
   //struct list* _list=list_commands;
   //struct noeud* node=(struct noeud*)creer_noeud();
   //init_data(node,buffer,_list->count);   
   //if (_list!=NULL)
   //{
   //	pthread_mutex_lock(&(_list->mutex));
//	//fill the list of command 
   //	ajouter_noeud(_list,node);	  
//	//pthread_cond_signal(&(_list->condition));  
//	pthread_mutex_unlock(&(_list->mutex));
//	sleep(2);
  // }

   if (strcmp("#E00007A13A000000001ZZ;",buffer)!=0)
   {
   	n = write(sock,"#R0A0010000;",12);
   
   	if (n < 0) {
      		printf("\r\nERROR writing to socket\r\n");
      	exit(1);
   	}
   }
 }
	
 void* sender(void* param)
 {
   printf("\r\nThread SENDER<pppx>	 created successfully\r\n");
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
		if (_noeud!=NULL) printf("\r\n extracted<pppx>:list[%d]=%s\r\n",index++,_noeud->data->commande);
		sleep(1);
	}
   }

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
