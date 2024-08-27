#ifndef BUSCASEQUENCIAL_H_INCLUDED
#define BUSCASEQUENCIAL_H_INCLUDED

//Realiza uma busca sequencial por um livro na base de dados
TLivro* buscaSequencial(int chave, FILE *in);

//Realiza uma busca sequencial por um usuario na base de dados
TUsuario* buscaSequencialUsuario(int chave, FILE *in);


#endif // BUSCASEQUENCIAL_H_INCLUDED
