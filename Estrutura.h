#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TM_C 100 

struct fila{
	char elem[TM_C];
	struct fila *prox;
};
typedef struct fila Fila;

struct descritor{
	Fila *inicio;
	Fila *fim;
};
typedef struct descritor Descritor;

char isEmpty(Descritor *D){
	return D->inicio==NULL;
}

void init(Descritor **D){
	*D = (Descritor*)malloc(sizeof(Descritor));
	(*D)->inicio=(*D)->fim=NULL;
}

void Enqueue(Descritor **D, char *x){
	Fila *nc= (Fila*)malloc(sizeof(Fila));
	nc->prox=NULL;
	strcpy(nc->elem,x);
	if(isEmpty(*D)){
		(*D)->inicio=(*D)->fim=nc;
	}
	else{
		(*D)->fim->prox=nc;
		(*D)->fim=nc;
	}
}

void Dequeue(Descritor **D, char *x){
	strcpy(x,(*D)->inicio->elem);
	Fila *f=NULL;
	f= (*D)->inicio;
	(*D)->inicio=(*D)->inicio->prox;
	free(f);
}

void ExibirFila(Descritor *D){
	Fila *aux=D->inicio;
	while(aux!=NULL){
		printf("%s\n",&aux->elem);
		aux=aux->prox;
	}
}

void SeparaString(char *s, Descritor **D){
	
	
	int i=0, j=0;
	char elem[TM_C], atual=0;
	elem[0]='\0';
	atual = s[i];
	
	while(atual!='\0'){
		if(atual!=32){
			elem[j]=s[i];
			j++;
		}

		else{
			if(j>0){
				elem[j]='\0';
				Enqueue(&(*D),elem);
				strcpy(elem,"");
				j=0;				
			}
	
		}
		i++;
		atual = s[i];
	}
	elem[j]='\0';
	if(j>0)
		Enqueue(&(*D),elem);	
}

int EscreveString(char *retorno, char *elem, int i, int j){
	while(elem[i]!='\0'){
		retorno[j]=elem[i];
		i++;j++;
	}
	retorno[j]=32;
	j++;
	return j;
}
void ValidaAspas(Descritor **D, char *retorno, char *flag){
	char elem[TM_C];
	int i=0,j=0;
	Dequeue(&(*D),elem);
	if(elem[0]==34){
		j=EscreveString(retorno,elem,i+1,j);
		while(!isEmpty(*D) && elem[strlen(elem)-1]==34){
			Dequeue(&(*D),elem);
			j=EscreveString(retorno,elem,i,j);
		}
		if(elem[strlen(elem)-1]==34){
			retorno[j-2]='\0';
			*flag=8;
		}
		else{
			*flag=80;
		}
	}
	else
		*flag=80;
}

void ValidaNumero(char *elem, char *flag){
	int i=0;
	while(elem[i]!='\0'){
		if(elem[i]<48 || elem[i]>57){
			*flag=79;
		}
		i++;
	}
	if(*flag!=79)
		*flag=11;
}

