#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "livros.h"
#include "usuarios.h"
#include "selecao_substituicao.h"

// Funcao para ordenar registros usando selecao por substituicao
int selecao_por_substituicao(FILE *arq, int M) {

    // Exibe uma mensagem indicando que as particoes estao sendo geradas
    printf("\n\nGerando Particoes Classificadas");

    // Posiciona o ponteiro do arquivo no início
    rewind(arq);

    int reg = 0;                    // Numero de registros ja lidos
    int nLivros = tamanho_arquivo(arq); // Numero total de registros no arquivo
    int qtdParticoes = 0;           // Quantidade de particoes geradas
    int t = tamanho_registro();      // Tamanho de cada registro
    char nomeParticao[20];           // Nome da particao a ser criada
    int totalDeCongelados = 0;       // Contador de registros congelados

    // Inicializa um array booleano para indicar se os registros estao congelados
    bool isCongelado[M];
    for(int i = 0; i < M; i++) {
        isCongelado[i] = false;      // Inicialmente todos os registros estao descongelados
    }

    // Array de ponteiros para armazenar os registros lidos na memoria
    TLivro *v[M];
    int i = 0;

    // Le M registros do arquivo e armazena no array v
    for(int i = 0; i < M; i++){
        fseek(arq, (reg) * t, SEEK_SET);  // Move o ponteiro do arquivo para a posicao do proximo registro
        v[i] = le(arq);                  // Le o registro e o armazena no array
        reg++;                           // Incrementa o contador de registros lidos
    }

    // Enquanto houver registros para ler do arquivo
    while (reg < nLivros) {

        // Reseta o contador de congelados
        totalDeCongelados = 0;

        // Gera o nome da primeira partição e abre o arquivo da partição
        sprintf(nomeParticao, "particao livros %i.dat", qtdParticoes+1);
        FILE *p;

        printf("\n\nCriando arquivo de particao: %s", nomeParticao);
        if ((p = fopen(nomeParticao, "w+b")) == NULL) {   // Abre o arquivo da partição para escrita
            printf("Erro ao abrir arquivo\n");
            exit(1);    // Se não conseguir abrir o arquivo, o programa termina
        }
        qtdParticoes++;  // Incrementa o número de partições geradas

        // Enquanto nem todos os registros estiverem congelados
        while(totalDeCongelados < M){

            int reg_menor_id = -1;  // Inicializa o indice do registro com o menor id

            // Encontra o registro com o menor id que nao esteja congelado
            for(int j = 0; j < M; j++) {
                if (!isCongelado[j] && (reg_menor_id == -1 || v[j]->id < v[reg_menor_id]->id)) {
                    reg_menor_id = j;
                }
            }

            // Se nao houver registros validos (todos congelados), interrompe o laço
            if (reg_menor_id == -1) {
                break;
            }

            // Salva o registro com o menor id na particao
            salva(v[reg_menor_id], p);

            // Armazena o id do registro que sera substituido e o indice do registro na memoria
            int antigo_id = v[reg_menor_id]->id;
            int novo_reg = reg_menor_id;

            // Substitui o registro lido pelo proximo do arquivo, se ainda houver registros
            if (reg < nLivros) {
                fseek(arq, reg * t, SEEK_SET);  // Move o ponteiro do arquivo para o proximo registro
                v[novo_reg] = le(arq);          // Le o proximo registro
                reg++;                          // Incrementa o contador de registros lidos
            } else {
                // Se nao houver mais registros no arquivo, congela o registro na memoria
                isCongelado[novo_reg] = true;
            }

            // Se o novo registro tem um id menor que o anterior, congela o registro
            if (v[novo_reg]->id < antigo_id) {
                totalDeCongelados++;  // Incrementa o contador de congelados
                isCongelado[novo_reg] = true;  // Marca o registro como congelado
            }

            // Se todos os registros estiverem congelados, imprime e fecha a particao
            if (totalDeCongelados == M) {
                imprimirBase(p);  // Imprime os registros na particao
                fclose(p);        // Fecha o arquivo da particao

                // Gera uma nova particao e reabre o arquivo de particao
                sprintf(nomeParticao, "particao livros %i.dat", qtdParticoes+1);
                FILE *p;
                printf("\n\nCriando arquivo de particao: %s", nomeParticao);
                if ((p = fopen(nomeParticao, "w+b")) == NULL) {   // Abre o arquivo da nova particao
                    printf("Erro ao abrir arquivo\n");
                    exit(1);   // Se nao conseguir abrir, encerra o programa
                }
                qtdParticoes++;  // Incrementa o numero de particoes geradas

                // Descongela todos os registros para o proximo ciclo
                totalDeCongelados = 0;
                for (int i = 0; i < M; i++) {
                    isCongelado[i] = false;  // Reseta o estado de congelamento dos registros
                }
            }
        }
        imprimirBase(p);  // Imprime a particao final
    }

    // Libera a memoria alocada para os registros que estao na memoria
    for (int i = 0; i < M; i++) {
        if (v[i] != NULL) {
            free(v[i]);
        }
    }

    // Retorna o numero total de particoes geradas
    return qtdParticoes;
}

