 #include "data.h"
 #include "defs.h"
 #include <stdlib.h>
 #include  <pthread.h>


 /* liste doublement chaînées circulaire */
 struct noeud {
 	struct  data *pdata;
 	struct  noeud* prec;
 	struct  noeud* suiv;
 };

 struct list {
 	int count;	 /* comptabilise les noeuds*/
	struct noeud* curseur;
        pthread_cond_t condition /*= PTHREAD_COND_INITIALIZER*/;
        pthread_mutex_t mutex /*= PTHREAD_MUTEX_INITIALIZER*/;
 };


 struct list* creer_list();
 struct noeud* creer_noeud();
 bool   ajouter_noeud(struct list* _list,struct noeud* _noeud);
 bool   supprimer(struct list* _list,struct noeud* _noeud);
 int    compter_noeud(struct list* _list);
 bool   init_noeud(struct noeud* _noeud,char *commande);
 bool   ligne_a_liste_de_noeud(struct noeud* _noeud);
 bool   aff_list(struct list* _list);
 bool   sort_list(struct list* _list);
