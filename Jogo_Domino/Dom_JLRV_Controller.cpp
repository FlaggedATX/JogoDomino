#include <stdio.h>
#include <stdlib.h>
#include "Dom_JLRV_Controller.h"
#include "Dom_JLRV_View.h"
#include "Dom_JLRV_Model.h"
#include "Dom_JLRV_View.cpp"

//Menu principal
void menu(peca conjunto[]){
    int rodando = 1;
    geradorDomino(conjunto);
    while(rodando == 1){
        int escolha;
        printMenu();
        if (scanf(" %d", &escolha) != 1){
            printf("\nEntrada encerrada. Saindo do programa...\n");
            break;
        }

        if(escolha == 1){
            menuJogar(conjunto);
        }
        else if(escolha == 2){
            printf("\n[Modo contra o computador ainda nao implementado nesta versao.]\n\n");
        }
        else if(escolha == 3){
            printf("\n[Nao ha jogo interrompido para retomar nesta versao.]\n\n");
        }
        else if(escolha == 4){
            regras();
        }
        else if(escolha == 5){
            printf("\n[Salvar em arquivo ainda nao implementado nesta versao.]\n\n");
        }
        else if(escolha == 6){
            printf("\n[Recuperar jogo salvo ainda nao implementado nesta versao.]\n\n");
        }
        else if(escolha == 0){
            rodando = 0;
        }
        else{
            printf("Opcao invalida. Tente novamente.\n");
        }
    }
}

//Embaralha o conjunto de 28 pecas (Fisher-Yates)
void embaralharConjunto(peca conjunto[]){
    for(int i = 27; i > 0; i--){
        int j = rand() % (i + 1);
        peca temp = conjunto[i];
        conjunto[i] = conjunto[j];
        conjunto[j] = temp;
    }
}

//Distribui as 7 primeiras pecas para o jogador 1 e as 7 seguintes para o jogador 2
void distribuirPecas(peca conjunto[], peca mao1[], peca mao2[]){
    for(int i = 0; i < 7; i++){
        mao1[i] = conjunto[i];
        mao2[i] = conjunto[i + 7];
    }
}

//Atualiza a extremidade da mesa de acordo com o lado jogado e o resultado da validacao
void atualizarMesa(peca pecaJogada, mesa *mesaAtual, char lado, int codigo){
    if (lado == 'E' || lado == 'e'){
        peca orientada = pecaJogada;
        if (codigo == 1){
            mesaAtual->ladoE = pecaJogada.lado2;
            orientada.lado1 = pecaJogada.lado2;
            orientada.lado2 = pecaJogada.lado1;
        }
        else if (codigo == 2){
            mesaAtual->ladoE = pecaJogada.lado1;
            orientada = pecaJogada;
        }

        if (codigo == 1 || codigo == 2){
            // Desloca a sequencia para a direita para abrir espaco na primeira posicao
            for (int i = mesaAtual->qtdPecas; i > 0; i--){
                mesaAtual->sequencia[i] = mesaAtual->sequencia[i - 1];
            }
            mesaAtual->sequencia[0] = orientada;
            mesaAtual->qtdPecas++;
        }
    }
    else if (lado == 'D' || lado == 'd'){
        peca orientada = pecaJogada;
        if (codigo == 3){
            mesaAtual->ladoD = pecaJogada.lado2;
            orientada = pecaJogada;
        }
        else if (codigo == 4){
            mesaAtual->ladoD = pecaJogada.lado1;
            orientada.lado1 = pecaJogada.lado2;
            orientada.lado2 = pecaJogada.lado1;
        }

        if (codigo == 3 || codigo == 4){
            mesaAtual->sequencia[mesaAtual->qtdPecas] = orientada;
            mesaAtual->qtdPecas++;
        }
    }
}

//Verifica se a peca escolhida encaixa no lado escolhido da mesa
int validarJogada(peca pecaValida, mesa mesaJogo, char lado){
    if (lado == 'E' || lado == 'e'){
        if (pecaValida.lado1 == mesaJogo.ladoE){
            return 1;
        }
        else if (pecaValida.lado2 == mesaJogo.ladoE){
            return 2;
        }
    }
    else if (lado == 'D' || lado == 'd'){
        if (pecaValida.lado1 == mesaJogo.ladoD){
            return 3;
        }
        else if (pecaValida.lado2 == mesaJogo.ladoD){
            return 4;
        }
    }
    return -1;
}

//Verifica se existe pelo menos uma peca na mao que encaixe em algum dos lados da mesa
int existeJogadaPossivel(peca mao[], mesa mesaJogo){
    for(int i = 0; i < 21; i++){
        if (mao[i].lado1 == -1 && mao[i].lado2 == -1) continue; // posicao vazia

        if (mao[i].lado1 == mesaJogo.ladoE || mao[i].lado2 == mesaJogo.ladoE) return 1;
        if (mao[i].lado1 == mesaJogo.ladoD || mao[i].lado2 == mesaJogo.ladoD) return 1;
    }
    return 0;
}

//Executa uma tentativa de jogada do jogador da vez.
int jogadaJogador(peca mao[], mesa *mesaAtual){
    char lado = escolherLado();
    int indice = escolherIndicePeca(mao);
    int codigo = validarJogada(mao[indice], *mesaAtual, lado);

    if (codigo == -1){
        jogadaInvalida();
        return 0;
    }

    atualizarMesa(mao[indice], mesaAtual, lado, codigo);
    mao[indice].lado1 = -1;
    mao[indice].lado2 = -1;
    return 1;
}

