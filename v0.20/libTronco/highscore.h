#ifndef HIGHSCORE_H
#define HIGHSCORE_H
 
#include <stdlib.h>
#include <stdio.h>

typedef struct {
	int id;
	char nome[10];
	int score;
} data;


#define MAX_SAVED_HIGHSCORE 5


/**
 * @brief      Abre um arquivo no modo anexar
 *
 * @return     O ponteiro para o arquivo aberto
 */
void openFile(FILE** arq);


void printData(data* banco_Dados, int limit);

void copyRankingToSend(data dest[], data* src);

int fileLength(FILE* arq);


/**
 * @brief      Organiza o highscore no modo decrescente
 *
 * @param      arq          O ponteiro para o arquivo aberto
 * @param      banco_Dados  O banco de dados que contém
 * @param[in]  limit        The limit
 *
 * @return     { description_of_the_return_value }
 */
void organizateData(FILE** arq);



/**
 * @brief      Obtem informações do usuário e grava no arquivo
 *
 * @param      arq          O arquivo onde será gravado essas informações
 * @param      banco_Dados  O banco de dados contendo as informações que serão
 *                          gravadas no arquivo
 * @param[in]  limit        Limite de highscores que serão salvos no arquivo highScore.txt
 */
void saveData(FILE** arq, data* banco_Dados, int limit);


/**
 * @brief      Lê os dados presentes no arquivo e sal.
 *
 * @param      arq          O arquivo o qual será lido as informações
 * @param      banco_Dados  O banco de dados que armazenará essas informações
 * @param      limit        Quantas informações foram lidas
 */
void readData(FILE* arq, data** banco_Dados);




#endif
