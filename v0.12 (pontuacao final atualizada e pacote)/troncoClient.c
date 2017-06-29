#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>
#include <string.h>

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

//musiquinha
  ALLEGRO_SAMPLE *sample=NULL;

int menu();
int jogo();
int o_jogo();
void configuraIP();
void configuraNome();
int telaDerrota();
int telaVitoria();
void intToString(char pontuacao[], int numero);

char ip[17];
char nome[17];
char pontuacao[5];
int pontuacaoInteiro;

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
				//estado = ranking();
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

			case 4:
				estado_jogo = telaVitoria();
				break;
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

	//nome = configuraNome();
	configuraIP();
	configuraNome();
	//Esse IP irá nos conectar a "nós mesmos", apenas para efeito de testes.
	// strcpy(ip, "127.0.0.1");
	char ServerIP[30];
	strcpy(ServerIP, ip);
	connectToServer(ServerIP);
	puts(ip);
	puts(nome);
	al_clear_to_color(al_map_rgb(10,50,30));
	al_flip_display();

	imagem = al_load_bitmap("./Resources/Tilesets/rastro_novinho_1.png");

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

		//recebendo a mensagem
		//experimentem trocar WAIT_FOR_IT por DONT_WAIT...
		recvMsgFromServer(&serverPackage, DONT_WAIT);
		// printaMatriz(serverPackage.matriz, imagem, serverPackage.dir);
		if(serverPackage.statusPlayer == 0){
			pontuacaoInteiro = serverPackage.pontuacao;
			return 4;
		}

		//enviando a mensagem
		sendMsgToServer(&clientPackage, sizeof(clientPackage));

		printaMatriz(serverPackage.matriz, imagem, serverPackage.dir);
		al_rest(0.1);
		al_flip_display();
	}
	return 4;
}

int menu(){

	////////////////////// PARTE PARA COMEÇAR A TOCAR A MUSICA ////////////////////////////
	intToString(pontuacao, 321);
	printf("%s", pontuacao);
	sample = al_load_sample( "Resources/musiquinha.ogg" );
	int start = 90;
		if (!sample){
	  printf( "Audio clip sample not loaded!\n" ); 
	  return -1;
	}

	al_play_sample(sample, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);

	///////////////// INICIALIZAÇÃO DAS FONTES //////////////////////
	int max_font = 200;
	int i = 0;	
	ALLEGRO_FONT *font_big;
	ALLEGRO_FONT *font_small = al_load_ttf_font("Resources/Fonts/Tr2n.ttf",60,0);

	//////////////////////// ANIMAÇÃO DE ENTRADA DO PROGRAMA, COM NOME DO GRUPO, DESENVOLVEDORES E TITULO DO JOGO ////////////////////
	al_clear_to_color(al_map_rgb(0,0,0));
	al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, HEIGTH/2 - 30,ALLEGRO_ALIGN_CENTRE, "Grupo 3 Apresenta:");
	al_flip_display();	
	al_rest(3.5);
	for(0; i <= 200; i = i + 7){
		al_clear_to_color(al_map_rgb(0,0,0));		
		font_big = al_load_ttf_font("Resources/Fonts/Tr2n.ttf",i,0);
		al_draw_text(font_big, al_map_rgb(255,255,255), WIDTH/2, start,ALLEGRO_ALIGN_CENTRE, "TRONco");
		al_flip_display();	
	}
	font_big = al_load_ttf_font("Resources/Fonts/Tr2n.ttf",200,0);

   	if (!font_big){
      		fprintf(stderr, "Could not load 'Tr2n.ttf'.\n");
   	}

   
   imagem = al_load_bitmap("./Resources/Tilesets/fundo.jpg");
   int cursorPos = 0;
   bool running = true;
   while(running){
	   //printar
	   //printf("%d\n", cursorPos);
	   al_clear_to_color(al_map_rgb(50,10,70));
	   al_draw_bitmap(imagem, 0, 0, 0);
	   al_draw_text(font_big, al_map_rgb(255,255,255), WIDTH/2, start,ALLEGRO_ALIGN_CENTRE, "TRONco");
	   al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, start + 200,ALLEGRO_ALIGN_CENTRE, "Jogar");
	   al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, start + 400,ALLEGRO_ALIGN_CENTRE, "Ranking");
	   al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, start + 600,ALLEGRO_ALIGN_CENTRE, "Sair");
	   //cursor
	   al_draw_circle(550, start + 25  + 200*(cursorPos + 1), 20, al_map_rgb(255,255,255), 6);
	   al_flip_display();

	
	   //pegar input
	   int tecla = 0;

	   // MÁQUINA DE ESTADOS PARA MOVIMENTAÇÃO DA BOLINHA QUE SIMBOLIZA O CURSOR
	   // RETORNA O VALOR DO CURSO QUANDO PRESSIONADO O ENTER
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
		 }else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
    	    running = false;
		 }
	  }
   }
   //al_rest(10.0);

   al_destroy_display(display);
}

