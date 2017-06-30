#ifndef __TRONCO_GAME_H__
#define __TRONCO_GAME_H__

#include "highscore.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define SIZEX 25
#define SIZEY 45
#define WIDTH 1450
#define HEIGTH 815
#define MAXCLIENTS 4 //Se ligar na biblioteca deles
#define SIZE_NOME 10
#define WANT_HIGHSCORE 1
#define WANNA_PLAY 2
#define WANNA_QUIT 3

typedef struct{
  int dir;
  // int wantHighscore;
  int gameOption;
  char nome[SIZE_NOME];
}clientMsg;

typedef struct{
  char matriz[SIZEX][SIZEY];
  int dir[MAXCLIENTS];
  int statusPlayer;
  int pontuacao;
  int flagVitoria; // 1 - Se foi o Vencedor 0 - se foi Perdedor
  // int status; Inteiro para dizer se o cliente esta vivo no jogo
  data highscore[5];
}serverMsg;

/**
 * @brief      Espera um tempo em mili segundos.
 *
 * @param[in]  mseconds  O tempo a esperar.
 */
void delay(unsigned int mseconds);

#endif