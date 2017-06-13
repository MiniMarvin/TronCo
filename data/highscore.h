#ifndef HIGHSCORE_H
#define HIGHSCORE_H
 
typedef struct {
	int id;
	char nome[50];
	int score;
} data;

FILE* organizateData(FILE* arq, data* banco_Dados, int limit);

void getData(FILE* arq);

void readData(FILE* arq, data* banco_Dados, int* limit);

#endif