//Dom_JLRV_Model - Projeto Domino
//17/08/2026
//Joao Pedro Lemos Romano Francisco da Silva
//Leopoldo Ortuzal Zuchieri
//Renato Corral Silva
//Victor Augusto Toledo Lucio Borghi

#ifndef DOM_JLRV_MODEL_H
#define DOM_JLRV_MODEL_H

typedef struct {
    int lado1;
    int lado2;
} peca;

//Representa as duas extremidades abertas da mesa
typedef struct {
    int ladoE;
    int ladoD;
    peca sequencia[28]; //pecas jogadas na mesa, em ordem, da esquerda para a direita
    int qtdPecas;        //quantidade de pecas atualmente na mesa
} mesa;

void geradorDomino(peca conjunto[]);
peca encontrarMaiorCarroca(peca mao1[], peca mao2[]);
void pecaInicial(peca jogador1[], peca jogador2[], peca *resultadoPeca);

#endif
