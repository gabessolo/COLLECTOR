 #ifndef __DATA__H__
 #define __DATA__H__

 #include <stdio.h>
 #include "defs.h"
 #include <time.h>
 
 #define COMMAND_SIZE 23
 #define ACK_SIZE     12

 typedef struct data{
  int   count;
  bool  sent;
  char  commande[COMMAND_SIZE];
  char  ack[ACK_SIZE];
  time_t  t_rec;
  time_t  t_send;
  time_t  t_ack;
  time_t  t_act;	/* dépose */
  time_t  t_rep;        /* date de dernière répétition*/
  
 } data;

 struct data*  creer_data();
 void init_data(struct data* _data,char* message);

 #endif  /* __DATA__H__ */
