#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "highscore.h"

/**
 * @brief      Abre um arquivo no modo anexar
 *
 * @param      arq   Ponteiro para o arquivo a ser aberto
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
 * @param      arq   O ponteiro para o arquivo aberto
 *
 * @return     Retorna o ponteiro do arquivo, já que o mesmo foi reaberto
 */
FILE* organizateData(FILE* arq) {
	
	// int i = 0, j = 0, aux = 1;
	// // FILE *arq1 = fopen("highScore2.txt", "w+");

	// //Salva a referencia de memória do banco de dados para swap
	// data tmp;

	// data* banco_Dados = NULL;

	// int limit ;
	// banco_Dados = readData(arq, banco_Dados);

	// //Reabre o arquivo apagando tudo o que havia sido salvo anteriormente
	// arq = freopen("highScore.txt", "w+", arq);
	
	// // 5 Highscores Salvos
	// for(i = limit - 1; i >= 1; i--) {

	// 	for(j = i; j >= 0; j--) {
			
	// 		//Organiza do maior para o menor
	// 		if(banco_Dados[ j].score > banco_Dados[ i].score) {
				
	// 			tmp = banco_Dados[ i];
	// 			banco_Dados[ i] = banco_Dados[ j];
	// 			banco_Dados[ j] = tmp;
				
	// 		}
				
			
	// 	}
		
	// 	//Salva o novo highscore organizado no mesmo arquivo
	// 	fprintf(arq, " %s %d\n", banco_Dados[i].nome, banco_Dados[i].score);
		
	// 	//Aumenta o indice do score
	// 	aux++;
	// 	// Debug
	// 	// printf("%d %s %d\n", i, banco_Dados[i].nome, banco_Dados[i].score);
	// }
	// fprintf(arq, " %s %d", banco_Dados[i].nome, banco_Dados[i].score);
	
	// fclose(arq);
	
	// openFile(&arq);
	// //Debug
	// printf("Organizado!\n");
	// return arq;
}

/**
 * @brief      Obtem informações do usuário e grava no arquivo
 *
 * @param      arq          O arquivo onde será gravado essas informações
 * @param      banco_Dados  O banco de dados contendo as informações que serão
 *                          gravadas no arquivo
 * @param[in]  limit        Limite de highscores que serão salvos no arquivo
 *                          highScore.txt
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
		fprintf(arq, " %s %d\n", banco_Dados[i].nome, banco_Dados[i].score);
	}
	// printf("%d %s %d\n", i, banco_Dados[i].nome, banco_Dados[i].score);
	// fprintf(arq1, "%d %s %d", i, banco_Dados[i].nome, banco_Dados[i].score);
	fprintf(arq, " %s %d",banco_Dados[i].nome, banco_Dados[i].score);
	
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
 * @param      banco_Dados  O ponteiro banco de dados que armazenará essas informações
 *
 * @return     O tamanho do arquivo
 */
data readData(FILE* arq, data* tmp) {

	data banco_Dados;

	fseek(arq, 0, SEEK_SET);
	int i = 0;
	// while(fscanf(arq, " %s %d", (banco_Dados)[i].nome, &(banco_Dados)[i].score) != EOF) {
	// 	printf("%d %s %d\n", i+1, (banco_Dados)[i].nome, (banco_Dados)[i].score);
	// 	i++;
	// 	// printf("Lendo\n");
	// }

	
	while(fscanf(arq, " %s %d", (banco_Dados).nome, &(banco_Dados).score) != EOF) {
		printf("%d %s %d\n", i+1, (banco_Dados).nome, (banco_Dados).score);
		// printf("Lendo\n");
	}


	return banco_Dados;
}