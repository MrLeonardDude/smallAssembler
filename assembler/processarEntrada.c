/*
 * Autor : Leonardo Maldonado Pagnez
 * RA : 172017
*/
#include "montador.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ERROLEXICAL 2
#define ERROGRAMATICAL 3
/*
 * Argumentos:
 *  entrada: cadeia de caracteres com o conteudo do arquivo de entrada.
 *  tamanho: tamanho da cadeia.
 * Retorna:
 *  1 caso haja erro na montagem;
 *  0 caso não haja erro.
 */

static unsigned esperarHex = 0;

static unsigned esperarDec = 0;

static unsigned esperarRot = 0;

static unsigned esperarNome = 0;

static unsigned esperarProx = 0;

static unsigned umOuOutro = 0;

static unsigned antesInstru = 0;

static unsigned podeDefRotulo = 1;

unsigned isComment(char* palavra, unsigned inicio){
	if( palavra[inicio] == '#')
		return 1;
	return 0;
}

unsigned isDefRotulo(char* palavra){
	unsigned i;

	// Checar se eh uma palavra permitida
	for(i = 0; palavra[i] != '\0'; i++ ){

		if(i == 1 &&  ( i > 1 && palavra[i] >= '0' && palavra[i] <= '9' )	){
			return ERROLEXICAL;
		}
		if(palavra[i+1] == '\0' && palavra[i] != ':'){
			return 0;
		}

		if(i > 1 && palavra[i+1] !='\0' && palavra[i] == ':')
			return ERROLEXICAL;


		if( palavra[i+1] != '\0' && ('a' > palavra[i] || palavra[i] > 'z' )	&& ('0' > palavra[i] || palavra[i] > '9') && ('A' > palavra[i] || palavra[i] > 'Z') ){
			return 0;
		}
	}



	if(esperarHex == 1 || esperarDec == 1 || esperarRot == 1 || esperarNome == 1 ){
		return ERROGRAMATICAL;
	}

	if(podeDefRotulo)
		podeDefRotulo = 0;
	else
		return ERROGRAMATICAL;
	return 1;

}

unsigned retirarAspas(char* palavra){

	for(unsigned i = 0; palavra[i] != '\0'; i++){
		if(i == 0 && palavra[i] == '"'){
			for(unsigned j = 0; palavra[j] != '\0'; j++)
				palavra[j] = palavra[j+1];
		}

		if(palavra[i+1] == '\0' && palavra[i] == '"'){
			palavra[i] = palavra[i+1];
			break;
		}
	}

	return 1;
}

unsigned isDiretiva(char* palavra){

	char strs [] = ".org";
	char strs1 [] = ".align";
	char strs2 [] = ".wfill";
	char strs3 [] = ".set";
	char strs4 [] = ".word";

	if( (strcmp(palavra, strs) == 0)){
		if(esperarHex == 1 || esperarDec == 1 || esperarRot == 1 || esperarNome == 1 )
				return ERROGRAMATICAL;
		esperarHex = 1;
		esperarDec = 1;
		umOuOutro = 1;
		return 1;
	}

	else if( ( strcmp(palavra, strs1)  == 0) ){
		if(esperarHex == 1 || esperarDec == 1 || esperarRot == 1 || esperarNome == 1 )
				return ERROGRAMATICAL;
		esperarDec = 1;
		return 1;
	}

	else if( (strcmp(palavra, strs2)  == 0)){
		if(esperarHex == 1 || esperarDec == 1 || esperarRot == 1 || esperarNome == 1 )
				return ERROGRAMATICAL;
		esperarDec = 1;
		esperarProx = 1;
		return 1;
	}

	else if( (strcmp(palavra, strs3)  == 0) ){
		if(esperarHex == 1 || esperarDec == 1 || esperarRot == 1 || esperarNome == 1 )
				return ERROGRAMATICAL;
		esperarNome = 1;
		esperarProx = 1;
		return 1;
	}

	else if( ( strcmp(palavra, strs4)  == 0)){
		if(esperarHex == 1 || esperarDec == 1 || esperarRot == 1 || esperarNome == 1 )
			return ERROGRAMATICAL;
		esperarHex = 1;
		esperarDec = 1;
		esperarRot = 1;
		esperarNome = 1;
		umOuOutro = 1;
		return 1;
	}

	return 0;
}

