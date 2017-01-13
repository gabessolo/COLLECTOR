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

 static struct list*  list_commands=NULL;
 static struct list*  list_ack     =NULL;

 static struct noeud* group_noeud[GROUP_NOEUD_SIZE];
 static char   group_message[GROUP_MESSAGE_SIZE];

 int sock_command_pppx=0;

 void* listener(void* param)
 {
   printf("\r\nThread LISTENER<collector>	created successfully\r\n");
   printf("\r\n ip_passerelle:%s port_passerelle(se connecte dessus):%d port_collector(port ecoute):%d  \r\n",ip_pppx,port_pppx,port_weblogi);
   
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
   
   listen(sockfd,5);
   clilen = sizeof(cli_addr);

   int nb_connect=1;
   int searchNode=0; //noeud de recherche du ACK
   while (true) 
   {
      printf("\r\nThread Listener<collector> en attente de client\r\n");
      newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
      printf("\r\nnb_connect<collector>: %d\r\n",nb_connect++);
		
      if (newsockfd < 0) {
         printf("\r\nERROR on accept\r\n");
         exit(1);
      }

      bool  clientDiscon=false;
      while(clientDiscon==false)
      {
      	doprocessing_collector(newsockfd,newsockfd,&searchNode,&clientDiscon);
      }

      printf("\r\nfin proccessing<collector> client : %d\r\n",nb_connect++);
		
   } /* end of while */
 }

 //reçoit les commandes de weblogi
 //envoi les ack de  pppx à weblogi
 void doprocessing_collector (int sock_command,int sock_ack_weblogi,int* searchNode /* prochain ACK*/,bool* clientDisconnected) {

   int n=0;
   //bzero(command,SIZE_BUFFER_RECV);
   printf("\r\nThread listener<collector> waiting for a New command\r\n");
   memset(command, '0',COMMAND_SIZE);
   do {
    n = read(sock_command,command,COMMAND_SIZE);
    if (n==0) { 
	*clientDisconnected=true;
         return;
    }
    command[n] = '\0';
    printf("\r\nCommand  Receaved from client =======> %s[%d] <======\r\n",command);
    usleep(300);
   }
   while (n!=COMMAND_SIZE && n!=ACK_SIZE && n!=SIGNEDEVIE_SIZE && n!=COMOK_SIZE);
   

   if (n < 0 ) {
      printf("\r\nERROR reading from socket\r\n");
      exit(1);
   }
   command[n]='\0';
   struct list* _list_c=list_commands; /* contient toutes les commandes en provenance de weblogi */
   struct list* _list_a=list_ack;      /* contient tous les ack en provenance du meuble via la passerelle */
   struct noeud* node=(struct noeud*)creer_noeud();
   
   init_noeud(node,_list_c->count);   
   init_command(node,command,_list_c->count);   
   if (_list_c!=NULL)
   {
   	pthread_mutex_lock(&(_list_c->mutex));
	//fill the list of command 
   	ajouter_noeud(_list_c,node);	  
   	printf("\r\nThread listener<collector>	add command :%s\r\n",command);
	//pthread_cond_signal(&(_list_c->condition));  
	pthread_mutex_unlock(&(_list_c->mutex));
	//sleep(2*T_AJ_CMD);
   }

   //if (strcmp(RE_INIT,command)!=0)
   {
   	//devrait être le retour de la passerelle stocké dans une  liste adaptée
	pthread_mutex_lock(&(_list_a->mutex));
	//pthread_cond_wait(&(_list_a->condition),&(_list_a->mutex));
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
   			printf("\r\nThread listener<collector>	sent ACK[%d]====> %s <==== to WEBLOGI\r\n",
					*searchNode,_noeud->data->ack);
			}
			}
		}
	}
	//retourne COMOK même si la passerelle n'a pas encore retourné COMOK
	//if (command,"SIGNEDEVIE") 
	//	n = send(sock_ack_weblogi,"0005COMOK",9,0);
	//printf("\r\nThread listener<collector>  ACK[%d] DO NOT exist Try again in 1 sec \r\n",*searchNode); /*sleep(1);*/
	
   
   	if (n < 0) {
      		printf("\r\nERROR writing to socket\r\n");
      	exit(1);
   	}
   }
   //sleep(T_SEND);
 }

 // envoi les commandes à pppx
 // réceptonne les ACK de pppx
 // les empile	
 void* sender(void* param)
 {
  
  printf("\r\nThread SENDER<collector> created successfully\r\n");
  bool clientDiscon=false;
  while(clientDiscon==false)
  {
  	while (init_socket_pppx(&sock_command_pppx)!=0)
		sleep(T_CON);
  
        printf("\r\n New Connexion to PPPX \r\n");

   	struct list* _list_c=list_commands;
   	struct list* _list_a=list_ack;
   	int index=0; /* index prochaine commande à envoyer à pppx*/
   	if (_list_c!=NULL && _list_a!=NULL)
   	{
   		while(true)
		{
			struct noeud* _NOEUD=NULL/* nécessaie pour une commande simple */;
		        int INDEX=0;
			
			pthread_mutex_lock(&(_list_c->mutex));
			//pthread_cond_wait(&(_list_c->condition),&(_list_c->mutex));
			memset(group_noeud,NULL,GROUP_NOEUD_SIZE);

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
				if (_pNoeud!=NULL)
				{
					char* com= _pNoeud->data->commande;
					if (com!=NULL && strcmp(com,"SIGNEDEVIE")!=0 && strcmp(com,RE_INIT)!=0 )
						nb_commandes++;
					else
					if (com!=NULL && (strcmp(com,"SIGNEDEVIE")==0 || strcmp(com,RE_INIT)==0) )
						nb_commandes=1;
					
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
						printf("\r\n collector TRY to send to pppx:list[%d],SIMPLE command:%s\r\n",
						index,_noeud->data->commande);
						if (_noeud->data->commande!=NULL)
						{ 
							_noeud->sent=true;
							sendreceave(sock_command_pppx,_noeud->data->commande,
									_list_a,&clientDiscon);
							printf("\r\n collector sent command to pppx:list[%d]=%s\r\n",
							index,_noeud->data->commande);
						}
					}	
				}
			////nécessite une commande groupée
			}else if (nb_commandes>1)
			{
			printf("\r\n collector %d commands to GROUP\r\n",nb_commandes);
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

				//char s_nb[2];
				//sprintf(s_nb,"%d",nb_commandes);	
			        //s_nb[1]='\0';
				//group[7]=atoi(s_nb);	

				group_message[7]=(char)nb_commandes;	//voir si c'est mieux
				
				int i=0,j=0;
				for(i=0;i<GROUP_NOEUD_SIZE;i++)
				if (group_noeud[i]!=NULL)
				{
					struct noeud* pNoeud=group_noeud[i];
					if (pNoeud!=NULL)
					{
						char* _com=pNoeud->data->commande;
						if ((_com!=NULL && strcmp(_com,"SIGNEDEVIE")!=0 && strcmp(_com,RE_INIT)!=0) )
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
				else if (nb_commandes!=GROUP_NOEUD_SIZE)
				{
					group_message[112]='Z';	
			        	group_message[113]='Z';	
			        	group_message[114]=';';	
			        	group_message[115]='\0'; ///????	

				}

			printf("\r\n collector TRY to send to pppx:list[%d],GROUP command:%s\r\n",group_message);
			sendreceave(sock_command_pppx,group_message,_list_a,&clientDiscon);
			printf("\r\n collector sent command GROUP to pppx:list[%d]=%s\r\n",group_message,nb_commandes);
			
			}
			//sleep(T_SEND);
			//sleep(T_READ);
		}
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


int sendreceave(int sockfd,char* command,struct list* _list /* liste des ack */,bool* clientDiscon)
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
        //sleep(T_SEND);	
	//if (strcmp(command,RE_INIT)!=0)
	{	
    		memset(recvBuff, '0',ACK_SIZE);
	    	do {
			n = read(sockfd, recvBuff, ACK_SIZE);
			if (n==0) {
	        	 printf("\r\n  pppx Disconnected \r\n");
			 *clientDiscon=true;
			 return 2;
			}
			recvBuff[n] = '\0';
	        	if (n==ACK_SIZE) printf("\r\nACK Receaved from pppx ==========> %s[%d] <==========\r\n",recvBuff,n);
	        	else printf("\r\nReceaved from pppx ==========> %s[%d] <==========\r\n",recvBuff,n);
		}
	        while(n!=ACK_SIZE && n!=COMOK_SIZE);


                //le message reçu doit être empilé sur une liste spécifique et retourné par doprocessing à weblogi  
    		if(n < 0)
    		{
        		printf("\n Read error \n");
    		}
   
   		struct noeud* node=(struct noeud*)creer_noeud();
   		init_noeud(node,_list->count);   
   		init_ack(node,recvBuff,_list->count);   
   		if (_list!=NULL /*&& strcmp(recvBuff,"0005COMOK")!=0 && strcmp(recvBuff,"0005COMHS")!=0*/)
   		{
   			pthread_mutex_lock(&(_list->mutex));
			//fill the list of command 
   			ajouter_noeud((_list),node);	  
			//pthread_cond_signal(&(_list->condition));  
			pthread_mutex_unlock(&(_list->mutex));
			//sleep(T_AJ_CMD);
	        	printf("\r\n put ACK[%s] into list \r\n",recvBuff);
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

