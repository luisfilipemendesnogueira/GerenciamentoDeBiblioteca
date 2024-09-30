#ifndef LISTA_USUARIOS_H_INCLUDED
#define LISTA_USUARIOS_H_INCLUDED

#include "usuarios.h"

// Estrutura da celula da lista
typedef struct celulas {
    FILE* arquivo;
    char nomeArquivo[100];
    struct celulas* prox;
}TCelulaArquivosUsuarios;

// Estrutura da lista de livros
typedef struct {
    TCelulaArquivosUsuarios* primeiro;
    TCelulaArquivosUsuarios* ultimo;
    int tamanho;
}TListaArquivosUsuarios;

void FLVaziaUsuario(TListaArquivosUsuarios *Lista);

void InserirUsuario(FILE *x, const char* nome, TListaArquivosUsuarios *Lista);

void ImprimirUsuario(TListaArquivosUsuarios Lista);

TCelulaArquivosUsuarios* PesquisarUsuario(TListaArquivosUsuarios Lista, const char* nome);

void ExcluirPrimeiroArquivoUsuario(TListaArquivosUsuarios *Lista);

#endif // LISTA_USUARIOS_H_INCLUDED