//Compra uma peca do monte (deposito) e coloca na primeira posicao livre da mao
void comprarPeca(peca mao[], peca monte[], int *topoMonte){
    if (*topoMonte >= 14){
        printf("\nO deposito esta vazio! Nao ha mais pecas para comprar.\n");
        return;
    }

    // Procura ate a posicao 21 por um espaco vazio
    for(int i = 0; i < 21; i++){
        if (mao[i].lado1 == -1 && mao[i].lado2 == -1){
            mao[i] = monte[*topoMonte];
            (*topoMonte)++;
            printf("\nVoce comprou uma peca!\n");
            return;
        }
    }

    printf("\nSua mao ja esta cheia, nao e possivel comprar.\n");
}

//Encontra a peca inicial e a remove da mao (busca em ate 21 posicoes)
int removerPecaInicial(peca mao1[], peca mao2[], peca resultadoPeca){
    for(int i = 0; i < 21; i++){
        if (mao1[i].lado1 == resultadoPeca.lado1 && mao1[i].lado2 == resultadoPeca.lado2){
            mao1[i].lado1 = -1;
            mao1[i].lado2 = -1;
            return 1;
        }
    }
    for(int i = 0; i < 21; i++){
        if (mao2[i].lado1 == resultadoPeca.lado1 && mao2[i].lado2 == resultadoPeca.lado2){
            mao2[i].lado1 = -1;
            mao2[i].lado2 = -1;
            return 2;
        }
    }
    return 1;
}

//Verifica se algum jogador bateu (jogou todas as pecas).
int checarVitoria(peca mao1[], peca mao2[]){
    int cont1 = 0, cont2 = 0;

    for(int i = 0; i < 21; i++){
        if (mao1[i].lado1 == -1 && mao1[i].lado2 == -1) cont1++;
        if (mao2[i].lado1 == -1 && mao2[i].lado2 == -1) cont2++;
    }

    // O jogador vence se todas as 21 posicoes da mao estiverem vazias (-1)
    if (cont1 == 21) return 1;
    else if (cont2 == 21) return 2;
    else return 0;
}

//Funcao responsavel por conduzir uma partida completa
void menuJogar(peca conjunto[]){
    limparTela();
    embaralharConjunto(conjunto);

    // Mãos com capacidade para 21 pecas
    peca mao1[21], mao2[21];
    
    // Inicializa todas as posicoes como vazias
    for(int i = 0; i < 21; i++){
        mao1[i].lado1 = -1; mao1[i].lado2 = -1;
        mao2[i].lado1 = -1; mao2[i].lado2 = -1;
    }
    
    distribuirPecas(conjunto, mao1, mao2);

    peca monte[14];
    for(int i = 0; i < 14; i++){
        monte[i] = conjunto[i + 14];
    }
    int topoMonte = 0;

    peca resultadoPeca;
    pecaInicial(mao1, mao2, &resultadoPeca);
    int jogadorAtual = removerPecaInicial(mao1, mao2, resultadoPeca);

    mesa mesaJogo;
    mesaJogo.ladoE = resultadoPeca.lado1;
    mesaJogo.ladoD = resultadoPeca.lado2;
    mesaJogo.sequencia[0] = resultadoPeca;
    mesaJogo.qtdPecas = 1;

    printf("\nA peca inicial [%d|%d] pertence ao jogador %d, que comeca a partida!\n",
           resultadoPeca.lado1, resultadoPeca.lado2, jogadorAtual);
    printf("Pressione ENTER para continuar...");
    getchar();
    getchar();

    int vitoria = 0;
    while(!vitoria){
        limparTela();
        mostrarMesa(mesaJogo);
        printf("\n>>> Vez do jogador %d <<<\n", jogadorAtual);

        peca *maoAtual = (jogadorAtual == 1) ? mao1 : mao2;
        printarMao(maoAtual);
        submenu();

        char escolha;
        if (scanf(" %c", &escolha) != 1){
            limparTela();
            printf("Entrada encerrada. Voltando ao menu inicial...\n\n");
            return;
        }

        if(escolha == 'J' || escolha == 'j'){
            int jogou = jogadaJogador(maoAtual, &mesaJogo);
            if(jogou){
                jogadorAtual = (jogadorAtual == 1) ? 2 : 1;
            }
        }
        else if(escolha == 'C' || escolha == 'c'){
            comprarPeca(maoAtual, monte, &topoMonte);
        }
        else if(escolha == 'P' || escolha == 'p'){
            if (existeJogadaPossivel(maoAtual, mesaJogo)){
                printf("\nVoce nao pode passar: ainda existe jogada possivel na sua mao.\n");
            }
            else if (topoMonte < 14){
                printf("\nVoce nao pode passar: ainda ha pecas no deposito para comprar.\n");
            }
            else{
                jogadorAtual = (jogadorAtual == 1) ? 2 : 1;
            }
        }
        else if(escolha == 'S' || escolha == 's'){
            limparTela();
            printf("Jogo interrompido. Voltando ao menu inicial...\n\n");
            return;
        }
        else{
            printf("Opcao invalida. Tente novamente.\n");
        }

        vitoria = checarVitoria(mao1, mao2);
    }

    limparTela();
    printf("========================================\n");
    if(vitoria == 1){
        printf("Parabens! O JOGADOR 1 bateu e venceu a partida!\n");
    } else {
        printf("Parabens! O JOGADOR 2 bateu e venceu a partida!\n");
    }
    printf("========================================\n\n");
}
