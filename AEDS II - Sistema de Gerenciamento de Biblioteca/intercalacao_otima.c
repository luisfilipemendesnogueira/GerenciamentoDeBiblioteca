#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "livros.h"
#include "usuarios.h"
#include "intercalacao_otima.h"
#include "lista_livros.h"
#include "lista_usuarios.h"

// Estrutura para armazenar um livro e o arquivo associado
typedef struct vetor{
        TLivro *livr;   // Ponteiro para um livro
        FILE *f;        // Ponteiro para o arquivo
} TVet;

// Estrutura para armazenar um usuario e o arquivo associado
typedef struct vetor1{
        TUsuario *usu;  // Ponteiro para um usuario
        FILE *f;        // Ponteiro para o arquivo
} TVetUsuario;

// Variavel global para armazenar o ID do primeiro elemento da lista
int idPrimeiroElementoDaLista = 0;

// Funcao de intercalacao otima
void intercalacao_otima(FILE *out, int num_p){

    int particao = 0;   // Contador de particoes
    char *nome[20];     // Array para armazenar os nomes dos arquivos
    int F = 0;          // Numero de arquivos a serem manipulados
    int totalParticoes = num_p;  // Numero total de particoes

    TVet v[num_p];      // Array de estruturas TVet para armazenar livros e arquivos
    TListaArquivosLivros listaArqLivros;  // Lista de arquivos de livros

    FLVazia(&listaArqLivros);  // Inicializa a lista de arquivos como vazia

    // Abre cada arquivo de particao e carrega o primeiro registro
    for (int i = 0; i < num_p; i++) {
        sprintf(nome, "particao livros %i.dat", particao+1);  // Gera o nome do arquivo de particao

        v[i].f = fopen(nome, "rb");  // Abre o arquivo para leitura binaria

        // Se o arquivo foi aberto corretamente
        if(v[i].f != NULL){
            v[i].livr = le(v[i].f);  // Le o primeiro registro do arquivo
            Inserir(v[i].f, nome, &listaArqLivros);  // Insere o arquivo na lista de arquivos
        }
        particao++;  // Incrementa o contador de particoes
    }

    // Verifica se o numero de arquivos F e valido
    while(F-1 > num_p || F-1 <= 1){
        printf("\nDiga o numero de arquivos F que o algoritmo ira manipular\n");
        scanf("%d", &F);  // Le o numero de arquivos F

        // Exibe uma mensagem de erro se F for invalido
        if(F-1 > num_p || F-1 <= 1){
          printf("\nErro: O numero desejado de arquivos e invalido\n");
        }
    }

    // Criar e abrir F - 1 arquivos para leitura
    FILE *arquivos_entrada[F - 1];  // Array para armazenar os ponteiros dos arquivos de entrada
    FILE *arquivo_saida;  // Ponteiro para o arquivo de saida

    printf("\nCriando %d arquivos de entrada e %d de saida", F-1, F-(F-1));
    printf("\n********************************************************************************************");

    // Enquanto houver mais de um arquivo na lista
    while(listaArqLivros.tamanho > 1){
        // Abre F - 1 arquivos para leitura
        for(int i = 0; i < F-1; i++){
            arquivos_entrada[i] = NULL;  // Inicializa o ponteiro com NULL
            sprintf(nome, "Arquivo %d.dat", i + 1);  // Gera o nome do arquivo
            arquivos_entrada[i] = fopen(nome, "w+b");  // Abre o arquivo para leitura e escrita binaria
        }

        // Itera sobre os arquivos e copia os registros
        for(int i = 0; i < F-1; i++){
            arquivos_entrada[i] = NULL;  // Inicializa o ponteiro com NULL
            // Verifica se existe um proximo arquivo na lista
            if(listaArqLivros.primeiro->prox == NULL){
                arquivos_entrada[i] = NULL;
                // Se o numero de particoes for impar, ajusta o valor de F
                if(num_p % 2 == 1){
                   F = F-2;
                } else{
                   F = F-1;
                }
            } else{
                rewind(listaArqLivros.primeiro->prox->arquivo);  // Volta o ponteiro para o inicio do arquivo
                // Copia os registros do arquivo para a entrada
                while (!feof(listaArqLivros.primeiro->prox->arquivo)) {
                    TLivro *livro = le(listaArqLivros.primeiro->prox->arquivo);  // Le um registro
                    if (livro != NULL) {
                        salva(livro, arquivos_entrada[i]);  // Salva o registro no arquivo de entrada
                        free(livro);  // Libera a memoria alocada para o registro
                    }
                }
                ExcluirPrimeiroArquivo(&listaArqLivros);  // Remove o primeiro arquivo da lista
            }
        }

        // Incrementa o numero total de particoes
        totalParticoes++;
        // Gera o nome do novo arquivo de particao
        sprintf(nome, "particao livros %i.dat", totalParticoes);
        arquivo_saida = fopen(nome, "w+b");  // Abre o arquivo de saida para escrita binaria
        intercalar(arquivo_saida, F-1);  // Chama a funcao para intercalar os arquivos
        Inserir(arquivo_saida, nome, &listaArqLivros);  // Insere o arquivo de saida na lista
        printf("\nO conteudo desses arquivos quando intercalados geram o arquivo ""%s:""\n", nome);
        printf("\nConteudo de ""%s:""", nome);
        imprimirBase(arquivo_saida);  // Imprime o conteudo do arquivo de saida
    }

    rewind(arquivo_saida);  // Volta o ponteiro do arquivo de saida para o inicio

    // Abre o arquivo de saida final "livro.dat"
    if ((out = fopen("livro.dat", "w+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);  // Sai do programa se houver erro ao abrir o arquivo
    }

    // Copia os registros do arquivo de saida para o arquivo final
    while (!feof(arquivo_saida)) {
        TLivro *livro = le(arquivo_saida);  // Le um registro do arquivo de saida
        if (livro != NULL) {
            salva(livro, out);  // Salva o registro no arquivo final
            free(livro);  // Libera a memoria alocada para o registro
        }
    }

    ExcluirPrimeiroArquivo(&listaArqLivros);  // Remove o primeiro arquivo da lista

    // Fecha os arquivos de entrada
    for(int i = 0; i < F-1; i++){
        fclose(arquivos_entrada[i]);
    }

    fclose(arquivo_saida);  // Fecha o arquivo de saida
    fclose(out);  // Fecha o arquivo final

    idPrimeiroElementoDaLista = 0;  // Reseta o ID do primeiro elemento da lista
}

void intercalar(FILE *out, int num_p) {

    int fim = 0; // Variavel de controle que sinaliza quando o procedimento de intercalacao termina.
    char *nome[20]; // Buffer para armazenar o nome dos arquivos das particoes.

    // Cria um vetor de estruturas TVet que contem ponteiros para livros e arquivos.
    TVet v[num_p];

    printf("\n\n\nImprimindo o conteudo dos atuais %d primeiros numeros da lista:\n\n", num_p);

    // Abre os arquivos das particoes e carrega o primeiro livro de cada arquivo no vetor.
    for (int i = 0; i < num_p; i++) {
        // Gera o nome do arquivo da particao correspondente.
        sprintf(nome, "particao livros %i.dat", idPrimeiroElementoDaLista + 1);

        // Abre o arquivo binario da particao correspondente para leitura.
        v[i].f = fopen(nome, "rb");

        // Exibe o conteudo da particao atual (arquivo) para visualizacao.
        printf("\nConteudo de \"%s:\"", nome);
        imprimirBase(v[i].f); // Funcao que imprime o conteudo do arquivo.
        printf("\n\n");

        // Move o cursor do arquivo para o inicio, para garantir a leitura correta.
        rewind(v[i].f);

        // Se o arquivo foi aberto corretamente, le o primeiro livro.
        if (v[i].f != NULL) {
            TLivro *l = le(v[i].f); // Funcao que le o proximo livro do arquivo.

            // Se nao houver livros (o arquivo esta vazio), insere um valor alto (INT_MAX).
            if (l == NULL) {
                v[i].livr = livro(INT_MAX, "", "", false, -1, false); // Sinaliza fim do arquivo com "valor alto".
            } else {
                v[i].livr = l; // Caso contrario, armazena o livro no vetor.
            }
        } else {
            fim = 1; // Se o arquivo nao pode ser aberto, define fim como 1 (erro na abertura).
        }

        idPrimeiroElementoDaLista++; // Incrementa o indice para processar o proximo arquivo.
    }

    // Inicia a fase de intercalacao: continua enquanto `fim` nao for verdadeiro.
    while (!(fim)) {
        int menor = INT_MAX; // Variavel para encontrar o menor ID (chave).
        int pos_menor; // Variavel para armazenar a posicao do menor ID.

        // Encontra o livro com o menor ID entre as particoes.
        for (int i = 0; i < num_p; i++) {
            if (v[i].livr->id < menor) { // Compara os IDs dos livros atuais no vetor.
                menor = v[i].livr->id; // Atualiza o menor ID encontrado.
                pos_menor = i; // Armazena a posicao do livro com menor ID.
            }
        }

        // Se o menor valor for INT_MAX, significa que todos os arquivos foram processados.
        if (menor == INT_MAX) {
            fim = 1; // Termina o procedimento de intercalacao.
        } else {
            // Salva o livro com o menor ID no arquivo de saida.
            salva(v[pos_menor].livr, out);

            // Atualiza a posicao `pos_menor` com o proximo livro do arquivo.
            TLivro *l = le(v[pos_menor].f); // Le o proximo livro da particao correspondente.

            // Se o proximo livro for NULL, o arquivo acabou, entao insere um valor alto (INT_MAX).
            if (l == NULL) {
                v[pos_menor].livr = livro(INT_MAX, "", "", false, -1, false); // Marca o fim do arquivo.
            } else {
                v[pos_menor].livr = l; // Atualiza o vetor com o proximo livro.
            }
        }
    }

    // Fecha os arquivos de todas as particoes de entrada apos o termino da intercalacao.
    for (int i = 0; i < num_p; i++) {
        fclose(v[i].f); // Fecha cada arquivo aberto.
    }
}

void intercalacao_otimaUsuario(FILE *out, int num_p){

    int particao = 0;
    char *nome[20];
    int F = 0;
    int totalParticoes = num_p;

    TVetUsuario v[num_p];
    TListaArquivosUsuarios listaArqUsuarios;

    FLVazia(&listaArqUsuarios);

    for (int i=0; i < num_p; i++) {
        sprintf(nome, "particao usuarios %i.dat", particao+1);

        v[i].f = fopen(nome, "rb");

        if(v[i].f != NULL){
            v[i].usu = leUsuario(v[i].f);
            InserirUsuario(v[i].f, nome, &listaArqUsuarios);
        }
        particao++;
    }

    while(F-1 > num_p || F-1 <= 1){
        printf("\nDiga o numero de arquivos F que o algoritmo ira manipular\n");
        scanf("%d", &F);

        if(F-1 > num_p || F-1 <= 1){
          printf("\nErro: O numero desejado de arquivos e invalido\n");
        }
    }

    // Criar e abrir F - 1 arquivos para leitura
    FILE *arquivos_entrada[F - 1];
    FILE *arquivo_saida;

    printf("\nCriando %d arquivos de entrada e %d de saida", F-1, F-(F-1));
    printf("\n********************************************************************************************");

    while(listaArqUsuarios.tamanho > 1){
        for(int i=0; i< F-1; i++){
            arquivos_entrada[i] = NULL;
            sprintf(nome, "Arquivo Usuario %d.dat", i + 1);
            arquivos_entrada[i] = fopen(nome, "w+b");
        }

        for(int i=0; i< F-1; i++){
            arquivos_entrada[i] = NULL;
            if(listaArqUsuarios.primeiro->prox == NULL){
                arquivos_entrada[i] = NULL;
                if(num_p % 2 == 1){
                   F = F-2;
                } else{
                   F = F-1;
                }
            } else{
                rewind(listaArqUsuarios.primeiro->prox->arquivo);
                while (!feof(listaArqUsuarios.primeiro->prox->arquivo)) {
                    TUsuario *usuario = leUsuario(listaArqUsuarios.primeiro->prox->arquivo);
                    if (usuario != NULL) {
                        salvaUsuario(usuario, arquivos_entrada[i]);
                        free(usuario);
                    }
                }
                ExcluirPrimeiroArquivoUsuario(&listaArqUsuarios);
            }
        }

        totalParticoes++;
        sprintf(nome, "particao usuarios %i.dat", totalParticoes);
        arquivo_saida = fopen(nome, "w+b");
        intercalarUsuario(arquivo_saida, F-1);
        InserirUsuario(arquivo_saida, nome, &listaArqUsuarios);
        printf("\nO conteudo desses arquivos quando intercalados geram o arquivo ""%s:""\n", nome);
        printf("\nConteudo de ""%s:""", nome);
        imprimirBaseUsuario(arquivo_saida);
    }

    rewind(arquivo_saida);

    if ((out = fopen("usuario.dat", "w+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    while (!feof(arquivo_saida)) {
        TUsuario *usuario = leUsuario(arquivo_saida);
        if (usuario != NULL) {
            salvaUsuario(usuario, out);
            free(usuario);
        }
    }


    ExcluirPrimeiroArquivoUsuario(&listaArqUsuarios);
    ImprimirUsuario(listaArqUsuarios);

    for(int i=0; i< F-1; i++){
        fclose(arquivos_entrada[i]);
    }

    fclose(arquivo_saida);
    fclose(out);

    idPrimeiroElementoDaLista = 0;
}

void intercalarUsuario(FILE *out, int num_p){

    int fim = 0; //variavel que controla fim do procedimento
    char *nome[20];

    //cria vetor de particoes
    TVetUsuario v[num_p];

    printf("\n\n\nImprimindo o conteudo dos atuais %d primeiros numeros da lista:\n\n", num_p);

    //abre arquivos das particoes, colocando variavel de arquivo no campo f do vetor
    //e primeiro usuario do arquivo no campo usu do vetor
    for (int i=0; i < num_p; i++) {
        sprintf(nome, "particao usuarios %i.dat", idPrimeiroElementoDaLista+1);

        v[i].f = fopen(nome, "rb");

        printf("\nConteudo de ""%s:""", nome);
        imprimirBaseUsuario(v[i].f);
        printf("\n\n");

        rewind(v[i].f);
        if (v[i].f != NULL) {
            TUsuario *u= leUsuario(v[i].f);
            if (u == NULL) {
                //se o arquivo estiver vazio
                //coloca HIGH VALUE nessa posicao do vetor
                v[i].usu = usuario(INT_MAX, "","", false);
            }
            else {
                //conseguiu ler usuario, coloca na posicao atual do vetor
                v[i].usu = u;
            }
        }
        else {
            fim = 1;
        }

        idPrimeiroElementoDaLista++;
    }

    while (!(fim)) { //conseguiu abrir todos os arquivos
        int menor = INT_MAX;
        int pos_menor;
        //encontra o usuario com menor chave (ID) no vetor
        for(int i = 0; i < num_p; i++){
            if(v[i].usu->id < menor){
                menor = v[i].usu->id;
                pos_menor = i;
            }
        }
        if (menor == INT_MAX) {
            fim = 1; //terminou processamento
        }
        else {
            //salva funcionario no arquivo de saida
            salvaUsuario(v[pos_menor].usu, out);
            //printf("%d ",pos_menor);
            //atualiza posicao pos_menor do vetor com proximo usuario do arquivo
            TUsuario *u = leUsuario(v[pos_menor].f);
            if (u == NULL) {
                //arquivo estava vazio
                //coloca HIGH VALUE nessa posi�ao do vetor
                v[pos_menor].usu = usuario(INT_MAX, "", "", false);
            }
            else {
                v[pos_menor].usu = u;
            }
        }
    }

    //fecha arquivos das particoes de entrada
    for(int i = 0; i < num_p; i++){
        fclose(v[i].f);
    }
}

