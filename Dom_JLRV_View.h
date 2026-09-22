//Dom_JLRV_View - Projeto Domino
//13/08/2026
//Joao Pedro Lemos Romano Francisco da Silva
//Leopoldo Ortuzal Zuchieri
//Renato Corral Silva
//Victor Augusto Toledo Lucio Borghi
#ifndef DOM_JLRV_VIEW_H
#define DOM_JLRV_VIEW_H
#include "Dom_JLRV_Model.h"

void printMenu();
void printarPecas(peca conjunto[]);
void regras();
void submenu();
void limparTela();
void printarMao(peca mao[]);
void mostrarMesa(mesa mesaAtual);
char escolherLado();
int escolherIndicePeca(peca mao[]);
void jogadaInvalida();
#endif
