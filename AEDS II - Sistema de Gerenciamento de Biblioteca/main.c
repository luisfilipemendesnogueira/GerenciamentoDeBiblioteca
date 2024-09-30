#include <stdio.h>
#include <stdlib.h>

#include "livros.h"
#include "usuarios.h"
#include "buscaSequencial.h"
#include "buscaBinaria.h"
#include "SelectionSort.h"
#include "emprestimo.h"
#include "devolver.h"
#include "selecao_substituicao.h"
#include "intercalacao_otima.h"

int main()
{
    FILE *arq;
    FILE *arqU;

    TLivro* l;
    TUsuario* u;
    int opc, escolha, opcao, idL, idU;
    int qtdParticoes = 0;
    int M = 0;
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
            escolha = ler_inteiro("\n| Bem-vindo, o que voce deseja?:\n| 1 - Consultar as informacoes de um livro pelo seu ID\n| 2 - Consultar as informacoes de um usuario pelo seu ID\n| 3 - Registrar emprestimo de livro\n| 4 - Registrar devolucao de livro\n| 5 - Cadastrar um livro novo\n| 6 - Cadastrar um usuario novo\n| 7 - Sair.\n\nEscolha: ");

            switch(escolha){
                case 1:
                    escolha = 0;
                    while(escolha != 3){
                        escolha = ler_inteiro("\n| Deseja que tipo de base de dados para realizar a busca?\n| 1 - Ordenada\n| 2 - Desordenada\n| 3 - Sair\n\nEscolha: ");
                        printf("\n");
                        switch(escolha){
                            case 1:
                                opcao = 0;
                                while(opcao != 3){
                                    opcao = ler_inteiro("\n| Qual o modelo de odernacao?\n| 1 - Ordenacao interna\n| 2 - Ordenacao Externa\n| 3 - Sair\n\nEscolha: ");
                                    printf("\n");
                                    switch(opcao){
                                        case 1:
                                            escolha = 0;
                                            while(escolha != 2){
                                                escolha = ler_inteiro("\n| Qual o modelo da busca?\n| 1 - Busca Binaria\n| 2 - Sair\n\nEscolha: ");
                                                printf("\n");
                                                switch(escolha){
                                                    case 1:
                                                        if(isLivroOrdenado == false){
                                                            isLivroOrdenado = true;
                                                            selection_sort(arq, tamanhoLivro);
                                                            imprimirBase(arq);
                                                            printf("\n");
                                                        }else{
                                                            printf("Este arquivo ja esta ordenado.");
                                                        }
                                                        opc = ler_inteiro("\n\nDigite o ID do livro que voce deseja acessar: ");
                                                        l = busca_binaria(opc, arq, 0, tamanhoLivro);
                                                        imprime(l);
                                                        printf("\n");
                                                        break;
                                                    case 2:
                                                        break;
                                                    default:
                                                        printf("Opcao invalida\n");
                                                        break;
                                                }
                                            }
                                            break;
                                        case 2:
                                            escolha = 0;
                                            while(escolha != 2){
                                                escolha = ler_inteiro("\n| Qual o modelo da busca?\n| 1 - Busca Binaria\n| 2 - Sair\n\nEscolha: ");
                                                printf("\n");
                                                switch(escolha){
                                                    case 1:
                                                        if(isLivroOrdenado == false){
                                                            printf("Quantos registros de Livros voce deseja ler na memoria principal?\n");
                                                            scanf("%d", &M);
                                                            if(M < 1 || M > tamanhoLivro){
                                                                break;
                                                            }
                                                            isLivroOrdenado = true;
                                                            qtdParticoes = selecao_por_substituicao(arq, M);
                                                            printf("\n\n\n\n********************************************************************************************");
                                                            printf("\nNumero de particoes geradas: %d\n", qtdParticoes);
                                                            intercalacao_otima(arq, qtdParticoes);
                                                            printf("\n\n\n\n********************************************************************************************");
                                                            printf("\nArquivo Ordenado:");
                                                            imprimirBase(arq);
                                                            printf("\n********************************************************************************************");
                                                            printf("\n");
                                                        } else{
                                                            printf("Este arquivo ja esta ordenado.");
                                                        }
                                                        opc = ler_inteiro("\n\nDigite o ID do livro que voce deseja acessar: ");
                                                        l = busca_binaria(opc, arq, 0, tamanhoLivro);
                                                        imprime(l);
                                                        printf("\n");
                                                        break;
                                                    case 2:
                                                        break;
                                                    default:
                                                        printf("Opcao invalida\n");
                                                        break;
                                                }
                                            }
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
                                escolha = 0;
                                while(escolha != 2){
                                    escolha = ler_inteiro("\n| Qual o modelo da busca?\n| 1 - Busca Sequencial\n| 2 - Sair\n\nEscolha: ");
                                    printf("\n");
                                    switch(escolha){
                                        case 1:
                                            if(isLivroOrdenado == true){
                                                printf("Este arquivo esta ordenado.");
                                            }
                                            opc = ler_inteiro("\n\nDigite o ID do livro que voce deseja acessar: ");
                                            l = buscaSequencial(opc, arq);
                                            imprime(l);
                                            printf("\n");
                                            break;
                                        case 2:
                                            break;
                                        default:
                                            printf("Opcao invalida\n");
                                            break;
                                    }
                                }
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
                    escolha = 0;
                    while(escolha != 3){
                        escolha = ler_inteiro("\n| Deseja que tipo de base de dados para realizar a busca?\n| 1 - Ordenada\n| 2 - Desordenada\n| 3 - Sair\n\nEscolha: ");
                        printf("\n");
                        switch(escolha){
                            case 1:
                                opcao = 0;
                                while(opcao != 3){
                                    opcao = ler_inteiro("\n| Qual o modelo de odernacao?\n| 1 - Ordenacao interna\n| 2 - Ordenacao Externa\n| 3 - Sair\n\nEscolha: ");
                                    printf("\n");
                                    switch(opcao){
                                        case 1:
                                            escolha = 0;
                                            while(escolha != 2){
                                                escolha = ler_inteiro("\n| Qual o modelo da busca?\n| 1 - Busca Binaria\n| 2 - Sair\n\nEscolha: ");
                                                printf("\n");
                                                switch(escolha){
                                                    case 1:
                                                        if(isUsuarioOrdenado == false){
                                                            isUsuarioOrdenado = true;
                                                            selection_sort(arqU, tamanhoUsuario);
                                                            imprimirBase(arqU);
                                                            printf("\n");
                                                        }else{
                                                            printf("Este arquivo ja esta ordenado.");
                                                        }
                                                        opc = ler_inteiro("\n\nDigite o ID do livro que voce deseja acessar: ");
                                                        u = busca_binaria_usuario(opc, arqU, 0, tamanhoUsuario);
                                                        imprimeUsuario(u);
                                                        printf("\n");
                                                        break;
                                                    case 2:
                                                        break;
                                                    default:
                                                        printf("Opcao invalida\n");
                                                        break;
                                                }
                                            }
                                            break;
                                        case 2:
                                            escolha = 0;
                                            while(escolha != 2){
                                                escolha = ler_inteiro("\n| Qual o modelo da busca?\n| 1 - Busca Binaria\n| 2 - Sair\n\nEscolha: ");
                                                printf("\n");
                                                switch(escolha){
                                                    case 1:
                                                        if(isUsuarioOrdenado == false){
                                                            printf("Quantos registros de Usuarios voce deseja ler na memoria principal?\n");
                                                            scanf("%d", &M);
                                                            if(M < 1 || M > tamanhoUsuario){
                                                                break;
                                                            }
                                                            isUsuarioOrdenado = true;
                                                            qtdParticoes = selecao_por_substituicaoUsuario(arqU, M);
                                                            printf("\n\n\n\n********************************************************************************************");
                                                            printf("\nNumero de particoes geradas: %d\n", qtdParticoes);
                                                            intercalacao_otimaUsuario(arqU, qtdParticoes);
                                                            printf("\n\n\n\n********************************************************************************************");
                                                            printf("\nArquivo Ordenado:");
                                                            imprimirBaseUsuario(arqU);
                                                            printf("\n********************************************************************************************");
                                                            printf("\n");
                                                        } else{
                                                            printf("Este arquivo ja esta ordenado.");
                                                        }
                                                        opc = ler_inteiro("\n\nDigite o ID do livro que voce deseja acessar: ");
                                                        u = busca_binaria_usuario(opc, arqU, 0, tamanhoUsuario);
                                                        imprimeUsuario(u);
                                                        printf("\n");
                                                        break;
                                                    case 2:
                                                        break;
                                                    default:
                                                        printf("Opcao invalida\n");
                                                        break;
                                                }
                                            }
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
                                escolha = 0;
                                while(escolha != 2){
                                    escolha = ler_inteiro("\n| Qual o modelo da busca?\n| 1 - Busca Sequencial\n| 2 - Sair\n\nEscolha: ");
                                    printf("\n");
                                    switch(escolha){
                                        case 1:
                                            if(isUsuarioOrdenado == true){
                                                printf("Este arquivo esta ordenado.");
                                            }
                                            opc = ler_inteiro("\n\nDigite o ID do livro que voce deseja acessar: ");
                                            u = buscaSequencialUsuario(opc, arqU);
                                            imprimeUsuario(u);
                                            printf("\n");
                                            break;
                                        case 2:
                                            break;
                                        default:
                                            printf("Opcao invalida\n");
                                            break;
                                    }
                                }
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
