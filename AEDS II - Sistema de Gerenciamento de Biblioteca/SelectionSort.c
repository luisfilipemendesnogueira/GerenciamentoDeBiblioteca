#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "livros.h"
#include "usuarios.h"
#include "SelectionSort.h"

void selection_sort(FILE *arq, int tam){
    int i, j, menor;
    for(i=0; i < tam - 1; i++){
        menor = i;
        for(j = i+1; j < tam; j++){
            fseek(arq, j * tamanho_registro(), SEEK_SET);
            TLivro* lj = le(arq);

            fseek(arq, menor * tamanho_registro(), SEEK_SET);
            TLivro* li = le(arq);

            if(lj->id < li->id){
                menor = j;
            }

            free(lj);
            free(li);
        }

        if(menor != i){
            fseek(arq, i * tamanho_registro(), SEEK_SET);
            TLivro* old_i = le(arq);

            fseek(arq, menor * tamanho_registro(), SEEK_SET);
            TLivro* new_i = le(arq);

            fseek(arq, i * tamanho_registro(), SEEK_SET);
            salva(new_i, arq);

            fseek(arq, menor * tamanho_registro(), SEEK_SET);
            salva(old_i, arq);

            free(old_i);
            free(new_i);
        }
    }

    fflush(arq);
}

void selection_sort_Usuario(FILE *arq, int tam){
    int i, j, menor;
    for(i=0; i < tam - 1; i++){
        menor = i;
        for(j = i+1; j < tam; j++){
            fseek(arq, j * tamanho_registroUsuario(), SEEK_SET);
            TUsuario* uj = leUsuario(arq);

            fseek(arq, menor * tamanho_registroUsuario(), SEEK_SET);
            TUsuario* ui = leUsuario(arq);

            if(uj->id < ui->id){
                menor = j;
            }

            free(uj);
            free(ui);
        }

        if(menor != i){
            fseek(arq, i * tamanho_registroUsuario(), SEEK_SET);
            TUsuario* old_i = leUsuario(arq);

            fseek(arq, menor * tamanho_registroUsuario(), SEEK_SET);
            TUsuario* new_i = leUsuario(arq);

            fseek(arq, i * tamanho_registroUsuario(), SEEK_SET);
            salvaUsuario(new_i, arq);

            fseek(arq, menor * tamanho_registroUsuario(), SEEK_SET);
            salvaUsuario(old_i, arq);

            free(old_i);
            free(new_i);
        }
    }

    fflush(arq);
}
