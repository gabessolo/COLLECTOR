 #include "list.h"
 #include "defs.h"
 #include <unistd.h>
 #include <fcntl.h>
 #include <sys/types.h>
 #include <sys/stat.h>
 
 #include <stdlib.h>

 char ip_pass[20];
 int  port_pass;

 int  main(int argc, char** argv)
 {
	creat_threads(ip_pass,port_pass);
	return 0;
 }



