#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

//para escrever texto
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

//input


int main(int argc, char **argv){
   
   if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
   }
   al_init_primitives_addon();
   al_init_font_addon(); // initialize the font addon
   al_init_ttf_addon();// initialize the ttf (True Type Font) addon
   

   //input
   ALLEGRO_DISPLAY *display = al_create_display(640,480);
   
   if (!display){
      fprintf(stderr, "Failed to create display.\n");
      return -1;
   }

   ALLEGRO_FONT *font_big = al_load_ttf_font("Tr2n.ttf",72,0);
   ALLEGRO_FONT *font_small = al_load_ttf_font("Tr2n.ttf",38,0);

   if (!font_big){
      fprintf(stderr, "Could not load 'Tr2n.ttf'.\n");
      return -1;
   }
   al_install_keyboard();
   ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
   fila_eventos = al_create_event_queue();
   al_register_event_source(fila_eventos, al_get_keyboard_event_source());
   al_register_event_source(fila_eventos, al_get_display_event_source(display));
   int cursorPos = 0;
   bool running = true;

   while(running){
	   //printar
	   //printf("%d\n", cursorPos);
	   al_clear_to_color(al_map_rgb(50,10,70));
	   al_draw_text(font_big, al_map_rgb(255,255,255), 640/2, 50,ALLEGRO_ALIGN_CENTRE, "TRONco");
	   al_draw_text(font_small, al_map_rgb(255,255,255), 640/2, 200,ALLEGRO_ALIGN_CENTRE, "Jogar");
	   al_draw_text(font_small, al_map_rgb(255,255,255), 640/2, 250,ALLEGRO_ALIGN_CENTRE, "Ranking");
	   al_draw_text(font_small, al_map_rgb(255,255,255), 640/2, 300,ALLEGRO_ALIGN_CENTRE, "Sair");
	   
	   //cursor
	   al_draw_circle(200, 165 + 50*(cursorPos + 1), 12, al_map_rgb(255,255,255),3);
	   al_flip_display();

	
	   //pegar input
	   int tecla = 0;

	   while(!al_is_event_queue_empty(fila_eventos)){
	  	ALLEGRO_EVENT evento;
		al_wait_for_event(fila_eventos, &evento);
	 	if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
		        switch(evento.keyboard.keycode){
				case ALLEGRO_KEY_UP:
				    tecla = 1;
				    cursorPos--;
				    if (cursorPos == -1) cursorPos = 2;
				    cursorPos = cursorPos%3;
				    break;
				case ALLEGRO_KEY_DOWN:
				    tecla = 2;
				    cursorPos++;
		                    cursorPos = cursorPos%3;
				    break;

				case ALLEGRO_KEY_ENTER:
				    tecla = 3;
				    running = false;
				    //jk = 0;
				    break;
			}
		 }
		  else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
		        running = false;
		  }
	   }
   }
   

   //al_rest(10.0);

   al_destroy_display(display);

   return 0;
}

