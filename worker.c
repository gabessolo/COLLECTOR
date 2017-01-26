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

 pthread_t tid[4];

 char ack[SIZE_BUFFER_RECV];
 char command[SIZE_BUFFER_RECV];

 char buffer[COMMAND_SIZE];
 
 extern int  port_pppx; 
 extern char* ip_pppx;

 static struct list*  list_commands=NULL;
 static struct list*  list_ack     =NULL;

 static struct noeud* group_noeud[GROUP_NOEUD_SIZE];
 static char   group_message[GROUP_MESSAGE_SIZE];

 int sock_listen_command=0;
 int sock_send_ack=0;
 
 int sock_send_command=0;
 int sock_listen_ack=0;

 int nb_connect=0;
 
 void* sender_ack(void* param)
 {
   printf("\r\nThread SENDER ACK created successfully\r\n");
   doprocessing_sender_ack();
 }
 
 void* sender_command(void* param)
 {
   printf("\r\nThread SENDER COMMAND created successfully\r\n");
   doprocessing_sender_command();
 }

 void* listener_command(void* param)
 {
   printf("\r\nThread LISTENER COMMAND created successfully\r\n");
   doprocessing_listener_command();
 }
 
 void* listener_ack(void* param)
 {
   printf("\r\nThread LISTENER ACK created successfully\r\n");
   doprocessing_listener_ack();
 }
 
 void doprocessing_listener_command ()
 {

   while(init_sock_server(&sock_listen_command,port_collecteur)<=0)
   {
   	printf("\r\n Errot connect on port_collecteur:%d\r\n",port_collecteur);
	sleep(T_CON);  
   }

   printf("\r\n listening command on port_collecteur:%d  \r\n",port_collecteur);
   if (list_commands==NULL)
	list_commands=(struct list*)creer_list();
	
   if (list_commands==NULL) {
      printf("\r\nERROR on creating list<commandes>\r\n");
   }
  
   struct list* _list_c=list_commands;   
   init_list(_list_c);   
 
   struct sockaddr_in  cli_addr;
   listen(sock_listen_command,5);
   int clilen = sizeof(struct sockaddr_in);

   while (true) 
   {
      int newsockfd = accept(sock_listen_command, (struct sockaddr *) &cli_addr, &clilen);
      printf("\r\nnb_connect: %d\r\n",nb_connect++);
		
      if (newsockfd < 0) {
         printf("\r\nERROR on accept listener command \r\n");
      	 continue;
      }
       
      sock_send_ack=newsockfd;

      collect_command(newsockfd);

      printf("\r\nfin proccessing<listener command> client : %d\r\n",nb_connect);
		
   } /* end of while */
 }

 //reçoit les commandes de weblogi
 void collect_command (int sock_command) {
   int n=0;
   //bzero(command,SIZE_BUFFER_RECV);
   printf("\r\nwaiting for a New command\r\n");
   bool clientDiscon=false;
   do {
   	memset(command, '0',COMMAND_SIZE);
    	n = read(sock_command,command,COMMAND_SIZE);
	if (n==0) { 
	clientDiscon=true;
	printf("\r\nERROR reading from socket\r\n");
	}
	
	command[n] = '\0';
	printf("\r\nCommand  Receaved from client =======> %s[%d] <======\r\n",command);
   

	if (strcmp(command,RE_INIT)==0)
		continue;
	
	if (strcmp(command,"SIGNEDEVIE")==0)
		continue;

	struct list* _list_c=list_commands; /* contient toutes les commandes en provenance de weblogi */
	struct noeud* node=(struct noeud*)creer_noeud();
   
	init_noeud(node,_list_c->count);   
	init_command(node,command,_list_c->count);   
	if (_list_c!=NULL)
	{
   		pthread_mutex_lock(&(_list_c->mutex));
	   	ajouter_noeud(_list_c,node);	  
	   	printf("\r\nThread listener-command ADD :%s\r\n",command);
		pthread_mutex_unlock(&(_list_c->mutex));
        }
   }
   while (clientDiscon==false);
 }

 //envoi les ack de  pppx à weblogi
 void doprocessing_sender_ack () {

 int searchNode=0;
 sock_send_ack=sock_listen_command;
 
 do
 {

	 struct list* _list_a=list_ack;      /* contient tous les ack en provenance du meuble via la passerelle */
	 //devrait être le retour de la passerelle stocké dans une  liste adaptée
	 bool serverDiscon=false;
         int searchNode=0; 
	 do{
 
		 pthread_mutex_lock(&(_list_a->mutex));
		//pthread_cond_wait(&(_list_a->condition),&(_list_a->mutex));
		 printf("\r\nThread sender_ack	TRY to extract  ACK no:%d\r\n",searchNode);
		 struct noeud* _noeud=extractMessage(_list_a,&searchNode/* prochain ACK*/);
		 pthread_mutex_unlock(&(_list_a->mutex));
		 if (_noeud!=NULL)
		 {
			if (_noeud->data!=NULL)
			{
				if (_noeud->data->ack!=NULL)
				{
					if (sock_send_ack!=0)
					{
						int n = send(sock_send_ack,_noeud->data->ack,strlen(_noeud->data->ack),0);
 						if (n <= 0) {
						    printf("\r\nERROR writing to socket send_ack\r\n");
						    serverDiscon=true;
						}
				   		printf("\r\nThread sender-ack sent ACK[%d]====> %s <==== to WEBLOGI\r\n",
						searchNode,_noeud->data->ack);
					}

				}
			}		
 		}
		sleep(T_READ);
   
 
	 }while(serverDiscon==false);
 
 }while(true);

 }

 // envoi les commandes à pppx
 void doprocessing_sender_command()
 {
  
  printf("\r\nThread SENDER-COM  created successfully\r\n");
  
  printf("\r\n ip_pppx:%s port_pppx(se connecte dessus):%d\r\n",ip_pppx,port_pppx);

  bool clientDiscon=false;
  int nb_commandes=1;
  while(clientDiscon==false)
  {
  	while (init_sock_client(&sock_send_command,ip_pppx,port_pppx,"PPPX")!=0)
		sleep(T_CON);

        sock_listen_ack=sock_send_command;  
        printf("\r\n New Connexion to PPPX \r\n");

   	struct list* _list_c=list_commands;
   	int index=0; /* index prochaine commande à envoyer à pppx*/
   	if (_list_c!=NULL )
   	{
   		while(true)
		{
			struct noeud* _NOEUD=NULL/* nécessaie pour une commande simple */;
		        int INDEX=0;
			
			pthread_mutex_lock(&(_list_c->mutex));
			//pthread_cond_wait(&(_list_c->condition),&(_list_c->mutex));
			memset(group_noeud,NULL,GROUP_NOEUD_SIZE);
			printf("\r\nTRY to send command\r\n");

			struct noeud* _noeud=extractMessage(_list_c,&index /* prochaine commande */);

			group_noeud[0]=_noeud;
			if (_noeud!=NULL)
			{
				_NOEUD=_noeud;
				INDEX=index;
			}

                 	_noeud=extractMessage(_list_c,&index /* prochaine commande */);
		        group_noeud[1]=_noeud;
			if (_noeud!=NULL)
			{
				_NOEUD=_noeud;
				INDEX=index;
			}
			_noeud=extractMessage(_list_c,&index /* prochaine commande */);
			if (_noeud!=NULL)
			{
				_NOEUD=_noeud;
				INDEX=index;
			}
		        group_noeud[2]=_noeud;
			if (_noeud!=NULL)
			{
				_NOEUD=_noeud;
				INDEX=index;
			}
			_noeud=extractMessage(_list_c,&index /* prochaine commande */);
		        group_noeud[3]=_noeud;
			if (_noeud!=NULL)
			{
				_NOEUD=_noeud;
				INDEX=index;
			}
			_noeud=extractMessage(_list_c,&index /* prochaine commande */);
		        group_noeud[4]=_noeud;
			if (_noeud!=NULL)
			{
				_NOEUD=_noeud;
				INDEX=index;
			}
			
			pthread_mutex_unlock(&(_list_c->mutex));
			
			int nb_commandes=0,i=0;
			for(i=0;i<GROUP_NOEUD_SIZE;i++)
			if (group_noeud[i]!=NULL)
			{
				struct noeud* _pNoeud=group_noeud[i];
				if (_pNoeud!=NULL && _pNoeud->sent==false)
				{
					char* com= _pNoeud->data->commande;
					if (com!=NULL  )
						nb_commandes++;
					
				}
			}
                        
			//printf("\r\nThread SENDER	runs \r\n");
			//une commande simple suffit
			if (nb_commandes==1)
			{
			        _noeud=_NOEUD;	
                                index=INDEX;

				if (_noeud!=NULL) 
				{	
				if (_noeud->data!=NULL) 
				{
				printf("\r\nTRY to send to pppx:list[%d],SIMPLE command:%s\r\n",
				index,_noeud->data->commande);
				if (_noeud->data->commande!=NULL)
				{ 
				_noeud->sent=true;
        
				if ((send(sock_send_command,_noeud->data->commande,strlen(_noeud->data->commande) ,0))
					== -1) {
					 printf("\r\nFailure Sending Simple command\n");
					 clientDiscon=true;
				}
				else {
					 printf("\r\ncommand being sent to pppx> : %s\n",_noeud->data->commande);
    				}

				printf("\r\n Sent command to pppx Successfull >>> list[%d]=%s <<< \r\n",
					 index,_noeud->data->commande);

				}
				}	
				}
			////nécessite une commande groupée
			}else if (nb_commandes>1)
			{
			printf("\r\n collecteur %d commands to GROUP\r\n",nb_commandes);
				//crée un message groupé
				//dialogue avec pppx
				//
 //	"0000#E04 001A03A000000001 002A13A000000001 003A03A000000001 004A13A000000001 005A03A000000001 ZZ;"
			        
				memset(group_message,'\0',GROUP_MESSAGE_SIZE);

				group_message[0]='0';	
			        group_message[1]='0';	
			        group_message[2]='0';	
			        group_message[3]='0';	
			        group_message[4]='#';	
			        group_message[5]='E';	
			        group_message[6]='0';

				char s_nb[2];
				sprintf(s_nb,"%d",nb_commandes);	
			        s_nb[1]='\0';
				group_message[7]=atoi(s_nb);	

				//group_message[7]=(char)nb_commandes;	//voir si c'est mieux
				
				int i=0,j=0;
				for(i=0;i<GROUP_NOEUD_SIZE;i++)
				if (group_noeud[i]!=NULL)
				{
					struct noeud* pNoeud=group_noeud[i];
					if (pNoeud!=NULL && pNoeud->data!=NULL)
					{
						char* _com=pNoeud->data->commande;
						if (_com!=NULL  )
						{
							pNoeud->sent=true;

							group_message[8 +16*j]=_com[4];	
							group_message[9 +16*j]=_com[5];	
							group_message[10+16*j]=_com[6];	
							group_message[11+16*j]=_com[7];	
							group_message[12+16*j]=_com[8];	
							group_message[13+16*j]=_com[9];	
							group_message[14+16*j]=_com[10];	
							group_message[15+16*j]=_com[11];	
							group_message[16+16*j]=_com[12];	
							group_message[17+16*j]=_com[13];	
							group_message[18+16*j]=_com[14];	
							group_message[19+16*j]=_com[15];	
							group_message[20+16*j]=_com[16];	
							group_message[21+16*j]=_com[17];	
							group_message[22+16*j]=_com[18];	
							group_message[23+16*j]=_com[19];	
							
							j++;
						}
					}	
				}
				//on termine avec ZZ;
				//
				//
			        if (nb_commandes!=GROUP_NOEUD_SIZE)
				{
					group_message[23+0+16*j]='Z';	
			        	group_message[23+1+16*j]='Z';	
			        	group_message[23+2+16*j]=';';	
				}
				else if (nb_commandes==GROUP_NOEUD_SIZE)
				{
					group_message[112]='Z';	
			        	group_message[113]='Z';	
			        	group_message[114]=';';	
			        	group_message[115]='\0'; ///????	

				}

			printf("\r\n collector TRY to send to pppx:list[%d],GROUP command:%s\r\n",group_message);
			if ((send(sock_send_command,group_message,strlen(group_message) ,0))== -1) {
				 printf("\r\nFailure Sending Group command\n");
				 clientDiscon=true;
			}
			else {
				 printf("\r\ncommand being sent >>>  %s <<< \n",_noeud->data->commande);
    			}

			
			}
			//sleep(T_SEND);
			//sleep(T_READ);
		}
   	}
     }
 }

 void  creat_threads()
 {

  ///////////////    threads creation
  //
  int err = pthread_create(&(tid[LISTEN_COM]), NULL, &listener_command, NULL);
  if (err != 0)
     printf("\r\ncan't create thread LISTEN-COMMAND :[%s]\r\n", strerror(err));
  
  err = pthread_create(&(tid[SEND_ACK]), NULL, &sender_ack, NULL);
  if (err != 0)
     printf("\r\ncan't create thread SEND-ACK :[%s]\r\n", strerror(err));

  err = pthread_create(&(tid[SEND_COM]), NULL, &sender_command, NULL);
  if (err != 0)
     printf("\r\ncan't create thread SEND-COM :[%s]\r\n", strerror(err));
  
  err = pthread_create(&(tid[LISTEN_ACK]), NULL, &listener_ack, NULL);
  if (err != 0)
     printf("\r\ncan't create thread LISTEN-ACK :[%s]\r\n", strerror(err));

  
  //////////////// wait until threads ends

  if(pthread_join(tid[LISTEN_ACK], NULL)) {
     printf("\r\nError joining thread LISTEN-ACK\r\n");
	return ;
  }
  if(pthread_join(tid[LISTEN_COM], NULL)) {
     printf("\r\nError joining thread LISTEN-COM\r\n");
	return ;
  }
  if(pthread_join(tid[SEND_ACK], NULL)) {
     printf("\r\nError joining thread SEND-ACK\r\n");
	return ;
  }
  if(pthread_join(tid[SEND_COM], NULL)) {
     printf("\r\nError joining thread SEND-COM\r\n");
	return ;
  }
 }

 int   init_sock_client(int* sockfd,const char* ip,int port,const char* serverName)
 {
    	struct sockaddr_in serv_addr; 

        if (sockfd==NULL)
	   return 1;	

    	if((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    	{
        	printf("\r\n Error : Could not create socket \r\n");
        	return 2;
    	} 

   	memset(&serv_addr, '0', sizeof(serv_addr)); 

    	serv_addr.sin_family = AF_INET;
    	serv_addr.sin_port = htons(port); 

    	if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0)
    	{
        	printf("\r\n inet_pton error occured\r\n");
        	return 3;
    	} 
   
    	printf("\r\nTry to connect on Server <'%s'-ip:'%s'> <%d> <sockfd:%d> \r\n",serverName,ip,port,*sockfd);
    	if( connect(*sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    	{
    		printf("\n Error : Connect Failed on server:'%s'\n",serverName);
    		return 4;
    	} 

	return 0;
}

int init_sock_server(int* sockfd,int port)
{   
   struct sockaddr_in serv_addr;

   if (sockfd==NULL)
     return 1;

   *sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (*sockfd < 0) {
      printf("\r\nERROR opening socket\r\n");
      return 2;
   }
   
   bzero((char *) &serv_addr, sizeof(serv_addr));
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(port);
   
   if (bind(*sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      printf("\r\nERROR on binding\r\n");
      return 3;
   }

   return 0;
}
 
void doprocessing_listener_ack ()
{

   printf("\r\n listening ACK on port_pppx:%d  \r\n",port_pppx);
   
   sock_listen_ack=sock_send_command;

   if (sock_listen_ack<=0)  
      printf("\r\nERROR on creating sock_listen_ack\r\n");

 
   if (list_ack==NULL)
	list_ack=(struct list*)creer_list();
	
   if (list_ack==NULL) {
      printf("\r\nERROR on creating list<ack>\r\n");
   }
  
   struct list* _list_a=list_ack;   
   init_list(_list_a);   
 
   struct sockaddr_in  cli_addr;
   listen(sock_listen_ack,5);
   int clilen = sizeof(struct sockaddr_in);

   while (true) 
   {
      int newsockfd = accept(sock_listen_ack, (struct sockaddr *) &cli_addr, &clilen);
      printf("\r\nnb_connect: %d\r\n",nb_connect++);
		
      if (newsockfd < 0) {
         printf("\r\nERROR on accept listener ACK\r\n");
      	 continue;
      }

      collect_ack(newsockfd);

      printf("\r\nfin proccessing<listener ACK> client : %d\r\n",nb_connect);
		
   } /* end of while */
}
 //reçoit les commandes de weblogi
 void collect_ack (int sock_ack) {
   int n=0;
   //bzero(command,SIZE_BUFFER_RECV);
   printf("\r\nwaiting for a New ACK\r\n");
   bool clientDiscon=false;
   do {
   	memset(ack, '0',ACK_SIZE);
    	n = read(sock_ack,ack,ACK_SIZE);
	if (n==0) { 
	clientDiscon=true;
	printf("\r\nERROR reading from socket ack \r\n");
	}
	
	ack[n] = '\0';
	printf("\r\nCommand  Receaved from client =======> %s[%d] <======\r\n",ack);
   

	if (strcmp(ack,"0005COMOK")==0)
		continue;
	
	if (strcmp(ack,"0005COMHS")==0)
		continue;

	struct list* _list_a=list_ack; /* contient toutes les commandes en provenance de weblogi */
	struct noeud* node=(struct noeud*)creer_noeud();
   
	init_noeud(node,_list_a->count);   
	init_ack(node,ack,_list_a->count);   
	if (_list_a!=NULL)
	{
   		pthread_mutex_lock(&(_list_a->mutex));
	   	ajouter_noeud(_list_a,node);	  
	   	printf("\r\nThread listener-ack ADD :%s\r\n",ack);
		pthread_mutex_unlock(&(_list_a->mutex));
        }
   }
   while (clientDiscon==false);
  

}
