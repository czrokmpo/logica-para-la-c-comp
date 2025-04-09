/* ===========================================================================
	Universidad Autonoma del Estado de Morelos
	Instituto de Investigacion en Ciencias Basicas y Aplicadas
	Centro de Investigacion en Ciencias
	Licenciatura en Inteligencia Artificial

	Logica para la ciencia computacional

	Conversor a Forma Normal Conjuntiva

    Carlos Octavio Palomares Reyes
	Cesar Mauricio Ocampo Ramirez
	
	Prof. Jorge Hermosillo

	Marzo 2025
  ============================================================================
  */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct{
    char items[MAX];
    int top;
}pila;

int isEmpty(pila *s);
void push(pila *s, char c);
char pop(pila *s);

int esOperador(char c);
int esProposicion(char c);
int isfbf(const char *formula);

int encontrarImp(char *formula, int inicio, int fin);
void IMPL_FREE(char *formula, int inicio, int fin, char *resultado);

void NNF(char *entrada, char *salida);
void CNF(char *entrada, char *salida);
void DISTR(char *izq, char *der, char *salida);

int main(){
    char formula[MAX], resultado[MAX];
    while(1){
        printf("Introduce una formula logica: ");
        scanf("%s", formula);

        if(isfbf(formula)){
            resultado[0]='\0';
            IMPL_FREE(formula, 0, strlen(formula)-1, resultado);
            printf("%s\n", resultado);
        }else{
            printf("La formula NO esta bien formada.\n");
        }
    }
    return 0;
}



int isEmpty(pila *s){
    return s->top==-1;
}

void push(pila *s, char c){
    if(s->top<MAX-1){
        s->items[++(s->top)]=c;
    }
}

char pop(pila *s){
    if(!isEmpty(s)){
        return s->items[(s->top)--];
    }
    return '\0';
}

int esOperador(char c){
    return(c == '*' || c == '+' || c == '-' || c == '~');
}
int esProposicion(char c){
    return(c >= 'p' && c <= 'z');
}

int isfbf(const char *formula) {
    pila p;
    p.top=-1;
    int i, longitud = strlen(formula);
    int esperarProp = 1; // Indica si se espera una proposición o "~" o "("
    int trampa, trampa2;
    trampa = trampa2 = 0;
    

    for (i = 0; i < longitud; i++) {
        char c = formula[i];

        if (c == '(') {
            push(&p, c);
            esperarProp = 1;
            
        } else if (c == ')') {
            if (isEmpty(&p)) return 0;
            pop(&p);
            //esperarProp = 0;
            trampa++;
        } else if (esProposicion(c)) {
            if (!esperarProp) return 0; 
            esperarProp = 0;
            
        } else if (esOperador(c)) {
            if (c == '~') {
                if (!esperarProp) return 0; // "~" solo antes de proposiciones o "("
                trampa2++;
            } else {
                if (esperarProp) return 0; // Operadores binarios necesitan operandos antes y después
                esperarProp = 1;
                trampa2++;
            }
            if (formula[i+1] == ')') return 0; ///
            
        } else {
            return 0; 
        }
    }
    
    if(longitud == 1  && esProposicion(formula[0]) ) return 1;
    if (formula[0] == '~' ) return 0;
    if (formula[longitud-1] != ')' ) return 0; ///
    if (!isEmpty(&p)) return 0;
    if(trampa == trampa2) return 1; else return 0;
    return isEmpty(&p) && !esperarProp;
}

int encontrarOperadorExterno(char *formula, int inicio, int fin) {
    if (formula[inicio] == '(' && formula[fin] == ')') {
        inicio++;
        fin--;
    }
    int nivel = 0;
    for (int i=fin; i>=inicio; i--) {  // derecha a izquierda para asociatividad correcta
        if (formula[i]==')') nivel++;
        else if (formula[i]=='(') nivel--;
        else if (nivel==0 && (formula[i]=='-' || formula[i]=='+' || formula[i] =='*')) {
            return i; //se encontro un operador externo
        }
    }
    return -1;  //sin operador externo
}

void IMPL_FREE(char *formula, int inicio, int fin, char *resultado) {
    int pos_op=encontrarOperadorExterno(formula, inicio, fin);

    if (pos_op==-1) { //no hay operador externo, copiar formula como tal
        strncpy(resultado, formula+inicio, fin-inicio +1);
        resultado[fin-inicio +1]='\0';
        return;
    }

    char operador=formula[pos_op];
    
    //indices subarbol izq
    int izq_ini=inicio+(formula[inicio]=='('? 1:0); //agrega 1 si la formula empieza con ( para no tomarlo en cuenta
    int izq_fin=pos_op -1;

    //indices subarbol der
    int der_ini=pos_op +1;
    int der_fin=fin-(formula[fin]==')'? 1:0);//resta 1 si la formula termina con ) para no tomarlo en cuenta

    char izq[MAX]="", der[MAX]="";

    IMPL_FREE(formula, izq_ini, izq_fin, izq);
    IMPL_FREE(formula, der_ini, der_fin, der);

    if (operador=='-') {
        snprintf(resultado,MAX,"((~%s)+%s)",izq, der);
    } else {
        snprintf(resultado,MAX,"(%s%c%s)",izq,operador,der);
    }
}


void NNF(char *entrada, char *salida){
}

void CNF(char *entrada, char *salida){
}

void DISTR(char *izq, char *der, char *salida){
}