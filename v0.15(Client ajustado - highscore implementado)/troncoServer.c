/**
 * OBS: O X é a variação vertical e o Y é a variação horizontal!!!
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "libTronco/tronco.h"
#include "libSocket/server.h"

#include <time.h>

#define RESTART_GAME

#define UP    0
#define DOWN  1
#define LEFT  2
#define RIGHT 3

#define CLIENTS_LIMIT 100

typedef struct{
	int x;
	int y;
	int dir;
}player;

void delay(unsigned int mseconds);
int cleanMatrix(char matrix[SIZEX][SIZEY], int player);
void printMatrix(char matrix[SIZEX][SIZEY]);

int main(int argc, char **argv) {

	FILE* score_file;
	openFile(&score_file);

	data* score;
	data save;

	char corpo = 'a';
	char cabeca = 'A';

	bool sair = false;
	int tecla = 0;
	int i, j, pontuacao = 0;
	int buff_dir[ROOM_SIZE]; // Buffer de direção do jogador
	int buff_pos_anteriorX[ROOM_SIZE]; // Buffer da posicao anterior para poder colocar a cabeca da Cobra no local correto
	int buff_pos_anteriorY[ROOM_SIZE]; // Buffer da posicao anterior para poder colocar a cabeca da Cobra no local correto
	int players_count = 0; // Define o número de jogadores conectados ao server que desejam jogar
	int gameStart = 0; // Define se o jogo vai iniciar ou não
	int players_alive = 0; // Define o número de players ainda vivos
	int winner = -1; // Define qual será o player ganhador do jogo
	int win_ct = 0b1111;


	char* clientMatrix = (char*) malloc(sizeof(char)*CLIENTS_LIMIT);
	for (i = 0; i < CLIENTS_LIMIT; i++) {
		clientMatrix[i] = 0;
	}

	clientMsg clientPackage;
	serverMsg serverPackage;
	struct msg_ret_t retorno;
	serverHighscore sendHighscore;


	// inicializa com a direção inicial de cada player.
	// for (i = 0; i < MAXCLIENTS; ++i) {
	//   // buff_dir[i] = 0;
	//   serverPackage.statusPlayer = 0; // Significa que ele esta vivo
	// }

	//// debugar direcao dos clients **********************************************8
	player jogador[ROOM_SIZE];
	jogador[0].x = 0; jogador[0].y = 0; jogador[0].dir = buff_dir[0] = DOWN; // Inicia no canto superior esquerdo
	jogador[1].x = 0; jogador[1].y = SIZEY - 1; jogador[1].dir = buff_dir[1] = LEFT; // Inicia no canto superior direito
	jogador[2].x = SIZEX - 1; jogador[2].y = 1; jogador[2].dir = buff_dir[2] = RIGHT; // Inicia no canto inferior esquerdo
	jogador[3].x =  SIZEX - 1; jogador[3].y = SIZEY - 1 ; jogador[3].dir = buff_dir[3] = UP; // Inicia no canto inferior direito


	char matrizJogo[SIZEX][SIZEY];
	serverInit(MAXCLIENTS);


	for(i = 0; i < SIZEX; i++){
		for(j = 0; j < SIZEY; j++){
			matrizJogo[i][j] = '0';
			printf("%c", matrizJogo[i][j]);
		}
		printf("\n");
	}

	while(!sair) {

		int id = acceptConnection();

		// Verifica se o player já estava logado.
		if(id != NO_CONNECTION){
			printf("Alguem se Conectou com ID %d\n", id);
		}

		// Recebe o status
		retorno = recvMsg(&clientPackage);

		if(retorno.status == MESSAGE_OK) {

			id = retorno.client_id;
			serverPackage.statusPlayer = GAME_WAITING; // Informa ao Client que deve esperar o jogo

			// O cliente não necessariamente quer jogar
			clientMatrix[id] = 0;
			players_count = 0;

			if(clientPackage.gameOption == WANT_HIGHSCORE) {
				printf("Sending highscore to id: %d\n", id);
				readData(score_file, &score);
				copyRankingToSend(sendHighscore.score, score);
				sendMsgToClient(&sendHighscore, sizeof(serverHighscore), id);
			}

			else if(clientPackage.gameOption == WANNA_QUIT) {
  				if(players_count > 0) players_count--;
					printf("Client disconnected: %d\n", id);

				disconnectClient(id);
			}

			else if(clientPackage.gameOption == WANNA_PLAY) {

				// Update the number of players in the room
				clientMatrix[id] = 1;

				if(gameStart == 1) { // Sala Fechada, pode iniciar o jogo.

					printf("%d players alive\n", players_alive);

					serverPackage.statusPlayer = GAME_PLAYING; // Informa ao Client que o jogo iniciou

					// Verifica se a direção é válida
					if(clientPackage.dir <= 4 && clientPackage.dir >= 0) {
						jogador[id].dir = clientPackage.dir;
					}

					//impede o jogador de virar para trás.
					if(jogador[id].dir == UP && buff_dir[id] == DOWN) jogador[id].dir = DOWN;
					if(jogador[id].dir == LEFT && buff_dir[id] == RIGHT) jogador[id].dir = RIGHT;
					if(jogador[id].dir == DOWN && buff_dir[id] == UP) jogador[id].dir = UP;
					if(jogador[id].dir == RIGHT && buff_dir[id] == LEFT) jogador[id].dir = LEFT;

					serverPackage.dir[id] = jogador[id].dir; // Adiciona a direção do player no pacote

					// Bufferiza a posição para utilizar no próximo ciclo e evitar andar para trás
					buff_pos_anteriorX[id] = jogador[id].x;
					buff_pos_anteriorY[id] = jogador[id].y;

					printf("jogador: %d | x: %d | y: %d | dir: ", id, jogador[id].x, jogador[id].y);

					// Atualiza a posição do player
					switch(jogador[id].dir){
						case UP:
							jogador[id].x--;
							// jogador[id].y--;
							break;
						case DOWN:
							jogador[id].x++;
							// jogador[id].y++;
							break;
						case LEFT:
							jogador[id].y--;
							// jogador[id].x--;
							break;
						case RIGHT:
							jogador[id].y++;
							// jogador[id].y++;
							break;
					}

					buff_dir[id] = jogador[id].dir; // Atualiza a última direção clicada

					/********************************************************
					 * O Jogador atingiu a condição de morte                *
					 ********************************************************/
					// Player acertou os limites do jogo
					if(jogador[id].x >= SIZEX || jogador[id].y >= SIZEY ||
					jogador[id].x < 0 || jogador[id].y < 0 || matrizJogo[jogador[id].x][jogador[id].y] != '0') {


						serverPackage.statusPlayer = GAME_LOST;
						if(players_alive > 0) {
							players_alive--; // Reduz o número de players ainda vivos
						}


						// sendMsgToClient(&serverPackage, sizeof(serverMsg), id);
						//#ifdef RESTART_GAME
						// Remove o rastro do jogador do jogo.
						serverPackage.pontuacao = cleanMatrix(matrizJogo, id);
						strcpy(save.nome, clientPackage.nome);
						save.score = serverPackage.pontuacao;
						saveData(score_file, &save, 1);

						// Recoloca o player em sua posição original do tabuleiro
						// e remove o bit dele da variável de players vivos
						switch(id){
							case 0:
								win_ct &= 0b1110;
								// jogador[0].x = 0; jogador[0].y = 0; jogador[0].dir = buff_dir[0] = DOWN; // Inicia no canto superior esquerdo
								break;
							case 1:
								win_ct &= 0b1101;
								// jogador[1].x = 0; jogador[1].y = SIZEY - 1; jogador[1].dir = buff_dir[1] = LEFT; // Inicia no canto superior direito
								break;
							case 2:
								win_ct &= 0b1011;
								// jogador[2].x = SIZEX - 1; jogador[2].y = 1; jogador[2].dir = buff_dir[2] = RIGHT; // Inicia no canto inferior esquerdo
								break;
							case 3:
								win_ct &= 0b0111;
								// jogador[3].x =  SIZEX - 1; jogador[3].y = SIZEY - 1 ; jogador[3].dir = buff_dir[3] = UP; // Inicia no canto inferior direito
								break;
						}
						pontuacao = 0;
						//disconnectClient(id);
						// #endif
					}
					// Jogador continua vivo
					else {
						// salvamos corpo + id, ja que 'a' + 1 = 'b'
						matrizJogo[jogador[id].x][jogador[id].y] = cabeca + id;
						matrizJogo[buff_pos_anteriorX[id]][buff_pos_anteriorY[id]] = corpo + id;
						printf("Posicao cabeca: %d %d---- Posicao Corpo: %d %d", jogador[id].x, jogador[id].y, buff_pos_anteriorX[id], buff_pos_anteriorY[id]);
					}

					// printf("\nAVISO - CUIDADO:\n");

					// Caso só tenha restado um player o jogador que restou ganha o jogo.
					// if(players_alive <= 1) {
					// 	// winner = id;
					// 	serverPackage.statusPlayer = GAME_WON;
					// 	gameStart = 0;
					// 	printf("\n\nPlayer %d won!!\n\n", id);
					// }
					// else if(id == winner) { // Informa Game Won só para o player ganhador
					// 	serverPackage.statusPlayer = GAME_WON;
					// 	gameStart = 0;
					// 	winner = -1;
					// }

					// Verifica se alguém ganhou
					if(win_ct == 1 || win_ct == 2 || win_ct == 4 || win_ct == 8) {
						if(1 << id == win_ct) { // Verifica se o jogador atual é o vencedor
							serverPackage.statusPlayer = GAME_WON;
							serverPackage.pontuacao = cleanMatrix(matrizJogo, id);

							// força o jogo todo a resetar
							gameStart = 0;
							players_alive = 0;
							players_count = 0;

							printf("\n\nPlayer %d won!!\n\n", id);
							
							// Force the game not to restart
							for(i = 0; i < MAXCLIENTS; i++) {
								clientMatrix[i] = 0;
							}
						}
					}

					puts("Enviando respostas...\n");
					for(i = 0; i < SIZEX; i++){
						for(j = 0; j < SIZEY; j++){
							 serverPackage.matriz[i][j] = matrizJogo[i][j];
						}
					}	
				}


				else if(gameStart == 0) { // Count the number of players that wanna play before the game start

					for(i = 0; i < MAXCLIENTS; i++) {
						players_count += clientMatrix[i];
					}

					// Checa se o número de players é o número mínimo de clients para jogar
					if(players_count >= ROOM_SIZE) {
						win_ct = 0b1111;

						// Recoloca os players na posição inicial pra preparar o jogo
						jogador[0].x = 0; jogador[0].y = 0; jogador[0].dir = buff_dir[0] = DOWN; // Inicia no canto superior esquerdo
						jogador[1].x = 0; jogador[1].y = SIZEY - 1; jogador[1].dir = buff_dir[1] = LEFT; // Inicia no canto superior direito
						jogador[2].x = SIZEX - 1; jogador[2].y = 1; jogador[2].dir = buff_dir[2] = RIGHT; // Inicia no canto inferior esquerdo
						jogador[3].x =  SIZEX - 1; jogador[3].y = SIZEY - 1 ; jogador[3].dir = buff_dir[3] = UP; // Inicia no canto inferior direito

						gameStart = 1;
						players_alive = ROOM_SIZE;

						serverPackage.statusPlayer = GAME_PLAYING;
						// TODO: Enviar a todos os players o status de que
						// estão jogando a partir de agora
						// Dividir quem vai receber a mensagem pelas salas
						
						broadcast(&serverPackage, sizeof(serverMsg));

						printf("\nIniciando o jogo...\n\n");

						continue;
					}
				}

				printf("%d players waiting\n", players_count);
				printf("Sent value: ", serverPackage.statusPlayer);
				sendMsgToClient(&serverPackage, sizeof(serverMsg), id);
			}
			else {
				if(players_count > 0) {
					players_count--;
				}
			}
		}
	}
	return 0;
}

void delay(unsigned int mseconds){
	clock_t goal = mseconds + clock();
	while (goal > clock());
}

int cleanMatrix(char matrix[SIZEX][SIZEY], int player) {
	int i, j, count = 0;
	for (i = 0; i < SIZEX; i++) {
		for (j = 0; j < SIZEY; j++) {
			if(matrix[i][j] == 'a' + player || matrix[i][j] == 'A' + player) {
			count++;
			matrix[i][j] = '0';
			}
		}
	}
return count;
}

void printMatrix(char matrix[SIZEX][SIZEY]) {
	int i, j;
	for (i = 0; i < SIZEX; i++) {
		for (j = 0; j < SIZEY; j++) {
			printf("%c",matrix[i][j]);
		}
		printf("\n");
	}
}
