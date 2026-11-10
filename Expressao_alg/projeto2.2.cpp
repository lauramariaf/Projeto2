#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

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
    
    cout << endl << "============= IMPRESSAO DA FILA =============" << endl;  
    for(Aux = E; Aux != NULL; Aux = Aux->Prox){
        if (strcmp(Aux->Operador, "#") == 0){ // N�mero
            cout << Aux->Numero;    
        }
        else{
            cout << Aux->Operador;  
        }
        
        if(Aux->Prox != NULL){
            cout << " ";
        }
    }
    cout << endl << "===========================================" << endl;
}

Elemento * Criar_Elemento(const char* Op, float Num){
    Elemento *Ret;

    Ret = (Elemento *) malloc(sizeof(Elemento));    
    if (Ret != NULL){
        strcpy(Ret->Operador, Op);
        Ret->Numero = Num;
        Ret->Prox = NULL;
    }
    
    return Ret;
}

// FIFO - First In, First Out
void Enfileirar(Fila *F, const char* Op, float Num){
    Elemento *ptE;
    
    ptE = Criar_Elemento(Op, Num);
    if (ptE != NULL){
        if (Fila_Vazia(F)){
            F->Inicio = ptE;            
        }
        else{
            F->Fim->Prox = ptE;                
        }
        
        F->Fim = ptE;
        ++F->Total;
    }   
}

// Dequeue
Elemento * Desenfileirar(Fila *F){
    Elemento *Ret = NULL;
    
    if (!Fila_Vazia(F)){
        Ret = F->Inicio;
        if (F->Inicio == F->Fim) {
            F->Inicio = NULL;
            F->Fim = NULL;
        }
        else{
            F->Inicio = F->Inicio->Prox;
        }
        --F->Total;         
    }
    
    return Ret;
}

// LIFO - Last In, First Out
void Empilhar(Pilha *P, const char* Op, float Num){
    Elemento *ptE;
    
    ptE = Criar_Elemento(Op, Num);
    if (ptE != NULL){
        if (!Pilha_Vazia(P)){      
            ptE->Prox = P->Topo;    
        }
                
        P->Topo = ptE;      
        ++P->Total;
    }   
}

// Pop
Elemento * Desempilhar(Pilha *P){
    Elemento *Ret = NULL;
    
    if (!Pilha_Vazia(P)){
        Ret = P->Topo;
        P->Topo = P->Topo->Prox;        
        --P->Total;         
    }
    
    return Ret;
}

int ObterPeso(const char* operador) {
    int peso = -1;
    if (strcmp(operador, "!") == 0) {
        peso = 4; // Maior prioridade (un�rio)
    }
    if (strcmp(operador, "*") == 0 || strcmp(operador, "/") == 0) {
        peso = 3; // Multiplica��o e divis�o
    }
    if (strcmp(operador, "+") == 0 || strcmp(operador, "-") == 0) {
        peso = 2; // Soma e subtra��o
    }
    if (strcmp(operador, ">") == 0 || strcmp(operador, "<") == 0 || 
        strcmp(operador, "<=") == 0 || strcmp(operador, "==") == 0 || 
        strcmp(operador, "!=") == 0  || strcmp(operador, ">=") == 0 ) {
        peso = 1; // Operadores relacionais
    }
    if (peso == -1) {
        cout << "operador invalido" << endl;
    }
    return peso;
}

float Calcular(float n1, float n2, const char* operador) {

    float resultado = 0.0f;

    if (strcmp(operador, "+") == 0) {
        resultado = n1 + n2;
    }
    if (strcmp(operador, "-") == 0) {
        resultado = n1 - n2;
    }
    if (strcmp(operador, "*") == 0) {
        resultado = n1 * n2;
    }
    if (strcmp(operador, "/") == 0) {
        if (n2 != 0) {
            resultado = n1 / n2;
        } else {
            cout << "Erro: Divis�o por zero!\n";
            resultado = 0.0f;
        }
    }
    if (strcmp(operador, ">") == 0) {
        resultado = (n1 > n2) ? 1.0f : 0.0f;
    }
    if (strcmp(operador, "<") == 0) {
        resultado = (n1 < n2) ? 1.0f : 0.0f;
    }
    if (strcmp(operador, "==") == 0) {
        resultado = (n1 == n2) ? 1.0f : 0.0f;
    }
    if (strcmp(operador, "!=") == 0) {
        resultado = (n1 != n2) ? 1.0f : 0.0f;
    }
    if (strcmp(operador, "<=") == 0) {
        resultado = (n1 <= n2) ? 1.0f : 0.0f;
    }
    if (strcmp(operador, ">=") == 0) {
        resultado = (n1 >= n2) ? 1.0f : 0.0f;
    }

    return resultado;
}

void atualiza_pilha(Pilha *Pnum, Pilha *Pop){
    
    float Num1, Num2, Resultado;
    char op[3] = "";
    if(!Pilha_Vazia(Pnum) && !Pilha_Vazia(Pop)){
        Num2 = Pnum->Topo->Numero;
        Desempilhar(Pnum);
        
        Num1 = Pnum->Topo->Numero;
        Desempilhar(Pnum);
        
        strcpy(op, Pop->Topo->Operador);
        Desempilhar(Pop);
        
        Resultado = Calcular(Num1, Num2, op);
        
        Empilhar(Pnum, "#", Resultado); // atualiza o resultado na pilha de n�meros
    }
}