unsigned isInstrucao(char* palavra){


	char strsL1 [] = "LOAD";
	char strsL2 [] = "LOAD-";
	char strsL3 [] = "LOAD|";
	char strsL4 [] = "LOADmq";
	char strsL5 [] = "LOADmq_mx";
	char strsS1 [] = "STOR";
	char strsS2 [] = "STORA";
	char strsJ1 [] = "JUMP";
	char strsJ2 [] = "JMP+";
	char strsA1 [] = "ADD";
	char strsA2 [] = "ADD|";
	char strsSu1 [] = "SUB";
	char strsSu2 [] = "SUB|";
	char strsM1 [] = "MUL";
	char strsD1 [] = "DIV";
	char strsL [] = "LSH";
	char strsR1 [] = "RSH";


	if( (strcmp(palavra, strsL1)  == 0)){
		if(esperarHex == 1 || esperarDec == 1 || esperarRot == 1 || esperarNome == 1 )
			return ERROGRAMATICAL;
		antesInstru = 1;
		esperarHex = 1;
		esperarNome = 1;
		esperarDec = 1;
		umOuOutro = 1;
		return 1;
	}
	else if( ( strcmp(palavra, strsL2)  == 0) ){
		if(esperarHex == 1 || esperarDec == 1 || esperarRot == 1 || esperarNome == 1 )
			return ERROGRAMATICAL;
		antesInstru = 1;
		esperarHex = 1;
		esperarNome = 1;
		esperarDec = 1;
		umOuOutro = 1;
		return 1;
	}

	else if( (strcmp(palavra, strsL3) == 0)){
		if(esperarHex == 1 || esperarDec == 1 || esperarRot == 1 || esperarNome == 1 )
			return ERROGRAMATICAL;

		antesInstru = 1;
		esperarHex = 1;
		esperarNome = 1;
		esperarDec = 1;
		umOuOutro = 1;
		return 1;
	}
	else if( ( strcmp(palavra, strsL4)  == 0)  ){
		if(esperarHex == 1 || esperarDec == 1 || esperarRot == 1 || esperarNome == 1 )
			return ERROGRAMATICAL;
		return 1;
	}
	else if( (strcmp(palavra, strsL5)  == 0) ){
		if(esperarHex == 1 || esperarDec == 1 || esperarRot == 1 || esperarNome == 1 )
			return ERROGRAMATICAL;

		antesInstru = 1;
		esperarHex = 1;
		esperarNome = 1;
		esperarDec = 1;
		umOuOutro = 1;
		return 1;
	}

	else if( (strcmp(palavra, strsS1)  == 0) ){
		if(esperarHex == 1 || esperarDec == 1 || esperarRot == 1 || esperarNome == 1 )
			return ERROGRAMATICAL;

		antesInstru = 1;
		esperarHex = 1;
		esperarNome = 1;
		esperarDec = 1;
		umOuOutro = 1;
		return 1;
	}

	else if( ( strcmp(palavra, strsS2)  == 0) ){
		if(esperarHex == 1 || esperarDec == 1 || esperarRot == 1 || esperarNome == 1 )
			return ERROGRAMATICAL;
		antesInstru = 1;
		esperarHex = 1;
		esperarNome = 1;
		esperarDec = 1;
		umOuOutro = 1;
		return 1;
	}
	else if( (strcmp(palavra, strsJ1)  == 0) ){
		if(esperarHex == 1 || esperarDec == 1 || esperarRot == 1 || esperarNome == 1 )
			return ERROGRAMATICAL;

		antesInstru = 1;
		esperarHex = 1;
		esperarNome = 1;
		esperarDec = 1;
		umOuOutro = 1;
		return 1;
	}

	else if( (strcmp(palavra, strsJ2)  == 0) ){
		if(esperarHex == 1 || esperarDec == 1 || esperarRot == 1 || esperarNome == 1 )
			return ERROGRAMATICAL;

		antesInstru = 1;
		esperarHex = 1;
		esperarNome = 1;
		esperarDec = 1;
		umOuOutro = 1;
		return 1;
	}

	else if( (strcmp(palavra, strsA1)  == 0) ){
		if(esperarHex == 1 || esperarDec == 1 || esperarRot == 1 || esperarNome == 1 )
			return ERROGRAMATICAL;
		antesInstru = 1;
		esperarHex = 1;
		esperarNome = 1;
		esperarDec = 1;
		umOuOutro = 1;
		return 1;
	}

	else if( (strcmp(palavra, strsA2)  == 0) ){
		if(esperarHex == 1 || esperarDec == 1 || esperarRot == 1 || esperarNome == 1 )
			return ERROGRAMATICAL;
		antesInstru = 1;
		esperarHex = 1;
		esperarNome = 1;
		esperarDec = 1;
		umOuOutro = 1;
		return 1;
	}

	else if( (strcmp(palavra, strsSu1)  == 0) ){
		if(esperarHex == 1 || esperarDec == 1 || esperarRot == 1 || esperarNome == 1 )
			return ERROGRAMATICAL;

		antesInstru = 1;
		esperarHex = 1;
		esperarNome = 1;
		esperarDec = 1;
		umOuOutro = 1;
		return 1;
	}

	else if( (strcmp(palavra, strsSu2)  == 0) ){
		if(esperarHex == 1 || esperarDec == 1 || esperarRot == 1 || esperarNome == 1 )
			return ERROGRAMATICAL;

		antesInstru = 1;
		esperarHex = 1;
		esperarNome = 1;
		esperarDec = 1;
		umOuOutro = 1;
		return 1;
	}

	else if( (strcmp(palavra, strsM1)  == 0) ){
		if(esperarHex == 1 || esperarDec == 1 || esperarRot == 1 || esperarNome == 1 )
			return ERROGRAMATICAL;
		antesInstru = 1;
		esperarHex = 1;
		esperarNome = 1;
		esperarDec = 1;
		umOuOutro = 1;
		return 1;
	}

	else if( (strcmp(palavra, strsD1)  == 0) ){
		if(esperarHex == 1 || esperarDec == 1 || esperarRot == 1 || esperarNome == 1 )
			return ERROGRAMATICAL;

		antesInstru = 1;
		esperarHex = 1;
		esperarNome = 1;
		esperarDec = 1;
		umOuOutro = 1;
		return 1;
	}

	else if( (strcmp(palavra, strsL)  == 0)){
		if(esperarHex == 1 || esperarDec == 1 || esperarRot == 1 || esperarNome == 1 )
			return ERROGRAMATICAL;

		return 1;
	}

	else if( ( strcmp(palavra, strsR1)  == 0) ){
		if(esperarHex == 1 || esperarDec == 1 || esperarRot == 1 || esperarNome == 1 )
			return ERROGRAMATICAL;

		return 1;
	}


	return 0;
}

