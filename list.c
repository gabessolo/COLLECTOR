 #include "list.h"
 #include <pthread.h>
 
 /* liste doublement chaînées circulaire */


 struct list* creer_list()
 {
	return (struct list*)malloc(sizeof(struct list));
 }

 struct noeud* creer_noeud()
 {
	return (struct noeud*)malloc(sizeof(struct noeud));
 } 

 bool  ajouter_noeud(struct list* _list,struct noeud* _noeud)
 {
	if (_list==NULL  || _noeud==NULL)
		return false;

   	//printf("\r\n  list:%p--[_list->count:%d]: %s\r\n",_list,_list->count,_noeud->data->commande);
        struct noeud* dernier_noeud=_list->curseur;
        if (dernier_noeud==NULL) 
	{
	       _noeud->suiv=_noeud->prec=_noeud;
	       _list->count=1;
	       _list->curseur=_noeud;		
	}
	else
	{    
	        _noeud->suiv=dernier_noeud->suiv;
		_noeud->prec=dernier_noeud;
		dernier_noeud->suiv=_noeud;
        	
		//mise à jour du nombre de noeud total
		_list->count++;
	}           
 //       printf("\r\nadd  list(%p)[count:%d][noeud:%p][index:%d][commande/ack:%s/%s]\r\n",_list,_list->count,_noeud,_noeud->index,_noeud->data->commande,_noeud->data->ack);
	return true;
 }

 bool  supprimer(struct list* _list,struct noeud* _noeud)
 {
	return true;
 }

 int    compter_noeud(struct list* _list)
 {
	if (_list==NULL)
		return 0;
	
	return  _list->count;
 }

 int init_list(struct list* _list)
 {
	if (_list==NULL)
		_list=creer_list();

	_list->curseur=NULL;
	_list->count=0;
   
	pthread_mutex_init(&(_list->mutex), NULL);
	pthread_cond_init(&(_list->condition), NULL);

	return 0;
 } 


 bool   init_noeud(struct noeud* _noeud,int index)
 {
	if (_noeud==NULL)
		return false;
	
	_noeud->data=creer_data();
	_noeud->data->count=1;	
	_noeud->data->commande[0]='\0';
	_noeud->data->ack[0]='\0';
	_noeud->index=index;	
	_noeud->sent=false;	
	return true;
 }

 bool aff_list(struct list* _list)
 {
	if (_list==NULL)
		return false;

	if (_list->curseur==NULL)
		return false;

	struct noeud* dernier_noeud=_list->curseur;
	struct noeud* premier_noeud=_list->curseur->suiv;
	
	printf("%s  nbr:%d\r\n",premier_noeud->data->commande,premier_noeud->data->count);
	while (premier_noeud!=dernier_noeud)
	{
		printf("%s  nbr:%d\r\n",premier_noeud->data->commande,premier_noeud->data->count);
		premier_noeud=premier_noeud->suiv;
	}
	return true;
 }	

 struct noeud* extractMessage(struct list* _list,int* searchNode)
 {
	if (_list==NULL /*|| searchNode==NULL*/)
		return NULL;

	if (_list->curseur==NULL)
		return NULL;
       
	//printf("\r\nsize of list:%d\r\n",_list->count);
	struct noeud* curseur=_list->curseur;
	struct noeud* dernier_noeud=NULL;
	struct noeud* _noeud=NULL;
	
	if (_list->count > 1)
	{
		curseur=_list->curseur->suiv;
		dernier_noeud=_list->curseur->suiv;
	}
	else if (*searchNode==0) 
	{
		_noeud=curseur;
		 *searchNode=*searchNode+1;
	}	
	
	//printf("\r\nlooking for %d\r\n",searchNode);
	if (_list->count > 1)
	do
	{
		//printf("\r\nlooking for %d==%d\r\n",searchNode,curseur->index);
		if (*searchNode==curseur->index)
		{
		    _noeud=curseur;
		   
		//    printf("\r\nSearch [%d] = cmd : %s - ack : %s\r\n",*searchNode,_noeud->data->commande,_noeud->data->ack);
		    *searchNode=*searchNode+1;	
                    break; 
		    
		}
                curseur=curseur->suiv;
	}while (curseur!=dernier_noeud);
       

	return _noeud;
 }	

