 #ifndef __DATA__H__
 #define __DATA__H__

 #include <stdio.h>
 #include "defs.h"
 #include <time.h>
 #include "list.h"
 

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
 void init_command(struct noeud* _noeud,char* message,int index);
 void init_ack(struct noeud* _noeud,char* message,int index);

 #endif  /* __DATA__H__ */
