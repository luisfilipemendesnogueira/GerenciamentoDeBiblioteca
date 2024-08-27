#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "livros.h"
#include "usuarios.h"

// Cria usuario.
TUsuario* usuario(int id, char* nome, char* cpf, bool pegouLivro) {
    TUsuario* usu = (TUsuario *) malloc(sizeof(TUsuario));
    //inicializa espaco de memoria com ZEROS
    if (usu) memset(usu, 0, sizeof(TUsuario));
    //copia valores para os campos de usu
    usu->id = id;
    strcpy(usu->nome, nome);
    strcpy(usu->cpf, cpf);
    usu->pegouLivro = pegouLivro;
    usu->qtdLivrosPegos = 0;
    return usu;
}

// Salva usuario no arquivo out, na posicao atual do cursor
void salvaUsuario(TUsuario* usu, FILE *out) {
    fwrite(&usu->id, sizeof(int), 1, out);
    //usu->nome ao inves de &usu->nome, pois string ja eh um ponteiro
    fwrite(usu->nome, sizeof(char), sizeof(usu->nome), out);
    fwrite(usu->cpf, sizeof(char), sizeof(usu->cpf), out);
    fwrite(&usu->pegouLivro, sizeof(bool), 1, out);
    fwrite(&usu->qtdLivrosPegos, sizeof(int), 1, out);
    fwrite(usu->livrosAssociado, sizeof(TLivro), 5, out);
}

// Le um usuario do arquivo  na posicao atual do cursor
// Retorna um ponteiro para usuario lido do arquivo
TUsuario* leUsuario(FILE *in) {
    TUsuario* usu = (TUsuario *) malloc(sizeof(TUsuario));
    if (0 >= fread(&usu->id, sizeof(int), 1, in)) {
        free(usu);
        return NULL;
    }
    fread(usu->nome, sizeof(char), sizeof(usu->nome), in);
    fread(usu->cpf, sizeof(char), sizeof(usu->cpf), in);
    fread(&usu->pegouLivro, sizeof(bool), 1, in);
    fread(&usu->qtdLivrosPegos, sizeof(int), 1, in);
    fread(usu->livrosAssociado, sizeof(TLivro), 5, in);
    return usu;
}

// Retorna tamanho do usuario em bytes
int tamanho_registroUsuario() {
    return sizeof(int)  //cod
           + sizeof(char) * 50 //nome
           + sizeof(char) * 50 //cpf
           + sizeof(bool) //pegouLivro
           + sizeof(int) //qtdLivrosPegos
           + sizeof(TLivro) * 5; //livrosAssociados
}

// Cria uma base de dados desordenada pelo ID do usuario
void criarBaseDesordenadaUsuario(FILE *out, int tam){

    rewind(out);

    int vetId[tam];
    TUsuario* u;
    char nome[50] = "Usuario ";

    for(int i=0;i<tam;i++)
        vetId[i] = i+1;

    embaralhaUsuario(vetId,tam);

    printf("\nGerando a base de dados de Usuarios...\n");

    for (int i=0;i<tam;i++){
        snprintf(nome, sizeof(nome), "Usuario %d", vetId[i]); // Transforma o ID em string e concatena a string do id na String do nome do usuario. (Ex: Se o id de um usuario eh 5, o nome do usuario sera "Usuario 5")

        u = usuario(vetId[i], nome, "123.456.789-10", false);
        salvaUsuario(u, out);
    }

    free(u);
}

//embaralha base de dados
void embaralhaUsuario(int* vet,int tam) {
    srand(time(NULL));
    for (int i = tam - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = vet[i];
        vet[i] = vet[j];
        vet[j] = tmp;
    }
}

//imprime a base de dados
void imprimirBaseUsuario(FILE *out){
printf("\n\nImprimindo a base de dados...\n\n");

    rewind(out);
    TUsuario* u;

    while ((u = leUsuario(out)) != NULL)
        imprimeUsuario(u);

    free(u);

}

// Imprime usuario
void imprimeUsuario(TUsuario* usu) {
    if(usu == NULL){
        return;
    }
    else{
        printf("**********************************************");
        printf("\nID do usuario: ");
        printf("%d", usu->id);
        printf("\nNome do usuario: ");
        printf("%s", usu->nome);
        printf("\nCPF: ");
        printf("%s", usu->cpf);
        printf("\nO usuario pegou algum livro emprestado? ");
        if(usu->pegouLivro){
            printf("Sim");
        }else{
            printf("Nao");
        }
        printf("\n\nQuantidade de livros que este Usuario pegou emprestado: %d", usu->qtdLivrosPegos);
        if (usu->qtdLivrosPegos > 0) {
            printf("\n\nLivros associados:\n");
            for (int i = 0; i < usu->qtdLivrosPegos; i++) {
                printf("Livro %d: ID = %d, Nome = '%s', Autor = '%s'\n",
                    i + 1, usu->livrosAssociado[i].id,
                    usu->livrosAssociado[i].nome,
                    usu->livrosAssociado[i].autor);
            }
        }
        printf("\n**********************************************");
    }
}

void cadastrar_usuario(FILE *arqU, int *tamanho){
    TUsuario* usuario_novo = (TUsuario *) malloc(sizeof(TUsuario));
    *tamanho += 1;

    fseek(arqU, 0, SEEK_END);

    usuario_novo->id = *tamanho;
    printf("\nDigite o nome do usuario: ");
    scanf("%s", usuario_novo->nome);
    fflush(stdin);
    printf("\nDigite o cpf do usuario: ");
    scanf("%s", usuario_novo->cpf);
    usuario_novo->pegouLivro = false;
    usuario_novo->qtdLivrosPegos = 0;

    // Inicializa o vetor de livros associados
    for (int i = 0; i < 5; i++) {
        usuario_novo->livrosAssociado[i].id = 0;
        usuario_novo->livrosAssociado[i].nome[0] = '\0';
        usuario_novo->livrosAssociado[i].autor[0] = '\0';
        usuario_novo->livrosAssociado[i].isEmprestado = false;
    }

    salvaUsuario(usuario_novo, arqU);
    free(usuario_novo);
}
