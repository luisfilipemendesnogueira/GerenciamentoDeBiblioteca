# Algoritmos e Estruturas de Dados II - Trabalho Prático
Sistema de Gerenciamento de Biblioteca em C.

IDE utilizada: Code::Blocks

## Parte 1
1 - Implemente estruturas para representar as entidades que fazem parte da aplicação
e crie bases de dados para estas entidades. As bases de dados devem ser criadas
desordenadas, de acordo com algum critério definido para identificar de forma única cada
entidade.

1. (As entidades desse trabalho são Livros e Usuários, o critério escolhido para identificar ambos é o ID)

2 - Implemente estratégias de buscas sequenciais e binárias para as iterar nas bases
definidas.

3 - Implemente algum método de ordenação em disco para ordenar as bases de dados, caso necessário. 

3. (Implementei o Selection Sort)

4 - Implemente operações que exemplique as interações entre as entidades da aplicação. 

4. (Implementei as funções de empréstimo e devolução de livros para os usuários)

## Parte 2
1 - Escolha e implemente um dos métodos de geração de partições ordenadas, sendo eles: Seleção por substituição; Seleção natural. 

1. (Implementei o seleção por substituição)

2 - Escolha e implemente um dos métodos de intercalação, sendo eles: Árvore Binária de vencedores; Intercalação ótima; 

2. (Implementei o intercalçaõ ótima)

# Parte 3
1 - Escolha e implemente APENAS UM dos métodos de gerenciamento de arquivos a seguir:

- Árvores B, incluindo funções para buscar, inserir e remover elementos.

- Árvores B+, incluindo funções para buscar, inserir e remover elementos.

- Tabelas Hash, incluindo funções para buscar, inserir e remover elementos. Especificar qual tipo de função hash será adotada e qual técnica será utilizada para o tratamento das colisões;

Obs: Implementar o método escolhido para tratar APENAS UMA entidade da aplicação 

1. (Implementei a tabela hash, a função adotada foi o método da divisão e a técnica utlizada para o tratamento de colisões foi o encadeamento exterior)
