#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "livros.h"
#include "usuarios.h"

// Cria livro.
TLivro* livro(int id, char* nome, char* autor, bool isEmprestado, int prox, bool ocupado) {
    TLivro* livr = (TLivro *) malloc(sizeof(TLivro));
    //inicializa espaco de memoria com ZEROS
    if (livr) memset(livr, 0, sizeof(TLivro));
    //copia valores para os campos de livr
    livr->id = id;
    strcpy(livr->nome, nome);
    strcpy(livr->autor, autor);
    livr->isEmprestado = isEmprestado;
    livr->usuarioAssociado = NULL;
    livr->prox = prox;
    livr->ocupado = ocupado;
    return livr;
}

// Salva livro no arquivo out, na posicao atual do cursor
void salva(TLivro* livr, FILE *out) {
    fwrite(&livr->id, sizeof(int), 1, out);
    //livr->nome ao inves de &livr->nome, pois string ja eh um ponteiro
    fwrite(livr->nome, sizeof(char), sizeof(livr->nome), out);
    fwrite(livr->autor, sizeof(char), sizeof(livr->autor), out);
    fwrite(&livr->isEmprestado, sizeof(bool), 1, out);
    // Verifica se o usuario associado nao eh NULL antes de salvar
    if (livr->usuarioAssociado != NULL) {
        fwrite(livr->usuarioAssociado, sizeof(TUsuario), 1, out);
    } else {
        // Se nao houver usu�rio associado, escreva um marcador
        TUsuario usuarioNulo = {0};
        fwrite(&usuarioNulo, sizeof(TUsuario), 1, out);
    }
    fwrite(&livr->prox, sizeof(int), 1, out);
    fwrite(&livr->ocupado, sizeof(bool), 1, out);
}

// Le um livro do arquivo  na posicao atual do cursor
// Retorna um ponteiro para livro lido do arquivo
TLivro* le(FILE *in) {
    TLivro* livr = (TLivro *) malloc(sizeof(TLivro));
    if (0 >= fread(&livr->id, sizeof(int), 1, in)) {
        free(livr);
        return NULL;
    }
    fread(livr->nome, sizeof(char), sizeof(livr->nome), in);
    fread(livr->autor, sizeof(char), sizeof(livr->autor), in);
    fread(&livr->isEmprestado, sizeof(bool), 1, in);
    // Aloca memoria para o usuario associado e le seus dados
    livr->usuarioAssociado = (TUsuario *) malloc(sizeof(TUsuario));
    fread(livr->usuarioAssociado, sizeof(TUsuario), 1, in);
    fread(&livr->prox, sizeof(int), 1, in);
    fread(&livr->ocupado, sizeof(bool), 1, in);
    return livr;
}

char* sufixo(int num) {
    int last_digit = num % 10;
    int last_two_digits = num % 100;

    if (last_two_digits >= 11 && last_two_digits <= 13) {
        return "th"; // Excecoees para 11, 12, 13
    }

    switch (last_digit) {
        case 1: return "st";
        case 2: return "nd";
        case 3: return "rd";
        default: return "th";
    }
}

// Retorna tamanho do livro em bytes
int tamanho_registro() {
    return sizeof(int)  //cod
           + sizeof(char) * 50 //nome
           + sizeof(char) * 50 //autor
           + sizeof(bool) //isEmprestado
           + sizeof(TUsuario)
           + sizeof(int)
           + sizeof(bool);
}

// Retorna a quantidade de registros no arquivo
int tamanho_arquivo(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    int tam = trunc(ftell(arq) / tamanho_registro());
    return tam;
}

// Cria uma base de dados desordenada pelo ID do livro
void criarBaseDesordenada(FILE *out, int tam){

    rewind(out);

    int vetId[tam];
    TLivro* l;
    char nome[50];
    char autor[50];
    char ordinal[10];

    for(int i=0;i<tam;i++)
        vetId[i] = i+1;

    embaralha(vetId,tam);

    printf("\nGerando a base de dados de Livros...\n");

    for (int i=0;i<tam;i++){
        strcpy(ordinal, sufixo(vetId[i])); // Pega o id e coloca o sufixo de seu numero ordinal em ingles. (Ex: Se o id de um livro eh 1, seu numero ordinal eh 1st)
        snprintf(nome, sizeof(nome), "%d%s Book", vetId[i], ordinal); // Transforma o ID em string e concatena as strings do id e do numero ordinal do id na String do nome do livro. (Ex: Se o id de um livro eh 5, o nome do livro sera "5th Book")
        snprintf(autor, sizeof(autor), "Mr. %d", vetId[i]); // Transforma o ID em string e concatena a string do id na String do autor do livro. (Ex: Se o id de um livro eh 47, seu nome sera "Mr. 47")

        l = livro(vetId[i], nome, autor, false, -1, false);
        salva(l, out);
    }

    free(l);
}

//embaralha base de dados
void embaralha(int* vet,int tam) {
    srand(time(NULL));
    for (int i = tam - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = vet[i];
        vet[i] = vet[j];
        vet[j] = tmp;
    }
}

//imprime a base de dados
void imprimirBase(FILE *out){
    printf("\nImprimindo dados do arquivo...");

    rewind(out);
    TLivro* l;
    int cont = 0;

    while ((l = le(out)) != NULL){
        printf("\n\nPosicao %d\n", cont);
        imprime(l);
        cont++;
    }

    free(l);

}

// Imprime livro
void imprime(TLivro* livr) {
    if(livr == NULL){
        return;
    }
    else{
        printf("**********************************************");
        printf("\nID do livro: ");
        printf("%d", livr->id);
        printf("\nNome do livro: ");
        printf("%s", livr->nome);
        printf("\nAutor: ");
        printf("%s", livr->autor);
        printf("\nO livro esta emprestado? ");
        if(livr->isEmprestado){
            printf("Sim");
            printf("\n\nEmprestimo realizado para o Usuario:\n");
            printf("ID = %d, Nome = '%s', CPF = '%s'\n",
                    livr->usuarioAssociado->id,
                    livr->usuarioAssociado->nome,
                    livr->usuarioAssociado->cpf);
        }else{
            printf("Nao");
        }
        printf("\nPosicao proximo no: ");
        if(livr->prox != -1){
            printf("%d", livr->prox /= tamanho_registro());
        } else{
            printf("%d", livr->prox);
        }
        printf("\nOcupado?: ");
        if(livr->ocupado){
            printf("Sim");
        }else{
            printf("Nao");
        }
        printf("\n**********************************************");
    }
}

void cadastrar_livro(FILE *arq, int *tamanho){
    TLivro* livro_novo = (TLivro *) malloc(sizeof(TLivro));
    *tamanho += 1;

    fseek(arq, 0, SEEK_END);

    livro_novo->id = *tamanho;
    printf("\nDigite o nome do livro: ");
    fgets(livro_novo->nome, sizeof(livro_novo->nome), stdin);
    livro_novo->nome[strcspn(livro_novo->nome, "\n")] = '\0'; // Remove o '\n' do final
    fflush(stdin);
    printf("\nDigite o nome do autor do livro: ");
    fgets(livro_novo->autor, sizeof(livro_novo->autor), stdin);
    livro_novo->autor[strcspn(livro_novo->autor, "\n")] = '\0'; // Remove o '\n' do final

    livro_novo->isEmprestado = false;
    livro_novo->usuarioAssociado = NULL;
    livro_novo->prox = -1;
    livro_novo->ocupado = false;

    salva(livro_novo, arq);
    free(livro_novo);
}
