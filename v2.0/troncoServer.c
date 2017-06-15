#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "libSocket/server.h"

#include <time.h>


#define SIZE 50
#define MAXCLIENTS 4 //Se ligar na biblioteca deles

#define RESTART_GAME

#define UP    0
#define DOWN  1
#define LEFT  2
#define RIGHT 3

typedef struct{
  int dir;
}clientMsg;

typedef struct{
  char matriz[SIZE][SIZE];
  // int status; Inteiro para dizer se o cliente esta vivo no jogo
}serverMsg;

typedef struct{
  int x;
  int y;
  int dir;
}player;

void delay(unsigned int mseconds);
void cleanMatrix(char matrix[][SIZE], int player);

int main(int argc, char **argv){
    
    char corpo = 'a';
    char cabeca = 'A';

    bool sair = false;
    int tecla = 0;
    int i, j; 
    int buff_dir[MAXCLIENTS]; // Buffer de direção do jogador
    // inicializa com a direção inicial de cada player.
    for (i = 0; i < MAXCLIENTS; ++i) {
      buff_dir[i] = 1;
    }

    player jogador[MAXCLIENTS];
    jogador[0].x = 0; jogador[0].y = 0; jogador[0].dir = 1;
    jogador[1].x = 0; jogador[1].y = SIZE - 1; jogador[1].dir = 0; 
 

    char matrizJogo[SIZE][SIZE];


    clientMsg clientPackage;
    serverMsg serverPackage;
    struct msg_ret_t retorno;

    serverInit(MAXCLIENTS);


   for(i = 0; i < SIZE; i++){
      for(j = 0; j < SIZE; j++){
         matrizJogo[i][j] = '0';
      }
   } 

   while (!sair){
        int id = acceptConnection();

        if(id != NO_CONNECTION)
          printf("Alguem se Conectou com ID %d\n", id);

        retorno = recvMsg(&clientPackage);

        if(retorno.status == MESSAGE_OK){
            id = retorno.client_id;
            jogador[id].dir = clientPackage.dir;

            //impede o jogador de virar para trás.
            if(jogador[id].dir == UP && buff_dir[i] == DOWN) jogador[id].dir = DOWN;
            if(jogador[id].dir == LEFT && buff_dir[i] == RIGHT) jogador[id].dir = RIGHT;
            if(jogador[id].dir == DOWN && buff_dir[i] == UP) jogador[id].dir = UP;
            if(jogador[id].dir == RIGHT && buff_dir[i] == LEFT) jogador[id].dir = LEFT;

            switch(jogador[id].dir){
              case 0:
                  jogador[id].x--;
                  break;
              case 1:
                  jogador[id].x++;
                  break;
              case 2:
                  jogador[id].y--;
                  break;
              case 3:
                  jogador[id].y++;
                  break;
            }

            // Atualiza a última direção clicada
            buff_dir[i] = jogador[id].dir;

            /********************************************************
             * O Jogador atingiu a condição de morte                *
             ********************************************************/
            // Player acertou os limites do jogo
            if( jogador[id].x >= SIZE || jogador[id].y >= SIZE ||
              jogador[id].x < 0 || jogador[id].y < 0 ) {
              disconnectClient(id);

              #ifdef RESTART_GAME
              // Remove o rastro do jogador do jogo.
              cleanMatrix(matrizJogo, id);
              // Recoloca o player em sua posição original do tabuleiro
              jogador[id].x = 25;
              jogador[id].y = 25;
              jogador[id].dir = 1;
              #endif
            }

            // Player acertou uma parede.
            if(matrizJogo[jogador[id].x][jogador[id].y] != '0'){
              disconnectClient(id);

              #ifdef RESTART_GAME
              // Remove o rastro do jogador do jogo.
              cleanMatrix(matrizJogo, id);
              // Recoloca o player numa posição qualquer do tabuleiro
              jogador[id].x = 25;
              jogador[id].y = 25;
              jogador[id].dir = 1;
              #endif
            }
            // Jogador continua vivo
            else {
              // salvamos corpo + id, ja que 'a' + 1 = 'b'
              matrizJogo[jogador[id].x][jogador[id].y] = corpo + id;
            }
            if(id == 1){ 
              printf("OI CARAIIIII\n");
              printf("%d %d\n",jogador[1].x, jogador[1].y);
            }

            puts("Enviando respostas...");
            for(i = 0; i < SIZE; i++){
              for(j = 0; j < SIZE; j++){
                 serverPackage.matriz[i][j] = matrizJogo[i][j];
                 //printf("%c",serverPackage.matriz[i][j]);
              }
              //printf("\n");
            }
            
            sendMsgToClient(&serverPackage, sizeof(serverMsg), id);
        }

        
    }
   return 0;
}

void delay(unsigned int mseconds){
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

void cleanMatrix(char matrix[][SIZE], int player) {
  for (int i = 0; i < SIZE; ++i) {
    for (int j = 0; j < SIZE; ++j) {
      if(matrix[i][j] == 'a' + player || matrix[i][j] == 'A' + player) {
        matrix[i][j] = '0';
      }
    }
  }
}