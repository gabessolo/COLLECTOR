  #include <sys/socket.h>
  #include <sys/types.h>
  #include <netinet/in.h>
  #include <netdb.h>
  #include <stdio.h>
  #include <string.h>
  #include <stdlib.h>
  #include <unistd.h>
  #include <errno.h>
  #include <arpa/inet.h> 
  #include "defs.h"
  #include "sock.h"
  #include "init.h"
  
 int sock_send_command=0;
 int   port_pppx; 
 int   port_collecteur; 
 char* ip_collecteur; 
 char* ip_pppx;
 char* ip_meuble;
 int   port_meuble;
 

 const char* scenario[]={
        "#B00000E73A000000001ZZ;",
        "#E00000E73A000000001ZZ;",
	"#E00008A13A000000001ZZ;",
        "0000#E03001A13A000000001005A13A000000001006A13A000000001009A13A000000001ZZ;",
	"0000#E06004A13A000000001005A13A000000001006A13A000000001007A13A000000001008A13A000000001009A13A000000001010A13A000000001ZZ;",
        "0000#E03001A13A000000001005A13A000000001006A13A000000001009A13A000000001ZZ;",
"0000#E07001A13A000000001021A13A000000001003A13A000000001023A13A000000001004A13A000000001024A13A000000001007A13A000000001027A13A000000001ZZ;",
        "0000#E03001A13A000000001005A13A000000001006A13A000000001009A13A000000001ZZ;",
"0000#E03009A13A000000001029A13A000000001012A13A000000001032A13A000000001ZZ;",
        "0000#E03001A13A000000001005A13A000000001006A13A000000001009A13A000000001ZZ;",
"0000#E07001A13A000000001021A13A000000001003A13A000000001023A13A000000001004A13A000000001024A13A000000001006A13A000000001026A13A000000001ZZ;",
"0000#E05007A13A000000001027A13A000000001009A13A000000001029A13A000000001012A13A000000001032A13A000000001ZZ;",
        "0000#E03001A13A000000001005A13A000000001006A13A000000001009A13A000000001ZZ;",
"0000#E07001A13A000000001021A13A000000001003A13A000000001023A13A000000001004A13A000000001024A13A000000001006A13A000000001026A13A000000001ZZ;"};
/*
	
       // "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
	"#E00008A13A000000001ZZ;",
        "#E00000E73A000000001ZZ;",
        "#E00000E73A000000001ZZ;",
        "#E05001A13A000000001005A13A000000001006A13A000000001009A13A000000001010A13A000000001011;"
	
       // "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
	"#E00008A13A000000001ZZ;",
        "#E00000E73A000000001ZZ;",
        "#E00000E73A000000001ZZ;",
	"#E00009A13A000000001ZZ;",
        "#E05001A13A000000001005A13A000000001006A13A000000001009A13A000000001010A13A000000001011;"
	
      //  "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
        "#E00000E73A000000001ZZ;",
        "#E00000E73A000000001ZZ;",
	"#E00009A13A000000001ZZ;",
	"#E00008A13A000000001ZZ;",
	"#E00007A13A000000001ZZ;",
        "#E05001A13A000000001005A13A000000001006A13A000000001009A13A000000001010A13A000000001011;"
	
      //  "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
        "#E00000E73A000000001ZZ;",
	"#E00009A13A000000001ZZ;",
	"#E00008A13A000000001ZZ;",
	"#E00007A13A000000001ZZ;",
	"#E00006A13A000000001ZZ;",
        "#E05001A13A000000001005A13A000000001006A13A000000001009A13A000000001010A13A000000001011;"
	
      //  "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
        "#E00000E73A000000001ZZ;",
	"#E00009A13A000000001ZZ;",
	"#E00008A13A000000001ZZ;",
	"#E00007A13A000000001ZZ;",
	"#E00006A13A000000001ZZ;",
	"#E00005A13A000000001ZZ;",
        "#E05001A13A000000001005A13A000000001006A13A000000001009A13A000000001010A13A000000001011;"
	
      //  "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
        "#E00000E73A000000001ZZ;",
	"#E00009A13A000000001ZZ;",
	"#E00008A13A000000001ZZ;",
	"#E00007A13A000000001ZZ;",
	"#E00006A13A000000001ZZ;",
	"#E00005A13A000000001ZZ;",
	"#E00004A13A000000001ZZ;",
        "#E05001A13A000000001005A13A000000001006A13A000000001009A13A000000001010A13A000000001011;"
	
      //  "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
       // "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
	"#E00009A13A000000001ZZ;",
	"#E00008A13A000000001ZZ;",
	"#E00007A13A000000001ZZ;",
	"#E00006A13A000000001ZZ;",
	"#E00005A13A000000001ZZ;",
	"#E00004A13A000000001ZZ;",
	"#E00003A13A000000001ZZ;",
        "#E05001A13A000000001005A13A000000001006A13A000000001009A13A000000001010A13A000000001011;"
	

       // "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
        "#E05001A13A000000001005A13A000000001006A13A000000001009A13A000000001010A13A000000001011;"
       // "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
	"#E00009A13A000000001ZZ;",
	"#E00008A13A000000001ZZ;",
	"#E00007A13A000000001ZZ;",
	"#E00006A13A000000001ZZ;",
	"#E00005A13A000000001ZZ;",
	"#E00004A13A000000001ZZ;",
	"#E00003A13A000000001ZZ;",
	"#E00002A13A000000001ZZ;",

       // "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
       // "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
	"#E00009A13A000000001ZZ;",
	"#E00008A13A000000001ZZ;",
	"#E00007A13A000000001ZZ;",
	"#E00006A13A000000001ZZ;",
	"#E00005A13A000000001ZZ;",
	"#E00004A13A000000001ZZ;",
	"#E00003A13A000000001ZZ;",
	"#E00002A13A000000001ZZ;",
	"#E00001A13A000000001ZZ;",
        "#E05001A13A000000001005A13A000000001006A13A000000001009A13A000000001010A13A000000001011;"

       // "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
      //  "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
	"#E00009A13A000000001ZZ;",
	"#E00008A13A000000001ZZ;",
	"#E00007A13A000000001ZZ;",
	"#E00006A13A000000001ZZ;",
	"#E00005A13A000000001ZZ;",
	"#E00004A13A000000001ZZ;",
	"#E00003A13A000000001ZZ;",
	"#E00002A13A000000001ZZ;",
	"#E00001A13A000000001ZZ;",
	"#E00010A13A000000001ZZ;",
	
       // "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
       // "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
	"#E00009A13A000000001ZZ;",
	"#E00008A13A000000001ZZ;",
	"#E00007A13A000000001ZZ;",
	"#E00006A13A000000001ZZ;",
	"#E00005A13A000000001ZZ;",
	"#E00004A13A000000001ZZ;",
	"#E00003A13A000000001ZZ;",
	"#E00002A13A000000001ZZ;",
	"#E00001A13A000000001ZZ;",
	"#E00010A13A000000001ZZ;",
	"#E00011A13A000000001ZZ;",
        "#E05001A13A000000001005A13A000000001006A13A000000001009A13A000000001010A13A000000001011;"
	
       // "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
       // "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
	"#E00009A13A000000001ZZ;",
	"#E00008A13A000000001ZZ;",
	"#E00007A13A000000001ZZ;",
	"#E00006A13A000000001ZZ;",
	"#E00005A13A000000001ZZ;",
	"#E00004A13A000000001ZZ;",
	"#E00003A13A000000001ZZ;",
	"#E00002A13A000000001ZZ;",
	"#E00001A13A000000001ZZ;",
	"#E00010A13A000000001ZZ;",
	"#E00011A13A000000001ZZ;",
	"#E00012A13A000000001ZZ;",
	
       // "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
       // "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
	"#E00009A13A000000001ZZ;",
	"#E00008A13A000000001ZZ;",
	"#E00007A13A000000001ZZ;",
	"#E00006A13A000000001ZZ;",
	"#E00005A13A000000001ZZ;",
	"#E00004A13A000000001ZZ;",
	"#E00003A13A000000001ZZ;",
	"#E00002A13A000000001ZZ;",
	"#E00001A13A000000001ZZ;",
	"#E00010A13A000000001ZZ;",
	"#E00011A13A000000001ZZ;",
	"#E00012A13A000000001ZZ;",
	"#E00013A13A000000001ZZ;",
        "#E05001A13A000000001005A13A000000001006A13A000000001009A13A000000001010A13A000000001011;"
	
       // "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
        "#E00000E73A000000001ZZ;",
	"#E00009A13A000000001ZZ;",
	"#E00008A13A000000001ZZ;",
	"#E00007A13A000000001ZZ;",
	"#E00006A13A000000001ZZ;",
	"#E00005A13A000000001ZZ;",
	"#E00004A13A000000001ZZ;",
	"#E00003A13A000000001ZZ;",
	"#E00002A13A000000001ZZ;",
	"#E00001A13A000000001ZZ;",
	"#E00010A13A000000001ZZ;",
	"#E00011A13A000000001ZZ;",
	"#E00012A13A000000001ZZ;",
	"#E00013A13A000000001ZZ;",
	"#E00014A13A000000001ZZ;",
	
        "#E00000E73A000000001ZZ;",
       // "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
	"#E00009A13A000000001ZZ;",
	"#E00008A13A000000001ZZ;",
	"#E00007A13A000000001ZZ;",
	"#E00006A13A000000001ZZ;",
	"#E00005A13A000000001ZZ;",
	"#E00004A13A000000001ZZ;",
	"#E00003A13A000000001ZZ;",
	"#E00002A13A000000001ZZ;",
	"#E00001A13A000000001ZZ;",
	"#E00010A13A000000001ZZ;",
	"#E00011A13A000000001ZZ;",
	"#E00012A13A000000001ZZ;",
	"#E00013A13A000000001ZZ;",
	"#E00014A13A000000001ZZ;",
	"#E00015A13A000000001ZZ;",
        "#E05001A13A000000001005A13A000000001006A13A000000001009A13A000000001010A13A000000001011;"
	
       // "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
        "#E00000E73A000000001ZZ;",
	"#E00009A13A000000001ZZ;",
	"#E00008A13A000000001ZZ;",
	"#E00007A13A000000001ZZ;",
	"#E00006A13A000000001ZZ;",
	"#E00005A13A000000001ZZ;",
	"#E00004A13A000000001ZZ;",
	"#E00003A13A000000001ZZ;",
	"#E00002A13A000000001ZZ;",
	"#E00001A13A000000001ZZ;",
	"#E00010A13A000000001ZZ;",
	"#E00011A13A000000001ZZ;",
	"#E00012A13A000000001ZZ;",
	"#E00013A13A000000001ZZ;",
	"#E00014A13A000000001ZZ;",
	"#E00015A13A000000001ZZ;",
	
        "#E00000E73A000000001ZZ;",
       // "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
	"#E00009A13A000000001ZZ;",
	"#E00008A13A000000001ZZ;",
	"#E00007A13A000000001ZZ;",
	"#E00006A13A000000001ZZ;",
	"#E00005A13A000000001ZZ;",
	"#E00004A13A000000001ZZ;",
	"#E00003A13A000000001ZZ;",
	"#E00002A13A000000001ZZ;",
	"#E00001A13A000000001ZZ;",
	"#E00010A13A000000001ZZ;",
	"#E00011A13A000000001ZZ;",
	"#E00012A13A000000001ZZ;",
	"#E00013A13A000000001ZZ;",
	"#E00014A13A000000001ZZ;",
	"#E00015A13A000000001ZZ;",
	"#E00016A13A000000001ZZ;",
	
       // "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
        "#E00000E73A000000001ZZ;",
	"#E00009A13A000000001ZZ;",
	"#E00008A13A000000001ZZ;",
	"#E00007A13A000000001ZZ;",
	"#E00006A13A000000001ZZ;",
	"#E00005A13A000000001ZZ;",
	"#E00004A13A000000001ZZ;",
	"#E00003A13A000000001ZZ;",
	"#E00002A13A000000001ZZ;",
	"#E00001A13A000000001ZZ;",
	"#E00010A13A000000001ZZ;",
	"#E00011A13A000000001ZZ;",
	"#E00012A13A000000001ZZ;",
	"#E00013A13A000000001ZZ;",
	"#E00014A13A000000001ZZ;",
	"#E00015A13A000000001ZZ;",
	"#E00016A13A000000001ZZ;",
	"#E00017A13A000000001ZZ;",
	
        "#E00000E73A000000001ZZ;",
       // "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
	"#E00009A13A000000001ZZ;",
	"#E00008A13A000000001ZZ;",
	"#E00007A13A000000001ZZ;",
	"#E00006A13A000000001ZZ;",
	"#E00005A13A000000001ZZ;",
	"#E00004A13A000000001ZZ;",
	"#E00003A13A000000001ZZ;",
	"#E00002A13A000000001ZZ;",
	"#E00001A13A000000001ZZ;",
	"#E00010A13A000000001ZZ;",
	"#E00011A13A000000001ZZ;",
	"#E00012A13A000000001ZZ;",
	"#E00013A13A000000001ZZ;",
	"#E00014A13A000000001ZZ;",
	"#E00015A13A000000001ZZ;",
	"#E00016A13A000000001ZZ;",
	"#E00017A13A000000001ZZ;",
	"#E00018A13A000000001ZZ;",
	
       // "SIGNEDEVIE",
       // "SIGNEDEVIE",
       // "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
	"#E00009A13A000000001ZZ;",
	"#E00008A13A000000001ZZ;",
	"#E00007A13A000000001ZZ;",
	"#E00006A13A000000001ZZ;",
	"#E00005A13A000000001ZZ;",
	"#E00004A13A000000001ZZ;",
	"#E00003A13A000000001ZZ;",
	"#E00002A13A000000001ZZ;",
	"#E00001A13A000000001ZZ;",
	"#E00010A13A000000001ZZ;",
	"#E00011A13A000000001ZZ;",
	"#E00012A13A000000001ZZ;",
	"#E00013A13A000000001ZZ;",
	"#E00014A13A000000001ZZ;",
	"#E00015A13A000000001ZZ;",
	"#E00016A13A000000001ZZ;",
	"#E00017A13A000000001ZZ;",
	"#E00018A13A000000001ZZ;",
	"#E00019A13A000000001ZZ;",
	"#E00020A13A000000001ZZ;"
  };
*/

  char message[BUFFER_SENT];  //20 messages max + message extinction
  char header_message[BUFFER_SENT];
  int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char recvBuff[12];
    struct sockaddr_in serv_addr; 

    ip_collecteur=init_collecteur(&ip_pppx,&port_pppx,&port_collecteur);
    char* serverName="COLLECTEUR";
    while(init_sock_client(&sock_send_command,ip_collecteur,port_collecteur,serverName)<0)
    {
   	printf("\r\nErreur création de socket client sur le port :%d\r\n",port_collecteur);
	sleep(T_CON);  
    }
    int index=0;
    int nb_case=sizeof(scenario)/sizeof(scenario[0]);
    printf("\r\nConnexion au serveur '%s' sur le port %d  réussie\r\n",serverName,port_collecteur);
    printf("\r\nnombre de commandes à émettre en boucle : %d\r\n",nb_case);
    while(1)
    {
    	int size_message=0; 
        size_message=strlen(scenario[index]);
        memset(message,'\0',size_message); 
        sprintf(message,"%s",scenario[index]);


	if ((send(sock_send_command,message, size_message,0))== -1) {
            printf("\r\nEchec d'envoi du message '%s'\n",message);
	    exit(1);
    	}
    	 else {
         printf("\r\nWL ===> '%s'\n",message);
    	}

	sleep(T_READ);
	{	
    		memset(recvBuff, '0',sizeof(recvBuff));
	    	n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
       		recvBuff[n] = 0;
	        printf("\r\n'%s'<=== COLLECTEUR\r\n",recvBuff);

    		if(n < 0)
    		{
        		printf("\r\nErreur de lecture\r\n");
    		}
 	}

        index++;
        if (index>=nb_case)
       	 index=0;

	sleep(T_SEND);
    }
    close(sockfd);

    return 0;
 }