// FUNCOA PARA TRANSFORMAR INTEIRO EM UMA STRING, PRINCIPALMENTE UTILIZADA PARA PRINTAR A PONTUACAO DOS PLAYERS
void intToString(char pontuacao[], int numero){
	sprintf(pontuacao, "%d", numero);
}

int telaDerrota(){

	bool concluido = false;

	ALLEGRO_FONT *font_big = al_load_ttf_font("Resources/Fonts/Tr2n.ttf",200,0);
	ALLEGRO_FONT *font_small = al_load_ttf_font("Resources/Fonts/Tr2n.ttf",60,0);

	imagem = al_load_bitmap("./Resources/Tilesets/fundo.jpg");
   	al_draw_bitmap(imagem, 0, 0, 0);
	al_draw_text(font_big, al_map_rgb(255,255,255), WIDTH/2, 150,ALLEGRO_ALIGN_CENTRE, "TRONco");
	al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, 350,ALLEGRO_ALIGN_CENTRE, "Voce perdeu a partida,");
	al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, 450,ALLEGRO_ALIGN_CENTRE, nome);
	al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2 - 100, 550,ALLEGRO_ALIGN_CENTRE, "E sua pontuacao foi: ");
	al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2 + 350, 550,ALLEGRO_ALIGN_CENTRE, "310");
	al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, 750,ALLEGRO_ALIGN_CENTRE, "PRESSIONE ENTER PARA SAIR");
	al_flip_display();

	// ENQUANTO NÃO FOR PRESSIONADO O ENTER O JOGADOR CONTINUA FORA DO GAME
	while(concluido == false){
		while (!al_is_event_queue_empty(fila_eventos)){
		        ALLEGRO_EVENT evento;
		        al_wait_for_event(fila_eventos, &evento);
		        if (evento.type == ALLEGRO_EVENT_KEY_CHAR){
		    		if(evento.keyboard.keycode == 'C'){// O caracter 'C' significa que o Enter foi Apertado
		            	concluido = true;
			        }
		    }
		}
	}	
	return 3;
}

int telaVitoria(){

	bool concluido = false;

	intToString(pontuacao, pontuacaoInteiro);

	ALLEGRO_FONT *font_big = al_load_ttf_font("Resources/Fonts/Tr2n.ttf",200,0);
	ALLEGRO_FONT *font_small = al_load_ttf_font("Resources/Fonts/Tr2n.ttf",60,0);

	imagem = al_load_bitmap("./Resources/Tilesets/fundo.jpg");
   	al_draw_bitmap(imagem, 0, 0, 0);
	al_draw_text(font_big, al_map_rgb(255,255,255), WIDTH/2, 150,ALLEGRO_ALIGN_CENTRE, "TRONco");
	al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, 350,ALLEGRO_ALIGN_CENTRE, "Voce venceu a partida, Parabens!");
	al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, 450,ALLEGRO_ALIGN_CENTRE, nome);
	al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2 - 100, 550,ALLEGRO_ALIGN_CENTRE, "E sua pontuacao foi: ");
	al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2 + 350, 550,ALLEGRO_ALIGN_CENTRE, pontuacao);
	al_draw_text(font_small, al_map_rgb(255,0,255), WIDTH/2, 750,ALLEGRO_ALIGN_CENTRE, "PRESSIONE ENTER PARA SAIR");
	al_flip_display();

	// ENQUANTO NÃO FOR PRESSIONADO O ENTER O JOGADOR CONTINUA FORA DO GAME
	while(concluido == false){
		while (!al_is_event_queue_empty(fila_eventos)){
		        ALLEGRO_EVENT evento;
		        al_wait_for_event(fila_eventos, &evento);
		        if (evento.type == ALLEGRO_EVENT_KEY_CHAR){
		    		if(evento.keyboard.keycode == 'C'){// O caracter 'C' significa que o Enter foi Apertado
		            	concluido = true;
			        }
		    }
		}
	}	
	return 3;
}

