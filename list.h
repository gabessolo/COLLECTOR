 #ifndef __LIST__H__
 #define __LIST__H__

 #include "data.h"
 #include "defs.h"
 #include <stdlib.h>
 #include  <pthread.h>


 /* liste doublement chaînées circulaire */
 struct noeud {
 	struct  data *data;
 	struct  noeud* prec;
 	struct  noeud* suiv;
        int     index;
	bool    sent;
 };

 struct list {
 	int count;	 /* comptabilise les noeuds*/
	struct noeud* curseur;
        pthread_cond_t condition;
        pthread_mutex_t mutex;
 };


 struct list* creer_list();
 struct noeud* creer_noeud();
 bool   ajouter_noeud(struct list* _list,struct noeud* _noeud);
 bool   supprimer(struct list* _list,struct noeud* _noeud);
 int    compter_noeud(struct list* _list);
 bool   init_noeud(struct noeud* _noeud,int index);
 bool   aff_list(struct list* _list);
 int    init_list(struct list* _list);
 struct noeud* extractMessage(struct list* _list,int* searchNode);


#endif /* __LIST__H__ */
