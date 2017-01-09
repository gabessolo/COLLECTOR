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


 pthread_t tid[2];

 void doprocessing_pppx (int sock,int* searchNode);
 void doprocessing_collector (int sock_command,int sock_ack_weblogi,int* searchNode /* prochain ACK*/);
 
 void* listener(void*);
 void* sender(void*);

 void  creat_threads(char* ip_pass,int port_weblogi);
 int   init_socket_pppx(int* sockfd);
 int   sendreceave(int sockfd,char* message,struct list* _list /* liste des ack */);

#endif /* __WORKER__H__ */
