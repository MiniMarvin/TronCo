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

// Evento de captura do teclado
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;

int main(int argc, char **argv) {
		
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

	if (!inicializar(&display, &fila_eventos)) {
		return -1;
	}

	//Esse IP irá nos conectar a "nós mesmos", apenas para efeito de testes.
	char ServerIP[30]={"127.0.0.1"};
	connectToServer(ServerIP);

	al_clear_to_color(al_map_rgb(10,50,30));
	al_flip_display();

	while (!sair) {
		printf("OI\n");
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
		recvMsgFromServer(&serverPackage, DONT_WAIT);
		printaMatriz(serverPackage.matriz);
		al_rest(0.7);
		al_flip_display();
	}
	 return 0;
}

