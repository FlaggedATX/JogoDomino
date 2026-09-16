//Dom_JLRV_Projeto - Projeto Domino
//13/08/2026
//Joao Pedro Lemos Romano Francisco da Silva
//Leopoldo Ortuzal Zuchieri
//Renato Corral Silva
//Victor Augusto Toledo Lucio Borghi

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Dom_JLRV_Model.cpp"
#include "Dom_JLRV_Controller.cpp"
#include "Dom_JLRV_Model.h"
#include "Dom_JLRV_Controller.h"

int main(){
    srand(time(NULL)); //faz com que a funcao rand() gere numeros diferentes a cada execucao do programa
    peca conjunto[28];
    menu(conjunto);
    return 0;
}
