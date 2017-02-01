 #include <stdio.h>
 #include <string.h>
 #include <pthread.h>
 #include <stdlib.h>
 #include <unistd.h>        
 #include <netdb.h>
 #include <netinet/in.h>
 #include "worker.h"
 #include "defs.h"
 #include "sock.h"

 char message[BUFFER_SENT];  //20 messages max + message extinction
 char header_message[BUFFER_SENT];

 pthread_t tid[4];

 char ack[BUFFER_RECV];
 char command[BUFFER_RECV];

 char buffer[COMMAND_SIZE];
 

 static struct list*  list_commands=NULL;
 static struct list*  list_ack     =NULL;

 static struct noeud* group_noeud[GROUP_NOEUD_SIZE];
 static char   group_message[GROUP_MESSAGE_SIZE];

 int sock_listen_command=0;
 int sock_send_ack=0;
 
 int sock_send_command=0;
 int sock_listen_ack=0;

 int nb_connect=0;


int   port_pppx; 
int   port_collecteur; 
char* ip_pppx;
char* ip_meuble;
int   port_meuble;
 

 ////////////////////
 //
 //
 //  SENDER
 //
 //////////////
 void* sender_ack(void* param)
 {
   printf("\r\nThread sender-ack créé avec succés\r\n");
   //doprocessing_sender_ack();
 }
 
 void* sender_command(void* param)
 {
   printf("\r\nThread sender-command créé avec succés\r\n");
   doprocessing_sender_command();
 }

////////////////
//
// LISTENER
//
// ///////////
 void* listener_command(void* param)
 {
   printf("\r\nThread listener-command créé avec succés\r\n");
   doprocessing_listener_command();
 }
 
 void* listener_ack(void* param)
 {
   printf("\r\nThread listener-ack créé avec succés\r\n");
   doprocessing_listener_ack();
 }
 
