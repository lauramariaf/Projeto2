#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

using namespace std;


// ESTRUTURAS DE DADOS (STRUCTS)

// Representa cada "nó" ou item da nossa fila/pilha.
struct Elemento {
    char Operador[10]; // Armazena o operador (ex: "+", "*", "(") ou "#" se for número.
    float Numero;      // Armazena o valor numérico (usado quando Operador == "#").
    Elemento *Prox;    // Ponteiro que aponta para o próximo elemento da lista encadeada.
};

// Representa a Fila (Estrutura FIFO: First In, First Out - Primeiro a entrar, primeiro a sair).
struct Fila {
    Elemento *Inicio; // Ponteiro para o primeiro elemento da fila.
    Elemento *Fim;    // Ponteiro para o último elemento da fila.
    int Total;        // Contador de elementos presentes na fila.
};

// Representa a Pilha (Estrutura LIFO: Last In, First Out - Último a entrar, primeiro a sair).
struct Pilha {
    Elemento *Topo;  // Ponteiro para o elemento no topo da pilha.
    int Total;       // Contador de elementos presentes na pilha.
};

// ============================================================================
// INICIALIZAÇÃO E VERIFICAÇÃO DE ESTRUTURAS
// ============================================================================

// Inicializa os ponteiros da Fila como NULL e zera a contagem.
void Ini_Fila(Fila *F) {
    F->Inicio = NULL;
    F->Fim = NULL;
    F->Total = 0;
}

// Inicializa o ponteiro do Topo da Pilha como NULL e zera a contagem.
void Ini_Pilha(Pilha *P) {
    P->Topo = NULL;
    P->Total = 0;
}

// Retorna 'true' se a fila estiver vazia (sem elementos), caso contrário 'false'.
bool Fila_Vazia(Fila *F) {
    return (F->Inicio == NULL);
}

// Retorna 'true' se a pilha estiver vazia, caso contrário 'false'.
bool Pilha_Vazia(Pilha *P) {
    return (P->Topo == NULL);
}

// Imprime o conteúdo completo da Fila na tela (para depuração/visualização).
void Imprimir(Elemento *E) {
    Elemento *Aux;
    
    cout << endl << "============= IMPRESSAO DA FILA =============" << endl;  
    for (Aux = E; Aux != NULL; Aux = Aux->Prox) {
        if (strcmp(Aux->Operador, "#") == 0) { // Se for número, imprime a propriedade Numero.
            cout << Aux->Numero;    
        } else { // Se for operador, imprime a string do Operador.
            cout << Aux->Operador;  
        }
        
        if (Aux->Prox != NULL) {
            cout << " ";
        }
    }
    cout << endl << "===========================================" << endl;
}

// ============================================================================
// ALOCAÇÃO DINÂMICA E MANIPULAÇÃO DA FILA E PILHA
// ============================================================================

// Aloca memória dinamicamente para um novo nó 'Elemento' e preenche seus campos.
Elemento * Criar_Elemento(const char* Op, float Num) {
    Elemento *Ret;

    // Reserva o tamanho necessário na memória RAM para o elemento.
    Ret = (Elemento *) malloc(sizeof(Elemento));    
    if (Ret != NULL) { // Se a alocação foi bem-sucedida
        strcpy(Ret->Operador, Op); // Copia a string do operador
        Ret->Numero = Num;          // Define o número
        Ret->Prox = NULL;           // O próximo ponteiro nasce nulo
    }
    
    return Ret; // Retorna o ponteiro do elemento criado
}

// Adiciona um novo elemento ao final da Fila (Enfileirar / Enqueue).
void Enfileirar(Fila *F, const char* Op, float Num) {
    Elemento *ptE;
    
    ptE = Criar_Elemento(Op, Num);
    if (ptE != NULL) {
        if (Fila_Vazia(F)) {
            F->Inicio = ptE; // Se estava vazia, o novo elemento é o início.
        } else {
            F->Fim->Prox = ptE; // Conecta o antigo fim ao novo elemento.
        }
        
        F->Fim = ptE; // Atualiza o ponteiro de fim para o novo nó.
        ++F->Total;   // Incrementa a quantidade total.
    }   
}

