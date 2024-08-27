#include <stdio.h>
#include <stdlib.h>

#include "livros.h"
#include "usuarios.h"
#include "buscaSequencial.h"
#include "buscaBinaria.h"
#include "SelectionSort.h"
#include "emprestimo.h"
#include "devolver.h"

int main()
{
    FILE *arq;
    FILE *arqU;
    TLivro* l;
    TUsuario* u;
    int opc, escolha, idL, idU;
    int tamanhoLivro = 50, tamanhoUsuario = 50;
    bool isLivroOrdenado, isUsuarioOrdenado = false;

    if ((arq = fopen("livro.dat", "w+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    if ((arqU = fopen("usuario.dat", "w+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    else{
        criarBaseDesordenada(arq, tamanhoLivro);
        criarBaseDesordenadaUsuario(arqU, tamanhoUsuario);
        while(escolha != 7){
            escolha = ler_inteiro("\n| Bem-vindo, o que voce deseja?:\n| 1 - Consultar as informacoes de um livro pelo seu ID\n| 2 - Consultar as informacoes de um usuario pelo seu ID\n| 3 - Registrar emprestimo de livro\n| 4 - Registrar devolucao de livro\n| 5 - Cadastrar um livro novo\n| 6 - Cadastrar um usuario novo\n| 7 - Sair.\nEscolha: ");

            switch(escolha){
                case 1:
                    while(escolha != 3){
                        escolha = ler_inteiro("\n| Qual o modelo da busca?\n| 1 - Busca Sequencial\n| 2 - Busca Binaria\n| 3 - Sair\n\nEscolha: ");
                        printf("\n");
                        switch(escolha){
                            case 1:
                                if(isLivroOrdenado == true){
                                    criarBaseDesordenada(arq, tamanhoLivro);
                                }
                                imprimirBase(arq);
                                opc = ler_inteiro("\n\nDigite o ID do livro que voce deseja acessar: ");
                                l = buscaSequencial(opc, arq);
                                imprime(l);
                                break;
                            case 2:
                                isLivroOrdenado = true;
                                selection_sort(arq, tamanhoLivro);
                                imprimirBase(arq);
                                opc = ler_inteiro("\n\nDigite o ID do livro que voce deseja acessar: ");
                                l = busca_binaria(opc, arq, 0, tamanhoLivro);
                                imprime(l);
                                break;
                            case 3:
                                break;
                            default:
                                printf("Opcao invalida\n");
                                break;
                        }
                    }
                    break;
                case 2:
                    while(escolha != 3){
                        escolha = ler_inteiro("\n| Qual o modelo da busca?\n| 1 - Busca Sequencial\n| 2 - Busca Binaria\n| 3 - Sair\n\nEscolha: ");
                        switch(escolha){
                            case 1:
                                if(isUsuarioOrdenado == true){
                                    criarBaseDesordenadaUsuario(arqU, tamanhoUsuario);
                                }
                                imprimirBaseUsuario(arqU);
                                opc = ler_inteiro("\n\nDigite o ID do usuario que voce deseja acessar: ");
                                u = buscaSequencialUsuario(opc, arqU);
                                imprimeUsuario(u);
                                break;
                            case 2:
                                isUsuarioOrdenado = true;
                                selection_sort_Usuario(arqU, tamanhoUsuario);
                                imprimirBaseUsuario(arqU);
                                opc = ler_inteiro("\n\nDigite o ID do usuario que voce deseja acessar: ");
                                u = busca_binaria_usuario(opc, arqU, 0, tamanhoUsuario);
                                imprimeUsuario(u);
                                break;
                            case 3:
                                break;
                            default:
                                printf("Opcao invalida\n");
                                break;
                        }
                    }
                    break;
                case 3:
                    idU = ler_inteiro("\n\nDigite o ID do usuario que deseja pegar um livro emprestado: ");
                    idL = ler_inteiro("\n\nDigite o ID do livro que o usuario deseja pegar emprestado: ");
                    emprestar_livro(idU, idL, arq, arqU);
                    break;
                case 4:
                    idU = ler_inteiro("\n\nDigite o ID do usuario que deseja devolver um livro: ");
                    idL = ler_inteiro("\n\nDigite o ID do livro que o usuario deseja devolver: ");
                    devolver_livro(idU, idL, arq, arqU);
                    break;
                case 5:
                    cadastrar_livro(arq, &tamanhoLivro);
                    break;
                case 6:
                    cadastrar_usuario(arqU, &tamanhoUsuario);
                    break;
                case 7:
                printf("Saindo...\n\n");
                break;
                default:
                    printf("Opcao invalida\n\n");
                    break;
                }
        }
    }
    return 0;
}

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int ler_inteiro(const char *mensagem) {
    int valor;
    while (true) {
        printf("%s", mensagem);
        if (scanf("%d", &valor) == 1) {
            limpar_buffer();
            return valor;
        } else {
            printf("\nEntrada invalida. Por favor, insira um numero inteiro.\n");
            limpar_buffer();
        }
    }
}