unsigned isDecimal(char* palavra){

	unsigned i;


	for(i = 0; palavra[i] != '\0'; i++ ){

		if ( i == 0 && 	(palavra[i] != '"' && !('0' <= palavra[i] && '9' >= palavra[i])  )   )
			return 0;

		if(i > 0 &&	palavra[i+1] != '\0' && !('0' <= palavra[i] && '9' >= palavra[i])	){
			return 0;
		}


		/*if(palavra[i] == ')' && flag)
			break;

		if(palavra[i] == '('){
			flag = 1;
			while(palavra[i] != ')'){
				if(	(palavra[i] < '0' || palavra[i] > '9') && palavra[i] != ':')
						return 0;
				i++;
			}
		}*/

		if(palavra[i+1] == '\0' && (palavra[i] != '"') && !('0' <= palavra[i] && '9' >= palavra[i])	)
			return ERROLEXICAL;
	}

	if(antesInstru)
		antesInstru =0;


	if(esperarDec == 0)
		return ERROGRAMATICAL;

	if(umOuOutro){
		esperarHex = 0;
		esperarDec = 0;
		esperarRot = 0;
		esperarNome = 0;
		umOuOutro = 0;
	}

	if(esperarProx){
			esperarProx = 0;
			esperarDec = 1;
			esperarHex = 1;
			esperarRot = 1;
			esperarNome = 1;
			umOuOutro = 1;
		}

	return 1;
}

