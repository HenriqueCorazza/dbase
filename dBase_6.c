#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <conio2.h>
#include <ctype.h>
#include <time.h>

struct Data{
	char dia[3],mes[3],ano[5];
};
typedef struct Data Data;


union Valor{
	float ValorN;
	char ValorD[10],ValorC[50],ValorM[50];
	char ValorL;
};

struct Dados{
	union Valor Valor;
	struct Dados *prox;		
	
};
typedef struct Dados Dados;

struct Campos{
	char FieldName[50];
	char Type;
	int Width, Dec;
	Dados *Patual, *Pdados;
	struct Campos *prox;
};
typedef struct Campos Campos;

struct Status{
	char bol;
	struct Status *prox;
};
typedef struct Status Status;

struct Arqs{
	struct Arqs *ant,*prox;
	Data data;
	Status *status;
	Campos *campos;
	char nomeDBF[50], hora[6];
	
};
typedef struct Arqs Arqs;

struct Dir {
	struct Dir *top, *bottom;
	Arqs *arqs;
	char und;
};
typedef struct Dir Dir;

#include "TadFilaDados.h"

void Setdefaultto(Dir ** unidade ){
	char var;
	
	printf("SET DEFAULT TO ");
	var = toupper(getch());
	
	if((*unidade)->und != var){
		if((*unidade)->top == NULL)
			*unidade = (*unidade)->bottom;
		else
			*unidade = (*unidade)->top;	
	}
	else
		printf("\nNao houve troca.");
			
}

char menu(){
	printf("\nDigite o comando...");
	return getch();
}

void inserirCampo(Arqs ** arq, Campos *campo){
	Arqs *aux = *arq;
	Campos *auxCampo;
	if(aux == NULL)
		printf("\n Arquivo inexistente...");
	else if(aux->campos == NULL)
		aux->campos = campo;	
	else{
		auxCampo = aux->campos;
		while(auxCampo->prox != NULL )
			auxCampo = auxCampo->prox;
		auxCampo->prox = campo;
	}
}

void criarCampos(Arqs ** arq){
	
	Campos *campo = (Campos*)malloc(sizeof(Campos));
	campo->Dec = 0;
	printf("\nNome do Campo: ");
	fflush(stdin);
	gets(campo->FieldName);
	
	campo->Patual = NULL;
	campo->Pdados = NULL;
	campo->prox = NULL;
	
	printf("\nTipo: ");
	campo->Type = toupper(getche());
	
	printf("\nLargura: ");
	scanf("%d",&campo->Width);
	
	
	/*
		Inserir dados preencheidos no arquivo enviado
	*/
	
	inserirCampo(&*arq,campo);
}

void inserirArq(Dir **dir, Arqs *arq){
	Arqs *aux;
	if((*dir)->arqs == NULL){
		(*dir)->arqs = arq;
	}
	else{
		aux = (*dir)->arqs;
		while(aux->prox != NULL)
			aux = aux->prox;
		aux->prox = arq;
	}
}
//
//tm* retornaData(void){
//  struct tm *data;     
//  
//  time_t segundos;
//  time(&segundos);   
//  
//  data = localtime(&segundos); 
//  return data;
//}

void AlteraStatus(Status *status, char bol){
	if(bol == 1)
		status->bol = 1;
	else if(bol == 0)
		status->bol = 0;
	else
		printf("\nValor invalido");
}

Status * NovoStatus(){
	Status *status = (Status*)malloc(sizeof(Status));
	status->bol = 1;
	status->prox = NULL;
}

void criarArquivo(Arqs **arq){
//	struct tm *data = retornaData();
	
	
	Arqs *novo = (Arqs*)malloc(sizeof(Arqs));
	novo->ant = novo->prox = NULL;
	//novo->data = 
	//novo->hora
	printf("Nome do arquivo: ");
	fflush(stdin);
	gets(novo->nomeDBF);
	
	
	novo->status = (Status*)malloc(sizeof(Status));
	novo->campos = NULL;
	
	*arq = novo;
}


void Create(Dir **dir){
	Arqs *novo = NULL;
	char op;
	
	criarArquivo(&novo);
	do{
		criarCampos(&novo);
		
		printf("Criar campos novamente?[qualquer tecla/s]");
		op = toupper(getche());
		
	}while(op == 'S');
	inserirArq(&*dir,novo);
}
void iniciarDir(Dir **dir){
	Dir *dirC = (Dir*)malloc(sizeof(Dir));
	dirC->top = NULL;
	dirC->arqs = NULL;
	dirC->  und = 'C';
	
	Dir *dirD = (Dir*)malloc(sizeof(Dir));
	dirD->top = dirC;
	dirD->	bottom = NULL;
	dirD->arqs = NULL;
	dirD->  und = 'D';
	
	dirC->	bottom =  dirD;
	
	*dir = dirC;
	
}

