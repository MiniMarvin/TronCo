#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>

#include "libSocket/client.h"
#include "libAllegro/AllegroCore.h"

#define AZUL al_map_rgb(22, 55, 132)
#define VERMELHO al_map_rgb(250, 120, 255)
#define JOGADOR1 al_map_rgb(168, 11, 77)
#define JOGADOR2 al_map_rgb(107, 180, 12) // VERDE

#define SIZE 50

typedef struct{
  int dir;
}clientMsg;

typedef struct{
  char matriz[SIZE][SIZE];
  // int status; Inteiro para dizer se o cliente esta vivo no jogo
}serverMsg;

void printaMatriz(char matriz[SIZE][SIZE]);
bool inicializar();

// Exibe uma tela 
ALLEGRO_DISPLAY *display = NULL;

// Evento de captura do teclado
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;

int main(int argc, char **argv){
    
    bool sair = false;
    int tecla = 0;
    int posX = 20, posY = 20;
    int direcao = 1;
    int i, j; 
    int sizeQuadrado = 8;// matrizQuadrado[600][480];
    int xInicial = 2, yInicial = 2;
    int velocidadeX = 10, velocidadeY = 10;
    int* jogadores = (int *) malloc(4 * sizeof(int));

    if (!inicializar()) {
        return -1;
    }
    clientMsg clientPackage;
    serverMsg serverPackage;
    //Esse IP irá nos conectar a "nós mesmos", apenas para efeito de testes.
    char ServerIP[30]={"127.0.0.1"};
    connectToServer(ServerIP);

  al_clear_to_color(al_map_rgb(10,50,30));
  al_flip_display();

   while (!sair){
      printf("OI\n");
      while(!al_is_event_queue_empty(fila_eventos)){
       printf("OI");
          ALLEGRO_EVENT evento;
          al_wait_for_event(fila_eventos, &evento);

          if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
              switch(evento.keyboard.keycode){
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
          else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
              sair = true;
          }
      }
      if(tecla){
          switch (tecla){
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

      clientPackage.dir = direcao;
      //enviando a mensagem
      sendMsgToServer(&clientPackage, sizeof(clientPackage));
      //recebendo a mensagem
      //experimentem trocar WAIT_FOR_IT por DONT_WAIT...
      recvMsgFromServer(&serverPackage, DONT_WAIT);
      printaMatriz(serverPackage.matriz);
      al_rest(0.01);
      al_flip_display();
  }
   return 0;
  }

bool inicializar(){

   if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
   }

    if (!al_init_primitives_addon()) {
        fprintf(stderr, "Falha ao inicializar add-on de primitivas.\n");
        return false;
    }

    display = al_create_display(550, 550);
    if(!display) {
        fprintf(stderr, "failed to create display!\n");
      return -1;
   }

   fila_eventos = al_create_event_queue();
   if(!fila_eventos){
      fprintf(stderr, "Falha ao criar fila de eventos.\n");
      al_destroy_display(display);
      return -1;
   }

   if (!al_install_keyboard())
    {
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
        return false;
    }
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(display));
 
    return true;
}

void printaMatriz(char matriz[][SIZE]){
   int i, j;
   int sizeQuadrado = 8;// matrizQuadrado[600][480];
   int xInicial = 2, yInicial = 2;
   for(i = 0; i < SIZE; i++){
      for(j = 0; j < SIZE; j++){
         xInicial = xInicial + sizeQuadrado + 2;// sizeQuadrado;
         switch(matriz[i][j]){
            case '0':
            //x1, y1, x2, y2
               al_draw_filled_rectangle(xInicial, yInicial,
                                    (xInicial + sizeQuadrado), (yInicial + sizeQuadrado),
                                    AZUL);
               break;
            case 'a':
               al_draw_filled_rectangle(xInicial, yInicial,
                                    (xInicial + sizeQuadrado), (yInicial + sizeQuadrado),
                                    JOGADOR1);
               break;
            case 'A':
               break;
            case 'b':
              al_draw_filled_rectangle(xInicial, yInicial,
                                    (xInicial + sizeQuadrado), (yInicial + sizeQuadrado),
                                    JOGADOR2);
               break;
            case 'B':
               break;
            case 'c':
               break;
            case 'C':
               break;
            case 'd':
               break;
            case 'D':
               break;
            default:
               break;
         }
      }
      xInicial = 2;
      yInicial = yInicial + sizeQuadrado + 2;
   }
}

