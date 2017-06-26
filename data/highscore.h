#ifndef HIGHSCORE_H
#define HIGHSCORE_H
 
typedef struct {
	char nome[50];
	int score;
} data;


/**
 * @brief      Espera um tempo em mili segundos.
 *
 * @param[in]  mseconds  O tempo a esperar.
 */
void delay(unsigned int mseconds);


/**
 * @brief      Abre um arquivo no modo anexar
 *
 * @param      arq   Ponteiro para o arquivo a ser aberto
 */
void openFile(FILE** arq);


/**
 * @brief      Organiza o highscore no modo decrescente
 *
 * @param      arq   O ponteiro para o arquivo aberto
 *
 * @return     Retorna o ponteiro do arquivo, já que o mesmo foi reaberto
 */
FILE* organizateData(FILE* arq);



/**
 * @brief      Obtem informações do usuário e grava no arquivo
 *
 * @param      arq          O arquivo onde será gravado essas informações
 * @param      banco_Dados  O banco de dados contendo as informações que serão
 *                          gravadas no arquivo
 * @param[in]  limit        Limite de highscores que serão salvos no arquivo
 *                          highScore.txt
 */
void saveData(FILE* arq, data* banco_Dados, int limit);


/**
 * @brief      Lê os dados presentes no arquivo e sal.
 *
 * @param      arq          O arquivo o qual será lido as informações
 * @param      banco_Dados  O ponteiro banco de dados que armazenará essas informações
 *
 * @return     O tamanho do arquivo
 */
data readData(FILE* arq, data* banco_Dados);




#endif