unsigned isHexadecimal(char* palavra){

	unsigned i;

	for(i = 0; palavra[i] != '\0'; i++ ){

		if( i == 0 && palavra[i] != '"' &&  palavra[i] != '0' ){
			return 0;
		}

		else if(i == 1 && palavra[i] != '0' && palavra[i] != 'x'){
			return 0;
		}

		else if(i == 2 && palavra[i] != 'x' && !((palavra[i] >= '0' && palavra[i] <= '9') || (palavra[i] >= 'A' && palavra[i] <= 'F') ) ){
			return 0;
		}

		else if(i > 2 && palavra[i+1] != '\0' && !((palavra[i] >= '0' && palavra[i] <= '9') || (palavra[i] >= 'A' && palavra[i] <= 'F') )){
			return ERROLEXICAL;
		}

		if(palavra[i+1] == '\0' && (palavra[i] != '"') && !(('0' <= palavra[i] && '9' >= palavra[i]) || ('A' <= palavra[i] && 'F' >= palavra[i] ) )	){
			return ERROLEXICAL;
		}


	}

	if(esperarHex == 0)
		return ERROGRAMATICAL;

	if(umOuOutro){
		esperarHex = 0;
		esperarDec = 0;
		esperarRot = 0;
		esperarNome = 0;
		umOuOutro = 0;
	}

	if(antesInstru)
		antesInstru =0;

	if(esperarProx){
			esperarProx = 0;
			esperarDec = 1;
			esperarHex = 1;
			esperarRot = 1;
			esperarNome = 1;
			umOuOutro = 1;
		}

	return 1;

}

unsigned isNome(char* palavra){

	unsigned i;

	// Checar se eh uma palavra permitida
	for(i = 0; palavra[i] != '\0'; i++ ){

		if(i == 0 && palavra[i] != '"' && (palavra[i] <= '0' && palavra[i] >= '9') && !((palavra[i] >= 'A' && palavra[i] <= 'Z') || (palavra[i] >= 'a' && palavra[i] <= 'z'))){
			return 0;
		}

		if( i == 1  && (palavra[i] <= '0' && palavra[i] >= '9' && palavra[0] == '"') && !((palavra[i] >= 'A' && palavra[i] <= 'Z') || (palavra[i] >= 'a' && palavra[i] <= 'z') )   ){
			return 0;
		}

		if(i > 1 && palavra[i+1] != '\0' && !((palavra[i] >= 'A' && palavra[i] <= 'Z') || (palavra[i] >= '0' && palavra[i] <= '9' ) || (palavra[i] >= 'a' && palavra[i] <= 'z')) ){
			return 0;
		}

		if(palavra[i+1] == '\0' && palavra[i] != '"' && !((palavra[i] >= 'A' && palavra[i] <= 'Z') || (palavra[i] >= 'a' && palavra[i] <= 'z'))){
			return 0;
		}
	}

	if(esperarNome == 0 && esperarRot == 0)
		return ERROGRAMATICAL;



	if(umOuOutro){
		esperarHex = 0;
		esperarDec = 0;
		esperarRot = 0;
		esperarNome = 0;
		umOuOutro = 0;
	}
	if(antesInstru)
		antesInstru =0;

	if(esperarProx){
		esperarProx = 0;
		esperarDec = 1;
		esperarHex = 1;
		esperarRot = 1;
		esperarNome = 1;
		umOuOutro = 1;
	}

	//Existe no banco de tokens, ou seja, ja foi definido
	/*for(i = 0; i < tamTokens; i++){
		if ( 	strcmp (palavra, tokens[i].palavra) == 0	){
			return 1;
		}
	}*/
	return 1;
}

unsigned preencherPalavra(char* source, char* dest, unsigned inicio, unsigned fim){

	unsigned i;

	for(i = 0 ;inicio + i < fim; i++){
		dest[i] = source[inicio + i];
	}

	dest[i] = (char) '\0';

	return 1;
}

