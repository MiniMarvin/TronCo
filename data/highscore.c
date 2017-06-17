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
FILE* organizateData(FILE* arq) {
	
	int i = 0, j = 0, aux = 1;
	// FILE *arq1 = fopen("highScore2.txt", "w+");

	//Salva a referencia de memória do banco de dados para swap
	data tmp;

	data* banco_Dados = NULL;

	int limit = readData(arq, &banco_Dados);

	//Reabre o arquivo apagando tudo o que havia sido salvo anteriormente
	arq = freopen("highScore.txt", "w+", arq);
	
	// 5 Highscores Salvos
	for(i = limit - 1; i >= 1; i--) {

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
	fprintf(arq, "%d %s %d", aux, banco_Dados[i].nome, banco_Dados[i].score);
	
	fclose(arq);
	
	openFile(&arq);
	//Debug
	printf("Organizado!\n");
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
void saveData(FILE* arq, data* banco_Dados, int limit) {
	
	//Move o cursor para o fim do arquivo
	fseek(arq, 0, SEEK_END);
	
	// printf("Aqui de boas\n");
	// printf("Limit: %d\n", limit);
	// printf("Dados: %d %s\n", banco_Dados[0].score, banco_Dados[0].nome);
	
	int i;
	int tam = ftell(arq);
	
	//Quebra de linha na última posição salva
	
	if(tam == 0) {
		printf("Fim do arquivo\n");
		// fprintf(arq, "\n");
	} else {
		fprintf(arq, "\n");
	}
	
	
	for ( i = 0; i < limit - 1; ++i) {
	
		// fprintf(arq1, "%d %s %d\n", i, banco_Dados[i].nome, banco_Dados[i].score);
		fprintf(arq, "%d %s %d\n", i, banco_Dados[i].nome, banco_Dados[i].score);
	}
	// printf("%d %s %d\n", i, banco_Dados[i].nome, banco_Dados[i].score);
	// fprintf(arq1, "%d %s %d", i, banco_Dados[i].nome, banco_Dados[i].score);
	fprintf(arq, "%d %s %d", i, banco_Dados[i].nome, banco_Dados[i].score);
	
	// organizateData(arq1);
	
	
	//Organiza os scores antes de sair
	organizateData(arq);
	
	// fclose(arq1);
	
	//Salva do buffer no arquivo
	fclose(arq);

	
	//Reabre o arquivo para não dar erro
	openFile(&arq);
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
int readData(FILE* arq, data** banco_Dados) {

	data* tmp;

	//Move o cursor para o início do arquivo
	rewind(arq);

	char str[100];
	int arq_len = 0;

	while(fgets(str, sizeof(str), arq) != NULL) {
		arq_len++;
	}
	printf("Tam: %d\n", arq_len);
	// printf("Alocado\n");
	tmp = (data*) malloc(arq_len*sizeof(data)); 

	rewind(arq);

	int i = 0;   
	
	// Lê as informações formatadas no seguinte estilo até encontrar o fim do arquivo
	while(fscanf(arq, "%d %s %d", &tmp[i].id, tmp[i].nome, &tmp[i].score) != EOF) {
		i++;
		// printf("Lendo\n");
	}
	if(banco_Dados != NULL)
		free(*banco_Dados);
	*banco_Dados = tmp;
	
	//Debug
	printf("Lido!\n");

	return arq_len;
	// return banco_Dados;
}