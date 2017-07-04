#ifndef __ALLEGRO_USEFULL_H__
#define __ALLEGRO_USEFULL_H__

#include "tronco.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
//para escrever texto
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#define AZUL al_map_rgb(22, 55, 132)
#define VERMELHO al_map_rgb(250, 120, 255)
#define JOGADOR1 al_map_rgb(168, 11, 77)
#define CABECA_JOGADOR1 al_map_rgb(250, 152, 0)
#define JOGADOR2 al_map_rgb(107, 180, 12) // VERDE
#define CABECA_JOGADOR2 al_map_rgb(255,255,51) // AMARELO AMARELADO
#define JOGADOR3 al_map_rgb (255,0,255) // Cor para Rafael
#define CABECA_JOGADOR3 al_map_rgb (250,235,215) // antique white
#define JOGADOR4 al_map_rgb (139,69,19) // saddle brown
#define CABECA_JOGADOR4 al_map_rgb (205,133,63) // peru

#define DISP_H 800
#define DISP_W 1440

/**
 * @brief      Plota na tela a matriz do jogo
 *
 * @param      matriz  A matriz
 */
void printaMatriz(char matriz[SIZEX][SIZEY], ALLEGRO_BITMAP *imagem, int dir[]);


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


int inicializarMenu(ALLEGRO_DISPLAY **display, ALLEGRO_EVENT_QUEUE **fila_eventos);


void get_desktop_resolution(int adapter, int *w, int *h);

#endif
