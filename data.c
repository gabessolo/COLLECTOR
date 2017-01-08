 #include "data.h"

 struct data*  creer_data()
 {
	return (struct data*)malloc(sizeof(struct data));
 }
 
 void init_data(struct data* _data,char* message)
 {
	if (_data==NULL || message==NULL)
		return NULL;
 	
	strcpy(_data->commande,message);
	_data->t_rec=time(NULL); 
 }	
