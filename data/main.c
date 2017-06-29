#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "highscore.h"

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
	
	data highscore[5];

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
					saveData(arq, banco_Gravacao, 5);
					break;
				case 'r':
					readData(arq, &banco_Dados);
					printf("%s %d\n", banco_Dados[0].nome, banco_Dados[0].score);
					break;
				case 'o':
					organizateData(arq);
					break;
				case 'p':
					limit = fileLength(arq);
					printData(banco_Dados, limit);
					break;
				case 'g':
					pegarDados(&banco_Gravacao, 5);
					// for (i = 0; i < 2; i++) {
					// 	printf("%s %d\n", banco_Gravacao[i].nome, banco_Gravacao[i].score);
					// }
				case 'c':
					copyRankingToSend(highscore, banco_Dados);
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

void pegarDados(data** banco_Dados, int count) {
	
	int i;

	if(*banco_Dados != NULL)
		free(*banco_Dados);

	*banco_Dados = (data*) malloc(count*sizeof(data));
	
	
	for (i = 0; i < count; i++) {
	
		scanf(" %s %d", (*banco_Dados)[i].nome, &(*banco_Dados)[i].score);
		// printf("%s %d\n", tmp[i].nome, tmp[i].score);
	}
	
	
	printf("Exit\n");
}