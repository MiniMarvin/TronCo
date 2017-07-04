#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "highscore.h"

/**
 * @brief      Abre um arquivo no modo anexar
 *
 * @return     O ponteiro para o arquivo aberto
 */
void openFile(FILE** arq) {
	
	
	*arq = fopen("highScore.txt", "a+");
	
	if(*arq == NULL) {
		perror("Erro na abertura do arquivo");
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
void organizateData(FILE** arq) {
	
	// printf("On organizate data\n");
	int i = 0, j = 0, aux = 1;
	// FILE *arq1 = fopen("highScore2.txt", "w+");

	//Salva a referencia de memória do banco de dados para swap
	data tmp;
	data* banco_Dados;

	int limit = fileLength(*arq);

	readData(*arq, &banco_Dados);

	// printf("Organizate data Limit: %d\n", limit);
	
	if(limit != 0) {
		// printf("On organizateData: \n");
		// printData(banco_Dados, limit);
		//Reabre o arquivo apagando tudo o que havia sido salvo anteriormente
		fclose(*arq);
		*arq = fopen("highScore.txt", "w+");
		
		// 5 Highscores Salvos
		for(i = 0; i < limit; i++) {

			for(j = i; j < limit; j++) {
				
				
				if(banco_Dados[ j].score > banco_Dados[ i].score) {
					
					tmp = banco_Dados[ i];
					banco_Dados[ i] = banco_Dados[ j];
					banco_Dados[ j] = tmp;
					
				}
					
				
			}
			
			//Salva o novo highscore organizado no mesmo arquivo
			
			// Debug
			// printf("%d %s %d\n", i, banco_Dados[i].nome, banco_Dados[i].score);
			// fprintf(arq, "%d %s %d\n", aux, banco_Dados[i].nome, banco_Dados[i].score);
		}

		for(i = 0; i < limit  - 1; i++) {

			fprintf(*arq, "%d %s %d\n", i+1, banco_Dados[i].nome, banco_Dados[i].score);
			
			//Aumenta o indice do score
			// printf("%s %d\n", banco_Dados[i].nome, banco_Dados[i].score);

		}

		// printf("Aux outside: %d\n", aux);
		fprintf(*arq, "%d %s %d", i+1, banco_Dados[i].nome, banco_Dados[i].score);
		
		fclose(*arq);
		
		openFile(arq);
		//Debug
		printf("Organizado!\n");
	} else {
		printf("Arquivo zerado!\n");
	}
}

/**
 * @brief      Obtem informações do usuário e grava no arquivo
 *
 * @param      arq          O arquivo onde será gravado essas informações
 * @param      banco_Dados  O banco de dados contendo as informações que serão
 *                          gravadas no arquivo
 * @param[in]  limit        Limite de highscores que serão salvos no arquivo highScore.txt
 */
void saveData(FILE** arq, data* banco_Dados, int limit) {

	// printf("On save data\n");

	int arq_len = fileLength(*arq);
	arq_len++;

	//Move o cursor para o fim do arquivo
	fseek(*arq, 0, SEEK_END);
	
	// printf("Aqui de boas\n");
	// printf("Limit: %d\n", limit);
	// printf("Dados: %d %s\n", banco_Dados[0].score, banco_Dados[0].nome);
	
	int i;
	int tam = ftell(*arq);
	
	//Quebra de linha na última posição salva
	
	if(tam == 0) {
		// printf("Fim do *arquivo\n");
		// fprintf(*arq, "\n");
	} else {
		fprintf(*arq, "\n");
	}
	

	
	for ( i = 0; i < limit - 1; ++i) {
	
		// fprintf(arq1, "%d %s %d\n", i, banco_Dados[i].nome, banco_Dados[i].score);
		if(banco_Dados[i].score != 0) {
			fprintf(*arq, "%d %s %d\n", arq_len, banco_Dados[i].nome, banco_Dados[i].score);
			arq_len++;
		}
		
	}
	// printf("%d %s %d\n", i, banco_Dados[i].nome, banco_Dados[i].score);
	// fprintf(arq1, "%d %s %d", i, banco_Dados[i].nome, banco_Dados[i].score);
	fprintf(*arq, "%d %s %d", arq_len, banco_Dados[i].nome, banco_Dados[i].score);
	
	// organizateData(arq1);
	//Salva do buffer no arquivo
	fclose(*arq);
	

	openFile(arq);
	
	//Organiza os scores antes de sair
	organizateData(arq);	
	
	// fclose(arq1);
	
	

	
	//Reabre o arquivo para não dar erro
	
	// arq = fopen("highScore.txt", "a+");

	// Debug
	printf("Gravado!\n");


	// return arq1;
}

/**
 * @brief      Lê os dados presentes no arquivo e sal.
 *
 * @param      arq          O arquivo o qual será lido as informações
 * @param      banco_Dados  O banco de dados que armazenará essas informações
 * @param      limit        Quantas informações foram lidas
 */
void readData(FILE* arq, data** banco_Dados) {

	data* tmp;

	// fclose(*arq);

	// *arq = fopen()

	int arq_len = fileLength(arq);
	// printf("Tam: %d\n", arq_len);

	//Move o cursor para o início do arquivo
	rewind(arq);

	char str[100];

	// printf("Alocado\n");
	*banco_Dados = (data*) malloc(arq_len*sizeof(data)); 

	int i = 0;   
	
	// Lê as informações formatadas no seguinte estilo até encontrar o fim do arquivo
	// while(fscanf(arq, "%d %s %d", &tmp[i].id, tmp[i].nome, &tmp[i].score) != EOF) {
	// 	i++;
	// 	// printf("Lendo\n");
	// }
	while( i < arq_len ) {
		
		fscanf(arq, "%d %s %d", &(*banco_Dados)[i].id, (*banco_Dados)[i].nome, &(*banco_Dados)[i].score);
		i++;
		// printf("Lendo\n");
		// printf("%d %s %d", (*banco_Dados)[i].id, (*banco_Dados)[i].nome, (*banco_Dados)[i].score);
	}
	// if(banco_Dados != NULL)
	// 	free(*banco_Dados);
	// *banco_Dados = tmp;
	

	// printf("On readData: \n");
	// printData(*banco_Dados, arq_len);

	//Debug
	printf("Lido!\n");

	// return arq_len;
	// return banco_Dados;
}



void printData(data* banco_Dados, int limit) {
	
	int i = 0;
	for ( i = 0; i < limit; i++) {
		if(strcmp(banco_Dados[i].nome, "") != 0)
			printf("%d %s %d\n", i+1, banco_Dados[i].nome, banco_Dados[i].score);
	}
	
}



int fileLength(FILE* arq) {

	rewind(arq);

	char str[100];

	int arq_len = 0;

	while(fgets(str, sizeof(str), arq) != NULL) {
		arq_len++;
	}

	fseek(arq, 0, SEEK_END);

	return arq_len;

}

void copyRankingToSend(data dest[], data* src) {

	int i;
	if(src != NULL) {
		for(i = 0; i < MAX_SAVED_HIGHSCORE; i++) {
			dest[i] = src[i];
		}
		printf("Copy ok\n");
	} else
		printf("Copy not ok!\n");	
}