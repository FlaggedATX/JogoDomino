//Dom_JLRV_Controller - Projeto Domino
//17/08/2026
//Joao Pedro Lemos Romano Francisco da Silva
//Leopoldo Ortuzal Zuchieri
//Renato Corral Silva
//Victor Augusto Toledo Lucio Borghi

#include <stdio.h>
#include <stdlib.h>
#include "Dom_JLRV_View.cpp"
#include "Dom_JLRV_Controller.h"


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
            menuJogarComputador(conjunto);
        }

        else if(escolha == 3){
            regras();
        }
        else if(escolha == 4){
            menuCarregarJogo();
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
    int indice = escolherIndicePeca(mao);
    int ladosDisponiveis = verificarLadosDisponiveis(mao[indice], *mesaAtual);

    char lado;
    if (ladosDisponiveis == 0){
        jogadaInvalida();
        return 0;
    }
    else if (ladosDisponiveis == 1){
        lado = 'E'; // so encaixa na esquerda, joga direto
    }
    else if (ladosDisponiveis == 2){
        lado = 'D'; // so encaixa na direita, joga direto
    }
    else{
        lado = escolherLado(); // encaixa nos dois, ai sim pergunta
    }

    int codigo = validarJogada(mao[indice], *mesaAtual, lado);

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
            jogo jogoAtual;
            jogoAtual.jogador = jogadorAtual;
            for(int i = 0; i < 21; i++){
    			jogoAtual.mao1[i] = mao1[i];
    			jogoAtual.mao2[i] = mao2[i];
			}
			for(int i = 0; i < 14; i++){
    			jogoAtual.monte[i] = monte[i];
			}
            jogoAtual.mesaJogo = mesaJogo;
            jogoAtual.topoMonte = topoMonte;
            jogoAtual.modoJogo = 1;
            int escolha_save = 2;
            while((escolha_save != 1) && (escolha_save != 0)){
            
				printf("Deseja Salvar?(1:Sim 0:Nao)\n");
            	scanf("%d", &escolha_save);
            	if((escolha_save != 1) && (escolha_save != 0)){
            		printf("Escolha invalida\n");
				}
			}
			if(escolha_save == 1){
				gravaCadastro(jogoAtual);
			}
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

//Grava o estado atual do jogo em um arquivo binario
void gravaCadastro(jogo sitJogo){
    FILE *fptr;

    fptr = fopen("Dom_JLRV_Save.dat", "wb");

    if (fptr == NULL){
        printf("\nErro ao abrir o arquivo para gravacao!\n");
        return;
    }
					//posicao na memoria do sitJogo, bytes de um Jogo, quantos elementos quero gravar, qual arquivo gravar
    size_t gravados = fwrite(&sitJogo, sizeof(jogo), 1, fptr);

    if (gravados != 1){
        printf("\nErro ao gravar os dados no arquivo!\n");
    }
    else{
        printf("\nJogo salvo com sucesso!\n");
    }
    fclose(fptr);
}

//Le o estado de um jogo salvo a partir do arquivo binario
//Retorna 1 se conseguiu carregar, 0 se nao havia jogo salvo ou deu erro
int leCadastro(jogo *sitJogo){
    FILE *fptr;

    fptr = fopen("Dom_JLRV_Save.dat", "rb");

    if (fptr == NULL){
        printf("\nNao ha jogo salvo (arquivo nao encontrado).\n");
        return 0;
    }

    size_t lidos = fread(sitJogo, sizeof(jogo), 1, fptr);

    fclose(fptr);

    if (lidos != 1){
        printf("\nErro ao ler os dados do arquivo. Arquivo pode estar corrompido.\n");
        return 0;
    }

    printf("\nJogo carregado com sucesso!\n");
    return 1;
}

//Retoma uma partida a partir de um estado de jogo previamente salvo em arquivo
void retomarJogo(jogo sitJogo){
    peca mao1[21], mao2[21];
    for(int i = 0; i < 21; i++){
        mao1[i] = sitJogo.mao1[i];
        mao2[i] = sitJogo.mao2[i];
    }

    peca monte[14];
    for(int i = 0; i < 14; i++){
        monte[i] = sitJogo.monte[i];
    }
    int topoMonte = sitJogo.topoMonte;

    mesa mesaJogo = sitJogo.mesaJogo;
    int jogadorAtual = sitJogo.jogador;

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
            jogo jogoAtual;
            jogoAtual.jogador = jogadorAtual;
            for(int i = 0; i < 21; i++){
                jogoAtual.mao1[i] = mao1[i];
                jogoAtual.mao2[i] = mao2[i];
            }
            for(int i = 0; i < 14; i++){
                jogoAtual.monte[i] = monte[i];
            }
            jogoAtual.mesaJogo = mesaJogo;
            jogoAtual.topoMonte = topoMonte;
			jogoAtual.modoJogo = 2;
            int escolha_save = 2;
            while((escolha_save != 1) && (escolha_save != 0)){
                printf("Deseja Salvar?(1:Sim 0:Nao)\n");
                scanf("%d", &escolha_save);
                if((escolha_save != 1) && (escolha_save != 0)){
                    printf("Escolha invalida\n");
                }
            }
            if(escolha_save == 1){
                gravaCadastro(jogoAtual);
            }
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
//Carrega um jogo salvo em arquivo e, se existir, retoma a partida
void menuCarregarJogo(){
    jogo jogoCarregado;
    int ok = leCadastro(&jogoCarregado);

    if (ok){
        if (jogoCarregado.modoJogo == 2){
            retomarJogoComputador(jogoCarregado);
        }
        else{
            retomarJogo(jogoCarregado);
        }
	}
}

//Jogadas do Computador

int computadorEscolherJogada(peca mao[], mesa mesaJogo, char *ladoEscolhido){
    for(int i = 0; i < 21; i++){
        if (mao[i].lado1 == -1 && mao[i].lado2 == -1) continue; // posicao vazia

        if (mao[i].lado1 == mesaJogo.ladoE || mao[i].lado2 == mesaJogo.ladoE){
            *ladoEscolhido = 'E';
            return i;
        }
        if (mao[i].lado1 == mesaJogo.ladoD || mao[i].lado2 == mesaJogo.ladoD){
            *ladoEscolhido = 'D';
            return i;
        }
    }
    return -1; // nenhuma peca da mao encaixa
}

//Usa computadorEscolherJogada para decidir a peca/lado, valida com validarJogada
//(reaproveitada do jogo humano) e atualiza a mesa. Retorna 1 se jogou, 0 caso contrario.
int jogadaComputador(peca mao[], mesa *mesaAtual){
    char lado;
    int indice = computadorEscolherJogada(mao, *mesaAtual, &lado);

    if (indice == -1){
        return 0;
    }

    int codigo = validarJogada(mao[indice], *mesaAtual, lado);

    atualizarMesa(mao[indice], mesaAtual, lado, codigo);

    printf("\nO computador jogou a peca [%d|%d] no lado %c.\n",
           mao[indice].lado1, mao[indice].lado2, lado);

    mao[indice].lado1 = -1;
    mao[indice].lado2 = -1;
    return 1;
}

//Mesma logica de comprarPeca, mas com mensagens voltadas ao computador
void comprarPecaComputador(peca mao[], peca monte[], int *topoMonte){
    if (*topoMonte >= 14){
        printf("\nO deposito esta vazio! O computador nao pode comprar.\n");
        return;
    }

    for(int i = 0; i < 21; i++){
        if (mao[i].lado1 == -1 && mao[i].lado2 == -1){
            mao[i] = monte[*topoMonte];
            (*topoMonte)++;
            printf("\nO computador comprou uma peca.\n");
            return;
        }
    }
}

//Conduz o turno automatico do computador: compra enquanto nao houver jogada possivel
//e ainda houver pecas no deposito; se conseguir jogada, joga; senao, passa a vez.
void turnoComputador(peca mao[], mesa *mesaAtual, peca monte[], int *topoMonte){
    printf("\n>>> Vez do computador <<<\n");

    while (!existeJogadaPossivel(mao, *mesaAtual) && *topoMonte < 14){
        comprarPecaComputador(mao, monte, topoMonte);
    }

    if (existeJogadaPossivel(mao, *mesaAtual)){
        jogadaComputador(mao, mesaAtual);
    }
    else{
        printf("\nO computador nao tem jogada possivel e o deposito esta vazio. Passando a vez.\n");
    }

    printf("Pressione ENTER para continuar...");
    getchar();
    getchar();
}

//Equivalente a menuJogar(), mas o Jogador 2 e controlado automaticamente
//pelas funcoes computador acima. Jogador 1 continua usando o submenu normal.
void menuJogarComputador(peca conjunto[]){
    limparTela();
    embaralharConjunto(conjunto);

    peca mao1[21], mao2[21];
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

    if (jogadorAtual == 1){
        printf("\nA peca inicial [%d|%d] e SUA. Voce comeca a partida!\n",
               resultadoPeca.lado1, resultadoPeca.lado2);
    } else {
        printf("\nA peca inicial [%d|%d] e do COMPUTADOR. Ele comeca a partida!\n",
               resultadoPeca.lado1, resultadoPeca.lado2);
    }
    printf("Pressione ENTER para continuar...");
    getchar();

    int vitoria = 0;
    while(!vitoria){
        if (jogadorAtual == 2){
            limparTela();
            mostrarMesa(mesaJogo);
            turnoComputador(mao2, &mesaJogo, monte, &topoMonte);
            jogadorAtual = 1;
        }
        else{
            limparTela();
            mostrarMesa(mesaJogo);
            printf("\n>>> Sua vez (Jogador 1) <<<\n");
            printarMao(mao1);
            submenu();

            char escolha;
            if (scanf(" %c", &escolha) != 1){
                limparTela();
                printf("Entrada encerrada. Voltando ao menu inicial...\n\n");
                return;
            }

            if(escolha == 'J' || escolha == 'j'){
                int jogou = jogadaJogador(mao1, &mesaJogo);
                if(jogou) jogadorAtual = 2;
            }
            else if(escolha == 'C' || escolha == 'c'){
                comprarPeca(mao1, monte, &topoMonte);
            }
            else if(escolha == 'P' || escolha == 'p'){
                if (existeJogadaPossivel(mao1, mesaJogo)){
                    printf("\nVoce nao pode passar: ainda existe jogada possivel na sua mao.\n");
                }
                else if (topoMonte < 14){
                    printf("\nVoce nao pode passar: ainda ha pecas no deposito para comprar.\n");
                }
                else{
                    jogadorAtual = 2;
                }
            }
            else if(escolha == 'S' || escolha == 's'){
                limparTela();
                jogo jogoAtual;
                jogoAtual.jogador = jogadorAtual;
                jogoAtual.modoJogo = 2; // partida contra o computador
                for(int i = 0; i < 21; i++){
                    jogoAtual.mao1[i] = mao1[i];
                    jogoAtual.mao2[i] = mao2[i];
                }
                for(int i = 0; i < 14; i++){
                    jogoAtual.monte[i] = monte[i];
                }
                jogoAtual.mesaJogo = mesaJogo;
                jogoAtual.topoMonte = topoMonte;

                int escolha_save = 2;
                while((escolha_save != 1) && (escolha_save != 0)){
                    printf("Deseja Salvar?(1:Sim 0:Nao)\n");
                    scanf("%d", &escolha_save);
                    if((escolha_save != 1) && (escolha_save != 0)){
                        printf("Escolha invalida\n");
                    }
                }
                if(escolha_save == 1){
                    gravaCadastro(jogoAtual);
                }
                printf("Jogo interrompido. Voltando ao menu inicial...\n\n");
                return;
            }
            else{
                printf("Opcao invalida. Tente novamente.\n");
            }
        }

        vitoria = checarVitoria(mao1, mao2);
    }

    limparTela();
    printf("========================================\n");
    if(vitoria == 1){
        printf("Parabens! VOCE venceu a partida contra o computador!\n");
    } else {
        printf("O COMPUTADOR venceu a partida. Tente novamente!\n");
    }
    printf("========================================\n\n");
}

//Equivalente a retomarJogo(), mas para partidas salvas no modo "contra o computador".
void retomarJogoComputador(jogo sitJogo){
    peca mao1[21], mao2[21];
    for(int i = 0; i < 21; i++){
        mao1[i] = sitJogo.mao1[i];
        mao2[i] = sitJogo.mao2[i];
    }

    peca monte[14];
    for(int i = 0; i < 14; i++){
        monte[i] = sitJogo.monte[i];
    }
    int topoMonte = sitJogo.topoMonte;

    mesa mesaJogo = sitJogo.mesaJogo;
    int jogadorAtual = sitJogo.jogador;

    int vitoria = 0;
    while(!vitoria){
        if (jogadorAtual == 2){
            limparTela();
            mostrarMesa(mesaJogo);
            turnoComputador(mao2, &mesaJogo, monte, &topoMonte);
            jogadorAtual = 1;
        }
        else{
            limparTela();
            mostrarMesa(mesaJogo);
            printf("\n>>> Sua vez (Jogador 1) <<<\n");
            printarMao(mao1);
            submenu();

            char escolha;
            if (scanf(" %c", &escolha) != 1){
                limparTela();
                printf("Entrada encerrada. Voltando ao menu inicial...\n\n");
                return;
            }

            if(escolha == 'J' || escolha == 'j'){
                int jogou = jogadaJogador(mao1, &mesaJogo);
                if(jogou) jogadorAtual = 2;
            }
            else if(escolha == 'C' || escolha == 'c'){
                comprarPeca(mao1, monte, &topoMonte);
            }
            else if(escolha == 'P' || escolha == 'p'){
                if (existeJogadaPossivel(mao1, mesaJogo)){
                    printf("\nVoce nao pode passar: ainda existe jogada possivel na sua mao.\n");
                }
                else if (topoMonte < 14){
                    printf("\nVoce nao pode passar: ainda ha pecas no deposito para comprar.\n");
                }
                else{
                    jogadorAtual = 2;
                }
            }
            else if(escolha == 'S' || escolha == 's'){
                limparTela();
                jogo jogoAtual;
                jogoAtual.jogador = jogadorAtual;
                jogoAtual.modoJogo = 2;
                for(int i = 0; i < 21; i++){
                    jogoAtual.mao1[i] = mao1[i];
                    jogoAtual.mao2[i] = mao2[i];
                }
                for(int i = 0; i < 14; i++){
                    jogoAtual.monte[i] = monte[i];
                }
                jogoAtual.mesaJogo = mesaJogo;
                jogoAtual.topoMonte = topoMonte;

                int escolha_save = 2;
                while((escolha_save != 1) && (escolha_save != 0)){
                    printf("Deseja Salvar?(1:Sim 0:Nao)\n");
                    scanf("%d", &escolha_save);
                    if((escolha_save != 1) && (escolha_save != 0)){
                        printf("Escolha invalida\n");
                    }
                }
                if(escolha_save == 1){
                    gravaCadastro(jogoAtual);
                }
                printf("Jogo interrompido. Voltando ao menu inicial...\n\n");
                return;
            }
            else{
                printf("Opcao invalida. Tente novamente.\n");
            }
        }

        vitoria = checarVitoria(mao1, mao2);
    }

    limparTela();
    printf("========================================\n");
    if(vitoria == 1){
        printf("Parabens! VOCE venceu a partida contra o computador!\n");
    } else {
        printf("O COMPUTADOR venceu a partida. Tente novamente!\n");
    }
    printf("========================================\n\n");
}

//Verifica em quais lados da mesa a peca escolhida encaixa, reaproveitando validarJogada.
//Retorna: 0 = nenhum lado 1 = somente esquerda 2 = somente direita 3 = ambos os lados
int verificarLadosDisponiveis(peca pecaValida, mesa mesaJogo){
    int encaixaEsquerda = (validarJogada(pecaValida, mesaJogo, 'E') != -1);
    int encaixaDireita  = (validarJogada(pecaValida, mesaJogo, 'D') != -1);

    if (encaixaEsquerda && encaixaDireita) return 3;
    else if (encaixaEsquerda) return 1;
    else if (encaixaDireita) return 2;
    else return 0;
}