// Remove e retorna o elemento do início da Fila (Desenfileirar / Dequeue).
Elemento * Desenfileirar(Fila *F) {
    Elemento *Ret = NULL;
    
    if (!Fila_Vazia(F)) {
        Ret = F->Inicio; // Pega o primeiro elemento.
        if (F->Inicio == F->Fim) { // Se só havia 1 elemento na fila:
            F->Inicio = NULL;
            F->Fim = NULL;
        } else {
            F->Inicio = F->Inicio->Prox; // O início passa a ser o segundo elemento.
        }
        --F->Total; // Decrementa a quantidade total.
    }
    
    return Ret; // Retorna o elemento removido (quem chamar deve dar free).
}

// Adiciona um novo elemento no topo da Pilha (Empilhar / Push).
void Empilhar(Pilha *P, const char* Op, float Num) {
    Elemento *ptE;
    
    ptE = Criar_Elemento(Op, Num);
    if (ptE != NULL) {
        if (!Pilha_Vazia(P)) {      
            ptE->Prox = P->Topo; // O novo nó aponta para o antigo topo.
        }
                
        P->Topo = ptE; // O topo da pilha passa a ser o novo nó.
        ++P->Total;
    }   
}

// Remove e retorna o elemento do topo da Pilha (Desempilhar / Pop).
Elemento * Desempilhar(Pilha *P) {
    Elemento *Ret = NULL;
    
    if (!Pilha_Vazia(P)) {
        Ret = P->Topo; // Pega o elemento do topo.
        P->Topo = P->Topo->Prox; // O topo passa a ser o elemento de baixo.
        --P->Total;         
    }
    
    return Ret; // Retorna o nó removido.
}

// ============================================================================
// LÓGICA DE AVALIAÇÃO DE EXPRESSÕES E PRECEDÊNCIA DE OPERADORES
// ============================================================================

// Retorna a prioridade (peso) de cada operador matemático ou relacional.
int ObterPeso(const char* operador) {
    int peso = -1;
    if (strcmp(operador, "!") == 0) {
        peso = 4; // Maior prioridade (Operador unário de negação)
    }
    if (strcmp(operador, "*") == 0 || strcmp(operador, "/") == 0) {
        peso = 3; // Multiplicação e Divisão
    }
    if (strcmp(operador, "+") == 0 || strcmp(operador, "-") == 0) {
        peso = 2; // Soma e Subtração
    }
    if (strcmp(operador, ">") == 0 || strcmp(operador, "<") == 0 || 
        strcmp(operador, "<=") == 0 || strcmp(operador, "==") == 0 || 
        strcmp(operador, "!=") == 0) {
        peso = 1; // Operadores relacionais/comparação
    }
    if (peso == -1) {
        cout << "operador invalido" << endl;
    }
    return peso;
}

// Executa o cálculo aritmético/lógico entre dois números com base no operador.
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
            cout << "Erro: Divisão por zero!\n";
            resultado = 0.0f;
        }
    }
    // Operadores relacionais retornam 1.0f para verdadeiro e 0.0f para falso:
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

// Pega os dois últimos números e o último operador das pilhas, calcula e empilha o resultado.
void atualiza_pilha(Pilha *Pnum, Pilha *Pop) {
    float Num1, Num2, Resultado;
    char op[3] = "";
    
    if (!Pilha_Vazia(Pnum) && !Pilha_Vazia(Pop)) {
        // Desempilha o segundo número (o último a ser inserido)
        Num2 = Pnum->Topo->Numero;
        Desempilhar(Pnum);
        
        // Desempilha o primeiro número
        Num1 = Pnum->Topo->Numero;
        Desempilhar(Pnum);
        
        // Desempilha o operador a ser aplicado
        strcpy(op, Pop->Topo->Operador);
        Desempilhar(Pop);
        
        // Realiza o cálculo
        Resultado = Calcular(Num1, Num2, op);
        
        // Re-empilha o resultado obtido na pilha de números
        Empilhar(Pnum, "#", Resultado); 
    }
}

