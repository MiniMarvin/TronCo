#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "highscore.h"

/**
 * @brief      Abre um arquivo no modo anexar
 *
 * @return     O ponteiro para o arquivo aberto
 */
FILE* openFile() {
	
	FILE* arq;
	
	arq = fopen("highScore.txt", "a+");
	
	if(arq != NULL) {
		return arq;
	} else {
		perror("Erro na abertura do arquivo");
		return NULL;
	}
	
}

/**
 * @brief      Organiza o highscore no modo decrescente
 *
 * @param      arq          O ponteiro para o arquivo aberto
 * @param      banco_Dados  O banco de dados que contém
 * @param[in]  limit        The limit
 *
 * @return     { description_of_the_return_value }
 */
FILE* organizateData(FILE* arq) {
	
	int i = 0, j = 0, aux = 1;
	// FILE *arq1 = fopen("highScore2.txt", "w+");

	//Salva a referencia de memória do banco de dados para swap
	data tmp;
	

	data* banco_Dados = NULL;

	readData(arq, banco_Dados);

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
 * @param      arq          O arquivo onde será gravado essas informações
 * @param      banco_Dados  O banco de dados contendo as informações que serão
 *                          gravadas no arquivo
 * @param[in]  limit        Limite de highscores que serão salvos no arquivo highScore.txt
 */
FILE* saveData(FILE* arq, data* banco_Dados) {
	
	//Move o cursor para o fim do arquivo
	fseek(arq, 0, SEEK_END);
	
	// FILE* arq1 = arq;

	// int id, score;
	// char nome[40];
	
	// // // Debug
	// // printf("Digite seu ID: ");
	// // scanf(" %d", &id);
	// // printf("Digite seu nome: ");
	// // scanf(" %s", nome);
	// // printf("Digite sua pontuacao: ");
	// // scanf(" %d", &score);

	//Salva no arquivo o que foi digitado seguido de uma quebra de linha
	
	for (int i = 0; i < limit; ++i) {
		fprintf(arq1, "%d %s %d\n", banco_Dados[i].id, banco_Dados[i].nome, banco_Dados[i].score);
	}
	
	fclose(arq1);

	arq1 = fopen("highScore.txt", "a+");

	// Debug
	printf("Gravado!\n");


	return arq1;
}

/**
 * @brief      Lê os dados presentes no arquivo e sal.
 *
 * @param      arq          O arquivo o qual será lido as informações
 * @param      banco_Dados  O banco de dados que armazenará essas informações
 * @param      limit        Quantas informações foram lidas
 */
void readData(FILE* arq, data* banco_Dados) {


	//Move o cursor para o início do arquivo
	rewind(arq);
	

	int i;

	if(banco_Dados == NULL) {

	}
   
	
	// Lê as informações formatadas no seguinte estilo até encontrar o fim do arquivo
	while(fscanf(arq, "%d %s %d\n", &banco_Dados[i].id, banco_Dados[i].nome, &banco_Dados[i].score) != EOF) {
		i++;
	}
	
	//Debug
	printf("Lido!\n");
}