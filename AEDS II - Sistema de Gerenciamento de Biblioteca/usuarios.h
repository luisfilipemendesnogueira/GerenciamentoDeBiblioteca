#ifndef USUARIOS_H_INCLUDED
#define USUARIOS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>

typedef struct Livro TLivro;

typedef struct Usuario{
    int id;
    char nome[50];
    char cpf[50];
    bool pegouLivro;
    TLivro livrosAssociado[5];
    int qtdLivrosPegos;
}TUsuario;

// Cria usuario.
TUsuario* usuario(int id, char* nome, char* cpf, bool pegouLivro);

// Salva usuario no arquivo out, na posicao atual do cursor
void salvaUsuario(TUsuario* usu, FILE *out);

// Le um usuario do arquivo  na posicao atual do cursor
// Retorna um ponteiro para usuario lido do arquivo
TUsuario* leUsuario(FILE *in);

// Retorna tamanho do usuario em bytes
int tamanho_registroUsuario();

// Cria uma base de dados desordenada pelo ID do usuario
void criarBaseDesordenadaUsuario(FILE *out, int tam);

//embaralha base de dados
void embaralhaUsuario(int* vet,int tam);

//imprime a base de dados
void imprimirBaseUsuario(FILE *out);

// Imprime usuario
void imprimeUsuario(TUsuario* usu);

#endif // USUARIOS_H_INCLUDED
