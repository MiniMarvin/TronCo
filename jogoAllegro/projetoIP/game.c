#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#define AZUL al_map_rgb(0, 255, 255)

#define SIZE 20

int main(int argc, char **argv){
   
   ALLEGRO_DISPLAY *display = NULL;
   int i, j; 
   int sizeQuadrado = 15;// matrizQuadrado[600][480];
   int xInicial = 2, yInicial = 2;
   int velocidadeX = 10, velocidadeY = 10;
   int* jogadores = (int *) malloc(4 * sizeof(int));

   if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
   }

   if (!al_init_primitives_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on de primitivas.\n");
        return false;
    }

   display = al_create_display(640, 480);
   if(!display) {
      fprintf(stderr, "failed to create display!\n");
      return -1;
   } 

   al_clear_to_color(al_map_rgb(10,50,30));
   al_flip_display();
   for(i = 0; i < SIZE; i++){
      for (j = 0; j < SIZE; j++){
      //sal_clear_to_color(al_map_rgb(10,50,30));
      // Retângulo preenchido: x1, y1, x2, y2, cor
      xInicial = xInicial + sizeQuadrado + 2;// sizeQuadrado;

      al_draw_filled_rectangle(xInicial, yInicial,
                              (xInicial + sizeQuadrado), (yInicial + sizeQuadrado),
                              AZUL);
      }
      xInicial = 2;
      yInicial = yInicial + sizeQuadrado + 2;
      //al_rest(0.5);
    }

    al_flip_display();
    
 
    // al_clear_to_color(al_map_rgb(0, 0, 0));
    // al_flip_display();
    // al_rest(0.5);
    ///////////////////
   // al_clear_to_color(al_map_rgb(10,50,30));
   
   // al_flip_display();

   al_rest(10);

   //al_destroy_display(display);

   return 0;
}

