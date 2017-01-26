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

  char message[SIZE_BUFFER_SENT];  //20 messages max + message extinction
  char header_message[SIZE_BUFFER_SENT];
  int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char recvBuff[12];
    struct sockaddr_in serv_addr; 


    int portno=PORT_COLLECTEUR;
    char* ip=(char*)malloc(20*sizeof(char));
    if (ip==NULL)
	return 1;

    strcpy(ip,IP_COLLECTEUR);
    


    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno); 

    if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 
   
    int count=1;
    printf("\r\nTry to connect <%d>\r\n",count);
    
    while ( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <= 0)
    {
    	printf("\r\n Error : Connect to collecteur  Failed \r\n");

	sleep(T_CON);
    } 

    int index=0;
    int nb_case=sizeof(scenario)/sizeof(scenario[0]);
    printf("\r\nsizeof(scenario):%d\r\n",nb_case);
    while(1)
    {

    	printf("\r\nSent nbr <%d>\r\n",index);
        //memset(header_message,'\0',SIZE_BUFFER_SENT); 
    	int size_message=0; 
//	for(index=0;index<count;index++)
  //  	{
          size_message=strlen(scenario[index]);
        memset(message,'\0',size_message); 
	  //sprintf(message,"%s%s",header_message,scenario[index]); 
          sprintf(message,"%s",scenario[index]);
    //    }


	if ((send(sockfd,message, size_message,0))== -1) {
            printf("\r\nFailure Sending Message\n");
	    exit(1);
    	}
    	 else {
         printf("\r\nMessage being  sent: %s\n",message);
    	}
	//sleep(/*T_SEND*/ 1);

	//if (strcmp(message,RE_INIT)!=0)
	{	
    		memset(recvBuff, '0',sizeof(recvBuff));
	    	n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
       		recvBuff[n] = 0;
	        printf("\r\n<pppx>Receaved:%s\r\n",recvBuff);

    		if(n < 0)
    		{
        		printf("\n Read error \n");
    		}
 	}//else
	//	sleep(T_NEXT);


        index++;
        if (index>=nb_case)
       	 index=0;

	//sleep(T_READ);
    }
    close(sockfd);

    return 0;
 }
