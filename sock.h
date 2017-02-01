#ifndef __SOCK__H__
#define __SOCK__H__

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>        
#include <netdb.h>
#include <netinet/in.h>
#include "worker.h"
#include "defs.h"

extern int   port_pppx; 
extern int   port_collecteur; 
extern char* ip_pppx;
extern char* ip_meuble;
extern int   port_meuble;

int init_sock_server(int* sockfd,int port);
 int   init_sock_client(int* sockfd,const char* ip, int port,const char* serverName);
void doprocessing_listener_ack ();

#endif /* __SOCK__H__ */
