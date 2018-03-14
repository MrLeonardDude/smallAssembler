/*
 * Autor : Leonardo Maldonado Pagnez
 * RA : 172017
*/
#ifndef TOKEN_H_
#define TOKEN_H_

typedef enum TipoDoToken {Instrucao, Diretiva, DefRotulo, Hexadecimal, Decimal, Nome} TipoDoToken;

typedef struct Token {
	TipoDoToken tipo;
	char* palavra;
	unsigned linha;
} Token;

static Token tokens[4096];
static unsigned tamTokens = 0;


unsigned adicionarToken(Token novoToken);
void removerToken(unsigned pos);
Token recuperaToken(unsigned pos);
void imprimeListaTokens();
unsigned getNumberOfTokens();



#endif /* TOKEN_H_ */
