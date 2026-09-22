//Dom_JLRV_Controller - Projeto Domino
//17/08/2026
//Joao Pedro Lemos Romano Francisco da Silva
//Leopoldo Ortuzal Zuchieri
//Renato Corral Silva
//Victor Augusto Toledo Lucio Borghi
#ifndef DOM_JLRV_CONTROLLER_H
#define DOM_JLRV_CONTROLLER_H
#include "Dom_JLRV_Model.h"

void embaralharConjunto(peca conjunto[]);
void menu(peca conjunto[]);
void menuJogar(peca conjunto[]);
void distribuirPecas(peca conjunto[], peca mao1[], peca mao2[]);
void atualizarMesa(peca pecaJogada, mesa *mesaAtual, char lado, int codigo);
int validarJogada(peca pecaValida, mesa mesaJogo, char lado);
int existeJogadaPossivel(peca mao[], mesa mesaJogo);
int jogadaJogador(peca mao[], mesa *mesaAtual);
int checarVitoria(peca mao1[], peca mao2[]);
void comprarPeca(peca mao[], peca monte[], int *topoMonte);
int removerPecaInicial(peca mao1[], peca mao2[], peca resultadoPeca);
void gravaCadastro(jogo sitJogo);
int leCadastro(jogo *sitJogo);
void retomarJogo(jogo sitJogo);
void menuCarregarJogo();

int computadorEscolherJogada(peca mao[], mesa mesaJogo, char *ladoEscolhido);
int jogadaComputador(peca mao[], mesa *mesaAtual);
void comprarPecaComputador(peca mao[], peca monte[], int *topoMonte);
void turnoComputador(peca mao[], mesa *mesaAtual, peca monte[], int *topoMonte);
void menuJogarComputador(peca conjunto[]);
void retomarJogoComputador(jogo sitJogo);
#endif
