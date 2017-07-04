#include "allegroUsefull.h"

bool inicializar(ALLEGRO_DISPLAY **display, ALLEGRO_EVENT_QUEUE **fila_eventos) {

  if(!al_init()) {
    fprintf(stderr, "failed to initialize allegro!\n");
    return -1;
  }

  if (!al_init_primitives_addon()) {
    fprintf(stderr, "Falha ao inicializar add-on de primitivas.\n");
    return false;
  }
  if (!al_init_image_addon()) {
    fprintf(stderr, "Falha ao inicializar add-on de imagens.\n");
    return false;
  }

  *display = al_create_display(900, 1600);
  if(!display) {
    fprintf(stderr, "failed to create display!\n");
    return -1;
  }

  *fila_eventos = al_create_event_queue();
  if(!fila_eventos) {
    fprintf(stderr, "Falha ao criar fila de eventos.\n");
    al_destroy_display(*display);
    return -1;
  }

  if (!al_install_keyboard()) {
    fprintf(stderr, "Falha ao inicializar o teclado.\n");
    return false;
  }
  al_register_event_source(*fila_eventos, al_get_keyboard_event_source());
  al_register_event_source(*fila_eventos, al_get_display_event_source(*display));

  return true;
}

 void printaMatriz(char matriz[][SIZE], ALLEGRO_BITMAP *imagem) {
   int i, j;
   int sizeQuadrado = 16;// matrizQuadrado[600][480];
   int xInicial = 0, yInicial = 0;


   for(i = 0; i < SIZE; i++) {
      for(j = 0; j < SIZE; j++) {
        
        xInicial = xInicial + sizeQuadrado + 2;// sizeQuadrado;

        switch(matriz[i][j]) {
          case '0':
          //x1, y1, x2, y2
             
             al_draw_filled_rectangle(xInicial, yInicial,
                                  (xInicial + sizeQuadrado), (yInicial + sizeQuadrado),
                                  AZUL);
             break;
          case 'a':

              // al_draw_bitmap_region(imagem, 64, 0, sizeQuadrado, sizeQuadrado, xInicial, yInicial, 0);
            al_draw_filled_rectangle(xInicial, yInicial,
                                  (xInicial + sizeQuadrado), (yInicial + sizeQuadrado),
                                  JOGADOR1);
            break;
          case 'A':
            al_draw_bitmap_region(imagem, 0, 0, sizeQuadrado, sizeQuadrado, xInicial, yInicial, 0);
            // al_draw_filled_rectangle(xInicial, yInicial,
            //                       (xInicial + sizeQuadrado), (yInicial + sizeQuadrado),
            //                       CABECA_JOGADOR1);
            break;
          case 'b':
            al_draw_filled_rectangle(xInicial, yInicial,
                                  (xInicial + sizeQuadrado), (yInicial + sizeQuadrado),
                                  JOGADOR2);
             break;
          case 'B':
            al_draw_filled_rectangle(xInicial, yInicial,
                                  (xInicial + sizeQuadrado), (yInicial + sizeQuadrado),
                                  CABECA_JOGADOR2);
            break;
          case 'c':
            al_draw_filled_rectangle(xInicial, yInicial,
                                  (xInicial + sizeQuadrado), (yInicial + sizeQuadrado),
                                  JOGADOR3);
            break;
          case 'C':
            al_draw_filled_rectangle(xInicial, yInicial,
                                  (xInicial + sizeQuadrado), (yInicial + sizeQuadrado),
                                  CABECA_JOGADOR3);
            break;
          case 'd':
            al_draw_filled_rectangle(xInicial, yInicial,
                                  (xInicial + sizeQuadrado), (yInicial + sizeQuadrado),
                                  JOGADOR4);
            break;
          case 'D':
            al_draw_filled_rectangle(xInicial, yInicial,
                                  (xInicial + sizeQuadrado), (yInicial + sizeQuadrado),
                                  CABECA_JOGADOR4);
            break;
          default:
            // al_draw_filled_rectangle(xInicial, yInicial,
            //                       (xInicial + sizeQuadrado), (yInicial + sizeQuadrado),
            //                       AZUL);
            break;
        }
      }
      xInicial = 2;
      yInicial = yInicial + sizeQuadrado + 2;
   }
}
