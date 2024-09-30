#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "livros.h"
#include "lista_livros.h"

void FLVazia(TListaArquivosLivros *Lista) {
    Lista->primeiro = (TCelulaArquivosLivros*)malloc(sizeof(TCelulaArquivosLivros));
    Lista->ultimo = Lista->primeiro;
    Lista->primeiro->prox = NULL;
    Lista->tamanho = 0;
}

void Inserir(FILE *x, const char* nome, TListaArquivosLivros *Lista) {
    Lista->ultimo->prox = (TCelulaArquivosLivros*)malloc(sizeof(TCelulaArquivosLivros));
    Lista->ultimo = Lista->ultimo->prox;
    Lista->ultimo->arquivo = x;
    strcpy(Lista->ultimo->nomeArquivo, nome);
    Lista->ultimo->prox = NULL;
    Lista->tamanho++;
}

void Imprimir(TListaArquivosLivros Lista) {
    TCelulaArquivosLivros* Aux = Lista.primeiro->prox;
    while (Aux != NULL) {
        printf("Arquivo: %s\n", Aux->nomeArquivo);
        Aux = Aux->prox;
    }
}

TCelulaArquivosLivros* Pesquisar(TListaArquivosLivros Lista, const char* nome) {
    TCelulaArquivosLivros* Aux = Lista.primeiro->prox;
    while (Aux != NULL) {
        if(strcmp(Aux->nomeArquivo, nome) == 0)
            return Aux;
        Aux = Aux->prox;
    }
    return NULL;
}

void ExcluirPrimeiroArquivo(TListaArquivosLivros *Lista) {
    if (Lista->primeiro->prox == NULL) {
        printf("A lista esta vazia.\n");
        return;
    }

    TCelulaArquivosLivros *Aux = Lista->primeiro->prox;
    Lista->primeiro->prox = Aux->prox;
    if (Aux == Lista->ultimo) {
        Lista->ultimo = Lista->primeiro;
    }
    fclose(Aux->arquivo);
    free(Aux);
    Lista->tamanho--;
}