char ReconhecerComando(Descritor **D, char *retorno, char *retorno2){
	char elem[TM_C];
	char flag=0;
	Dequeue(&(*D),elem);
	
	if(stricmp(elem,"SET")==0){
		if(!isEmpty(*D))
			Dequeue(&(*D),elem);
		
		if(stricmp(elem,"DEFAULT")==0){
			if(!isEmpty(*D))
				Dequeue(&(*D),elem);
			if(stricmp(elem,"TO")==0){
				if(!isEmpty(*D))
					Dequeue(&(*D),elem);
				if(stricmp(elem,"C:")==0){
					flag=1;
					retorno[0]='C';
					retorno[1]='\0';
				}
				else
					if(stricmp(elem,"D:")==0){
						flag=1;
						retorno[0]='D';
						retorno[1]='\0';
					}
			}
		}
		else
			if(stricmp(elem,"DELETED")==0){
				if(isEmpty(*D))
					Dequeue(&(*D),elem);
				if(stricmp(elem,"OFF")==0){
					flag=16;
					strcpy(retorno,elem);
				}
				
				else{
					if(stricmp(elem,"ON")==0){
						flag=16;
						strcpy(retorno,elem);
					}
				}
			}
	}
	else{
		if(stricmp(elem,"CREATE")==0){
			if(!isEmpty(*D))
				Dequeue(&(*D),elem);
			if((elem[strlen(elem)-4]=='.') && (elem[strlen(elem)-3]=='D' || elem[strlen(elem)-3]=='d') && (elem[strlen(elem)-2]=='B' || elem[strlen(elem)-2]=='b') && (elem[strlen(elem)-1]=='F' || elem[strlen(elem)-1]=='f'))
			{
				strcpy(retorno,elem);
				flag=2;
			}
			
		}
		else{
			if(stricmp(elem,"DIR")==0){
				flag=3;
			}
			else{
				if(stricmp(elem,"QUIT")==0){
					flag=4;
				}
				else{
					if(stricmp(elem,"USE")==0){
						if(!isEmpty(*D))
							Dequeue(&(*D),elem);
						if((elem[strlen(elem)-4]=='.') && (elem[strlen(elem)-3]=='D' || elem[strlen(elem)-3]=='d') && (elem[strlen(elem)-2]=='B' || elem[strlen(elem)-2]=='b') && (elem[strlen(elem)-1]=='F' || elem[strlen(elem)-1]=='f')){
								strcpy(retorno,elem);
								printf("%s",retorno);
								flag=5;
						}
					}
					else{
						if(stricmp(elem,"LIST")==0){
							if(!isEmpty(*D))
								Dequeue(&(*D),elem);
							if(stricmp(elem,"STRUCTURE")==0){
								flag=6;
							}
							else{
								if(stricmp(elem,"FOR")==0){
									if(!isEmpty(*D))
										Dequeue(&(*D),elem);
									strcpy(retorno,elem);
									if(!isEmpty(*D))
										Dequeue(&(*D),elem);
									if(stricmp(elem,"=")==0){
										ValidaAspas(&(*D),retorno2,&flag);
										if(flag!=80)
											flag=8;
										}
								}	
							}
						}
						else{
							if(stricmp(elem,"CLEAR")==0){
								flag=9;
							}
							else{
								if(stricmp(elem,"APPEND")==0)
									flag=7;
								else{
									if(stricmp(elem,"LOCATE")==0){
										if(!isEmpty(*D))
											Dequeue(&(*D),elem);
										if(stricmp(elem,"FOR")==0){
											if(!isEmpty(*D))
												Dequeue(&(*D),elem);
											strcpy(retorno,elem);
											if(!isEmpty(*D))
												Dequeue(&(*D),elem);
											if(stricmp(elem,"=")==0){
												ValidaAspas(&(*D),retorno2,&flag);
												if(flag!=80)
													flag=10;	
											}
										}
									}
									else{
										if(stricmp(elem,"GOTO")==0){
											if(!isEmpty(*D))
												Dequeue(&(*D),elem);
											ValidaNumero(elem,&flag);
											if(flag!=79){
												flag=11;
											}
										}
										else{
											if(stricmp(elem,"DISPLAY")==0){
												flag=12;
											}
											else{
												if(stricmp(elem,"EDIT")==0){
													flag=13;
												}
												else{
													if(stricmp(elem,"DELETE")==0){
														if(!isEmpty(*D)){
															Dequeue(&(*D),elem);
															if(stricmp(elem,"ALL")==0){
																flag=14;
																strcpy(retorno,elem);
															}
														}
														else
															flag=14;
													}
													else{
														if(stricmp(elem,"RECALL")==0){
															if(!isEmpty(*D)){
																Dequeue(&(*D),elem);
																if(stricmp(elem,"ALL")==0){
																	flag=15;
																	strcpy(retorno,elem);
																}
															}
															else
																flag=15;
														}
														else{
															if(stricmp(elem,"PACK")==0){
																flag=17;
															}
															else{
																if(stricmp(elem,"ZAP")==0)
																	flag=18;
															}
														}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
	if(!isEmpty(*D)){
		flag=0;
		init(&(*D));
	}
	if(flag==80)
		strcpy(retorno,"Aspas incorretas ou faltantes!");
	if(flag==79)
		strcpy(retorno,"Numero faltante ou incorreto!");
	if(flag==0){
		strcpy(retorno,"Comando digitado incorretamente!");
		flag=120;
	}
	return flag;
}