unsigned identificarErro(){

	if(esperarHex == 1 || esperarDec == 1 || esperarRot == 1 || esperarNome == 1 )
		return ERROLEXICAL;
	return 0;
}

int processarEntrada(char* entrada, unsigned tamanho){
/*antesInstru = 1;
		esperarHex = 1;
		esperarNome = 1;
		esperarDec = 1;
		umOuOutro = 1;
 *	Ideia Geral:
 *		1. Encontra-se uma palavra.
 *      2. Determina se eh um rótulo, instrução, diretiva, comentário ou Fim da Linha. Volta p 1
 *		3. Se eh um rótulo, a próxima palavra eh uma diretiva, instrução, comentário ou Fim da Linha. Volta p 1
 *		4. Se eh uma diretiva ou instrução, a próxima palavra eh um comentário ou Fim da Linha. Volta p 1
 *      5. Se eh um comentário, ignorar ateh encontrar Fim da Linha. Volta p 1
 *		6. Se eh o Fim da Linha, contabilize o final da linha e comece a próxima linha e volte para 1 enquanto char lidos forem menor que tamanho.
 *      7. Se não eh nenhum dos anteriores, trate o erro encontrado como Lehxico ou Gramatical.
 */
	unsigned TokenControler = 0;
	unsigned pointerVetor = 0;
	unsigned tamanhoCurrPalavra = 0;
	unsigned inicioPalavra = 0;
	unsigned currLinha = 1;
	unsigned check = 0;
	Token TokenAdd[4096];
	char* palavra = malloc (	sizeof (char) *(4096 + 1)	);

	for(unsigned f = 0; f < 4096; f++)
		TokenAdd[f].palavra = malloc( sizeof(char) * (4096+1) );

	if(entrada[pointerVetor] == '\n')
		currLinha++;

	// Percorrer a entrada
	while( entrada[pointerVetor] != '\0'){

		if( entrada[pointerVetor] == '\n'){
			pointerVetor++;
		}

		podeDefRotulo = 1;
		//Percorrer uma linha
		while( entrada[pointerVetor] != '\0' && entrada[pointerVetor] != '\n'){

			while(entrada[pointerVetor] == ' ')
				pointerVetor++;

			if(entrada[pointerVetor] == '\0')
				break;

			if(entrada[pointerVetor] == '\n'){
				tamanhoCurrPalavra = 0;
				break;
			}

			//Percorre a palavra encontrada
			while( entrada[pointerVetor] != ' ' && entrada[pointerVetor] != '\n'){
				tamanhoCurrPalavra++;
				pointerVetor++;
			}
			//voltar para pegar a palavra
			inicioPalavra = pointerVetor - tamanhoCurrPalavra;


			preencherPalavra(entrada, palavra, inicioPalavra, pointerVetor);

			//testar se eh um comentario
			if( 	isComment(entrada, inicioPalavra)	){

				while(entrada[pointerVetor] != '\n' && entrada[pointerVetor] != '\0')
					pointerVetor++;

				tamanhoCurrPalavra = 0;
				break;
			}


			else if( (check = isDecimal(palavra)) ){

				if( check == ERROLEXICAL){
					printf("ERRO LEXICO: palavra inválida na linha %d!\n", currLinha);
					return 1;
				}
				else if( check == ERROGRAMATICAL){
					printf("ERRO GRAMATICAL: palavra na linha %d!\n", currLinha);
					return 1;
				}
				//N eh erro
				TokenAdd[TokenControler].tipo = Decimal;
				preencherPalavra(entrada, TokenAdd[TokenControler].palavra, inicioPalavra, pointerVetor);
				TokenAdd[TokenControler].linha = currLinha;
				retirarAspas(TokenAdd[TokenControler].palavra);
				adicionarToken(TokenAdd[TokenControler++]);
				//printf("eh decimal : %s\n", TokenAdd[TokenControler-1].palavra);
			}

			else if(	(check = isHexadecimal(palavra))	){

				if( check == ERROLEXICAL){
					printf("ERRO LEXICO: palavra inválida na linha %d!", currLinha);
					return 1;
				}
				else if( check == ERROGRAMATICAL){
									printf("ERRO GRAMATICAL: palavra na linha %d!", currLinha);
									return 1;
								}
				//N eh erro

				TokenAdd[TokenControler].tipo = Hexadecimal;
				preencherPalavra(entrada, TokenAdd[TokenControler].palavra, inicioPalavra, pointerVetor);
				retirarAspas(TokenAdd[TokenControler].palavra);
				TokenAdd[TokenControler].linha = currLinha;
				adicionarToken(TokenAdd[TokenControler++]);
				//printf("eh Hexadecimal :%s\n", TokenAdd[TokenControler-1].palavra);
			}

			else if( (	check = isDiretiva(palavra)	) ){
				if( check == ERROLEXICAL){
					printf("ERRO LEXICO: palavra inválida na linha %d!\n", currLinha);
					return 1;
				}
				else if( check == ERROGRAMATICAL){
					printf("ERRO GRAMATICAL: palavra na linha %d!\n", currLinha);
					return 1;
				}

				//n eh erro
				TokenAdd[TokenControler].tipo = Diretiva;
				preencherPalavra(entrada, TokenAdd[TokenControler].palavra, inicioPalavra, pointerVetor);
				TokenAdd[TokenControler].linha = currLinha;
				adicionarToken(TokenAdd[TokenControler++]);
				//printf("eh Diretiva: %s\n", TokenAdd[TokenControler-1].palavra);
			}

			else if( (check = isInstrucao(palavra) )	){

				if( check == ERROLEXICAL){
					printf("ERRO LEXICO: palavra inválida na linha %d!\n", currLinha);
					return 1;
				}
				else if( check == ERROGRAMATICAL){
					printf("ERRO GRAMATICAL: palavra na linha %d!\n", currLinha);
					return 1;
				}
				//N eh erro
				TokenAdd[TokenControler].tipo = Instrucao;
				preencherPalavra(entrada, TokenAdd[TokenControler].palavra, inicioPalavra, pointerVetor);
				TokenAdd[TokenControler].linha = currLinha;
				adicionarToken(TokenAdd[TokenControler++]);
				//printf("eh Instrucao :%s\n", TokenAdd[TokenControler-1].palavra);
			}


			else if( (	check = isNome(palavra)	 ) ){

				if( check == ERROLEXICAL){
					printf("ERRO LEXICO: palavra inválida na linha %d!\n", currLinha);
					return 1;
				}

				else if( check == ERROGRAMATICAL){
					printf("ERRO GRAMATICAL: palavra na linha %d!\n", currLinha);
					return 1;
				}
				//N eh erro
				TokenAdd[TokenControler].tipo = Nome;
				preencherPalavra(entrada, TokenAdd[TokenControler].palavra, inicioPalavra, pointerVetor);
				TokenAdd[TokenControler].linha = currLinha;
				retirarAspas(TokenAdd[TokenControler].palavra);
				adicionarToken(TokenAdd[TokenControler++]);
				//printf("eh nome : %s\n", TokenAdd[TokenControler-1].palavra);
			}

			//testar se eh um rotulo
			else if( (check = isDefRotulo(palavra))	){

				if( check == ERROLEXICAL){
					printf("ERRO LEXICO: palavra inválida na linha %d!\n", currLinha);
					return 1;
				}
				else if( check == ERROGRAMATICAL){
					printf("ERRO GRAMATICAL: palavra na linha %d!\n", currLinha);
					return 1;
				}

				//N eh erro
					TokenAdd[TokenControler].tipo = DefRotulo;
					preencherPalavra(entrada, TokenAdd[TokenControler].palavra, inicioPalavra, pointerVetor);
					TokenAdd[TokenControler].linha = currLinha;
					adicionarToken(TokenAdd[TokenControler++]);
					//printf("eh DefRotulo : %s\n", TokenAdd[TokenControler-1].palavra);

			}

			//reseta variavel para a proxima palavra
			tamanhoCurrPalavra = 0;
		}

		if(identificarErro()){
			printf("ERRO Lexico: palavra na linha %d!\n", currLinha);
			return 1;
		}

		currLinha++;

	}


	return 0;
}
