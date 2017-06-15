#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

int main(int argc, char **argv){
   
   ALLEGRO_DISPLAY *display = NULL;

   if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
   }

   display = al_create_display(640, 480);
   if(!display) {
      fprintf(stderr, "failed to create display!\n");
      return -1;
   }

   // Retângulo preenchido: x1, y1, x2, y2, cor
   
   // Linha: x1, y1, x2, y2

 

   al_clear_to_color(al_map_rgb(10,50,30));
   //al_draw_filled_rectangle(88.0, 10.0, 340.0, 77.0, al_map_rgb(0, 255, 255));
   
   al_flip_display();

   al_rest(10.0);

   //al_destroy_display(display);

   return 0;
}


