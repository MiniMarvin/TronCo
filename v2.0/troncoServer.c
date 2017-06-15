#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>

#include "libSocket/server.h"
#include "libAllegro/AllegroCore.h"

#include <time.h>


#define SIZE 50
#define MAXCLIENTS 4 //Se ligar na biblioteca deles

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

int main(int argc, char **argv){
    
   char corpo = 'a';
   char cabeca = 'A';

    bool sair = false;
    int tecla = 0;
    int i, j; 

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

            if(matrizJogo[jogador[id].x][jogador[id].y] != '0'){
              disconnectClient(id);
            }else{
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