#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "livros.h"

int pLivroJaCriado = 0; // Variavel global para controlar quantos livros ja foram criados na base de dados
long posicao_encontrada = -1; // Variavel para armazenar a posicao de um registro encontrado no arquivo
bool isJaInserida = false; //Verifica se a base de dados ja foi inserida uma vez

// Funcao que inicializa a tabela hash com 'm' compartimentos, com cada um apontando para -1 (NULL)
void inicializaTabHash(FILE *tab, int m){
    int pNull = -1;
    for (int i = 0; i < m; i++) {
        fwrite(&pNull, sizeof(int), 1, tab);
    }
    printf("Arquivo tabHash.dat criado e inicializado com %d compartimentos.\n", m);
}

// Funcao hash "METODO DA DIVISAO", que calcula o endereco na tabela hash com base no ID do Livro
int funcao_hash(int id, int m) {
    return id % m;    //h(id) = id mod m
}

//A tecnica utilizada abaixo para tratamento de colisoes foi a de "ENCADEAMENTO EXTERIOR"

// Funcao para buscar um livro na tabela hash
TLivro* busca_hash(FILE *tab, FILE *arq, int m, int id){
    int endereco = funcao_hash(id, m);
    int pProx;
    TLivro *livro;

    // Posiciona no compartimento da tabela hash correspondente ao endereco calculado
    fseek(tab, endereco * sizeof(int)   , SEEK_SET);
    fread(&pProx, sizeof(int), 1, tab); // Le o ponteiro para o primeiro registro da lista encadeada

    // Percorre a lista encadeada buscando o registro
    while(pProx != -1){
        fseek(arq, pProx, SEEK_SET);
        livro = le(arq);

        if(livro->ocupado == true){
            if (livro->id == id) {        // Verifica se o registro eh o que estamos buscando
                posicao_encontrada = ftell(arq) - tamanho_registro();
                return livro;
            }
        }

        pProx = livro->prox; // Passa para o próximo elemento da lista encadeada
    }

    printf("\nRegistro nao encontrado.\n");  // Se terminar o loop sem encontrar o registro
}

// Funcao para inserir um novo livro na tabela hash
void inserir_hash(FILE *tab, FILE *arq, int m, TLivro *novoLivro, int tamOriginal) {
    int endereco = funcao_hash(novoLivro->id, m);
    int pProx;
    int pPrimeiroLiberado = -1, pAnterior = -1; // Variaveis para armazenar a posição do primeiro nó liberado e do nó anterior

    TLivro *livroAtual; // Ponteiro para o livro atual no loop
    TLivro *livroAnterior; // Ponteiro para o livro anterior no loop

    // Posiciona no compartimento da tabela hash correspondente ao endereco calculado
    fseek(tab, endereco * sizeof(int), SEEK_SET);
    fread(&pProx, sizeof(int), 1, tab); // Le o ponteiro para o primeiro registro da lista encadeada

    // Percorre a lista encadeada buscando o registro
    while (pProx != -1) {
        fseek(arq, pProx, SEEK_SET);
        livroAtual = le(arq);

        if (livroAtual->id == novoLivro->id) {    // Verifica se o registro já existe
            printf("\nErro: Registro com id %d ja existe!\n", novoLivro->id);
            return;  // Se o registro ja existe, nao insere e sai da funcao de inserir
        }

        if (livroAtual->ocupado == false && pPrimeiroLiberado == -1) {
            pPrimeiroLiberado = pProx;  // Guarda o endereco do primeiro nó liberado encontrado
            break;  // Sai do while
        }

        pAnterior = pProx; // Atualiza a posicao do nó anterior
        pProx = livroAtual->prox; // Passa para o proximo elemento da lista encadeada
    }

    printf("Livro de ID: %d adicionado com sucesso\n", novoLivro->id);

    // Se encontrou um nó liberado, reutiliza o espaço dele
    if (pPrimeiroLiberado != -1) {
        fseek(arq, pPrimeiroLiberado, SEEK_SET);// Posiciona no nó liberado
        TLivro* livroLiberado = le(arq); // Le o nó liberado
        novoLivro->prox = livroLiberado->prox;  // Nó inserido aponta para o nó que o nó liberado apontava
        novoLivro->ocupado = true; // Marca o nó inserido como ocupado
        fseek(arq, (novoLivro->id - 1) * tamanho_registro(), SEEK_SET);
        salva(novoLivro, arq); // Grava o nó inserido

        // Atualiza o ponteiro do nó anterior para o novo nó
        if (pAnterior != -1) {
            fseek(arq, pAnterior, SEEK_SET); // Posiciona no nó anterior
            livroAnterior = le(arq);
            livroAnterior->prox = (novoLivro->id - 1) * tamanho_registro(); // Atualiza o ponteiro do nó anterior
            fseek(arq, pAnterior, SEEK_SET); // Volta para o início do nó anterior
            salva(livroAnterior, arq); // Grava o nó anterior com o ponteiro atualizado
        } else {
            // Se nao existe nó anterior, atualiza a tabela hash diretamente
            long posicao = (novoLivro->id - 1) * tamanho_registro();
            fseek(tab, endereco * sizeof(int), SEEK_SET);
            fwrite(&posicao, sizeof(int), 1, tab);
        }
    } else {
        // Caso nao haja nó liberado, insere no final da lista
        int novaPosicao;
        if (novoLivro->id <= tamOriginal) { //Verifica se o livro a ser inserido faz parte da base de dados criada originalmente
            // Insere no final da lista
            fseek(arq, pLivroJaCriado * tamanho_registro(), SEEK_SET);
            novaPosicao = ftell(arq);   // Calcula a posicao do novo registro
            novoLivro->prox = -1;           // O novo nó é o ultimo, portanto prox = -1
            novoLivro->ocupado = true;      // Marca o novo registro como ocupado
            salva(novoLivro, arq);        // Grava o novo registro no final
            pLivroJaCriado++;
        } else { //Se o livro a ser inserido não faz parte da base de dados criada originalmente
            // Insere no final da lista
            fseek(arq, 0, SEEK_END);
            fseek(arq, -tamanho_registro(), SEEK_CUR);
            novaPosicao = ftell(arq);   // Calcula a posicao do novo registro
            novoLivro->prox = -1;           // O novo nó é o ultimo, portanto prox = -1
            novoLivro->ocupado = true;      // Marca o novo registro como ocupado
            salva(novoLivro, arq);        // Grava o novo registro no final
        }

        // Atualiza o ponteiro do ultimo nó da lista encadeada para apontar para o novo nó inserido
        if (pAnterior != -1) {
            fseek(arq, pAnterior, SEEK_SET); // Posiciona no nó anterior
            TLivro *livroUltimo = le(arq);
            livroUltimo->prox = novaPosicao; // Atualiza o ponteiro para o novo nó
            fseek(arq, pAnterior, SEEK_SET); // Volta para o inicio do nó anterior
            salva(livroUltimo, arq); // Grava o nó anterior com o ponteiro atualizado
        } else {
            // Se nao existe nó anterior (lista vazia), atualiza a tabela hash diretamente
            fseek(tab, endereco * sizeof(int), SEEK_SET);
            fwrite(&novaPosicao, sizeof(int), 1, tab);
        }
    }
}

