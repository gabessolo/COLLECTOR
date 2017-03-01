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

 #define CLOG_MAIN
 #include "clog.h"
 

 const int MY_LOGGER = 0; /* Unique identifier for logger */
 
 char message[BUFFER_SENT];  //20 messages max + message extinction
 char header_message[BUFFER_SENT];

 pthread_t tid[4];

 char ack[ACK_SIZE];
 char command[BUFFER_RECV];
 //char message[BUFFER_RECV];

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
   doprocessing_sender_ack();
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
   	fprintf(stderr,"Erreur de création de la socket d'écoute sur le port:%d",port_collecteur);
	sleep(T_CON);  
   }

   //printf("écoute les commandes sur le  port : %d",port_collecteur);
   if (list_commands==NULL)
	list_commands=(struct list*)creer_list();
	
   if (list_commands==NULL) {
      fprintf(stderr,"Erreur à la création de la liste des commandes");
      return;
   }
  
   struct list* _list_c=list_commands;   
   init_list(_list_c);   
 
   struct sockaddr_in  cli_addr;
   listen(sock_listen_command,MAX_CLIENTS_COLLECTEUR);
   int clilen = sizeof(struct sockaddr_in);

   while (true) 
   {
      sock_send_ack = accept(sock_listen_command, (struct sockaddr *) &cli_addr, &clilen);
      //printf("\r\nnb_connect: %d\r\n",nb_connect);
		
      if (sock_listen_command < 0) {
         fprintf(stderr,"Erreur connexion");
      	 continue;
      }
       
      collect_command(sock_send_ack);
      //printf("\r\nfin de traîtement du client:%d\r\n",nb_connect++);
		
   } /* end of while */
 }

 //reçoit les commandes de weblogi
 void collect_command (int sock_command) {
   int n=0;
   //bzero(command,SIZE_BUFFER_RECV);
   bool clientDiscon=false;
   do {
   	//printf("\r\nAttente d'une commande\r\n");
   	memset(command, '0',COMMAND_SIZE);
    	n = read(sock_command,command,COMMAND_SIZE);
	if (n==0) { 
	clientDiscon=true;
	fprintf(stderr,"Erreur de lecture sur socket");
        continue; 
	}
	
	command[n] = '\0';
	clog_info(CLOG(MY_LOGGER),"WL ===> '%s'",command);

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

          if (list_ack==NULL)
		list_ack=(struct list*)creer_list();
	
	  if (list_ack==NULL) {
	      fprintf(stderr,"Erreur à la  création de la liste des ACK");
	  }
	
	 struct list* _list_a=list_ack;      /* contient tous les ack en provenance du meuble via la passerelle */
	 //devrait être le retour de la passerelle stocké dans une  liste adaptée
	 bool serverDiscon=false;
         int searchNode=0; 
	 do{
 
		 pthread_mutex_lock(&(_list_a->mutex));
		//pthread_cond_wait(&(_list_a->condition),&(_list_a->mutex));
		 //printf("\r\nextraction du  ACK no:%d\r\n",searchNode);
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
						int n = send(sock_send_ack,_noeud->data->ack,ACK_SIZE,0);
 						if (n <= 0) {
						    fprintf(stderr,"Erreur écriture de socket");
						    serverDiscon=true;
						}
				   		clog_info(CLOG(MY_LOGGER),"C  ===> '%s'",_noeud->data->ack);
					}

				}
			}		
 		}
	//	sleep(T_READ);
 
	 }while(serverDiscon==false);
 }

 // envoi les commandes à pppx
 void doprocessing_sender_command()
 {
  
  init_collecteur(&ip_pppx,&port_pppx,&port_collecteur);

  bool clientDiscon=false;
  int nb_commandes=1;
  char* serverName="PPPX";

  if (list_commands==NULL)
	list_commands=(struct list*)creer_list();
	
  if (list_commands==NULL) {
      fprintf(stderr,"Erreur à la création de la liste des commandes");
      return;
  }

  while(true)
  {
  	while (init_sock_client(&sock_send_command,ip_pppx,port_pppx,serverName)<0)
		sleep(T_CON);

        sock_listen_ack=sock_send_command;  
  	//printf("Connexion réussie sur le serveur '%s'-ip:'%s' port:%d",serverName,ip_pppx,port_pppx);

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
				if (_noeud->data->commande!=NULL)
				{ 
				_noeud->sent=true;
        			memset(command,'\0',COMMAND_SIZE);
                                strcpy(command,_noeud->data->commande);	
				if ((send(sock_send_command,command,COMMAND_SIZE,0))
					== -1) {
					 fprintf(stderr,"Echec émission commande Simple");
					 clientDiscon=true;
				}else
				clog_info(CLOG(MY_LOGGER),"C ==> [%d]'%s'",
				index,_noeud->data->commande);

				}
				}	
				}
			////nécessite une commande groupée
			}else if (nb_commandes>1)
			{
				//printf("\r\ncollecteur %d commandes à grouper\r\n",nb_commandes);
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
			        group_message[6]='0'; //zone

				sprintf(&group_message[7],"%d",(nb_commandes-1));	

				
				int iNoeud=0,jMessage=0;
				for(iNoeud=0;iNoeud<GROUP_NOEUD_SIZE;iNoeud++)
				if (group_noeud[iNoeud]!=NULL)
				{
					struct noeud* pNoeud=group_noeud[iNoeud];
					if (pNoeud!=NULL && pNoeud->data!=NULL)
					{
						char* _com=pNoeud->data->commande;
						if (_com!=NULL  )
						{
							pNoeud->sent=true;

							group_message[8 +16*jMessage]=_com[4];	
							group_message[9 +16*jMessage]=_com[5];	
							group_message[10+16*jMessage]=_com[6];	
							group_message[11+16*jMessage]=_com[7];	
							group_message[12+16*jMessage]=_com[8];	
							group_message[13+16*jMessage]=_com[9];	
							group_message[14+16*jMessage]=_com[10];	
							group_message[15+16*jMessage]=_com[11];	
							group_message[16+16*jMessage]=_com[12];	
							group_message[17+16*jMessage]=_com[13];	
							group_message[18+16*jMessage]=_com[14];	
							group_message[19+16*jMessage]=_com[15];	
							group_message[20+16*jMessage]=_com[16];	
							group_message[21+16*jMessage]=_com[17];	
							group_message[22+16*jMessage]=_com[18];	
							group_message[23+16*jMessage]=_com[19];	
							
							jMessage++;
						}
					}	
				}
				//on termine avec ZZ;
				//
				//
				group_message[23+1+16*(jMessage-1)]='Z';	
			       	group_message[23+2+16*(jMessage-1)]='Z';	
			       	group_message[23+3+16*(jMessage-1)]=';';	
			       	group_message[23+4+16*(jMessage-1)]='\0';	

			if ((send(sock_send_command,group_message,strlen(group_message) ,0))== -1) {
				 fprintf(stderr,"Echec d'envoi Group command");
				 clientDiscon=true;
			}
			else {
				 clog_info(CLOG(MY_LOGGER),"C ===> '%s'===> P",group_message);
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

   int r=0;
    /* Initialize the logger */
   r = clog_init_path(MY_LOGGER, "collecteur_log.txt");
   if (r != 0) {
       fprintf(stderr, "Logger initialization failed.\n");
       return NULL;
   }
   /* Set minimum log level to info (default: debug) */
   clog_set_level(MY_LOGGER, CLOG_INFO);
   clog_info(CLOG(MY_LOGGER),"Thread listener-command créé avec succés");
  
  ///////////////    threads creation
  //
  int err = pthread_create(&(tid[LISTEN_COM]), NULL, &listener_command, NULL);
  if (err != 0)
     fprintf(stderr,"can't create thread LISTEN-COMMAND :[%s]", strerror(err));
  sleep(T_CREAT);
  
  err = pthread_create(&(tid[SEND_COM]), NULL, &sender_command, NULL);
  if (err != 0)
     fprintf(stderr,"can't create thread SEND-COM :[%s]", strerror(err));
  sleep(T_CREAT);
  
  err = pthread_create(&(tid[LISTEN_ACK]), NULL, &listener_ack, NULL);
  if (err != 0)
     fprintf(stderr,"can't create thread LISTEN-ACK :[%s]", strerror(err));
  sleep(T_CREAT);
  
  err = pthread_create(&(tid[SEND_ACK]), NULL, &sender_ack, NULL);
  if (err != 0)
     fprintf(stderr,"can't create thread SEND-ACK :[%s]", strerror(err));
  sleep(T_CREAT);
  
  
  
  //////////////// wait until threads ends
  
  if(pthread_join(tid[LISTEN_ACK], NULL)) {
     fprintf(stderr,"Error joining thread LISTEN-ACK");
	return ;
  } 
  if(pthread_join(tid[LISTEN_COM], NULL)) {
     fprintf(stderr,"Error joining thread LISTEN-COM");
	return ;
  }
  if(pthread_join(tid[SEND_ACK], NULL)) {
     fprintf(stderr,"Error joining thread SEND-ACK");
	return ;
  }
  if(pthread_join(tid[SEND_COM], NULL)) {
     fprintf(stderr,"Error joining thread SEND-COM");
	return ;
  }
   clog_free(MY_LOGGER);
 }

void doprocessing_listener_ack ()
{
  // sock_listen_ack=sock_send_command;

   if (sock_listen_ack<=0){  
      fprintf(stderr,"Erreur à la création de sock_listen_ack");
      return;
   }
 
   if (list_ack==NULL)
	list_ack=(struct list*)creer_list();
	
   if (list_ack==NULL) {
      fprintf(stderr,"Erreur à la  création de la liste des ACK");
   }
  
   struct list* _list_a=list_ack;   
   init_list(_list_a);   

   collect_ack(sock_listen_ack);
}
 //reçoit les commandes de weblogi
 void collect_ack (int sock_ack) {
   int n=0;
   //bzero(command,SIZE_BUFFER_RECV);
   bool clientDiscon=false;
   do {
   	//printf("\r\nAttente du prochain ACK\r\n");
   	memset(ack, '0',ACK_SIZE);
    	n = read(sock_ack,ack,ACK_SIZE);
	if (n==0) { 
	clientDiscon=true;
	fprintf(stderr,"Erreur lecture socket ACK");
	continue;
        }
	
	ack[n] = '\0';
	clog_info(CLOG(MY_LOGGER),"P ===> '%s'",ack);
   

	if (strcmp(ack,"0005COMOK")==0)
		continue;
	
	if (strcmp(ack,"0005COMHS")==0)
		continue;

	struct list* _list_a=list_ack; 
	struct noeud* node=(struct noeud*)creer_noeud();
   
	init_noeud(node,_list_a->count);   
	init_ack(node,ack,_list_a->count);   
	if (_list_a!=NULL)
	{
   		pthread_mutex_lock(&(_list_a->mutex));
	   	ajouter_noeud(_list_a,node);	  
	   	//printf("\r\nThread listener-ack ADD :%s\r\n",ack);
		pthread_mutex_unlock(&(_list_a->mutex));
        }
   }
   while (clientDiscon==false);
}
