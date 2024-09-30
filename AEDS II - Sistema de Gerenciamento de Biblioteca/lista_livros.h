#ifndef LISTA_LIVROS_H_INCLUDED
#define LISTA_LIVROS_H_INCLUDED

#include "livros.h"

// Estrutura da c�lula da lista
typedef struct celula {
    FILE* arquivo;
    char nomeArquivo[100];
    struct celula* prox;
}TCelulaArquivosLivros;

// Estrutura da lista de livros
typedef struct {
    TCelulaArquivosLivros* primeiro;
    TCelulaArquivosLivros* ultimo;
    int tamanho;
}TListaArquivosLivros;

void FLVazia(TListaArquivosLivros *Lista);

void Inserir(FILE *x, const char* nome, TListaArquivosLivros *Lista);

void Imprimir(TListaArquivosLivros Lista);

TCelulaArquivosLivros* Pesquisar(TListaArquivosLivros Lista, const char* nome);

void ExcluirPrimeiroArquivo(TListaArquivosLivros *Lista);

#endif // LISTA_LIVROS_H_INCLUDED
