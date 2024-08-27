#ifndef LIVROS_H_INCLUDED
#define LIVROS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>

typedef struct Usuario TUsuario;

typedef struct Livro{
    int id;
    char nome[50];
    char autor[50];
    bool isEmprestado;
    TUsuario* usuarioAssociado;
}TLivro;

// Cria livro.
TLivro* livro(int id, char* nome, char* autor, bool isEmprestado);

// Salva livro no arquivo out, na posicao atual do cursor
void salva(TLivro* livr, FILE *out);

// Le um livro do arquivo  na posicao atual do cursor
// Retorna um ponteiro para livro lido do arquivo
TLivro* le(FILE *in);

char* sufixo(int num);

// Retorna tamanho do livro em bytes
int tamanho_registro();

// Cria uma base de dados desordenada pelo ID do livro
void criarBaseDesordenada(FILE *out, int tam);

//embaralha base de dados
void embaralha(int* vet,int tam);

//imprime a base de dados
void imprimirBase(FILE *out);

// Imprime livro
void imprime(TLivro* livr);

#endif // LIVROS_H_INCLUDED
