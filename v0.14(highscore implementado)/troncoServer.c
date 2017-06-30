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

typedef struct{
  int x;
  int y;
  int dir;
}player;

void delay(unsigned int mseconds);
int cleanMatrix(char matrix[SIZEX][SIZEY], int player);
void printMatrix(char matrix[SIZEX][SIZEY]);

int main(int argc, char **argv){
    
    FILE* score_file;
    openFile(&score_file);

    data* score;
    readData(score_file, &score);


    char corpo = 'a';
    char cabeca = 'A';

    bool sair = false;
    int tecla = 0;
    int i, j, pontuacao = 0; 
    int buff_dir[MAXCLIENTS]; // Buffer de direção do jogador
    int buff_pos_anteriorX[MAXCLIENTS]; // Buffer da posicao anterior para poder colocar a cabeca da Cobra no local correto
    int buff_pos_anteriorY[MAXCLIENTS]; // Buffer da posicao anterior para poder colocar a cabeca da Cobra no local correto


    clientMsg clientPackage;
    serverMsg serverPackage;
    struct msg_ret_t retorno;

    copyRankingToSend(serverPackage.highscore, score);

    printData(serverPackage.highscore, 3);
    // inicializa com a direção inicial de cada player.
    for (i = 0; i < MAXCLIENTS; ++i) {
      // buff_dir[i] = 0;
      serverPackage.statusPlayer = 1; // Significa que ele esta vivo
    }

    //// debugar direcao dos clients **********************************************8
    player jogador[MAXCLIENTS];
    jogador[0].x = 0; jogador[0].y = 0; jogador[0].dir = buff_dir[0] = DOWN;
    jogador[1].x = 0; jogador[1].y = SIZEY - 1; jogador[1].dir = buff_dir[1] = DOWN;
    // jogador[2].x = SIZEX - 1; jogador[2].y = SIZEY - 1; jogador[2].dir = LEFT; // iniciando o y no lugar errado
    jogador[2].x = SIZEX - 1; jogador[2].y = 0; jogador[2].dir = buff_dir[2] = LEFT; // iniciando o y no lugar errado
    // jogador[3].x =  SIZEX - 1; jogador[3].y = SIZEY - 1; jogador[3].dir = buff_dir[3] = RIGHT;  
    jogador[3].x =  SIZEX - 10; jogador[3].y = SIZEY/2; jogador[3].dir = buff_dir[3] = RIGHT;  
 

    char matrizJogo[SIZEX][SIZEY];

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

        serverPackage.statusPlayer = 1;
        //sendMsgToClient(&serverPackage, sizeof(serverMsg), id);

        // printf("1:\n");
        // printMatrix(matrizJogo);

        if(id != NO_CONNECTION){
          printf("Alguem se Conectou com ID %d\n", id);
        }
        

        // if(id == 2) {
        //   printf("\ndireção original: %d", jogador[id].dir);
        //   return 0;
        // }

        // printf("2:\n");
        // printMatrix(matrizJogo);

        retorno = recvMsg(&clientPackage);
        // printf("3:\n");
        // printMatrix(matrizJogo);

        if(retorno.status == MESSAGE_OK){

          id = retorno.client_id;

          if(clientPackage.wantHighscore == 1) {
            printf("Sending highscore to id: %d\n", id);

            sendMsgToClient(&serverPackage, sizeof(serverMsg), id);

          } else {

            
            if(clientPackage.dir <= 4) {
              jogador[id].dir = clientPackage.dir;
            }

            //impede o jogador de virar para trás.
            if(jogador[id].dir == UP && buff_dir[id] == DOWN) jogador[id].dir = DOWN;
            if(jogador[id].dir == LEFT && buff_dir[id] == RIGHT) jogador[id].dir = RIGHT;
            if(jogador[id].dir == DOWN && buff_dir[id] == UP) jogador[id].dir = UP;
            if(jogador[id].dir == RIGHT && buff_dir[id] == LEFT) jogador[id].dir = LEFT;

            serverPackage.dir[id] = jogador[id].dir;

            buff_pos_anteriorX[id] = jogador[id].x;
            buff_pos_anteriorY[id] = jogador[id].y;

            printf("jogador: %d | x: %d | y: %d | dir: ", id, jogador[id].x, jogador[id].y);

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
              jogador[id].x < 0 || jogador[id].y < 0 || matrizJogo[jogador[id].x][jogador[id].y] != '0') {
              serverPackage.statusPlayer = 0;
              sendMsgToClient(&serverPackage, sizeof(serverMsg), id);
              //#ifdef RESTART_GAME
              // Remove o rastro do jogador do jogo.
              serverPackage.pontuacao = cleanMatrix(matrizJogo, id);
              
              data save;
              strcpy(save.nome, clientMsg.nome);
              save.score = pontuacao;
              saveData(score_file, &data, 1);
              // Recoloca o player em sua posição original do tabuleiro
              switch(id){
                case 0:
                  jogador[0].x = 0; jogador[0].y = 0; jogador[0].dir = DOWN;
                  buff_dir[0] = DOWN;
                  break;
                case 1:
                  jogador[1].x = 0; jogador[1].y = SIZEY - 1; jogador[1].dir = DOWN;
                  buff_dir[0] = DOWN;
                  break;
                case 2:
                  jogador[2].x = SIZEX - 1; jogador[2].y = SIZEY - 1; jogador[2].dir = LEFT;
                  buff_dir[0] = LEFT;
                  break;
                case 3:
                  jogador[3].x = 0; jogador[3].y = SIZEY - 1; jogador[3].dir = RIGHT;
                  buff_dir[0] = RIGHT;
                  break;
              }
              pontuacao = 0;
              disconnectClient(id);
              //#endif
            }
            // Jogador continua vivo
            else {
              printf("\nAVISO - CUIDADO - PRIMEIRO:\n");
              // printMatrix(matrizJogo);
              // salvamos corpo + id, ja que 'a' + 1 = 'b'
              matrizJogo[jogador[id].x][jogador[id].y] = cabeca + id;
              matrizJogo[buff_pos_anteriorX[id]][buff_pos_anteriorY[id]] = corpo + id;
              printf("Posicao cabeca: %d %d---- Posicao Corpo: %d %d", jogador[id].x, jogador[id].y, buff_pos_anteriorX[id], buff_pos_anteriorY[id]);
              sendMsgToClient(&serverPackage, sizeof(serverMsg), id);
            }
            printf("\nAVISO - CUIDADO:\n");
            // printMatrix(matrizJogo);
            // if(id == 1){ 
            //   printf("OI CARAIIIII\n");
            //   printf("%d %d\n",jogador[1].x, jogador[1].y);
            // }

            puts("Enviando respostas...\n");
            for(i = 0; i < SIZEX; i++){
              for(j = 0; j < SIZEY; j++){
                 serverPackage.matriz[i][j] = matrizJogo[i][j];
                 // printf("%c",serverPackage.matriz[i][j]);
                 // if(serverPackage.matriz[i][j] != '0' && serverPackage.matriz[i][j] != 'a' && serverPackage.matriz[i][j] != 'A'){
                 //  printf("\n\n\n%d\n\n\n", serverPackage.matriz[i][j]);
                 // }
              }
              // printf("\n");
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
