#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "highscore.h"

FILE* organizateData(FILE* arq, data* banco_Dados, int limit) {
	
	int i = 0, j = 0, aux = 1;
	// FILE *arq1 = fopen("highScore2.txt", "w+");

	//Salva a referencia de memória do banco de dados para swap
	data tmp;
	
	//Reabre o arquivo apagando tudo o que havia sido salvo anteriormente
	arq = freopen("highScore.txt", "w+", arq);
	
	// 5 Highscores Salvos
	for(i = limit - 1; i >= 0; i--) {

		for(j = i; j >= 0; j--) {
			
			//Organiza do maior para o menor
			if(banco_Dados[ j].score > banco_Dados[ i].score) {
				
				tmp = banco_Dados[ i];
				banco_Dados[ i] = banco_Dados[ j];
				banco_Dados[ j] = tmp;
				
			}
				
			
		}
		
		//Salva o novo highscore organizado no mesmo arquivo
		fprintf(arq, "%d %s %d\n", aux, banco_Dados[i].nome, banco_Dados[i].score);
		
		//Aumenta o indice do score
		aux++;
		// Debug
		// printf("%d %s %d\n", i, banco_Dados[i].nome, banco_Dados[i].score);
	}
	
	
	//Debug
	printf("Organized\n");
	return arq;
}

/**
 * @brief      Obtem informações do usuário e grava no arquivo
 *
 * @param      arq   O arquivo onde será gravado essas informações
 */
void getData(FILE* arq) {
	
	//Move o cursor para o fim do arquivo
	fseek(arq, 0, SEEK_END);
	
	int id, score;
	char nome[40];
	
	// Debug
	printf("Digite seu ID: ");
	scanf(" %d", &id);
	printf("Digite seu nome: ");
	scanf(" %s", nome);
	printf("Digite sua pontuacao: ");
	scanf(" %d", &score);

	//Salva no arquivo o que foi digitado seguido de uma quebra de linha
	fprintf(arq, "%d %s %d\n", id, nome, score);

	// Debug
	printf("Gravado!\n");
}

/**
 * @brief      Lê os dados presentes no arquivo e sal.
 *
 * @param      arq          O arquivo o qual será lido as informações
 * @param      banco_Dados  O banco de dados que armazenará essas informações
 * @param      limit        Quantas informações foram lidas
 */
void readData(FILE* arq, data* banco_Dados, int* limit) {


	//Move o cursor para o início do arquivo
	rewind(arq);
	
   
	int id, score, i = 0;
	char nome[50];
	
	// Lê as informações formatadas no seguinte estilo até encontrar o fim do arquivo
	while(i < limit) {
		fscanf(arq, "%d %s %d\n", &banco_Dados[i].id, banco_Dados[i].nome, &banco_Dados[i].score);
		// fscanf(arq, "%d %s %d\n", &id, nome, &score) != EOF
		// Copia os dados para um banco de dados no programa
		// banco_Dados[i].id = id;
		// strcpy(banco_Dados[i].nome, nome);
		// banco_Dados[i].score = score;
		
		// Debug
		// printf("%d %s %d\n", id, nome, score);
		// printf("I: %d\n", i);
		i++;
	}
	*(limit) = i;
	
	//Debug
	printf("Lido!\n");
}