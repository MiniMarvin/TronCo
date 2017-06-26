#ifndef __TRONCO_GAME_H__
#define __TRONCO_GAME_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define SIZEX 25
#define SIZEY 45

typedef struct{
  int dir;
}clientMsg;

typedef struct{
  char matriz[SIZEX][SIZEY];
  // int status; Inteiro para dizer se o cliente esta vivo no jogo
}serverMsg;

/**
 * @brief      Espera um tempo em mili segundos.
 *
 * @param[in]  mseconds  O tempo a esperar.
 */
void delay(unsigned int mseconds);

#endif