 #include "list.h"
 #include "defs.h"
 #include <unistd.h>
 #include <fcntl.h>
 #include <sys/types.h>
 #include <sys/stat.h>
 
 #include <stdlib.h>

 char* ip_pass=NULL;
 int   port_wl=0;

 int  main(int argc, char** argv)
 {
	
	init(&ip_pass,&port_weblogi);
 	//printf("\r\n<main>ip_pass:%s -- port_weblogi:%d\r\n",ip_pass,port_weblogi);
        creat_threads(ip_pass,port_weblogi);
	return 0;
 }



