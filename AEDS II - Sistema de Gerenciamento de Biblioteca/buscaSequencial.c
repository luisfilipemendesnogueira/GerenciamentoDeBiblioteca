#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "livros.h"
#include "usuarios.h"
#include "buscaSequencial.h"

//Realiza uma busca sequencial por um livro na base de dados
TLivro* buscaSequencial(int chave, FILE *in){

    TLivro *l;
    int achou;
    rewind(in);

    while ((l = le(in)) != NULL){

        if(l->id == chave){
           achou = 1;
           break;
        }
    }
        if(achou == 1)
            return l;
        else{
            printf("Livro nao encontrado\n");
            return NULL;
        }

        free(l);
}

//Realiza uma busca sequencial por um usuario na base de dados
TUsuario* buscaSequencialUsuario(int chave, FILE *in){

    TUsuario *u;
    int achou;
    rewind(in);

    while ((u = leUsuario(in)) != NULL){

        if(u->id == chave){
           achou = 1;
           break;
        }
    }
        if(achou == 1)
            return u;
        else{
            printf("Usuario nao encontrado\n");
            return NULL;
        }

        free(u);
}
