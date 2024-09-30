#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "livros.h"
#include "usuarios.h"
#include "buscaBinaria.h"

//Realiza uma busca binaria por um livro na base de dados
TLivro* busca_binaria(int chave, FILE *in, int inicio, int fim) {

    TLivro *l = NULL;
    int id = -1;
    rewind(in);

    if(chave > fim){
        printf("\nLivro nao encontrado\n");
        return NULL;
    }

    while (inicio <= fim && id != chave) {

        int meio = trunc((inicio + fim) / 2);
        printf("\nInicio: %d; Fim: %d; Meio: %d\n", inicio, fim, meio);
        fseek(in, (meio -1) * tamanho_registro(), SEEK_SET);
        l = le(in);
        id = l->id;

        if (l) {
            if (id > chave) {
                fim = meio - 1;
            } else {
                inicio = meio + 1;
            }
        }
    }

    if (id == chave) {
        return l;
    } else{
        printf("\nLivro nao encontrado\n");
        return NULL;
    }
}

//Realiza uma busca binaria por um usuario na base de dados
TUsuario* busca_binaria_usuario(int chave, FILE *in, int inicio, int fim) {

    TUsuario *u = NULL;
    int id = -1;
    rewind(in);

    if(chave > fim){
        printf("\nUsuario nao encontrado\n");
        return NULL;
    }

    while (inicio <= fim && id != chave) {

        int meio = trunc((inicio + fim) / 2);
        printf("\nInicio: %d; Fim: %d; Meio: %d\n", inicio, fim, meio);
        fseek(in, (meio -1) * tamanho_registroUsuario(), SEEK_SET);
        u = leUsuario(in);
        id = u->id;

        if (u) {
            if (id > chave) {
                fim = meio - 1;
            } else {
                inicio = meio + 1;
            }
        }
    }

    if (id == chave) {
        return u;
    } else{
        printf("\nUsuario nao encontrado\n");
        return NULL;
    }
}