void ExibirDir(Dir *dir){
	Arqs *auxArq;
	size_t tam;
	
	if(dir->arqs == NULL)
		printf("\nDiretorio sem arquivos :( ");
	else{
		printf("\nDatabaseFiles\tRecords\tLast Update\tSize\n");
		auxArq = dir->arqs;
		
		while(auxArq != NULL)
		{
			tam = sizeof(auxArq);
			printf("%s\t%d\t%s\t%zu\n",auxArq->nomeDBF,0,"00/00/0000",tam);
			auxArq = auxArq->prox;
		}	
		
	}
	
}


Arqs* Use(Dir *dir){
	Arqs *auxArq;
	char nome[20];
	
	if(dir->arqs == NULL){
		printf("\nDiretorio sem arquivos :( ");
		return NULL;
	}
	else{
		
		printf("\nDigite o nome do arquivo: ");
		fflush(stdin);
		gets(nome);
		
		auxArq = dir->arqs;
		while(auxArq != NULL && strcmp(auxArq->nomeDBF,nome) != 0 )
			auxArq = auxArq->prox;
			
		if(auxArq == NULL){
			printf("\nArquivo nao encontrado");
			return NULL;
		}	
		else{
			printf("\nArquivo:%s, foi aberto com sucesso...",auxArq->nomeDBF);
			return auxArq;
		}
		
	}
	
}

//Finalizar
void ListStructure(Arqs *arq,char dir){
	int i = 0;
	Campos *auxCampos = NULL;
	
	if(arq == NULL){
		printf("\nArquivo vazio...");
	}else if(arq->campos == NULL){
		printf("\nCampos vazios...");
	}
	else{
		printf("\nStructure for database: %c:%s.DBF",dir,arq->nomeDBF);
		printf("\nNumber of data records: %d",0);//Add numero de records
		printf("\nDate of last update: %s","01/01/2025");// add data e tempo real ou escrito
		printf("\nField\tField Name\tType\tWidth\tDec");
		
		auxCampos = arq->campos;
		
		for(i = 0; auxCampos != NULL; i++){
			printf("\n%d\t%s\t%c\t%d\t%d",i,auxCampos->FieldName,auxCampos->Type,auxCampos->Width,auxCampos->Dec);
			auxCampos = auxCampos->prox;
		}
		printf("\n** Total **\n");
	}
}

//Finalizar
void inserirDados(Campos *campo, Dados *dado){
	Dados *aux;
	if(campo->Patual == NULL){
		campo->Patual = dado;
		campo->Pdados = dado;
	}
	else
	{
		aux = campo->Patual;
		
		while(aux->prox != NULL)
			aux = aux->prox;
		aux->prox = dado;
	}
		
}

void Numeric(Campos *campos){
	Dados *novoDado =  (Dados*)malloc(sizeof(Dados));
	novoDado->prox = NULL;
	scanf("%f",&novoDado->Valor.ValorN);
	inserirDados(campos,novoDado);
}

void Character(Campos *campos){
	Dados *novoDado =  (Dados*)malloc(sizeof(Dados));
	novoDado->prox= NULL;
	fflush(stdin);
	gets(novoDado->Valor.ValorC);
	inserirDados(campos,novoDado);

}

//Finalizar - adicionar os demais tipos de variavel a receber
void Append(Arqs **arq){
	int i = 0, numero;
	char terminal;
	char *character;
	
	Campos *auxCampos = NULL;
	
	if((*arq) == NULL)
		printf("\nArquivo vazio...\n");
	else if((*arq)->campos == NULL){
		printf("\nNao ha campos para inserir...");
	}
	else
	{
		auxCampos = (*arq)->campos;
		
		for(i = 0; auxCampos != NULL; i++){
			printf("\n%-10s",auxCampos->FieldName);
			
			terminal = auxCampos->Type;
			
			switch(terminal){
				
				case 'N':
					Numeric(auxCampos);
					break;
					
				case 'C':
					Character(auxCampos);
					break;
					
				default: printf("\nTipo nao identificado...");
			}
			
			auxCampos = auxCampos->prox;
		}
	}
	printf("\nFIM");
}

char BuscarAtributo(Campos *campo, char *field, char *tipo){
	Campos *aux = campo;
	char flag = 0;
	char atributo[50];
	if(aux == NULL)
		return 0;
	else {
		strcpy(atributo,aux->FieldName);
		while(aux != NULL && stricmp(field,aux->FieldName) != 0){
			aux = aux->prox;
		}
		if(aux != NULL){
			
			if(aux->Type == 'N' || aux->Type == 'L')
				*tipo = 0;
			else
				*tipo = aux->Type;
					
			return 1;
		}	
		return 0;
	}
}

char ComparaLetras(char *original, char *buscado){
	int tamOriginal = strlen(original), tamBuscado = strlen(buscado), i;
	
	if(tamBuscado > tamOriginal)
		return 0;
	else if(tamBuscado <= 0)
		printf("\nString sem tamanho");
	else {
		for(i = 0; i < tamBuscado; i++)
			if(original[i] != buscado[i])
				return 0;
		
		return 1;
	}
	
}


