 #ifndef __DEFS__H__
 #define __DEFS__H__

 #define TAILLE_BUFFER  	64
 #define LISTENER 		0
 #define SENDER	  		1

 typedef int bool;

 #define false 			0
 #define true 			1

 #define IP_PASS		(char*)"192.168.38.102"
 #define PORT_PASS		(int)39000

 #define COMMAND_SIZE 		(23+1)
 #define ACK_SIZE     		(12+1)
 #define SIZE_BUFFER_RECV	COMMAND_SIZE 
 #define SIZE_BUFFER_SENT	SIZE_BUFFER_RECV 

 #define RE_INIT               "#E00007A13A000000001ZZ;"

#endif /* __DEFS__H__ */
