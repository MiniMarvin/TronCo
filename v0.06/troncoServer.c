#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "libTronco/tronco.h"
#include "libSocket/server.h"

#include <time.h>

#define MAXCLIENTS 4 //Se ligar na biblioteca deles

#define RESTART_GAME

#define UP    0
#define DOWN  1
#define LEFT  2
#define RIGHT 3

typedef struct{
  int x;
  int y;
  int dir;
}player;

void delay(unsigned int mseconds);
void cleanMatrix(char matrix[SIZEX][SIZEY], int player);
void printMatrix(char matrix[SIZEX][SIZEY]);

int main(int argc, char **argv){
    
    char corpo = 'a';
    char cabeca = 'A';

    bool sair = false;
    int tecla = 0;
    int i, j; 
    int buff_dir[MAXCLIENTS]; // Buffer de direção do jogador
    int buff_pos_anteriorX[MAXCLIENTS]; // Buffer da posicao anterior para poder colocar a cabeca da Cobra no local correto
    int buff_pos_anteriorY[MAXCLIENTS]; // Buffer da posicao anterior para poder colocar a cabeca da Cobra no local correto

    // inicializa com a direção inicial de cada player.
    for (i = 0; i < MAXCLIENTS; ++i) {
      buff_dir[i] = 1;
    }

    player jogador[MAXCLIENTS];
    jogador[0].x = 0; jogador[0].y = 0; jogador[0].dir = UP;
    jogador[1].x = 0; jogador[1].y = SIZEY - 1; jogador[1].dir = DOWN;
    jogador[2].x = SIZEX - 1; jogador[2].y = 0; jogador[2].dir = LEFT;
    jogador[3].x = SIZEX - 1; jogador[3].y = SIZEY - 1; jogador[3].dir = LEFT;  
 

    char matrizJogo[SIZEX][SIZEY];


    clientMsg clientPackage;
    serverMsg serverPackage;
    struct msg_ret_t retorno;

    serverInit(MAXCLIENTS);


   for(i = 0; i < SIZEX; i++){
      for(j = 0; j < SIZEY; j++){
         matrizJogo[i][j] = '0';
         printf("%c", matrizJogo[i][j]);
      }
      printf("\n");
   } 

   while(!sair){
        int id = acceptConnection();

        sendMsgToClient(&serverPackage, sizeof(serverMsg), id);

        // printf("1:\n");
        // printMatrix(matrizJogo);

        if(id != NO_CONNECTION){
          printf("Alguem se Conectou com ID %d\n", id);
        }
        

        // printf("2:\n");
        // printMatrix(matrizJogo);

        retorno = recvMsg(&clientPackage);
        // printf("3:\n");
        // printMatrix(matrizJogo);

        if(retorno.status == MESSAGE_OK){

            id = retorno.client_id;
            jogador[id].dir = clientPackage.dir;

            //impede o jogador de virar para trás.
            if(jogador[id].dir == UP && buff_dir[i] == DOWN) jogador[id].dir = DOWN;
            if(jogador[id].dir == LEFT && buff_dir[i] == RIGHT) jogador[id].dir = RIGHT;
            if(jogador[id].dir == DOWN && buff_dir[i] == UP) jogador[id].dir = UP;
            if(jogador[id].dir == RIGHT && buff_dir[i] == LEFT) jogador[id].dir = LEFT;

            buff_pos_anteriorX[id] = jogador[id].x;
            buff_pos_anteriorY[id] = jogador[id].y;

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
            buff_dir[id] = jogador[id].dir;

            // printf("\nAVISO - CUIDADO:\n");
            // printMatrix(matrizJogo);
            // return 0;
            
            /********************************************************
             * O Jogador atingiu a condição de morte                *
             ********************************************************/
            // Player acertou os limites do jogo
            if(jogador[id].x >= SIZEX || jogador[id].y >= SIZEY ||
              jogador[id].x < 0 || jogador[id].y < 0 ) {
              disconnectClient(id);

              //#ifdef RESTART_GAME
              // Remove o rastro do jogador do jogo.
              cleanMatrix(matrizJogo, id);
              // Recoloca o player em sua posição original do tabuleiro
              switch(id){
                case 0:
                  jogador[0].x = 0; jogador[0].y = 0; jogador[0].dir = UP;
                break;
                case 1:
                  jogador[1].x = 0; jogador[1].y = SIZEY - 1; jogador[1].dir = DOWN;
                break;
              }

              //#endif
            }

            // Player acertou uma parede.
            if(matrizJogo[jogador[id].x][jogador[id].y] != '0'){
              disconnectClient(id);

              //#ifdef RESTART_GAME
              // Remove o rastro do jogador do jogo.
              cleanMatrix(matrizJogo, id);
              // Recoloca o player numa posição qualquer do tabuleiro
              switch(id){
                case 0:
                  jogador[0].x = 0; jogador[0].y = 0; jogador[0].dir = UP;
                break;
                case 1:
                  jogador[1].x = 0; jogador[1].y = SIZEY - 1; jogador[1].dir = DOWN;
                break;
              }
              //#endif
            }
            // Jogador continua vivo
            else {
              printf("\nAVISO - CUIDADO - PRIMEIRO:\n");
              printMatrix(matrizJogo);
              // salvamos corpo + id, ja que 'a' + 1 = 'b'
              matrizJogo[jogador[id].x][jogador[id].y] = cabeca + id;
              matrizJogo[buff_pos_anteriorX[id]][buff_pos_anteriorY[id]] = corpo + id;
              printf("Posicao cabeca: %d %d---- Posicao Corpo: %d %d", jogador[id].x, jogador[id].y, buff_pos_anteriorX[id], buff_pos_anteriorY[id]);
            }
            printf("\nAVISO - CUIDADO:\n");
            printMatrix(matrizJogo);
            // if(id == 1){ 
            //   printf("OI CARAIIIII\n");
            //   printf("%d %d\n",jogador[1].x, jogador[1].y);
            // }

            puts("Enviando respostas...");
            for(i = 0; i < SIZEX; i++){
              for(j = 0; j < SIZEY; j++){
                 serverPackage.matriz[i][j] = matrizJogo[i][j];
                 printf("%c",serverPackage.matriz[i][j]);
                 // if(serverPackage.matriz[i][j] != '0' && serverPackage.matriz[i][j] != 'a' && serverPackage.matriz[i][j] != 'A'){
                 //  printf("\n\n\n%d\n\n\n", serverPackage.matriz[i][j]);
                 // }
              }
              printf("\n");
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

void cleanMatrix(char matrix[SIZEX][SIZEY], int player) {
  int i, j;
  for (i = 0; i < SIZEX; i++) {
    for (j = 0; j < SIZEY; j++) {
      if(matrix[i][j] == 'a' + player || matrix[i][j] == 'A' + player) {
        matrix[i][j] = '0';
      }
    }
  }
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