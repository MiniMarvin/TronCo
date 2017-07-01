#ifndef __TRONCO_GAME_H__
#define __TRONCO_GAME_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define SIZEX 25
#define SIZEY 45
#define WIDTH 1450
#define HEIGTH 815
#define MAXCLIENTS 4 //Se ligar na biblioteca deles

typedef struct{
  int dir;
  int status;
}clientMsg;

typedef struct{
  int x, x_anterior;
  int y, y_anterior;
  int pontuacao;
  int dir;
  int status;
}player;

typedef struct{
  //char matriz[SIZEX][SIZEY];
  int dir[4];
  int id;
}serverMsg;

/**
 * @brief      Espera um tempo em mili segundos.
 *
 * @param[in]  mseconds  O tempo a esperar.
 */
void delay(unsigned int mseconds);

#endif
