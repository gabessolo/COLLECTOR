 #ifndef __DEFS__H__
 #define __DEFS__H__

 #define TAILLE_BUFFER  	64
 #define LISTENER 		0
 #define SENDER	  		1

 typedef int bool;

 #define false 			0
 #define true 			1

 //#define IP_PPPX		"127.0.0.1"
 #define IP_PPPX		"172.18.128.8"
 #define PORT_PPPX		39000
 #define PORT_COLLECTOR		5000

 #define COMMAND_SIZE 		23
 #define ACK_SIZE     		(12+4)
 #define SIGNEDEVIE_SIZE     	10
 #define COMOK_SIZE     	9
 #define SIZE_BUFFER_RECV	COMMAND_SIZE 
 #define SIZE_BUFFER_SENT	SIZE_BUFFER_RECV 

 #define RE_INIT	       "#E00000E73A000000001ZZ;"
 
 #define T_SEND                2  
 #define T_READ                1 
 #define T_NEXT                1 
 #define T_AJ_CMD              2 
 #define T_CON                 1 

 #define GROUP_NOEUD_SIZE      5
 #define GROUP_MESSAGE_SIZE    (115+1)

 #define EX_MSG_GROUP 	"0000#E04001A03A000000001002A13A000000001003A03A000000001004A13A000000001005A03A000000001ZZ;"
 //	"0000#E04 001A03A000000001 002A13A000000001 003A03A000000001 004A13A000000001 005A03A000000001 ZZ;"

#endif /* __DEFS__H__ */
