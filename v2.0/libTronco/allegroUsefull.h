#ifndef __ALLEGRO_USEFULL_H__
#define __ALLEGRO_USEFULL_H__

#include "tronco.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>


#define AZUL al_map_rgb(22, 55, 132)
#define VERMELHO al_map_rgb(250, 120, 255)
#define JOGADOR1 al_map_rgb(168, 11, 77)
#define JOGADOR2 al_map_rgb(107, 180, 12) // VERDE


/**
 * @brief      Plota na tela a matriz do jogo
 *
 * @param      matriz  A matriz
 */
void printaMatriz(char matriz[SIZE][SIZE]);


 /**
  * @brief      Inicializa o allegro e realiza as definições necessárias.
  *
  * @param      display       Ponteiro para a tela do allegro que será utilizada.
  * @param      fila_eventos  Ponteiro para a fila eventos que será utilizada.
  *
  * @return     1 - Caso tenha ocorrido sucesso na inicialização
  *             demais valores significam falha.
  */
bool inicializar(ALLEGRO_DISPLAY **display, ALLEGRO_EVENT_QUEUE **fila_eventos);


#endif