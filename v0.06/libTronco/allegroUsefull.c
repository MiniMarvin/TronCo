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

  /************************************************************************************
   * Inicialização e reajuste do display para caber na tela
   ************************************************************************************/
  // Gera um display 100% da tela
  al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
  // *display = al_create_display(windowWidth, windowHeight);

  *display = al_create_display(1550, 870);
  if(!display) {
    fprintf(stderr, "failed to create display!\n");
    return -1;
  }

  int windowWidth = al_get_display_width(*display);
  int windowHeight = al_get_display_height(*display);
  int screenWidth = 640;
  int screenHeight = 480;

  float sx = windowWidth / (float)screenWidth;
  float sy = windowHeight / (float)screenHeight;

  ALLEGRO_TRANSFORM trans;
  al_identity_transform(&trans);
  al_scale_transform(&trans, sx, sy);
  al_use_transform(&trans);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
  


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

  // Colocar IF's
  al_init_primitives_addon();
  al_init_font_addon(); // initialize the font addon
  al_init_ttf_addon();// initialize the ttf (True Type Font) addon

  

  al_register_event_source(*fila_eventos, al_get_keyboard_event_source());
  al_register_event_source(*fila_eventos, al_get_display_event_source(*display));

  return true;
}

void printaMatriz(char matriz[SIZEX][SIZEY], ALLEGRO_BITMAP *imagem) {
   int i, j;
   int sizeQuadrado = 32;// matrizQuadrado[600][480];
   int xInicial = 0, yInicial = 0;
   
   for(i = 0; i < SIZEX; i++) {
      for(j = 0; j < SIZEY; j++) {

        switch(matriz[i][j]) {
          case '0':
          //x1, y1, x2, y2
             al_draw_bitmap_region(imagem, 0, 0, sizeQuadrado, sizeQuadrado, xInicial, yInicial, 0);
             // al_draw_filled_rectangle(xInicial, yInicial,
             //                      (xInici al + sizeQuadrado), (yInicial + sizeQuadrado),
             //                      AZUL);
             break;
          case 'a':

              // al_draw_bitmap_region(imagem, 64, 0, sizeQuadrado, sizeQuadrado, xInicial, yInicial, 0);
			     al_draw_bitmap_region(imagem, 0, 0, sizeQuadrado, sizeQuadrado, xInicial, yInicial, 0);
           al_draw_bitmap_region(imagem, 128, 32, sizeQuadrado, sizeQuadrado, xInicial, yInicial, 0);
            // al_draw_filled_rectangle(xInicial, yInicial,
                                  // (xInicial + sizeQuadrado), (yInicial + sizeQuadrado),
                                  // JOGADOR1);
            break;
          case 'A':
            al_draw_bitmap_region(imagem, 1 * 32, 32, sizeQuadrado, sizeQuadrado, xInicial, yInicial, 0);
            // al_draw_bitmap_region(imagem, 0, 0, sizeQuadrado, sizeQuadrado, xInicial, yInicial, 0);
            // al_draw_filled_rectangle(xInicial, yInicial,
            //                       (xInicial + sizeQuadrado), (yInicial + sizeQuadrado),
            //                       CABECA_JOGADOR1);
            break;
          case 'b':
            al_draw_bitmap_region(imagem, 0, 0, sizeQuadrado, sizeQuadrado, xInicial, yInicial, 0);
            al_draw_bitmap_region(imagem, 128, 64, sizeQuadrado, sizeQuadrado, xInicial, yInicial, 0);
             break;
          case 'B':
            al_draw_bitmap_region(imagem, 1 * 32, 64, sizeQuadrado, sizeQuadrado, xInicial, yInicial, 0);
            break;
          case 'c':
            al_draw_bitmap_region(imagem, 0, 0, sizeQuadrado, sizeQuadrado, xInicial, yInicial, 0);
            al_draw_bitmap_region(imagem, 128, 96, sizeQuadrado, sizeQuadrado, xInicial, yInicial, 0);
            break;
          case 'C':
            al_draw_bitmap_region(imagem, 1 * 32, 96, sizeQuadrado, sizeQuadrado, xInicial, yInicial, 0);
            break;
          case 'd':
            al_draw_bitmap_region(imagem, 0, 0, sizeQuadrado, sizeQuadrado, xInicial, yInicial, 0);
            al_draw_bitmap_region(imagem, 128, 128, sizeQuadrado, sizeQuadrado, xInicial, yInicial, 0);
            break;
          case 'D':
            al_draw_bitmap_region(imagem, 1 * 32, 128, sizeQuadrado, sizeQuadrado, xInicial, yInicial, 0);
            break;
          default:
            // al_draw_filled_rectangle(xInicial, yInicial,
            //                       (xInicial + sizeQuadrado), (yInicial + sizeQuadrado),
            //                       AZUL);
            break;
        }
        xInicial = xInicial + sizeQuadrado;// sizeQuadrado;
      }
      xInicial = 0;
      yInicial = yInicial + sizeQuadrado;
   }
}