// Algoritmo principal que consome a Fila de tokens e calcula a expressão usando 2 pilhas.
float controlador (Fila *F) {
    Pilha Pnum, Pop; // Pilha de Números e Pilha de Operadores
    Ini_Pilha(&Pnum);
    Ini_Pilha(&Pop);
    float resultado = 0.0f;

    // Processa item por item da fila de entrada
    while (!Fila_Vazia(F)) {
        Elemento *atual = Desenfileirar(F);

        // Se o elemento for um número, coloca direto na pilha de números
        int pesoAtual = ObterPeso(atual->Operador);
        if (pesoAtual == -1 && strcmp(atual->Operador, "#") != 0) {
            cout << "Erro: Operador desconhecido encontrado!" << endl;
            return 0.0f; 
        }    
    // Aqui você pode limpar as pilhas e retornar um valor padrão ou sair da função com segurança
    

        if (strcmp(atual->Operador, "#") == 0) {
            Empilhar(&Pnum, "#", atual->Numero);
        }
        else {
            // Se for parêntese abrindo, empilha na pilha de operadores
            if (strcmp(atual->Operador, "(") == 0) {
                Empilhar(&Pop, "(", 0);
            }
            else {
                // Se for parêntese fechando, resolve tudo dentro até achar o '('
                if (strcmp(atual->Operador, ")") == 0) {
                    while (!Pilha_Vazia(&Pop) && strcmp(Pop.Topo->Operador, "(") != 0) {
                        atualiza_pilha(&Pnum, &Pop);
                    }
                    if (!Pilha_Vazia(&Pop)) { // Descarta o parêntese '(' que sobrou no topo
                        Elemento *aux = Desempilhar(&Pop);
                        free(aux);
                    }
                }
                else {
                    // Operador comum: desempilha e calcula enquanto houver operadores no topo
                    // com maior ou igual precedência que o operador atual
                    while (!Pilha_Vazia(&Pop) && strcmp(Pop.Topo->Operador, "(") != 0 &&
                        ObterPeso(Pop.Topo->Operador) >= ObterPeso(atual->Operador)) {
                        atualiza_pilha(&Pnum, &Pop);
                    }
                    Empilhar(&Pop, atual->Operador, 0); // Empilha o operador atual
                }
            }
        }
    }

    // Resolve as operações restantes que ficaram pendentes na pilha
    while (!Pilha_Vazia(&Pop)) {
        atualiza_pilha(&Pnum, &Pop);
    }

    // O último valor restante na pilha de números é o resultado final da expressão
    if (!Pilha_Vazia(&Pnum)) {
        resultado = Pnum.Topo->Numero;
        Elemento *Removido = Desempilhar(&Pnum);
        free(Removido);
    }

    return resultado;
}

// Valida se o balanceamento de parênteses da expressão está correto
bool Verificar_Caracter(Fila *F) {
    Pilha P;
    Elemento *Aux;
    Elemento *Removido;
    bool Erro = false;

    Ini_Pilha(&P);
    Aux = F->Inicio;

    // Percorre a fila procurando por parênteses
    while (Aux != NULL && !Erro) {
        if (strcmp(Aux->Operador, "(") == 0) {
            Empilhar(&P, "(", 0); // Empilha a abertura
        }
        else {
            if (strcmp(Aux->Operador, ")") == 0) {
                if (Pilha_Vazia(&P)) { // Fechou sem ter aberto: Erro!
                    Erro = true;
                }
                else {
                    Removido = Desempilhar(&P); // Parêntese casou, remove da pilha
                    free(Removido);
                }
            }
        }
        Aux = Aux->Prox;
    }

    // Se sobrou algum '(' sem fechar, a pilha não estará vazia
    if (!Pilha_Vazia(&P)) {
        Erro = true;
    }

    return !Erro; // Retorna true se estiver tudo OK, false se houver erro
}