/////////////
//
// DOPROCESSING
//
////////////////

 void doprocessing_listener_command ()
 {

   init_collecteur(&ip_pppx,&port_pppx,&port_collecteur);
   while(init_sock_server(&sock_listen_command,port_collecteur)<0)
   {
   	printf("\r\nErreur de création de la socket d'écoute sur le port:%d\r\n",port_collecteur);
	sleep(T_CON);  
   }

   printf("\r\nécoute les commandes sur le  port : %d\r\n",port_collecteur);
   if (list_commands==NULL)
	list_commands=(struct list*)creer_list();
	
   if (list_commands==NULL) {
      printf("\r\nErreur à la création de la liste des commandes\r\n");
      exit(0);
   }
  
   struct list* _list_c=list_commands;   
   init_list(_list_c);   
 
   struct sockaddr_in  cli_addr;
   listen(sock_listen_command,MAX_CLIENTS_COLLECTEUR);
   int clilen = sizeof(struct sockaddr_in);

   while (true) 
   {
      sock_send_ack = accept(sock_listen_command, (struct sockaddr *) &cli_addr, &clilen);
      printf("\r\nnb_connect: %d\r\n",nb_connect);
		
      if (sock_send_ack < 0) {
         printf("\r\nErreur connexion\r\n");
      	 continue;
      }
       
      collect_command(sock_send_ack);
      printf("\r\nfin de traîtement du client:%d\r\n",nb_connect++);
		
   } /* end of while */
 }

 //reçoit les commandes de weblogi
 void collect_command (int sock_command) {
   int n=0;
   //bzero(command,SIZE_BUFFER_RECV);
   bool clientDiscon=false;
   do {
   	printf("\r\nAttente d'une commande\r\n");
   	memset(command, '0',COMMAND_SIZE);
    	n = read(sock_command,command,COMMAND_SIZE);
	if (n==0) { 
	clientDiscon=true;
	printf("\r\nErreur de lecture sur socket\r\n");
        continue; 
	}
	
	command[n] = '\0';
	printf("\r\nWL ===> '%s'\r\n",command);

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
	   	//printf("\r\nAjout '%s'\r\n",command);
		pthread_mutex_unlock(&(_list_c->mutex));
        }
   }
   while (clientDiscon==false);
 }

 //envoi les ack de  pppx à weblogi
 void doprocessing_sender_ack () {

 int searchNode=0;
 
 do
 {
	 struct list* _list_a=list_ack;      /* contient tous les ack en provenance du meuble via la passerelle */
	 //devrait être le retour de la passerelle stocké dans une  liste adaptée
	 bool serverDiscon=false;
         int searchNode=0; 
	 do{
 
		 pthread_mutex_lock(&(_list_a->mutex));
		//pthread_cond_wait(&(_list_a->condition),&(_list_a->mutex));
		 printf("\r\nThread sender_ack	tente une extraction du  ACK no:%d\r\n",searchNode);
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
						    printf("\r\nErreur écriture de socket\r\n");
						    serverDiscon=true;
						}
				   		printf("\r\nThread sender-ack envoi ACK[%d]==> %s <==\r\n",
						searchNode,_noeud->data->ack);
					}

				}
			}		
 		}
		//sleep(T_READ);
 
	 }while(serverDiscon==false);
 
 }while(true);

 }

 // envoi les commandes à pppx
 void doprocessing_sender_command()
 {
  
  init_collecteur(&ip_pppx,&port_pppx,&port_collecteur);

  bool clientDiscon=false;
  int nb_commandes=1;
  char* serverName="PPPX";

  while(true)
  {
  	while (init_sock_client(&sock_send_command,ip_pppx,port_pppx,serverName)<0)
		sleep(T_CON);

        sock_listen_ack=sock_send_command;  
  	printf("\r\nConnexion réussie sur le serveur '%s'-ip:'%s' port:%d\r\n",serverName,ip_pppx,port_pppx);

   	struct list* _list_c=list_commands;
   	int index=0; /* index prochaine commande à envoyer à pppx*/
   	if (_list_c!=NULL )
   	{
  		while(clientDiscon==false)
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
				if (_pNoeud!=NULL && _pNoeud->sent==false)
				{
					char* com= _pNoeud->data->commande;
					if (com!=NULL  )
						nb_commandes++;
					
				}
			}
                        
			//printf("\r\nThread SENDER	runs \r\n");
			//une commande simple suffit
			if (nb_commandes==0) {
			  sleep(T_EXTRACT);
			  continue;
			}	
			else if (nb_commandes==1)
			{
			        _noeud=_NOEUD;	
                                index=INDEX;

				if (_noeud!=NULL) 
				{	
				if (_noeud->data!=NULL) 
				{
				printf("\r\nTentative d'envoi ==> list[%d],SIMPLE:'%s'\r\n",
				index,_noeud->data->commande);
				if (_noeud->data->commande!=NULL)
				{ 
				_noeud->sent=true;
        
				if ((send(sock_send_command,_noeud->data->commande,strlen(_noeud->data->commande) ,0))
					== -1) {
					 printf("\r\nEchech émission commande Simple\n");
					 clientDiscon=true;
				}
				}
				}	
				}
			////nécessite une commande groupée
			}else if (nb_commandes>1)
			{
				printf("\r\ncollecteur %d commands à grouper\r\n",nb_commandes);
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

			printf("\r\ncollecteur essaye d'envoyer à pppx >> list[%d],GROUP command:%s\r\n",group_message);
			if ((send(sock_send_command,group_message,strlen(group_message) ,0))== -1) {
				 printf("\r\nEchec d'envoi Group command\n");
				 clientDiscon=true;
			}
			else {
				 printf("\r\nCommande en cours d'envoi >>> %s <<< \n",_noeud->data->commande);
    			}

			
			}
			sleep(T_SEND);
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
  sleep(T_CREAT);
  
  err = pthread_create(&(tid[SEND_COM]), NULL, &sender_command, NULL);
  if (err != 0)
     printf("\r\ncan't create thread SEND-COM :[%s]\r\n", strerror(err));
  sleep(T_CREAT);

 /* 
  err = pthread_create(&(tid[LISTEN_ACK]), NULL, &listener_ack, NULL);
  if (err != 0)
     printf("\r\ncan't create thread LISTEN-ACK :[%s]\r\n", strerror(err));
  sleep(T_CREAT);
  
  err = pthread_create(&(tid[SEND_ACK]), NULL, &sender_ack, NULL);
  if (err != 0)
     printf("\r\ncan't create thread SEND-ACK :[%s]\r\n", strerror(err));
  sleep(T_CREAT);*/
  //////////////// wait until threads ends

/*  if(pthread_join(tid[LISTEN_ACK], NULL)) {
     printf("\r\nError joining thread LISTEN-ACK\r\n");
	return ;
  }*/
  if(pthread_join(tid[LISTEN_COM], NULL)) {
     printf("\r\nError joining thread LISTEN-COM\r\n");
	return ;
  }/*
  if(pthread_join(tid[SEND_ACK], NULL)) {
     printf("\r\nError joining thread SEND-ACK\r\n");
	return ;
  }*/
  if(pthread_join(tid[SEND_COM], NULL)) {
     printf("\r\nError joining thread SEND-COM\r\n");
	return ;
  }
 }

void doprocessing_listener_ack ()
{

   init_pppx(&ip_meuble,&port_meuble,&port_pppx);
   printf("\r\nécoute les  ACK sur le port :%d  \r\n",port_pppx);
   
   sock_listen_ack=sock_send_command;

   if (sock_listen_ack<=0){  
      printf("\r\nErreur à la création de sock_listen_ack\r\n");
      exit(1);
   }
 
   if (list_ack==NULL)
	list_ack=(struct list*)creer_list();
	
   if (list_ack==NULL) {
      printf("\r\nErreur à la  création de la liste des ACK\r\n");
   }
  
   struct list* _list_a=list_ack;   
   init_list(_list_a);   
 
   struct sockaddr_in  cli_addr;
   listen(sock_listen_ack,MAX_CLIENTS_PPPX);
   int clilen = sizeof(struct sockaddr_in);

   while (true) 
   {
      int newsockfd = accept(sock_listen_ack, (struct sockaddr *) &cli_addr, &clilen);
      printf("\r\nnb_connect: %d\r\n",nb_connect);
		
      if (newsockfd < 0) {
         printf("\r\nErreur à la connexion sur le listener-ack \r\n");
      	 continue;
      }

      collect_ack(newsockfd);

      printf("\r\nfin de traîtement du client : %d\r\n",nb_connect++);
		
   } /* end of while */
}
 //reçoit les commandes de weblogi
 void collect_ack (int sock_ack) {
   int n=0;
   //bzero(command,SIZE_BUFFER_RECV);
   bool clientDiscon=false;
   do {
   	printf("\r\nAttente prochain ACK\r\n");
   	memset(ack, '0',ACK_SIZE);
    	n = read(sock_ack,ack,ACK_SIZE);
	if (n==0) { 
	clientDiscon=true;
	printf("\r\nErreur lecture socket ACK \r\n");
	}
	
	ack[n] = '\0';
	printf("\r\nCommande  Reçu ==> %s <=\r\n",ack);
   

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
