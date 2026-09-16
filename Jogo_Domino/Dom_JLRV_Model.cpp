#include <stdio.h>
#include "Dom_JLRV_Model.h"

//Gera as 28 pecas do jogo (0-0 ate 6-6)
void geradorDomino(peca conjunto[]){
    int vetor = 0;
    for (int i = 0; i <= 6; i++){
        for (int j = i; j <= 6; j++){
            conjunto[vetor].lado1 = i;
            conjunto[vetor].lado2 = j;
            vetor++;
        }
    }
}

//Funcao que encontra a maior carroca entre as maos dos jogadores
peca encontrarMaiorCarroca(peca mao1[], peca mao2[]){
    peca carroca;
    carroca.lado1 = -1;
    carroca.lado2 = -1;

    for(int valor = 6; valor >= 0; valor--){
        for(int i = 0; i < 21; i++){
            if(mao1[i].lado1 == valor && mao1[i].lado2 == valor){
                carroca = mao1[i];
                return carroca;
            }
        }
        for(int i = 0; i < 21; i++){
            if(mao2[i].lado1 == valor && mao2[i].lado2 == valor){
                carroca = mao2[i];
                return carroca;
            }
        }
    }
    return carroca;
}

//Escolhe a peca que abre o jogo
void pecaInicial(peca jogador1[], peca jogador2[], peca *resultadoPeca)
{
    int maiorValorCarroca = -1;
    peca melhorPeca = {-1, -1};

    int maiorSomaGeral = -1;
    peca maiorPecaGeral = {-1, -1};

    //Procura no Jogador 1 (evitando espaços vazios inicializados com -1)
    for(int n = 0; n < 21; n++)
    {
        if (jogador1[n].lado1 == -1) continue;

        if(jogador1[n].lado1 == jogador1[n].lado2)
        {
            if (jogador1[n].lado1 > maiorValorCarroca)
            {
                maiorValorCarroca = jogador1[n].lado1;
                melhorPeca = jogador1[n];
            }
        }

        int soma1 = jogador1[n].lado1 + jogador1[n].lado2;
        if (soma1 > maiorSomaGeral)
        {
            maiorSomaGeral = soma1;
            maiorPecaGeral = jogador1[n];
        }
    }

    //Procura no Jogador 2 (evitando espaços vazios)
    for(int n = 0; n < 21; n++)
    {
        if (jogador2[n].lado1 == -1) continue;

        if(jogador2[n].lado1 == jogador2[n].lado2)
        {
            if (jogador2[n].lado1 > maiorValorCarroca)
            {
                maiorValorCarroca = jogador2[n].lado1;
                melhorPeca = jogador2[n];
            }
        }

        int soma2 = jogador2[n].lado1 + jogador2[n].lado2;
        if (soma2 > maiorSomaGeral)
        {
            maiorSomaGeral = soma2;
            maiorPecaGeral = jogador2[n];
        }
    }

    if (maiorValorCarroca != -1)
    {
        *resultadoPeca = melhorPeca;
    }
    else
    {
        *resultadoPeca = maiorPecaGeral;
    }
}
