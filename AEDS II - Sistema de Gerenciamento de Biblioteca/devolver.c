#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "livros.h"
#include "usuarios.h"
#include "devolver.h"
#include "buscaSequencial.h"

void devolver_livro(int idU, int idL, FILE *arq, FILE *arqU){
        TLivro *livro;
        TUsuario *usuario;

        long posAtual;
        int encontradoUsuario = 0, encontradoLivro = 0;

        rewind(arq);
        rewind(arqU);

        usuario = buscaSequencialUsuario(idU, arqU);
        if (usuario == NULL) {
            return;
        }

        if(usuario->qtdLivrosPegos <= 0){
            printf("O usuario nao possui livro emprestado.\n");
            free(usuario);
            return;
        }

        encontradoUsuario = 1;

        if(encontradoUsuario){
            livro = buscaSequencial(idL, arq);

            if (livro == NULL) {
                printf("Livro nao encontrado.\n");
                return;
            }

            if(livro->isEmprestado == false){
                printf("Este livro nao foi emprestado\n");
            } else{
                encontradoLivro = 1;
            }
        }

        if (encontradoUsuario && encontradoLivro) {
            for(int i = 0; i < usuario->qtdLivrosPegos; i++){
                 if(usuario->livrosAssociado[i].id == idL){
                    for(int j = i; j < usuario->qtdLivrosPegos; j++){
                        usuario->livrosAssociado[j] = usuario->livrosAssociado[j+1];
                    }
                 }
            }

            usuario->qtdLivrosPegos--;

            if(usuario->qtdLivrosPegos == 0){
                usuario->pegouLivro = false;
            }

            posAtual = ftell(arqU) - tamanho_registroUsuario();
            fseek(arqU, posAtual, SEEK_SET);
            salvaUsuario(usuario, arqU);

            livro->isEmprestado = false;
            livro->usuarioAssociado = NULL;
            posAtual = ftell(arq) - tamanho_registro();
            fseek(arq, posAtual, SEEK_SET);
            salva(livro, arq);

            printf("Devolucao realizada\n");
        } else {
            printf("Devolucao nao realizada\n");
        }
        free(usuario);
        free(livro);
}