void configuraIP(){

	ALLEGRO_FONT *font_big = al_load_ttf_font("Resources/Fonts/Tr2n.ttf",200,0);
	ALLEGRO_FONT *font_small = al_load_ttf_font("Resources/Fonts/Tr2n.ttf",60,0);
	if (!font_big){
      fprintf(stderr, "Could not load 'Tr2n.ttf'.\n");
   	}else{
   		fprintf(stderr, "OI BEBER\n");
   	}
   	bool concluido = false;
	imagem = al_load_bitmap("./Resources/Tilesets/fundo.jpg");
   	al_draw_bitmap(imagem, 0, 0, 0);
	al_draw_text(font_big, al_map_rgb(255,255,255), WIDTH/2, 150,ALLEGRO_ALIGN_CENTRE, "TRONco");
	al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, 350,ALLEGRO_ALIGN_CENTRE, "Digite o IP para conectar:");
	al_flip_display(); 

	ALLEGRO_EVENT evento;
	al_wait_for_event(fila_eventos, &evento);
	while(concluido == false){
		while (!al_is_event_queue_empty(fila_eventos)){
		        ALLEGRO_EVENT evento;
		        al_wait_for_event(fila_eventos, &evento);
		        if (evento.type == ALLEGRO_EVENT_KEY_CHAR){
		    		if(strlen(ip) <= 16){
		            	char temp[] = {evento.keyboard.unichar, '\0'};
		            	printf("\n%c\n", evento.keyboard.unichar);
			            if(evento.keyboard.unichar == '.'){
			                strcat(ip, temp);
			            }else if (evento.keyboard.unichar >= '0' && evento.keyboard.unichar <= '9'){
			                strcat(ip, temp);
			            }else if(evento.keyboard.keycode == 'C' && strlen(ip) != 0){// O caracter 'C' significa que o Enter foi Apertado
			            	//strcat(ip, "Legal");
			            	concluido = true;
			            }
			            printf("\n%s\n", ip);
			            al_draw_bitmap(imagem, 0, 0, 0);
						al_draw_text(font_big, al_map_rgb(255,255,255), WIDTH/2, 150,ALLEGRO_ALIGN_CENTRE, "TRONco");
						al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, 350,ALLEGRO_ALIGN_CENTRE, "Digite o IP para conectar:");
						al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, 550,ALLEGRO_ALIGN_CENTRE, ip);
						al_flip_display();
		    		}else{
		    			concluido = true;
		    		}
		    	// APAGA ALGUM VALOR JA DIGITADO ANTERIORMENTE NO VALOR DO IP
		        if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(ip) != 0){
		            ip[strlen(ip) - 1] = '\0';
		        }
		    }
		}	
	}
}

void configuraNome(){

	ALLEGRO_FONT *font_big = al_load_ttf_font("Resources/Fonts/Tr2n.ttf",200,0);
	ALLEGRO_FONT *font_small = al_load_ttf_font("Resources/Fonts/Tr2n.ttf",60,0);
	if (!font_big){
      fprintf(stderr, "Could not load 'Tr2n.ttf'.\n");
   	}else{
   		fprintf(stderr, "OI BEBER\n");
   	}
   	bool concluido = false;

	imagem = al_load_bitmap("./Resources/Tilesets/fundo.jpg");
   	al_draw_bitmap(imagem, 0, 0, 0);
	al_draw_text(font_big, al_map_rgb(255,255,255), WIDTH/2, 150,ALLEGRO_ALIGN_CENTRE, "TRONco");
	al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, 350,ALLEGRO_ALIGN_CENTRE, "Digite o seu nome:");
	al_flip_display(); 

	ALLEGRO_EVENT evento;
	al_wait_for_event(fila_eventos, &evento);
	while(concluido == false){
		while (!al_is_event_queue_empty(fila_eventos)){
		        ALLEGRO_EVENT evento;
		        al_wait_for_event(fila_eventos, &evento);
		        if (evento.type == ALLEGRO_EVENT_KEY_CHAR){
		    		if (strlen(nome) <= 16){
		            	char temp[] = {evento.keyboard.unichar, '\0'};
		            	printf("\n%c\n", evento.keyboard.unichar);
			            if((evento.keyboard.unichar >= 'a' && evento.keyboard.unichar <= 'z') ||
			            	(evento.keyboard.unichar >= 'A' && evento.keyboard.unichar <= 'Z')){
			                strcat(nome, temp);
			            }else if(evento.keyboard.keycode == 'C' && strlen(nome) != 0){// O caracter 'C' significa que o Enter foi Apertado
			            	concluido = true;
			            }
			            printf("\n%s\n", nome);
			            al_draw_bitmap(imagem, 0, 0, 0);
						al_draw_text(font_big, al_map_rgb(255,255,255), WIDTH/2, 150,ALLEGRO_ALIGN_CENTRE, "TRONco");
						al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, 350,ALLEGRO_ALIGN_CENTRE, "Digite o seu nome:");
						al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, 550,ALLEGRO_ALIGN_CENTRE, nome);
						al_flip_display();
		    		}else{
		    			concluido = true;
		    		}
		        if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(nome) != 0){
		            nome[strlen(nome) - 1] = '\0';
		        }
		    }
		}	
	}
}
