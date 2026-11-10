
//Criar o menu
//Criar um vetor de char e enfileirá-lo 

#include <iostream>
#include <string.h>

using namespace std;

struct Elemento{
	char Operador[10]; 
	float Numero;
	Elemento *Prox;
};

struct Fila{
	Elemento *Inicio;
	Elemento *Fim;
	int Total;
};

struct Pilha{
	Elemento *Topo;
	int Total;
};

void Ini_Fila(Fila *F)
{
	F->Inicio = NULL;
	F->Fim = NULL;
	F->Total = 0;
}

void Ini_Pilha(Pilha *P)
{
	P->Topo = NULL;
	P->Total = 0;
}

bool Fila_Vazia(Fila *F){
	return (F->Inicio == NULL);
}

bool Pilha_Vazia(Pilha *P){
	return (P->Topo == NULL);
}

void Imprimir(Elemento *E){
	Elemento *Aux;
	
	cout << endl << "IMPRESS肙" << endl<<endl;	
	for(Aux = E;Aux!=NULL;Aux = Aux->Prox){
		if (strcmp(Aux->Operador, "#") == 0){ //N鷐ero
			cout << Aux->Numero;	
		}
		else{
			cout << Aux->Operador;	
		}
		
		
		if(Aux->Prox != NULL){
			cout << " ";
		}
	}
}

Elemento * Criar_Elemento(const char* Op, float Num){
	Elemento *Ret;

	Ret = (Elemento *) malloc(sizeof(Elemento));	
	if (Ret !=NULL){
		strcpy(Ret->Operador, Op);
		Ret->Numero = Num;
		Ret->Prox = NULL;
	}
	
	return Ret;
}

//FIFO - First In, First Out
//Enqueue (Queue = fila!!)
void Enfileirar(Fila *F, const char* Op, float Num){
	Elemento *ptE;
	
	ptE = Criar_Elemento(Op, Num);
	if (ptE !=NULL){
		if (Fila_Vazia(F)){
			//Atualiza incio e fim
			F->Inicio = ptE;			
		}
		else{
			//Atualiza o fim
			F->Fim->Prox = ptE;				
		}
		
		F->Fim = ptE;
		++F->Total;
	}	
}

//Dequeue
Elemento * Desenfileirar(Fila *F){
	Elemento *Ret = NULL;
	
	if (!Fila_Vazia(F)){
		Ret = F->Inicio;
		if (F->Inicio == F->Fim) {
			//1 elemento
			F->Inicio = NULL;
			F->Fim = F->Inicio;
		}
		else{
			//Mais de 1 elemento
			F->Inicio = F->Inicio->Prox;
		}
		--F->Total;			
	}
	
	return Ret;
}

//LIFO - Last In, First Out
//Push (Stack = pilha!!)
void Empilhar(Pilha *P, const char* Op, float Num){
	Elemento *ptE;
	
	ptE = Criar_Elemento(Op, Num);
	if (ptE !=NULL){
		if (!Pilha_Vazia(P)){		
			//Atualiza o fim
			ptE->Prox = P->Topo;	
		}
				
		P->Topo = ptE;		
		++P->Total;
	}	
}

//Pop
Elemento * Desempilhar(Pilha *P){
	Elemento *Ret = NULL;
	
	if (!Pilha_Vazia(P)){
		Ret = P->Topo;
		P->Topo = P->Topo->Prox;		
		--P->Total;			
	}
	
	return Ret;
}

bool Verificar_Caracter(Fila *F) {
    Pilha P;
    Elemento *Aux;
    Elemento *Removido;
    bool Erro = false;

    Ini_Pilha(&P);

    Aux = F->Inicio;

    while (Aux != NULL && !Erro) {

        if (strcmp(Aux->Operador, "(") == 0) {
            Empilhar(&P, "(", 0);
        }
        else if (strcmp(Aux->Operador, ")") == 0) {

            if (Pilha_Vazia(&P)) {
                Erro = true;
            }
            else {
                Removido = Desempilhar(&P);
                free(Removido);
            }
        }

        Aux = Aux->Prox;
    }

    if (!Pilha_Vazia(&P)) {
        Erro = true;
    }

    return !Erro;
}


void Menu(Fila *F){
	int op;
	char expressao[100]; 

	do{
		cout << "============= MENU ==============" << endl;
		cout << "[1] - Digite uma expressão " << endl; 
		cout << "[0] - Sair " << endl;
		cout << "\nDigite sua opcao: ";
		cin >> op;

		switch (op)
		{
		case 1:
			cout << "Digite uma expressão: ";
			cin.getline(expressao, sizeof(expressao));
			for(int i = 0; expressao[i] != '\0'; i++){
				//Enfileirar(F, expressao[i], expressao[i]); // enfileirar caracteres -> void enfileirar (Fila, carac); 
			}
			break;
		
		default:
			break;
		}


	}while (op != 0);
	
}

/*
int lerOpcao()
{
    int valor;
    while(!(cin >> valor))
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Entrada invalida! Digite um numero: ";
    }
    cin.ignore(1000, '\n');
    return valor;
}
*/

Fila F;
Pilha P;
Elemento *E;
int main(){
	setlocale(LC_ALL, "Portuguese");
		
	return 0;
}

