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

 void doprocessing (int sock);
 
 void* listener(void*);
 void* sender(void*);

 void  creat_threads(char* ip_pass,int port_weblogi);

#endif /* __WORKER__H__ */
