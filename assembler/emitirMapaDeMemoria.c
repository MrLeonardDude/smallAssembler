/*
 * Autor : Leonardo Maldonado Pagnez
 * RA : 172017
 */

#include "montador.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LOADMQ 			0x0A
#define LOADMQMX 		0x09
#define STORMX 			0x21
#define LOADMX 			0x01
#define LOADminusMX 	0x02
#define LOADmodMX		0x03
#define JUMPMX			0x0D
#define JUMPM_dir		0x0E
#define JUMPplus 		0x0F
#define JUMPplus_dir	0x10
#define ADDMX			0x05
#define ADDmodMX		0x07
#define SUBMX			0x06
#define SUBmodMX		0x08
#define MULmx			0x0B
#define DIVmx 			0x0C
#define LSH 			0x14
#define RSH 			0x15
#define STORpar			0x12
#define STORpar_dir		0x13

#define org				1
#define align			2
#define wfill 			3
#define set				4
#define word			5

/* Retorna:
 *  1 caso haja erro na montagem;
 *  0 caso nao haja erro.
 */

unsigned int count(unsigned int i) {
	unsigned int ret=1;
	while (i/=16) ret++;
		return ret;
}

unsigned checkDir(char *palavra){

	char strs [] = ".org";
	char strs1 [] = ".align";
	char strs2 [] = ".wfill";
	char strs3 [] = ".set";

	if( (strcmp(palavra, strs) == 0)) return org;

	else if( ( strcmp(palavra, strs1)  == 0) )return align;

	else if( (strcmp(palavra, strs2)  == 0)) return wfill;

	else if( (strcmp(palavra, strs3)  == 0) ) return set;

	return word;
}

int checkInstr(char* palavra){
		char strsL1 [] 	= "LOAD";
		char strsL2 [] 	= "LOAD-";
		char strsL3 [] 	= "LOAD|";
		char strsL4 [] 	= "LOADmq";
		char strsL5 [] 	= "LOADmq_mx";
		char strsS1 [] 	= "STOR";
		char strsS2 [] 	= "STORA";
		char strsJ1 [] 	= "JUMP";
		char strsJ2 [] 	= "JMP+";
		char strsA1 [] 	= "ADD";
		char strsA2 []	= "ADD|";
		char strsSu1 [] = "SUB";
		char strsSu2 [] = "SUB|";
		char strsM1 [] 	= "MUL";
		char strsD1 [] 	= "DIV";
		char strsL [] 	= "LSH";


		if( (strcmp(palavra, strsL1)  == 0)){
			return LOADMX;
		}
		else if( ( strcmp(palavra, strsL2)  == 0) ){
			return LOADminusMX;
		}

		else if( (strcmp(palavra, strsL3) == 0)){
			return LOADmodMX;
		}
		else if( ( strcmp(palavra, strsL4)  == 0)  ){
			return LOADMQ;
		}
		else if( (strcmp(palavra, strsL5)  == 0) ){
			return LOADMQMX;
		}

		else if( (strcmp(palavra, strsS1)  == 0) ){
			return STORMX;
		}

		else if( ( strcmp(palavra, strsS2)  == 0) ){
			return STORpar;
		}
		else if( (strcmp(palavra, strsJ1)  == 0) ){
			return JUMPMX;
		}

		else if( (strcmp(palavra, strsJ2)  == 0) ){
			return JUMPplus;
		}

		else if( (strcmp(palavra, strsA1)  == 0) ){
			return ADDMX;
		}

		else if( (strcmp(palavra, strsA2)  == 0) ){
			return ADDmodMX;
		}

		else if( (strcmp(palavra, strsSu1)  == 0) ){
			return SUBMX;
		}

		else if( (strcmp(palavra, strsSu2)  == 0) ){
		return SUBmodMX;
		}

		else if( (strcmp(palavra, strsM1)  == 0) ){
			return MULmx;
		}

		else if( (strcmp(palavra, strsD1)  == 0) ){

			return DIVmx;
		}

		else if( (strcmp(palavra, strsL)  == 0)){

			return LSH;
		}

		return RSH;
}

unsigned int tipoDePuloOuStor(char* palavra, int pos){

	char strsS2 [] 	= "STORA";
	char strsJ1 [] 	= "JUMP";
	char strsJ2 [] 	= "JMP+";


	if( ( strcmp(palavra, strsS2)  == 0) ){
		if(pos)
			return 0x12;
		return 0x13;
	}

	else if( (strcmp(palavra, strsJ1)  == 0) ){
		if(pos)
			return 0x0D;
		return 0x0E;
	}

	else if( (strcmp(palavra, strsJ2)  == 0) ){
		if(pos)
			return 0x0F;
		return 0x10;
	}

	return 0;
}

