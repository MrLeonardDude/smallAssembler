/*
 * Autor : Leonardo Maldonado Pagnez
 * RA : 172017
*/

#include "token.h"
#include <stdio.h>
#include <stdlib.h>


unsigned getNumberOfTokens() {
  return tamTokens;
}

unsigned adicionarToken(Token novoToken) {
  if (tamTokens >= 4095) {
    printf("N�o h� mais espa�o na lista de tokens!\n");
    exit(1);
  }

  tokens[tamTokens] = novoToken;

  return tamTokens++;
}

void removerToken(unsigned pos) {
  if (pos > tamTokens) {
    printf("Voc� tentou acessar uma posi��o fora dos limites da lista de tokens!\n");
    exit(1);
  }


  for (unsigned i = pos; i < tamTokens-1; i++)
    tokens[i] = tokens[i+1];

  --tamTokens;
}

Token recuperaToken(unsigned pos) {
  if (pos > tamTokens) {
    printf("Voc� tentou acessar uma posi��o fora dos limites da lista de tokens!\n");
    exit(1);
  }

  return tokens[pos];
}

void imprimeListaTokens() {
  printf("Lista de tokens\n");
  for (unsigned i = 0; i < tamTokens; i++) {
    printf("Token %d\n", i);
    printf("\tTipo: %d\n", (int) tokens[i].tipo);
    printf("\tPalavra: %s\n", tokens[i].palavra);
    printf("\tLinha: %u\n", tokens[i].linha);
  }
}