// Funcao para ordenar registros usando sele��o por substitui��o
int selecao_por_substituicaoUsuario(FILE *arqU, int M) {

    printf("\n\nGerando Particoes Classificadas");

    rewind(arqU);

    int reg = 0;
    int nUsuarios = tamanho_arquivoUsuario(arqU);
    int qtdParticoes = 0;
    int t = tamanho_registroUsuario();
    char nomeParticao[20];
    int totalDeCongelados = 0;

    bool isCongelado[M];
    for(int i = 0; i < M; i++) {
        isCongelado[i] = false;
    }

    TUsuario *v[M];
    int i = 0;

    //le M registros do arquivo e coloca no array em memoria
    for(int i = 0; i < M; i++){
        fseek(arqU, (reg) * t, SEEK_SET);
        v[i] = leUsuario(arqU);
        reg++;
    }

    while (reg < nUsuarios) {

        totalDeCongelados = 0;

        //cria o primeiro arquivo de particao
        sprintf(nomeParticao, "particao usuarios %i.dat", qtdParticoes+1);
        FILE *p;
        printf("\n\nCriando arquivo de particao: %s", nomeParticao);
        if ((p = fopen(nomeParticao, "w+b")) == NULL) {
            printf("Erro ao abrir arquivo\n");
            exit(1);
        }
        qtdParticoes++;

        //seleciona, no array em memoria, o registro r com menor chave, que � o id
        while(totalDeCongelados < M){
            int reg_menor_id = -1;
            for(int j = 0; j < M; j++) {
                if (!isCongelado[j] && (reg_menor_id == -1 || v[j]->id < v[reg_menor_id]->id)) {
                    reg_menor_id = j;
                }
            }

            if (reg_menor_id == -1) {
                // se todos os registros estao congelados ou se nao ha mais registros, interrompe o laco
                break;
            }

            //Grava o registro r na particao de saida
            salvaUsuario(v[reg_menor_id], p);

            int antigo_id = v[reg_menor_id]->id; // ID do registro que sera substituido
            int novo_reg = reg_menor_id; // Indice na memoria do registro que sera substituido

            // Substitui o registro com menor chave pelo proximo registro do arquivo
            if (reg < nUsuarios) {
                fseek(arqU, reg * t, SEEK_SET);
                v[novo_reg] = leUsuario(arqU);
                reg++;
            } else {
                // Se nao ha mais registros para ler, marca como congelado
                isCongelado[novo_reg] = true;
            }

            if (v[novo_reg]->id < antigo_id) {
                // Se o novo registro tem uma chave menor que o antigo, aumenta o contador de congelados
                totalDeCongelados++;
                isCongelado[novo_reg] = true;
            }

            // Se todos os registros da memoria foram congelados, fecha a particao
            if (totalDeCongelados == M) {
                imprimirBaseUsuario(p);
                fclose(p);

                // Abre uma nova particao de saida
                sprintf(nomeParticao, "particao usuarios %i.dat", qtdParticoes+1);
                FILE *p;
                printf("\n\nCriando arquivo de particao: %s", nomeParticao);
                if ((p = fopen(nomeParticao, "w+b")) == NULL) {
                    printf("Erro ao abrir arquivo\n");
                    exit(1);
                }
                qtdParticoes++;

                // Descongela os registros congelados
                totalDeCongelados = 0;
                for (int i = 0; i < M; i++) {
                    isCongelado[i] = false;
                }
            }
        }
        imprimirBaseUsuario(p);
    }

     // Libera a memoria dos registros restantes na memoria
    for (int i = 0; i < M; i++) {
        if (v[i] != NULL) {
            free(v[i]);
        }
    }

    return qtdParticoes;
}
