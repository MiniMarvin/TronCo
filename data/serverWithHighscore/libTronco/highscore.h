#ifndef HIGHSCORE_H
#define HIGHSCORE_H
 
typedef struct {
	int id;
	char nome[10];
	int score;
} data;



/**
 * @brief      Abre um arquivo no modo anexar
 *
 * @return     O ponteiro para o arquivo aberto
 */
void openFile(FILE** arq);


/**
 * @brief      Organiza o highscore no modo decrescente
 *
 * @param      arq          O ponteiro para o arquivo aberto
 * @param      banco_Dados  O banco de dados que contém
 * @param[in]  limit        The limit
 *
 * @return     { description_of_the_return_value }
 */
FILE* organizateData(FILE* arq);



/**
 * @brief      Obtem informações do usuário e grava no arquivo
 *
 * @param      arq          O arquivo onde será gravado essas informações
 * @param      banco_Dados  O banco de dados contendo as informações que serão
 *                          gravadas no arquivo
 * @param[in]  limit        Limite de highscores que serão salvos no arquivo highScore.txt
 */
void saveData(FILE* arq, data* banco_Dados, int limit);


/**
 * @brief      Lê os dados presentes no arquivo e sal.
 *
 * @param      arq          O arquivo o qual será lido as informações
 * @param      banco_Dados  O banco de dados que armazenará essas informações
 * @param      limit        Quantas informações foram lidas
 */
void readData(FILE* arq, data** banco_Dados);

int fileLenght(FILE* arq);


#endif