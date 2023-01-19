#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

typedef struct pilha{
    int inicio;
    int fim;
    char *elementos;
    int capacidade;
}Pilha;

typedef struct fila{
    int inicio;
    int fim;
    float *elementos;
}Fila;

int fatorial(int n){
    if(n == 1){
        return 1;
    }else if(n > 1){
        return n * fatorial(n - 1);
    }else{
        return;
    }
}

int conversor_binario(int n){
    if(n <= 1){
        return n;
    }else{
        return 10 * (conversor_binario(n/2)) + (n % 2);
    }
}

int criar_precedencia(char operando){
    if(operando == '+' || operando == '-'){
        return 1;
    }else if(operando == '/' || operando == '*' || operando == 'm'){
        return 2;
    }else if(operando == '^' || operando == 'l' || operando == '!'){
        return 3;
    }else if(operando == '(' || operando == ')'){
        return 4;
    }else{
        return -1; //operador inválido
    }
}

float calcula(float a, float b, char op){
    if(op == '+'){
        return a + b;
    }else if(op == '-'){
        return a - b;   
    }else if(op == '/'){
        if(b > 0)
            return a / b;
        else{
            printf("Operacao Invalida");
            exit(EXIT_FAILURE);
        }       
    }else if(op == '*'){
        return a * b;   
    }else if(op == '^'){
        return pow(a, b);   
    }else if(op == 'm'){
        return (int) a % (int) b;   
    }else if(op == 'l'){
        if(a <= 0){
            printf("Operacao invalida");
            exit(EXIT_FAILURE);
        }
        return log10(a);  
    }else if(op == '!'){
        return fatorial(a);
    }else if(op == 'b'){
        return conversor_binario(a);
    }else{
        printf("Operacao invalida");
        exit(EXIT_FAILURE); // operando inválido
    }
}

bool pilha_vazia(Pilha *p){
    return p->fim == 0;
}

bool pop(Pilha *p){
    if(pilha_vazia(p)){
        return false;
    }else{
        p->fim--;
        return true;
    }
}

Fila *fila_criar(){
    Fila *nova_fila = malloc(sizeof(Fila));

    nova_fila->elementos = malloc(100 * sizeof(float));
    nova_fila->inicio = 0;
    nova_fila->fim = 0;

    return nova_fila;
}

void fila_libera(Fila *f){
    free(f);
}

bool fila_vazia(Fila *f){
    return f->fim == 0;
}

void fila_enfileirar(Fila *f, float a){
    f->elementos[f->fim] = a;
    f->fim++;
}

bool fila_desenfileirar(Fila *f, Pilha *p){
    char aux = p->elementos[p->fim - 1];
    float num;

    if(fila_vazia(f)){
        return false;
    }

    if(aux == 'l' || aux == '!' || aux == 'b'){
        num = calcula(f->elementos[f->fim - 1], 10, aux);
        f->fim--;
        fila_enfileirar(f, num);
        pop(p);
    }else if(aux == '+' || aux == '-' || aux == '*' || 
    aux == '/' || aux == '^' || aux == 'l' || aux == 'm' || aux == '(' || aux == ')'){
        num = calcula(f->elementos[f->fim - 2], f->elementos[f->fim - 1], aux);
        f->fim -= 2;
        fila_enfileirar(f, num);
        pop(p);
    }

    return true;
}

Pilha *pilha_criar(){
    Pilha *nova_pilha = malloc(sizeof(Pilha));

    nova_pilha->elementos = malloc(100 * sizeof(char));
    nova_pilha->inicio = 0;
    nova_pilha->fim = 0;

    return nova_pilha;
}