// ============================================================================
// PARSER / ENTRADA DE DADOS E INTERFACE DO USUÁRIO
// ============================================================================

// Lê o texto digitado pelo usuário, separa em tokens (números e operadores) e enfileira.
void ProcessarExpressao(Fila *F) {
    char expressao[100]; 
    system("cls"); // Limpa a tela do console (Windows)
    cout << "Digite uma expressão: ";
    cin.getline(expressao, sizeof(expressao));
    
    Ini_Fila(F); 

    int i = 0;
    while (expressao[i] != '\0') { // Percorre a string até o caractere nulo final
        // Ignora espaços em branco e tabulações
        if (expressao[i] == ' ' || expressao[i] == '\t') {
            i++;
        }
        else {
            // Se for início de um número (dígito 0-9 ou ponto decimal)
            if ((expressao[i] >= '0' && expressao[i] <= '9') || expressao[i] == '.') {
                char numStr[50];
                int j = 0;
                
                // Extrai todos os caracteres que compõem o número completo
                while (expressao[i] != '\0' && ((expressao[i] >= '0' && expressao[i] <= '9') || expressao[i] == '.')) {
                    numStr[j++] = expressao[i++];
                }
                numStr[j] = '\0';
                
                // Converte a string extraída (numStr) para float manualmente
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
                            numVal = numVal * 10.0f + digito; // Parte inteira
                        } else {
                            numVal = numVal + (digito / divisorDecimal); // Parte decimal
                            divisorDecimal *= 10.0f;
                        }
                    }
                    k++;
                }

                Enfileirar(F, "#", numVal); // Adiciona o número à fila com marcador "#"
            }
            else {
                // Se for um operador relacional composto por 2 caracteres (<=, ==, !=)
                if ((expressao[i] == '<' && expressao[i+1] == '=') ||
                    (expressao[i] == '=' && expressao[i+1] == '=') ||
                    (expressao[i] == '!' && expressao[i+1] == '=')) {
                    char opStr[3];
                    opStr[0] = expressao[i];
                    opStr[1] = expressao[i+1];
                    opStr[2] = '\0';
                    
                    Enfileirar(F, opStr, 0);
                    i += 2; // Pula os dois caracteres processados
                }
                else {
                    // Se for um operador simples de 1 caractere (+, -, *, /, (, ), >, <)
                    char opStr[2];
                    opStr[0] = expressao[i];
                    opStr[1] = '\0';
                    
                    Enfileirar(F, opStr, 0);
                    i++; // Avança para o próximo caractere
                }
            }
        }
    }
    
    // Valida a sintaxe de parênteses antes de calcular
    if (Verificar_Caracter(F)) {
        float resultadoFinal = controlador(F);
        cout << "\nResultado da expressão: " << resultadoFinal << endl;
    } else {
        cout << "Erro: Parênteses incorretos!" << endl;
    }

    cout << endl;
    system("pause"); // Pausa a tela para leitura no Windows
}

// Menu interativo simples do programa
void Menu(Fila *F) {
    int op;

    do {
        system("cls");
        cout << "\n============= MENU ==============" << endl;
        cout << "[1] - Digite uma expressão " << endl; 
        cout << "[0] - Sair " << endl;
        cout << "\nDigite sua opcao: ";
        cin >> op;

        switch (op) {
        case 1: {
            cin.ignore(); // Limpa o caractere '\n' que sobrou no buffer do cin
            ProcessarExpressao(F); 
            break;
        }
        case 0:
            cout << "Saindo...\n";
            break;
        default:
            cout << "Opção inválida!\n";
            system("pause");
            break;
        }

    } while (op != 0);
}

// Variáveis Globais de Fila e Pilha
Fila F;
Pilha P;

// Função principal e ponto de entrada da aplicação
int main() {
    setlocale(LC_ALL, "Portuguese"); // Configura codificação para caracteres acentuados
    
    Ini_Fila(&F); // Inicializa a fila global
    Menu(&F);     // Inicia a execução pelo menu
        
    return 0; // Finaliza o programa indicando sucesso (código 0)
}