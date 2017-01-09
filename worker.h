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


 pthread_t tid[2];

 void doprocessing_pppx (int sock,int* searchNode,bool* clientDiscon);
 void doprocessing_collector (int sock_command,int sock_ack_weblogi,int* searchNode /* prochain ACK*/,bool* clientDiscon);
 int sendreceave(int sockfd,char* command,struct list* _list /* liste des ack */,bool* clientDiscon);
 
 void* listener(void*);
 void* sender(void*);

 void  creat_threads(char* ip_pass,int port_weblogi);
 int   init_socket_pppx(int* sockfd);

#endif /* __WORKER__H__ */