void push(Pilha *p, char c, Fila *f){
    if(pilha_vazia(p)){
        p->elementos[p->fim] = c;
        p->fim++;
    }else{
        int aux = criar_precedencia(c);

        if(p->elementos[p->fim - 1] == '('){
            p->elementos[p->fim] = c;
            p->fim++;
        }else if(criar_precedencia(p->elementos[p->fim - 1]) < aux){
            p->elementos[p->fim] = c;
            p->fim++;
        }else if(criar_precedencia(p->elementos[p->fim - 1]) >= aux && c != '('){
            int i = p->fim - 1;
            float num;
            while(criar_precedencia(p->elementos[i]) >= aux){
                if(p->elementos[i] == 'l' || p->elementos[i] == '!'|| p->elementos[i] == 'b'){
                    num = calcula(f->elementos[f->fim - 1], 10, p->elementos[i]);
                    f->fim--;
                    fila_enfileirar(f, num);
                    pop(p);
                }else{
                    num = calcula(f->elementos[f->fim - 2], f->elementos[f->fim - 1], p->elementos[i]);
                    f->fim -= 2;
                    fila_enfileirar(f, num);
                    pop(p);
                }
                i--;
            }
            p->elementos[p->fim] = c;
            p->fim++;
        }else if(c == ')'){
            int i = p->fim - 1;
            float num;
            while(p->elementos[i] != '('){
                if(p->elementos[i] == 'l' || p->elementos[i] == '!'|| p->elementos[i] == 'b'){
                    num = calcula(f->elementos[f->fim - 1], 10, p->elementos[i]);
                    f->fim--;
                    fila_enfileirar(f, num);
                    pop(p);
                }else{
                    num = calcula(f->elementos[f->fim - 2], f->elementos[f->fim - 1], p->elementos[i]);
                    f->fim -= 2;
                    fila_enfileirar(f, num);
                    pop(p);
                }
                i--;
            }
            pop(p);
        }
    }
}//Diferente do push convencional, deve-se aplicar um algoritmo para saber se o operando vai entrar ou não, e essa função efetivamente faz isso.

void pilha_libera(Pilha *p){
    free(p);
}

bool conta_paranteses(char str[], int tam){
    int esq = 0, dir = 0;

    for(int i = 0; i < tam; i++){
        if(str[i] == '('){
            esq++;
        }

        if(str[i] == ')'){
            dir++;
        }
    }

    if(esq == dir){
        return true;
    }else{
        return false;
    }
}

void resolve_expressao(Fila *f, Pilha *p){
    char str[100];
    char *pt;
    scanf("%[^\n]", str);
    int flag = conta_paranteses(str, strlen(str));
    
    if(flag == false){
        printf("Operacao invalida");
        exit(EXIT_FAILURE);
    }

    pt = strtok(str, " ()");
    char operando;
    float numero;

    while(pt){
        if(pt[0] == '+' || pt[0] == '-' || pt[0] == '*' || pt[0] == '/' || pt[0] == 'l' || pt[0] == 'm' || pt[0] == '^' || pt[0] == ')' || pt[0] == '(' || pt[0] == '!' || pt[0] == 'b'){
            operando = pt[0];
            push(p, operando, f);
        }else if(pt[0] >= 48 && pt[0] <= 57){
            numero = strtol(pt, NULL, 10);
            fila_enfileirar(f, numero);
        }else{
            printf("Operacao invalida\n");
            exit(EXIT_FAILURE);
        }
        pt = strtok(NULL, " ");
    }
    
    while(f->fim != 1){
        fila_desenfileirar(f, p);
    }

    if(p->elementos[p->fim - 1] == 'l' || p->elementos[p->fim - 1] == '!' || p->elementos[p->fim - 1] == 'b'){  // condição para caso o log/bin/fat sejam 
        fila_desenfileirar(f, p);     // o primeiro elemento digitado
    }
} //Essa função serve para receber a operação matemática e transformar ela na notação polonesa, que é a maneira que o algoritmo funciona

int main(){

    printf("Seja Bem vindo(a) a calculadora utilizando o algoritmo Shunting Yard.\nAutoria: Eliton Pereira e Guilherme Araujo.\n\nDigite a sua expressao: (Lembrando de deixar espaco entre numeros e operandos, mas nao deixer entre parenteses e numeros):\n");

    printf("Ex: 4 + 2 esta certo, mas 4+2 nao (o resultado sai incorreto)\nEx 2: (4 + 2) - 1 esta certo, mas ( 4 + 2 ) - 1 nao\n\n");

    Pilha *p = pilha_criar();
    Fila *f =  fila_criar();
    
    resolve_expressao(f, p);

    printf("\nResultado: %f", f->elementos[f->fim - 1]);
    fila_libera(f);
    pilha_libera(p);

    return 0;
}