float controlador (Fila *F){
    Pilha PNUM, POP;
    Ini_Pilha(&PNUM);
    Ini_Pilha(&POP);
    float resultado = 0.0f;

    while(!Fila_Vazia(F)){
        Elemento *atual = Desenfileirar(F);

        if (strcmp(atual->Operador, "#") == 0) {
            Empilhar(&PNUM, "#", atual->Numero);
        }
        else{
            if(strcmp(atual->Operador, "(") == 0){
                Empilhar(&POP, "(", 0);
            }
            else{
                if(strcmp(atual->Operador, ")") == 0){
                    while(!Pilha_Vazia(&POP) && strcmp(POP.Topo->Operador, "(") != 0) {
                        atualiza_pilha(&PNUM, &POP);
                    }
                    if(!Pilha_Vazia(&POP)){
                        Elemento *aux = Desempilhar(&POP);
                        free(aux);
                    }
                }
                else{
                    while (!Pilha_Vazia(&POP) && strcmp(POP.Topo->Operador, "(") != 0 &&
                        ObterPeso(POP.Topo->Operador) >= ObterPeso(atual->Operador)) {
                        atualiza_pilha(&PNUM, &POP);
                    }
                    Empilhar(&POP, atual->Operador, 0);
                }
            }
        }
    }

    while (!Pilha_Vazia(&POP)) {
        atualiza_pilha(&PNUM, &POP);
    }

    if (!Pilha_Vazia(&PNUM)) {
        resultado = PNUM.Topo->Numero;
        Elemento *Removido = Desempilhar(&PNUM);
        free(Removido);
    }

    return resultado;
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
        else {
            if (strcmp(Aux->Operador, ")") == 0) {
                if (Pilha_Vazia(&P)) {
                    Erro = true;
                }
                else {
                    Removido = Desempilhar(&P);
                    free(Removido);
                }
            }
        }
        Aux = Aux->Prox;
    }

    if (!Pilha_Vazia(&P)) {
        Erro = true;
    }

    return !Erro;
}

void ProcessarExpressao(Fila *F, char expressao[]) {
    Ini_Fila(F);
    int i = 0;
    while (expressao[i] != '\0') {
        if (expressao[i] == ' ' || expressao[i] == '\t') {
            i++;
        }
        else {
            if ((expressao[i] >= '0' && expressao[i] <= '9') || expressao[i] == '.') {
                char numStr[50];
                int j = 0;
                
                while (expressao[i] != '\0' && ((expressao[i] >= '0' && expressao[i] <= '9') || expressao[i] == '.')) {
                    numStr[j++] = expressao[i++];
                }
                numStr[j] = '\0';
                
                float numVal = 0.0f;
                int k = 0;
                bool pontoEncontrado = false;
                float divisorDecimal = 10.0f;

                while (numStr[k] != '\0') {
                    if (numStr[k] == '.') {
                        pontoEncontrado = true;
                    } else {
                        int digito = numStr[k] - '0';
                        if (!pontoEncontrado) {
                            numVal = numVal * 10.0f + digito;
                        } else {
                            numVal = numVal + (digito / divisorDecimal);
                            divisorDecimal *= 10.0f;
                        }
                    }
                    k++;
                }

                Enfileirar(F, "#", numVal);   
            }
            else {
                if ((expressao[i] == '<' && expressao[i+1] == '=') ||
                    (expressao[i] == '=' && expressao[i+1] == '=') ||
                    (expressao[i] == '!' && expressao[i+1] == '=') || 
                    (expressao[i] == '>' && expressao[i+1] == '=')) {
                    char opStr[3];
                    opStr[0] = expressao[i];
                    opStr[1] = expressao[i+1];
                    opStr[2] = '\0';
                    
                    Enfileirar(F, opStr, 0);
                    i += 2; // Pula os dois caracteres do operador
                }
                else {
                    char opStr[2];
                    opStr[0] = expressao[i];
                    opStr[1] = '\0';
                    
                    Enfileirar(F, opStr, 0);
                    i++;
                }
            }
        }
    }
    
}

void Menu(Fila *F){
    int op;

    do{
        system("cls");
        cout << "\n============= MENU ==============" << endl;
        cout << "[1] - Digite uma expressao " << endl; 
        cout << "[2] - Verificar parenteses " <<  endl;
        cout << "[3] - Calcular"<<endl;
        cout << "[0] - Sair " << endl;
        cout << "\nDigite sua opcao: ";
        cin >> op;

        switch (op)
        {
        case 1: {
            cin.ignore();
            char expressao[100]; 
            system("cls");
            cout << "Digite uma expressao: ";
            cin.getline(expressao, sizeof(expressao));

            ProcessarExpressao(F, expressao); 
            break;
        }
        case 2: {
           if (Fila_Vazia(F)) {
                cout << "Erro: cadastre uma expressao primeiro!\n";
            } else {
                if (Verificar_Caracter(F)) {
                    cout << "Expressao correta!\n";
                } else {
                    cout << "Erro: Parenteses incorretos!\n";
                }
            }
            cout << endl;
            system("pause");
            break;
        }
        case 3:{
            if (Verificar_Caracter(F)) {
                float resultadoFinal = controlador(F);
                cout << "\nResultado da expressao: " << resultadoFinal << endl;
            } else {
                cout << "Erro: Parenteses incorretos!" << endl;
            }
            cout << endl;
            system("pause");
            break;
        }
        

        case 0:
            cout << "Saindo...\n";
            break;
        default:
            cout << "Opcao invalida!\n";
            system("pause");
            break;
        }

    } while (op != 0);
}

Fila F;
Pilha P;

int main(){
    setlocale(LC_ALL, "Portuguese");
    
    Ini_Fila(&F);
    Menu(&F);
        
    return 0;
}
