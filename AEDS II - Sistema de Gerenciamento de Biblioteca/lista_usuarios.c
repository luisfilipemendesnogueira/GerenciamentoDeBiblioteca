#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "usuarios.h"
#include "livros.h"
#include "lista_usuarios.h"

void FLVaziaUsuario(TListaArquivosUsuarios *Lista) {
    Lista->primeiro = (TCelulaArquivosUsuarios*)malloc(sizeof(TCelulaArquivosUsuarios));
    Lista->ultimo = Lista->primeiro;
    Lista->primeiro->prox = NULL;
    Lista->tamanho = 0;
}

void InserirUsuario(FILE *x, const char* nome, TListaArquivosUsuarios *Lista) {
    Lista->ultimo->prox = (TCelulaArquivosUsuarios*)malloc(sizeof(TCelulaArquivosUsuarios));
    Lista->ultimo = Lista->ultimo->prox;
    Lista->ultimo->arquivo = x;
    strcpy(Lista->ultimo->nomeArquivo, nome);
    Lista->ultimo->prox = NULL;
    Lista->tamanho++;
}

void ImprimirUsuario(TListaArquivosUsuarios Lista) {
    TCelulaArquivosUsuarios* Aux = Lista.primeiro->prox;
    while (Aux != NULL) {
        printf("Arquivo: %s\n", Aux->nomeArquivo);
        Aux = Aux->prox;
    }
}

TCelulaArquivosUsuarios* PesquisarUsuario(TListaArquivosUsuarios Lista, const char* nome) {
    TCelulaArquivosUsuarios* Aux = Lista.primeiro->prox;
    while (Aux != NULL) {
        if(strcmp(Aux->nomeArquivo, nome) == 0)
            return Aux;
        Aux = Aux->prox;
    }
    return NULL;
}

void ExcluirPrimeiroArquivoUsuario(TListaArquivosUsuarios *Lista) {
    if (Lista->primeiro->prox == NULL) {
        printf("A lista esta vazia.\n");
        return;
    }
    TCelulaArquivosUsuarios *Aux = Lista->primeiro->prox;

    Lista->primeiro->prox = Aux->prox;
    if (Aux == Lista->ultimo) {
        Lista->ultimo = Lista->primeiro;
    }
    fclose(Aux->arquivo);
    free(Aux);
    Lista->tamanho--;
}