unsigned retirarDuploPonto(char* palavra){

	for(unsigned i = 0; palavra[i] != '\0'; i++){
		if(i == 0 && palavra[i] == ':'){
			for(unsigned j = 0; palavra[j] != '\0'; j++)
				palavra[j] = palavra[j+1];
		}

		if(palavra[i+1] == '\0' && palavra[i] == ':'){
			palavra[i] = palavra[i+1];
			break;
		}
	}

	return 1;
}

/*Objetivo do Algoritmo:
 * 1. Ler os tokens do vetor
 * 2. Transforma-los apropriamente em codigo binario
*/

int emitirMapaDeMemoria(){

	/*matrizes que guardam as palavras dos nomes (rotulos e simbolos)*/
	char *matrizPalavras[4096], *matrizSimbolos[4096];

	// variaveis unsigneds de controle do sistema
	unsigned sizeTokens, sizeDefRots = 0, sizePosDefRots = 0, currLinha = 0, dir, dirOuEsq = 0, sizeDefSimbs =0, ctrl =0, count = 0, count2 = 0, instr;

	//matrizes que guardam os valores/ posicoes que cada simbolo/ rotulo possui dentro do mapa
	unsigned *pos, *DirOuEsq, *SimbsValor;

	//token auxiliar que possui todos os tokens capturados na parte i
	Token TokenAdd[4096];

	//Alocando memoria para as matrizes
	for(int i =0; i < 4096; i++)
		matrizPalavras[i] = (char *) malloc(sizeof(char) * 4096);
	for(int i =0; i < 4096; i++)
		matrizSimbolos[i] = (char *) malloc(sizeof(char) * 4096);
	pos = (unsigned *) malloc(sizeof(unsigned) * 4096);
	DirOuEsq = (unsigned *) malloc(sizeof(unsigned) * 4096);
	SimbsValor = (unsigned *) malloc(sizeof(unsigned) * 4096);


	//Leitura do vetor de Tokens para : Encontrar todos os rotulos
	sizeTokens = getNumberOfTokens();


	//Recuperando os tokens encontrados na Parte 1
	for(int i =0; i < sizeTokens; i++){
		TokenAdd[i] = recuperaToken(i);
	}

	//Procurando todas as definicoes de Tokens, retirando o ':'
	for (int i = 0; i < sizeTokens; i++){
		if (TokenAdd[i].tipo == DefRotulo){
			strcpy(matrizPalavras[sizeDefRots], TokenAdd[i].palavra);
			retirarDuploPonto(matrizPalavras[sizeDefRots++]);

		}
	}

	//Funca subprincipal: Preenche todas as posicoes dos rotulos e preenche todos os valores do simbolos encontrados
	for (int j = 0; j < sizeTokens; j++){
		switch(TokenAdd[j].tipo){
			case DefRotulo:
				pos[sizePosDefRots] = currLinha;
				DirOuEsq[sizePosDefRots++] = dirOuEsq;
				break;
			case Diretiva:
				dir = checkDir(TokenAdd[j].palavra);
				if(dir == org){
					if(TokenAdd[j+1].tipo == Decimal)
						currLinha = atoi(TokenAdd[j+1].palavra);
					else if(TokenAdd[j+1].tipo == Hexadecimal)
						currLinha = (unsigned) strtol(TokenAdd[j+1].palavra, NULL, 0);

				}
				else if(dir == align){
					if(dirOuEsq){
						dirOuEsq = 0;
						currLinha++;
					}
				}
				else if(dir == wfill){
					currLinha = currLinha + atoi(TokenAdd[j+1].palavra);
				}
				else if(dir == word)
					currLinha++;
				else if(dir == set){
					strcpy(matrizSimbolos[sizeDefSimbs], TokenAdd[j+1].palavra);
					if(TokenAdd[j+2].tipo == Decimal)
						SimbsValor[sizeDefSimbs++] = atoi(TokenAdd[j+2].palavra);
					else if(TokenAdd[j+2].tipo == Hexadecimal)
											SimbsValor[sizeDefSimbs++] = (unsigned) strtol(TokenAdd[j+2].palavra, NULL, 0);

				}
				break;
			case Instrucao:
				if(dirOuEsq){
					currLinha++;
					dirOuEsq = 0;
				}
				else
					dirOuEsq = 1;
				break;
			default:
				break;

		}
	}

	//Prepara-se para poercorrer os tokens novamente
	currLinha = 0;
	dirOuEsq = 0;

	//Funcao principal: percorre os tokens e encontra todas as informacoes que preenchem o mapa: diretivas e instrucoes
	//					simbolos, rotulos, hexadecimais e decimais sao tokens de auxilio para preencher a instrucao e encontrar o endereco a ser retornado
	for(int i = 0; i < sizeTokens; i++){

		//Switch que difere instrucoes das diretivas
		switch(TokenAdd[i].tipo){
			case Diretiva:
				//Identifica a diretiva encontrada
				dir = checkDir(TokenAdd[i].palavra);

				//Diretiva encontrada org => pula para o endereco requisitado no token a frente
				if(dir == org){
					if(TokenAdd[i+1].tipo == Decimal)
							currLinha = atoi(TokenAdd[i+1].palavra);
					else if(TokenAdd[i+1].tipo == Hexadecimal)
						currLinha = (unsigned) strtol(TokenAdd[i+1].palavra, NULL, 0);
				}

				//Diretiva encontrada align => se o ponteiro estiver encontrado apos uma instrucao a esquerda, alinha e preenche com 00 000 o resto a direita
				else if(dir == align){
					if(dirOuEsq){
						printf("00 000\n");
						currLinha++;
						dirOuEsq = 0;
					}
				}

				//Diretiva encontra wfill => preenche os enderecos selecionados com o valor encontrado no token a seguir
				else if(dir == wfill){
					count = 0;
					count2 = 0;
					while(ctrl < atoi(TokenAdd[i+1].palavra) ){
						if(TokenAdd[i+2].tipo == Nome){
							while	(	count < sizeDefRots && strcmp(matrizPalavras[count], TokenAdd[i+2].palavra) != 0		)
								count++;

							while(	count2 < sizeDefSimbs && strcmp(matrizSimbolos[count2], TokenAdd[i+2].palavra) != 0	){
								count2++;
							}

							if(!(count < sizeDefRots) && !(strcmp(matrizPalavras[count], TokenAdd[i+2].palavra) != 0)){
								printf("Imposs�vel montar o c�digo!\n");
								return 0;

							}

							else if(strcmp(matrizPalavras[count], TokenAdd[i+2].palavra) == 0 )
								printf("%03X 00 000 00 %03X\n", currLinha++, pos[count] );

							else if(!(count2 < sizeDefSimbs) && !(strcmp(matrizSimbolos[count2], TokenAdd[i+2].palavra) != 0)){
								printf("Imposs�vel montar o c�digo!\n");
								return 0;
							}

							else
								printf("%03X 00 000 00 %03X\n", currLinha++, SimbsValor[count2]);

						}

						else if(TokenAdd[i+2].tipo == Decimal){
							printf("%03X 00 000 00 %03X\n", currLinha++, atoi(TokenAdd[i+2].palavra));
						}
						else if(TokenAdd[i+2].tipo == Hexadecimal){
													printf("%03X 00 000 00 %03X\n", currLinha++, (unsigned)strtol(TokenAdd[i+2].palavra, NULL, 0));
												}
						ctrl++;
					}
					ctrl = 0;
				}

				//Diretiva encontrada word => preenche uma palavra na memoria
				else if(dir == word){
					count = 0;
					count2 = 0;
					if(i+1 > sizeTokens ){
						printf("Imposs�vel montar o c�digo!\n");
						return 1;
					}

					if(TokenAdd[i+1].tipo == Nome){
						while	(	count < sizeDefRots && strcmp(matrizPalavras[count], TokenAdd[i+1].palavra) != 0		)
							count++;
						while(	count2 < sizeDefSimbs && strcmp(matrizSimbolos[count2], TokenAdd[i+1].palavra) != 0	){
							count2++;
						}

						if(strcmp(matrizPalavras[count], TokenAdd[i+1].palavra) == 0 )
							printf("%03X 00 000 00 %03X\n", currLinha++, pos[count] );

						else if( (count2 + 1 > sizeDefSimbs) && (strcmp(matrizSimbolos[count2], TokenAdd[i+1].palavra) != 0)){
							printf("USADO MAS N�O DEFINIDO: %s!\n", TokenAdd[i+1].palavra);
							return 0;
						}

						else if(!(count2 < sizeDefSimbs) && !(strcmp(matrizSimbolos[count2], TokenAdd[i+1].palavra) != 0)){
							printf("USADO MAS N�O DEFINIDO: %s!\n", TokenAdd[i+1].palavra);
							return 0;
						}
						else
							printf("%03X 00 000 00 %03X\n", currLinha++, SimbsValor[count2]);
					}

					else if(TokenAdd[i+1].tipo == Decimal){
							printf("%03X 00 000 00 %03X\n", currLinha++, atoi(TokenAdd[i+1].palavra));
					}
					else if(TokenAdd[i+1].tipo == Hexadecimal){
												printf("%03X 00 000 00 %03X\n", currLinha++, (unsigned) strtol(TokenAdd[i+1].palavra, NULL, 0));
										}
				}
				break;

			//Identificada uma instrucao
			case Instrucao:

				//Descobre qual instrucao
				instr = checkInstr(TokenAdd[i].palavra);

				//Descobre se o ponteiro esta a esquerda ou a direita na posicao atual da memoria
				if(dirOuEsq){
					if(instr == LSH || instr == RSH)
						printf("%02X 000\n", instr);

					else if( instr == JUMPMX || instr == JUMPplus || instr == STORpar){
						if(TokenAdd[i+1].tipo == Decimal)
							printf("%02X %03X\n", instr + 1, atoi(TokenAdd[i+1].palavra));
						else if(TokenAdd[i+1].tipo == Hexadecimal)
							printf("%02X %03X\n", instr + 1, (unsigned)strtol(TokenAdd[i+1].palavra, NULL, 0));
						else if(TokenAdd[i+1].tipo == Nome){
							count = 0;
							while(	count < sizeDefRots && strcmp(matrizPalavras[count], TokenAdd[i+1].palavra) != 0		)
								count++;
							if(!(count < sizeDefRots) && !(strcmp(matrizPalavras[count], TokenAdd[i+1].palavra) != 0)){
								printf("USADO MAS N�O DEFINIDO: %s!\n", TokenAdd[i+1].palavra);
								return 0;
							}
							else
								printf("%02X %03X\n", instr+1, pos[count] );
							count = 0;
						}

					}
					else{

						if(TokenAdd[i+1].tipo == Nome){
																		count = 0;
																		while(	count < sizeDefRots && strcmp(matrizPalavras[count], TokenAdd[i+1].palavra) != 0		)
																			count++;
												if(!(count < sizeDefRots) && !(strcmp(matrizPalavras[count], TokenAdd[i+1].palavra) != 0)){
													printf("USADO MAS N�O DEFINIDO: %s!\n", TokenAdd[i+1].palavra);
													return 0;
												}
												else
													printf("%02X %03X\n", instr, pos[count] );
												count = 0;
											}
						else if(TokenAdd[i+1].tipo == Decimal)
							printf("%02X %03X\n", instr, atoi(TokenAdd[i+1].palavra));
						else if(TokenAdd[i+1].tipo == Hexadecimal)
							printf("%02X %03X\n", instr, (unsigned) strtol(TokenAdd[i+1].palavra, NULL, 0));
					}
					dirOuEsq = 0;
					currLinha++;

				}
				else{
					if(instr == LSH || instr ==RSH)
						printf("%03X %02X 000 ", currLinha, instr);
					else if(TokenAdd[i+1].tipo == Nome){
												count = 0;
												while(	count < sizeDefRots && strcmp(matrizPalavras[count], TokenAdd[i+1].palavra) != 0		)
													count++;
						if(!(count < sizeDefRots) && !(strcmp(matrizPalavras[count], TokenAdd[i+1].palavra) != 0)){
							printf("USADO MAS N�O DEFINIDO: %s!\n", TokenAdd[i+1].palavra);
							return 0;
						}
						else
							printf("%03X %02X %03X ", currLinha, instr, pos[count] );
						count = 0;
					}

					else if(TokenAdd[i+1].tipo == Decimal)
						printf("%03X %02X %03X ", currLinha, instr, atoi(TokenAdd[i+1].palavra));
					else if(TokenAdd[i+1].tipo == Hexadecimal)
						printf("%03X %02X %03X ", currLinha, instr, (unsigned) strtol(TokenAdd[i+1].palavra, NULL, 0));
					dirOuEsq = 1;
				}
				break;
			default:
				break;


		}
	}

	//preenchimento final caso ultimo token seja uma instrucao a esquerda
	if(dirOuEsq)
		printf("00 000\n");


	//Liberacao de memoria
	for(int i = 0; i < 4096; i++)
		free(matrizPalavras[i]);

	for(int i = 0; i < 4096; i++)
			free(matrizSimbolos[i]);
	free(pos);
	free(DirOuEsq);
	free(SimbsValor);

	return 0;


}
