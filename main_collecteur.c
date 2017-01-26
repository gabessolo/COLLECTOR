 #include "list.h"
 #include "defs.h"
 #include <unistd.h>
 #include <fcntl.h>
 #include <sys/types.h>
 #include <sys/stat.h>
 
 #include <stdlib.h>

 char* ip_pppx=NULL;
 int   port_pppx=0;

 int  main(int argc, char** argv)
 {
	
	init(&ip_pppx,&port_pppx);
        creat_threads();
	return 0;
 }



