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
        "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
	"#E000011231;",
	
        "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
	"#E000011231;",
	"#E000021231;",
	
        "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
	"#E000011231;",
	"#E000021231;",
	"#E000031231;",
	
        "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
	"#E000011231;",
	"#E000021231;",
	"#E000031231;",
	"#E000041231;",
	
        "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
	"#E000011231;",
	"#E000021231;",
	"#E000031231;",
	"#E000041231;",
	"#E000051231;",
	
        "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
	"#E000011231;",
	"#E000021231;",
	"#E000031231;",
	"#E000041231;",
	"#E000051231;",
	"#E000061231;",
	
        "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
        "SIGNEDEVIE",
	"#E000011231;",
	"#E000021231;",
	"#E000031231;",
	"#E000041231;",
	"#E000051231;",
	"#E000061231;",
	"#E000071231;",
	

        "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
        "SIGNEDEVIE",
 	"#E000011231;",
	"#E000021231;",
	"#E000031231;",
	"#E000041231;",
	"#E000051231;",
	"#E000061231;",
	"#E000071231;",
	"#E000081231;",

        "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
        "SIGNEDEVIE",
	"#E000011231;",
	"#E000021231;",
	"#E000031231;",
	"#E000041231;",
	"#E000051231;",
	"#E000061231;",
	"#E000071231;",
	"#E000081231;",
	"#E000091231;",

        "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
        "SIGNEDEVIE",
	"#E000011231;",
	"#E000021231;",
	"#E000031231;",
	"#E000041231;",
	"#E000051231;",
	"#E000061231;",
	"#E000071231;",
	"#E000081231;",
	"#E000091231;",
	"#E000101231;",
	
        "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
        "SIGNEDEVIE",
	"#E000011231;",
	"#E000021231;",
	"#E000031231;",
	"#E000041231;",
	"#E000051231;",
	"#E000061231;",
	"#E000071231;",
	"#E000081231;",
	"#E000091231;",
	"#E000101231;",
	"#E000111231;",
	
        "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
        "SIGNEDEVIE",
	"#E000011231;",
	"#E000021231;",
	"#E000031231;",
	"#E000041231;",
	"#E000051231;",
	"#E000061231;",
	"#E000071231;",
	"#E000081231;",
	"#E000091231;",
	"#E000101231;",
	"#E000111231;",
	"#E000121231;",
	
        "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
        "SIGNEDEVIE",
	"#E000011231;",
	"#E000021231;",
	"#E000031231;",
	"#E000041231;",
	"#E000051231;",
	"#E000061231;",
	"#E000071231;",
	"#E000081231;",
	"#E000091231;",
	"#E000101231;",
	"#E000111231;",
	"#E000121231;",
	"#E000131231;",
	
        "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
	"#E000011231;",
	"#E000021231;",
	"#E000031231;",
	"#E000041231;",
	"#E000051231;",
	"#E000061231;",
	"#E000071231;",
	"#E000081231;",
	"#E000091231;",
	"#E000101231;",
	"#E000111231;",
	"#E000121231;",
	"#E000131231;",
	"#E000141231;",
	
        "#E00000E73A000000001ZZ;",
        "SIGNEDEVIE",
	"#E000011231;",
	"#E000021231;",
	"#E000031231;",
	"#E000041231;",
	"#E000051231;",
	"#E000061231;",
	"#E000071231;",
	"#E000081231;",
	"#E000091231;",
	"#E000101231;",
	"#E000111231;",
	"#E000121231;",
	"#E000131231;",
	"#E000141231;",
	"#E000151231;",
	
        "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
	"#E000011231;",
	"#E000021231;",
	"#E000031231;",
	"#E000041231;",
	"#E000051231;",
	"#E000061231;",
	"#E000071231;",
	"#E000081231;",
	"#E000091231;",
	"#E000101231;",
	"#E000111231;",
	"#E000121231;",
	"#E000131231;",
	"#E000141231;",
	"#E000151231;",
	"#E000161231;",
	
        "#E00000E73A000000001ZZ;",
        "SIGNEDEVIE",
	"#E000011231;",
	"#E000021231;",
	"#E000031231;",
	"#E000041231;",
	"#E000051231;",
	"#E000061231;",
	"#E000071231;",
	"#E000081231;",
	"#E000091231;",
	"#E000101231;",
	"#E000111231;",
	"#E000121231;",
	"#E000131231;",
	"#E000141231;",
	"#E000151231;",
	"#E000161231;",
	"#E000171231;",
	
        "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
	"#E000011231;",
	"#E000021231;",
	"#E000031231;",
	"#E000041231;",
	"#E000051231;",
	"#E000061231;",
	"#E000071231;",
	"#E000081231;",
	"#E000091231;",
	"#E000101231;",
	"#E000111231;",
	"#E000121231;",
	"#E000131231;",
	"#E000141231;",
	"#E000151231;",
	"#E000161231;",
	"#E000171231;",
	"#E000181231;",
	
        "#E00000E73A000000001ZZ;",
        "SIGNEDEVIE",
	"#E000011231;",
	"#E000021231;",
	"#E000031231;",
	"#E000041231;",
	"#E000051231;",
	"#E000061231;",
	"#E000071231;",
	"#E000081231;",
	"#E000091231;",
	"#E000101231;",
	"#E000111231;",
	"#E000121231;",
	"#E000131231;",
	"#E000141231;",
	"#E000151231;",
	"#E000161231;",
	"#E000171231;",
	"#E000181231;",
	"#E000191231;",
	
        "SIGNEDEVIE",
        "SIGNEDEVIE",
        "SIGNEDEVIE",
        "#E00000E73A000000001ZZ;",
	"#E000011231;",
	"#E000021231;",
	"#E000031231;",
	"#E000041231;",
	"#E000051231;",
	"#E000061231;",
	"#E000071231;",
	"#E000081231;",
	"#E000091231;",
	"#E000101231;",
	"#E000111231;",
	"#E000121231;",
	"#E000131231;",
	"#E000141231;",
	"#E000151231;",
	"#E000161231;",
	"#E000171231;",
	"#E000181231;",
	"#E000191231;",
	"#E000201231;"
  };

  char message[SIZE_BUFFER_SENT];  //20 messages max + message extinction
  char header_message[SIZE_BUFFER_SENT];
  int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr; 

    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    } 

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 
   
    int count=1;
    printf("\r\nTry to connect <%d>\r\n",count);
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
    	printf("\n Error : Connect Failed \n");
    	return 1;
    } 

    int index=0;
    int nb_case=sizeof(scenario)/sizeof(scenario[0]);
    printf("\r\nsizeof(scenario):%d\r\n",nb_case);
    while(1)
    {

    	printf("\r\nSent nbr <%d>\r\n",index);
        memset(message,'\0',SIZE_BUFFER_SENT); 
        //memset(header_message,'\0',SIZE_BUFFER_SENT); 
    	int size_message=0; 
//	for(index=0;index<count;index++)
  //  	{
          size_message=strlen(scenario[index]);
	  //sprintf(message,"%s%s",header_message,scenario[index]); 
          sprintf(message,"%s",scenario[index]);
    //    }
	message[size_message]='\0';


	if ((send(sockfd,message, size_message,0))== -1) {
            printf("\r\nFailure Sending Message\n");
	    exit(1);
    	}
    	 else {
         printf("\r\nMessage being  sent: %s\n",message);
    	}
	sleep(T_SEND);

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

	sleep(T_READ);
    }
    close(sockfd);

    return 0;
 }
