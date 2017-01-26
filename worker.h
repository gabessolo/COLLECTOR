 #ifndef __WORKER__H__
 #define __WORKER__H__

 #include <stdio.h>
 #include <string.h>
 #include <pthread.h>
 #include <stdlib.h>
 #include <unistd.h>        
 #include <netdb.h>
 #include <netinet/in.h>
 #include "list.h"
 #include "data.h"


 pthread_t tid[4];

 void doprocessing_listener_command ();
 void doprocessing_listener_ack();
 void doprocessing_sender_ack();
 void doprocessing_sender_command();

 
 void* listener_command(void*);
 void* listener_ack(void*);
 void* sender_ack(void*);
 void* sender_command(void*);

 void  creat_threads();
 int   init_sock_client(int* sockfd,const char* ip, int port,const char* serverName);
 int   init_sock_server(int* sockfd,int port);

#endif /* __WORKER__H__ */
