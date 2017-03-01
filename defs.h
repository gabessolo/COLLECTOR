 #ifndef __DEFS__H__
 #define __DEFS__H__

 #define TAILLE_BUFFER  	64

 #define LISTEN_COM 		0
 #define LISTEN_ACK 		1
 #define SEND_COM	  	2
 #define SEND_ACK	  	3

 typedef int bool;

 #define false 			0
 #define true 			1

 #define IP_PPPX		"127.0.0.1"
 //#define IP_PPPX	        "172.18.128.8"
 //#define IP_PPPX		"192.168.38.102"
// #define IP_WL	        "172.18.128.8"
 //#define IP_WL	        "127.0.0.1"
 
 #define PORT_MEUBLE		2101
#define PORT_PPPX	        PORT_MEUBLE	
 //#define PORT_COLLECTEUR	5000
 #define PORT_COLLECTEUR	39000
 //#define IP_COLLECTEUR        "172.18.128.8"
 #define IP_COLLECTEUR	        "127.0.0.1"
 #define IP_MEUBLE	        "127.0.0.1"

 #define COMMAND_SIZE 		23
 #define ACK_SIZE     		12
 #define SIGNEDEVIE_SIZE     	10
 #define COMOK_SIZE     	9
 #define BUFFER_SENT	COMMAND_SIZE 

 #define RE_INIT	       "#E00000E73A000000001ZZ;"
 
 #define T_SEND                1  
 #define T_READ                1 
 #define T_NEXT                1 
 #define T_AJ_CMD              1 
 #define T_CON                 1 
 #define T_CREAT               1 
 #define T_EXTRACT             1 

 #define GROUP_NOEUD_SIZE      5
 #define GROUP_MESSAGE_SIZE    115

 #define BUFFER_RECV	GROUP_MESSAGE_SIZE 
 #define EX_MSG_GROUP 	"0000#E04001A03A000000001002A13A000000001003A03A000000001004A13A000000001005A03A000000001ZZ;"
 //	"0000#E04 001A03A000000001 002A13A000000001 003A03A000000001 004A13A000000001 005A03A000000001 ZZ;"

 #define MAX_CLIENTS_COLLECTEUR 5
 #define MAX_CLIENTS_PPPX MAX_CLIENTS_COLLECTEUR 

#endif /* __DEFS__H__ */
