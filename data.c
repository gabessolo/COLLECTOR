 #include "data.h"
 #include "list.h"

 struct data*  creer_data()
 {
	return (struct data*)malloc(sizeof(struct data));
 }
 
 void init_command(struct noeud* _noeud,char* message,int index)
 {
	if (_noeud==NULL  )
		return;

	(_noeud)->index=index;
	struct data* _data=(struct data*)creer_data();

	(_noeud)->data=_data;
	
	
	if (_data==NULL || message==NULL)
		return NULL;
 	
	strcpy(_data->commande,message);
	_data->t_rec=time(NULL);
	
 }	
 void init_ack(struct noeud* _noeud,char* message,int index)
 {
	if (_noeud==NULL  )
		return;

	(_noeud)->index=index;
	struct data* _data=(struct data*)creer_data();

	(_noeud)->data=_data;
	
	
	if (_data==NULL || message==NULL)
		return NULL;
 	
	strcpy(_data->ack,message);
	_data->t_rec=time(NULL);
	
 }	
