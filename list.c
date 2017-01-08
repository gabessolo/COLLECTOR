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
	if (_list==NULL)
		return false;

        struct noeud* dernier_noeud=_list->curseur;
        if (dernier_noeud==NULL) 
	{
	       dernier_noeud=_noeud;
	       dernier_noeud->suiv=dernier_noeud->prec=dernier_noeud;
		_list->count=1;
		_list->curseur=dernier_noeud;
	}
	else
	{    
        	struct noeud* premier_noeud=dernier_noeud;
        	while (dernier_noeud->suiv != premier_noeud)
		{
	     	   dernier_noeud=dernier_noeud->suiv;
		}

	        _noeud->prec=dernier_noeud;
	        _noeud->suiv=dernier_noeud->suiv;
		dernier_noeud->suiv=_noeud;

		//mise à jour du nombre de noeud total
		_list->count++;
	}           
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


	return 0;
 } 


 bool   init_noeud(struct noeud* _noeud,char *message)
 {
	if (_noeud==NULL)
		return false;
	
	_noeud->pdata=creer_data();
	_noeud->pdata->count=1;	
	strcpy(_noeud->pdata->commande,message);	
	return true;
 }

 bool aff_list(struct list* _list)
 {
	if (_list==NULL)
		return false;

	if (_list->curseur==NULL)
		return false;

	struct noeud* premier_noeud=_list->curseur;
	struct noeud* dernier_noeud=_list->curseur->suiv;
	
	printf("%s  nbr:%d\r\n",premier_noeud->pdata->commande,premier_noeud->pdata->count);
	while (premier_noeud!=dernier_noeud)
	{
		printf("%s  nbr:%d\r\n",dernier_noeud->pdata->commande,dernier_noeud->pdata->count);
		dernier_noeud=dernier_noeud->suiv;
	}
	return true;
 }	
