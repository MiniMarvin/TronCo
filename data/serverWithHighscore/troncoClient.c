#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>

#include "libSocket/client.h"
#include "libAllegro/AllegroCore.h"
#include "libTronco/tronco.h"
#include "libTronco/allegroUsefull.h"

#include <time.h>

// #define AZUL al_map_rgb(22, 55, 132)
// #define VERMELHO al_map_rgb(250, 120, 255)
// #define JOGADOR1 al_map_rgb(168, 11, 77)
// #define JOGADOR2 al_map_rgb(107, 180, 12) // VERDE

// #define SIZE 50

// typedef struct{
//   int dir;
// }clientMsg;

// typedef struct{
//   char matriz[SIZE][SIZE];
//   // int status; Inteiro para dizer se o cliente esta vivo no jogo
// }serverMsg;

// Exibe uma tela 
ALLEGRO_DISPLAY *display = NULL;

ALLEGRO_BITMAP *imagem = NULL;

// Evento de captura do teclado
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;


int menu();
int jogo();
int o_jogo();
int ranking();

int main(int argc, char **argv) {
	int estado = 0;
	bool running = true;

	if (!inicializar(&display, &fila_eventos)) {
		return -1;
	}
   
	/*
	0 = menu
	1 = jogo
	2 = highscores
	3 = sair
	*/
	while(running){
		switch(estado){
			case 0:	//menu
				estado = menu();
				break;
			
			case 1:
				estado = jogo();
				break;

			case 2:
				estado = ranking();
				break;
			case 3:
				running = false;
				break;
		}
	}


	 return 0;
}



int jogo(){
	int estado_jogo = 2;
	/*
		0 = colocar nome se não tiver nome já cadastrado
		1 = salinha de espera
		2 = o jogo em si
		3 = volta pro menu
	*/

	int playing = true;

	while(playing){
		switch(estado_jogo){
			case 0:
				//estado_jogo = getNome();
				break;

			case 1:
				//estado_jogo = salinha();
				break;

			case 2:
				estado_jogo = o_jogo();
				break;

			case 3:
				return 0;
		}
	}
}


int o_jogo(){
	bool sair = false;
	int tecla = 0;
	int posX = 20, posY = 20;
	int direcao = 1, buff_direcao = 1;
	int i, j; 
	int sizeQuadrado = 8;// matrizQuadrado[600][480];
	int xInicial = 2, yInicial = 2;
	int velocidadeX = 10, velocidadeY = 10;
	int* jogadores = (int *) malloc(4 * sizeof(int));

	clientMsg clientPackage;
	serverMsg serverPackage;

	//Esse IP irá nos conectar a "nós mesmos", apenas para efeito de testes.
	char ServerIP[30]={"127.0.0.1"};
	connectToServer(ServerIP);

	al_clear_to_color(al_map_rgb(10,50,30));
	al_flip_display();

	imagem = al_load_bitmap("./Resources/Tilesets/troncoSheet.png");

	while(!sair){
		printf("OI\n");
		if(imagem == NULL) printf("\nOUTRA COISA\n");
		while(!al_is_event_queue_empty(fila_eventos)) {
			printf("KEY PRESSED\n");
			ALLEGRO_EVENT evento;
			al_wait_for_event(fila_eventos, &evento);

			if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
				switch(evento.keyboard.keycode) {
					case ALLEGRO_KEY_UP:
							tecla = 1;
							break;
					case ALLEGRO_KEY_DOWN:
							tecla = 2;
							break;
					case ALLEGRO_KEY_LEFT:
							tecla = 3;
							break;
					case ALLEGRO_KEY_RIGHT:
							tecla = 4;
							break;
				}
			}
			else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				sair = true;
			}
		}

		if(tecla) {
			switch (tecla) {
				case 1:
					direcao = 0;
					break;
				case 2:
					direcao = 1;
					break;
				case 3:
					direcao = 2;
					break;
				case 4:
					direcao = 3;
					break;
			}
			tecla = 0;
		}

		clientPackage.dir = direcao; // Atualiza para o caso do valor ser inconsistente.

		//enviando a mensagem
		sendMsgToServer(&clientPackage, sizeof(clientPackage));

		//recebendo a mensagem
		//experimentem trocar WAIT_FOR_IT por DONT_WAIT...
		recvMsgFromServer(&serverPackage, WAIT_FOR_IT);
		printaMatriz(serverPackage.matriz, imagem);
		al_rest(0.0);
		al_flip_display();
	}

	return 3;
}

int menu(){
	//fontes
	ALLEGRO_FONT *font_big = al_load_ttf_font("Resources/Fonts/Tr2n.ttf",200,0);
	ALLEGRO_FONT *font_small = al_load_ttf_font("Resources/Fonts/Tr2n.ttf",60,0);
   	if (!font_big){
      fprintf(stderr, "Could not load 'Tr2n.ttf'.\n");
   	}

   /*
   al_install_keyboard();
   ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
   fila_eventos = al_create_event_queue();
   al_register_event_source(fila_eventos, al_get_keyboard_event_source());
   al_register_event_source(fila_eventos, al_get_display_event_source(display));
	*/

   int cursorPos = 0;
   bool running = true;

   while(running){
	   //printar
	   //printf("%d\n", cursorPos);
	   al_clear_to_color(al_map_rgb(50,10,70));
	   al_draw_text(font_big, al_map_rgb(255,255,255), WIDTH/2, 150,ALLEGRO_ALIGN_CENTRE, "TRONco");
	   al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, 400,ALLEGRO_ALIGN_CENTRE, "Jogar");
	   al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, 600,ALLEGRO_ALIGN_CENTRE, "Ranking");
	   al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, 800,ALLEGRO_ALIGN_CENTRE, "Sair");
	   
	   //cursor
	   al_draw_circle(550, 222 + 200*(cursorPos + 1), 20, al_map_rgb(255,255,255), 4);
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
				    printf("Cursor: %d\n", cursorPos + 1);
				    return cursorPos + 1;
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
}


int ranking() {
	
	char ServerIP[30]={"127.0.0.1"};
	connectToServer(ServerIP);
	
	recvMsgFromServer(&serverPackage, WAIT_FOR_IT);
	
	return 2;	
}