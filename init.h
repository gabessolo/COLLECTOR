#ifndef __INIT__H__
#define __INIT__H__

#include <stdio.h>

int  init_pppx(char** ip_meuble,int* port_meuble,int* port_pppx);
char*  init_collecteur(char** ip_pppx,int* port_pppx,int* port_collecteur);

#endif /* __INIT__H__ */