char BuscaNosCampos(Campos *campos, char *valor, char *atributo){
	Campos *aux = campos;
	Dados *auxDados;
	int i;
	char terminal, busca;
	for(i = 0;aux != NULL; i++){
		if(aux->Patual != NULL){
			if(!stricmp(aux->FieldName,atributo)){
				
				terminal = aux->Type;
				auxDados = aux->Patual;
				
				switch(terminal){
					
					case 'C':
						busca = ComparaLetras(auxDados->Valor.ValorC,valor);
						break;
					case 'D':
						busca = ComparaLetras(auxDados->Valor.ValorD,valor);
						break;
					case 'M':
						busca = ComparaLetras(auxDados->Valor.ValorM,valor);
						break;
						
					default: printf("\nTipo nao identificado");				
				}
				
				if(busca)
					return 1;
				else
					return 0;
			}
			
		}
		aux = aux->prox;
	}
	return 0;
	
}


void List(Arqs *arq, char *atributo, char *valor, char flagFor){
	Campos *auxCampos, *auxCamposInterno, *auxBusca;
	Dados *auxDados;
	char terminal, busca = -1, tipo = -1, buscaCampos = 1;
	int i = 1;
	char valorDado[50];
	
	if(arq == NULL)
		printf("\nArquivo vazio...");
	else if(arq->campos == NULL)
		printf("\nCampos vazios...");
	else if(arq->campos->Patual == NULL)
		printf("\nNao ha campos nos dados");
	else{
		
		if(flagFor){
			busca = BuscarAtributo(arq->campos,atributo,&tipo);
		}
		
		if(busca == 0){
			printf("\nO campo %s nao existe...",atributo);
		}
		else if(tipo == 0){
			printf("\nO campo FOR deve utilizado com string");
		}
		else{
			
			auxCampos = arq->campos;
			printf("\n%10.10s","Record");
			for(; auxCampos != NULL ; i ++){
				printf("%10.10s",auxCampos->FieldName);
				auxCampos = auxCampos->prox;
			}
		
			auxCampos = arq->campos;
			auxCamposInterno = auxCampos;
			
			for(i = 1;auxCamposInterno->Patual != NULL; i++){
				
				if(flagFor){
					buscaCampos = BuscaNosCampos(auxCamposInterno, valor, atributo);
					
					if(buscaCampos)
						printf("\n%10.d",i);
				}
				else
					printf("\n%10.d",i);
				
				while(auxCamposInterno != NULL){
					
					
					terminal = auxCamposInterno->Type;
					auxDados = auxCamposInterno->Patual;
					
					if(buscaCampos){
						
						if(terminal == 'N'){
							printf("%10.f",auxDados->Valor.ValorN);
						}else if(terminal == 'L'){
							printf("%10.c",auxDados->Valor.ValorD);
						}
						else{
							switch(terminal){
								case 'C':
									strcpy(valorDado,auxDados->Valor.ValorC);
									break;
								case 'D':
									strcpy(valorDado,auxDados->Valor.ValorD);
									break;
								case 'M':
									strcpy(valorDado,auxDados->Valor.ValorM);
									break;
							}
							printf("%10s",valorDado);
						}			
					}
						
					auxCamposInterno->Patual = auxCamposInterno->Patual->prox;
					auxCamposInterno = auxCamposInterno->prox;
					
				}
				if(buscaCampos)
					printf("\n");
				
				auxCamposInterno = auxCampos;
				
			}
			
			//restaurar Patual ds itens
			while(auxCampos != NULL)
			{
				auxCampos->Patual = auxCampos->Pdados;
				auxCampos = auxCampos->prox;
			}
				
			
		}	
		
	}
}

void executar (){
	char op;
	Arqs *aberto = NULL;
	
	Dir *dir = NULL;
	iniciarDir(&dir);
	
	do{
		op = menu();
		
		switch(op){
			case 'h': 
				Setdefaultto(&dir);
				break;
				
			case 'd': 
				Create(&dir);
				break;
				
			case 'e': 
				ExibirDir(dir);
				break;
			
			case 'u': 
				aberto = Use(dir);
				break;
				
			case 's': 
				ListStructure(aberto,dir->und);
				break;
			
			case 'a': 
				Append(&aberto);
				break;
				
			//List Comun
			case 'l': 
				List(aberto,(char*)"",(char*)"",0);
				break;
				
			//List for	
			case 'f': 
				List(aberto,(char*)"nome",(char*)"Jo",1);
				break;
			
			default: printf("\nErro -  comando inexistene!\n");
		}
		
	}while(op != 27);
	printf("\nProcesso finalizado!");
	
}

int main (){

	executar();

	return 0;
}