// Funcao para remover um livro da tabela hash
void remover_hash(FILE *tab, FILE *arq, int m, int id){
    TLivro* livro;
    livro = busca_hash(tab, arq, m, id); // Busca o livro pelo ID

    if (livro != NULL) {  // Verifica se o livro foi encontrado
        livro->ocupado = false; // Marca o livro como liberado
        printf("\nLivro de ID: %d removido com sucesso\n", livro->id);
        fseek(arq, posicao_encontrada, SEEK_SET);  // Reposiciona o cursor para voltar na posicao do registro que sera removido
        salva(livro, arq);                      // Sobrescreve o livro no arquivo
        free(livro);
    } else {
        printf("\nLivro com ID %d nao foi encontrado.\n", id);
    }
}

// Funcao que imprime o conteudo da tabela hash
void imprimir_tabela_hash(FILE *tab, FILE *arq){
    rewind(tab);
    rewind(arq);

    TLivro* livro;
    int cont = 0;
    int pProx;
    bool fim = false;

    while(fread(&pProx, sizeof(int), 1, tab) == 1){
        printf("\n%d || -> ", cont);
        if(pProx != -1){
            fseek(arq, pProx, SEEK_SET);
            livro = le(arq);
            if(livro->ocupado == true){
                printf("|%d| %s|", livro->id, livro->nome);
                printf(" -> ");
            }
            while(livro->prox != -1){
                fseek(arq, livro->prox, SEEK_SET);
                livro = le(arq);
                if(livro->ocupado == true){
                    printf("|%d| %s|", livro->id, livro->nome);
                    printf(" -> ");
                }
            }
        }
        printf("\n");
        cont++;
    }
    printf("\n");
}

// Funcao que insere na tabela os registros de livros já criados na base de dados
void inserir_base_hash(FILE *tab, FILE *arq, int m, int tam, int tamOriginal){
    if(isJaInserida == false){
        isJaInserida = true;
        TLivro *livro = NULL;
        for(int i = 0; i<tam; i++){
            fseek(arq, i * tamanho_registro(), SEEK_SET);
            livro = le(arq);
            if (livro != NULL) {
                inserir_hash(tab, arq, m, livro, tamOriginal);
            }
        }
        printf("\nA base de dados criada foi inserida com sucesso na tabela hash!\n");
        free(livro);
    } else {
        printf("A base de dados ja foi inserida na tabela hash!\n");
    }
}
