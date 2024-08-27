#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "livros.h"
#include "usuarios.h"
#include "emprestimo.h"
#include "buscaSequencial.h"

void emprestar_livro(int idU, int idL, FILE *arq, FILE *arqU){
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

        if(usuario->qtdLivrosPegos >= 5){
            printf("O usuario ja pegou 5 livros emprestados.\n");
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

            if(livro->isEmprestado == true){
                printf("Este livro ja foi emprestado\n");
            } else{
                encontradoLivro = 1;
            }
        }

        if (encontradoUsuario && encontradoLivro) {
            usuario->livrosAssociado[usuario->qtdLivrosPegos] = *livro;
            usuario->qtdLivrosPegos++;
            usuario->pegouLivro = true;

            posAtual = ftell(arqU) - tamanho_registroUsuario();
            fseek(arqU, posAtual, SEEK_SET);
            salvaUsuario(usuario, arqU);

            livro->isEmprestado = true;
            livro->usuarioAssociado = usuario;
            posAtual = ftell(arq) - tamanho_registro();
            fseek(arq, posAtual, SEEK_SET);
            salva(livro, arq);

            printf("Emprestimo realizado\n");
        } else {
            printf("Emprestimo nao realizado\n");
        }
        free(usuario);
        free(livro);
}
