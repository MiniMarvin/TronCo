#ifndef __TRONCO_GAME_H__
#define __TRONCO_GAME_H__

#include "highscore.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define SIZEX 25
#define SIZEY 45
#define WIDTH 1366
#define HEIGTH 768
#define ROOM_SIZE 4
#define MAXCLIENTS 12 //Se ligar na biblioteca deles
#define SIZE_NOME 10

// Opções de jogo
#define WANT_HIGHSCORE 1
#define WANNA_PLAY 2
#define WANNA_QUIT 3

//
#define GAME_WAITING 0
#define GAME_PLAYING 1
#define GAME_WON 2
#define GAME_LOST 3

typedef struct {
  data score[5];
} serverHighscore;


typedef struct{
  int dir;
  // int wantHighscore;
  int gameOption;
  char nome[SIZE_NOME];
}clientMsg;

typedef struct{
  char matriz[SIZEX][SIZEY];
  int dir[MAXCLIENTS];
  char statusPlayer;
  int pontuacao;
  int flagVitoria; // 1 - Se foi o Vencedor 0 - se foi Perdedor
  // int status; Inteiro para dizer se o cliente esta vivo no jogo
  // data highscore[5];
}serverMsg;

/**
 * @brief      Espera um tempo em mili segundos.
 *
 * @param[in]  mseconds  O tempo a esperar.
 */
void delay(unsigned int mseconds);

#endif
