#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #include "highscore.h"


typedef struct {
	int id;
	char nome[50];
	int score;
} data;

void printData(data* banco_Dados, int limit);
void pegarDados(data** banco_Dados, int count);

int main(int argc, char const *argv[]) {
	
	// data* banco_Dados = (data*) malloc(40*sizeof(data));
	
	// printf("Sizeof banco_Dados: %ld %ld\n", sizeof(banco_Dados), sizeof(data));
	
	char ch = ' ';

	int limit;
	int i = 0;
	
	// arquivo onde ficará salvo o highscore 
	FILE *arq;
	
	data *banco_Dados = NULL;
	
	data* banco_Gravacao = NULL;
	
	// Abre o arquivo no modo anexar
	openFile(&arq);
	// arq = fopen("highScore.txt", "a+");
	
	//Testa para ver se o arquivo abriu corretamente
	if(arq != NULL) {
		
		// Enquanto o usuário não digitar s, o programa continua rodando
		while(ch != 's') {
			scanf(" %c", &ch);
		
			switch(ch) {
				case 'a':
					saveData(arq, banco_Gravacao, 2);
					break;
				case 'r':
					limit = readData(arq, &banco_Dados);
					break;
				case 'o':
					organizateData(arq);
					break;
				case 'p':
					printData(banco_Dados, limit);
					break;
				case 'g':
					pegarDados(&banco_Gravacao, 2);
					// for (i = 0; i < 2; i++) {
					// 	printf("%s %d\n", banco_Gravacao[i].nome, banco_Gravacao[i].score);
					// }
					break;
			}
		}
		
		fclose(arq);
		free(banco_Dados);
	} else {

		//Printa a mensagem abaixo e o erro que ocorreu
		perror("Erro abrindo arquivo");
	}
	
	return 0;
}


void printData(data* banco_Dados, int limit) {
	
	int i = 0;
	for ( i = 0; i < limit; i++) {
		printf("%d %s %d\n", i+1, banco_Dados[i].nome, banco_Dados[i].score);
	}
	
}


void pegarDados(data** banco_Dados, int count) {
	
	int i;
	data* tmp = *banco_Dados;
	if(tmp != NULL) {
		free(tmp);
		tmp = (data*) malloc(count*sizeof(data));
	} else {
		tmp = (data*) malloc(count*sizeof(data));
	}
	
	for (i = 0; i < count; i++) {
	
		scanf(" %s %d", tmp[i].nome, &tmp[i].score);
		// printf("%s %d\n", tmp[i].nome, tmp[i].score);
	}
	
	*banco_Dados = tmp;
	
	printf("Exit\n");
}