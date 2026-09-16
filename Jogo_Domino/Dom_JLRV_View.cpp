#include "Dom_JLRV_View.h"
#include <stdio.h>
#include <stdlib.h>

void printMenu(){
    printf("========================================\n");
    printf("              JOGO DOMINO\n");
    printf("========================================\n");
    printf("[1] - Iniciar jogo (2 jogadores)\n");
    printf("[2] - Iniciar jogo (contra o computador)\n");
    printf("[3] - Retornar ao jogo interrompido\n");
    printf("[4] - Regras gerais do Jogo\n");
    printf("[5] - Salvar o jogo em arquivo\n");
    printf("[6] - Recuperar o jogo salvo em arquivo\n");
    printf("[0] - Sair do programa\n");
    printf("Opcao selecionada: ");
}

void regras(){
    printf("========================================\n");
    printf("          REGRAS DO DOMINO\n");
    printf("========================================\n\n");
    printf("- 2 jogadores participam da partida.\n");
    printf("- Cada jogador recebe 7 pecas aleatorias.\n\n");
    printf("INICIO DA PARTIDA:\n");
    printf("- Comeca quem tiver a peca 6-6.\n");
    printf("- Se ninguem tiver, comeca quem tiver a maior\n");
    printf("  peca dupla: 5-5, 4-4, 3-3, etc.\n\n");
    printf("JOGADAS:\n");
    printf("- Os jogadores se alternam.\n");
    printf("- O jogador deve colocar uma peca em uma das\n");
    printf("  duas extremidades abertas da mesa.\n");
    printf("- Um dos numeros da peca deve ser igual ao numero\n");
    printf("  da extremidade escolhida.\n\n");
    printf("COMPRA DE PECAS:\n");
    printf("- O jogador pode comprar quantas pecas quiser.\n");
    printf("- Pode comprar para conseguir uma peca que permita\n");
    printf("  jogar ou para blefar.\n");
    printf("- Se nao puder jogar, deve comprar ate conseguir\n");
    printf("  jogar ou ate o deposito ficar vazio.\n\n");
    printf("PASSAR A VEZ:\n");
    printf("- So pode passar se nao tiver nenhuma jogada possivel\n");
    printf("  e o deposito estiver vazio.\n\n");
    printf("FIM DA PARTIDA:\n");
    printf("- Se um jogador colocar sua ultima peca, ele 'bate'\n");
    printf("  e vence a partida.\n");
    printf("- Se o deposito estiver vazio e nenhum jogador puder\n");
    printf("  jogar, a partida fica fechada.\n\n");
    printf("PARTIDA FECHADA:\n");
    printf("- Vence quem tiver a menor quantidade de pecas.\n");
    printf("- Se houver empate, soma-se os pontos das pecas restantes.\n");
    printf("- Vence quem tiver a menor soma de pontos.\n\n");
    printf("EMPATE:\n");
    printf("- Ocorre somente se os dois jogadores tiverem a mesma\n");
    printf("  quantidade de pecas e a mesma soma de pontos.\n\n");
    printf("REGRA GERAL:\n");
    printf("- Vence quem terminar com menos pecas.\n");
    printf("- Em caso de empate na quantidade, vence quem tiver\n");
    printf("  menos pontos.\n");
    printf("========================================\n");
}

void submenu(){
    printf("[J] - Jogar (possiveis n1 ou n2)\n");
    printf("[C] - Comprar (a qualquer momento)\n");
    printf("[P] - Passar (permitido somente em caso especial)\n");
    printf("[S] - Sair (interromper o jogo voltando ao menu inicial)\n");
    printf("Opcao selecionada: ");
}

void mostrarMesa(mesa mesaAtual){
    printf("==================Mesa==================\n");
    for (int i = 0; i < mesaAtual.qtdPecas; i++){
        printf("[%d|%d]", mesaAtual.sequencia[i].lado1, mesaAtual.sequencia[i].lado2);
    }
    printf("\n");
    printf("========================================\n");
}

void printarPecas(peca conjunto[]){
    for (int gerar = 0; gerar < 28; gerar++){
        printf("[%d|%d]", conjunto[gerar].lado1, conjunto[gerar].lado2);
    }
    printf("\n\n");
}

void limparTela(){
    system("clear");
}

//Mostra apenas as pecas que o jogador ainda tem na mao (busca ate 21)
void printarMao(peca mao[]){
    for(int i = 0; i < 21; i++){
        if (!(mao[i].lado1 == -1 && mao[i].lado2 == -1)){
            printf("[%d|%d]", mao[i].lado1, mao[i].lado2);
        }
    }
    printf("\n");
}

int escolherIndicePeca(peca mao[]){
    int indice;

    while (1)
    {
        printf("\n--- SUAS PECAS ---\n");
        for (int i = 0; i < 21; i++){
            if (!(mao[i].lado1 == -1 && mao[i].lado2 == -1)) {
                printf("Indice [%d]: [%d|%d]\n", i, mao[i].lado1, mao[i].lado2);
            }
        }

        printf("Digite o indice da peca que deseja jogar: \n");
        if (scanf(" %d", &indice) != 1){
            indice = 0;
        }

        if (indice < 0 || indice >= 21) {
            printf("Indice invalido! Digite um valor correspondente as suas pecas.\n");
            continue;
        }

        if (mao[indice].lado1 == -1 && mao[indice].lado2 == -1){
            printf("Indice invalido! Essa posicao nao contem uma peca. Escolha outra.\n");
            continue;
        }

        return indice;
    }
}

void jogadaInvalida(){
    printf("Jogada invalida! A peca escolhida nao encaixa nesse lado.\n");
}

char escolherLado(){
    char escolha = 'X';
    do{
        printf("\nEscolha o lado (E - esquerda / D - direita): ");
        if (scanf(" %c", &escolha) != 1){
            return 'E';
        }
        if (escolha != 'E' && escolha != 'D' && escolha != 'e' && escolha != 'd'){
            printf("\nEscolha invalida.\n");
        }
    }while(escolha != 'E' && escolha != 'D' && escolha != 'e' && escolha != 'd');
    return escolha;
}
