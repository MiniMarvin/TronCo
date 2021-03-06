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

// Ponteiro para exibição de tela
ALLEGRO_DISPLAY *display = NULL;

ALLEGRO_BITMAP *imagem = NULL;

// Evento de captura do teclado
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;

// Ponteiro para a reprodução de música
ALLEGRO_SAMPLE *sample = NULL;

int menu();
int jogo();
int o_jogo();
void configuraIP();
void configuraNome();
int telaDerrota();
int telaVitoria();
void intToString(char pontuacao[], int numero);
int ranking();
int printStartScreen();
int telaRanking(data *highscore, int len);
void espera();

char ip[17];
char nome[17];
char pontuacao[5];
int pontuacaoInteiro;

void print_val(int val) {

	if(val == GAME_WON) {
		printf("Received: GAME_WON\n");
	}
	else if(val == GAME_WAITING) {
		printf("Received: GAME_WAITING\n");
	}
	else if(val == GAME_LOST) {
		printf("Received: GAME_LOST\n");
	}
	else if(val == GAME_PLAYING) {
		printf("Received: GAME_PLAYING\n");
	}
	else {
		printf("Received: %d\n", val);	
	}
}

int main(int argc, char **argv) {
	int estado = 0;
	bool running = true;

	// connectToServer("127.0.0.1");

	if (!inicializar(&display, &fila_eventos)) {
		return -1;
	}

	printStartScreen();

	// Configura o ip do server quando inicia o software
	if(strlen(ip) == 0){
		configuraIP();
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
				// Envia essa mensagem para o server para sabermos quando um client desconectou
				//Se nao tiver se conectado vai levar broken pipe, papai
				// TODO: definir essa parte, porém, o clientPackage não existe nessa
				// função, necessário modificar o local onde isso está sendo realizado.
				// clientPackage.gameOption = WANNA_QUIT;
				// sendMsgToServer(&clientPackage, sizeof(clientPackage));
				running = false;
				break;
		}
	}
	 return 0;
}

int printStartScreen() {

	// PROJETO IP - 2017.1
	// PROFESSOR: ACM
	// MONITOR: MGM6
	// ALUNOS: ~CMG ~JKSS ~JRSF ~MAFS3 ~MBGJ ~TAB ~TASM2

	////////////////////// PARTE PARA COMEÇAR A TOCAR A MUSICA ////////////////////////////
	int start = 90;
	intToString(pontuacao, 321);
	printf("%s", pontuacao);
	sample = al_load_sample( "Resources/musiquinha.ogg" );

	if (!sample){
	  printf( "Audio clip sample not loaded!\n" );
	  return -1;
	}

	al_play_sample(sample, 1.0, 0.0,1.0,ALLEGRO_PLAYMODE_LOOP,NULL);

	///////////////// INICIALIZAÇÃO DAS FONTES //////////////////////
	int max_font = 200;
	int i = 0;
	ALLEGRO_FONT *font_big = al_load_ttf_font("Resources/Fonts/Tr2n.ttf",200,0);;
	ALLEGRO_FONT *font_small = al_load_ttf_font("Resources/Fonts/Tr2n.ttf",60,0);

	//////////////////////// ANIMAÇÃO DE ENTRADA DO PROGRAMA, COM NOME DO GRUPO, DESENVOLVEDORES E TITULO DO JOGO ////////////////////

	al_clear_to_color(al_map_rgb(0,0,0));
	al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, 100, ALLEGRO_ALIGN_CENTRE, "Projeto IP - 2017.1");
	al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, 200, ALLEGRO_ALIGN_CENTRE, "PROFESSOR: ACM");
	al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, 300, ALLEGRO_ALIGN_CENTRE, "MONITOR: MGM6");
	al_draw_text(font_small, al_map_rgb(255,255,255), 2, 400, ALLEGRO_ALIGN_LEFT, "ALUNOS: CMG JKSS JRSF MAFS3 MBGJ TAB TASM2");
	al_flip_display();
	al_rest(5);

	start = 90;

	for(int i = 0; i <= 200; i = i + 7){
		al_clear_to_color(al_map_rgb(0,0,0));
		font_big = al_load_ttf_font("Resources/Fonts/Tr2n.ttf",i,0);
		al_draw_text(font_big, al_map_rgb(255,255,255), (float) WIDTH/2, (float) start,ALLEGRO_ALIGN_CENTRE, "TRONco");
		al_flip_display();
	}
}

int menu(){

   imagem = al_load_bitmap("./Resources/Tilesets/fundo.jpg");
   int cursorPos = 0;
   bool running = true;
   int start = 110;

	 ///////////////// INICIALIZAÇÃO DAS FONTES //////////////////////
	 int max_font = 200;
	 int i = 0;
	 ALLEGRO_FONT *font_big = al_load_ttf_font("Resources/Fonts/Tr2n.ttf",200,0);;
	 ALLEGRO_FONT *font_small = al_load_ttf_font("Resources/Fonts/Tr2n.ttf",60,0);

   while(running){
	   //printar
	   //printf("%d\n", cursorPos);
	   al_clear_to_color(al_map_rgb(50,10,70));
	   al_draw_bitmap(imagem, 0, 0, 0);
	   al_draw_text(font_big, al_map_rgb(255,255,255),(float)  WIDTH/2, start,ALLEGRO_ALIGN_CENTRE, "TRONco");
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
				}
				else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
					running = false;
				}
			}
   }
   //al_rest(10.0);

   al_destroy_display(display);
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
			case 5:
				estado_jogo = telaDerrota();
				break;
		}
	}
}


int o_jogo(){

	bool sair = false;
	int tecla = 0;
	int direcao = 10, buff_direcao = 10;

	clientMsg clientPackage;
	serverMsg serverPackage;
	serverMsg temp;

	configuraNome();
	//Esse IP irá nos conectar a "nós mesmos", apenas para efeito de testes.
	// strcpy(ip, "127.0.0.1");
	// char ServerIP[30];
	// strcpy(ServerIP, ip);
	// connectToServer(ServerIP);
	puts(ip);
	puts(nome);
	al_clear_to_color(al_map_rgb(10,50,30));
	al_flip_display();

	// Zera a matriz de forma inicial
	for(int i = 0; i < SIZEX; i++) {
		for(int j = 0; j < SIZEY; j++){
			serverPackage.matriz[i][j] = '0';
		}
	}

	// Informa ao server que o player deseja jogar
	clientPackage.gameOption = WANNA_PLAY; // Atualiza para o caso do valor ser inconsistente.
	sendMsgToServer(&clientPackage, sizeof(clientPackage));

	// Desenha a tela de espera
	espera();

	do {
		// continua comunicando ao server que deseja jogar
		// clientPackage.gameOption = WANNA_PLAY; // Atualiza para o caso do valor ser inconsistente.
		// sendMsgToServer(&clientPackage, sizeof(clientPackage));

		recvMsgFromServer(&serverPackage, WAIT_FOR_IT); // espera uma resposta do server
		print_val(serverPackage.statusPlayer);
	} while(serverPackage.statusPlayer == GAME_WAITING);

	imagem = al_load_bitmap("./Resources/Tilesets/rastro_novinho_1.png");

	strcpy(clientPackage.nome, nome);

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
		clientPackage.gameOption = WANNA_PLAY; // Atualiza para o caso do valor ser inconsistente.

		// enviando a mensagem
		sendMsgToServer(&clientPackage, sizeof(clientPackage));

		// Recebendo a mensagem, experimentem trocar WAIT_FOR_IT por DONT_WAIT...
		recvMsgFromServer(&serverPackage, DONT_WAIT);
		// printaMatriz(serverPackage.matriz, imagem, serverPackage.dir);
		
		print_val(serverPackage.statusPlayer);

		if(serverPackage.statusPlayer == GAME_WON){

			//Limpa o buffer
			recvMsgFromServer(&temp, WAIT_FOR_IT);

			pontuacaoInteiro = serverPackage.pontuacao;

			clientPackage.gameOption = WANNA_NOTHING; // Atualiza para o caso do valor ser inconsistente.
			printf("\n\nEu ganhei!!!\n\n");

			sendMsgToServer(&clientPackage, sizeof(clientPackage)); // informa ao server que não quer fazer nada.

			return 4; // Vai para a tela de Vitoria
		} 
		else if(serverPackage.statusPlayer == GAME_LOST){
			
			//Limpa o buffer
			recvMsgFromServer(&temp, WAIT_FOR_IT);

			pontuacaoInteiro = serverPackage.pontuacao;

			printf("\n\nReceived: %d\n\n", serverPackage.statusPlayer);

			clientPackage.gameOption = WANNA_NOTHING; // Atualiza para o caso do valor ser inconsistente.
			sendMsgToServer(&clientPackage, sizeof(clientPackage)); // informa ao server que não quer fazer nada.
			return 5; // Vai para a tela de Derrota
		}

		printaMatriz(serverPackage.matriz, imagem, serverPackage.dir);
		al_rest(0.1);
		al_flip_display();
	}

	return 0; // SE CLICOU NO X DA JANELA
}

// FUNCOA PARA TRANSFORMAR INTEIRO EM UMA STRING, PRINCIPALMENTE UTILIZADA PARA PRINTAR A PONTUACAO DOS PLAYERS
void intToString(char pontuacao[], int numero){
	sprintf(pontuacao, "%d", numero);
}

int telaDerrota() {

	bool concluido = false;
	char pontuacao[10] = "";

	ALLEGRO_FONT *font_big = al_load_ttf_font("Resources/Fonts/Tr2n.ttf",200,0);
	ALLEGRO_FONT *font_small = al_load_ttf_font("Resources/Fonts/Tr2n.ttf",60,0);

	imagem = al_load_bitmap("./Resources/Tilesets/fundo.jpg");
   	al_draw_bitmap(imagem, 0, 0, 0);
	al_draw_text(font_big, al_map_rgb(255,255,255), WIDTH/2, 150,ALLEGRO_ALIGN_CENTRE, "TRONco");
	al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, 350,ALLEGRO_ALIGN_CENTRE, "Voce perdeu a partida,");
	al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, 450,ALLEGRO_ALIGN_CENTRE, nome);
	al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2 - 100, 550,ALLEGRO_ALIGN_CENTRE, "E sua pontuacao foi: ");
	// al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2 + 350, 550,ALLEGRO_ALIGN_CENTRE, "310");
	intToString(pontuacao, pontuacaoInteiro);
	al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2 + 350, 550,ALLEGRO_ALIGN_CENTRE, pontuacao);
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
	return 3; //
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

void espera() {
	ALLEGRO_FONT *font_big = al_load_ttf_font("Resources/Fonts/Tr2n.ttf",200,0);
	ALLEGRO_FONT *font_small = al_load_ttf_font("Resources/Fonts/Tr2n.ttf",60,0);
	
	if (!font_big){
		fprintf(stderr, "Could not load 'Tr2n.ttf'.\n");
   	} else{
		fprintf(stderr, "OI BEBER\n");
   	}

   	
	imagem = al_load_bitmap("./Resources/Tilesets/fundo.jpg");
   	al_draw_bitmap(imagem, 0, 0, 0);
	al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, 150,ALLEGRO_ALIGN_CENTRE, "Esperando Outros Players");
	al_flip_display();
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
	while(concluido == false) {
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

	connectToServer(ip);
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
	al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, 550,ALLEGRO_ALIGN_CENTRE, nome);
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
					al_draw_bitmap(imagem, 0, 0, 0);
					al_draw_text(font_big, al_map_rgb(255,255,255), WIDTH/2, 150,ALLEGRO_ALIGN_CENTRE, "TRONco");
					al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, 350,ALLEGRO_ALIGN_CENTRE, "Digite o seu nome:");
					al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, 550,ALLEGRO_ALIGN_CENTRE, nome);
					al_flip_display();
		        }
		    }
		}
	}
}

int telaRanking(data *highscore, int len) {

	// bool concluido = false;
	int concluido = 0;

	clientMsg clientPackage;
	serverMsg serverPackage;
	char txt[100] = "";

	ALLEGRO_FONT *font_big = al_load_ttf_font("Resources/Fonts/Tr2n.ttf",200,0);
	ALLEGRO_FONT *font_small = al_load_ttf_font("Resources/Fonts/Tr2n.ttf",60,0);

	imagem = al_load_bitmap("./Resources/Tilesets/fundo.jpg");
   	al_draw_bitmap(imagem, 0, 0, 0);
	al_draw_text(font_big, al_map_rgb(255,255,255), WIDTH/2, 100,ALLEGRO_ALIGN_CENTRE, "Ranking-TronCo");
	
	if(len != 0) {
		for(int i = 0; i < len; i++){
			intToString(pontuacao, highscore[i].score);

			if(strcmp(highscore[i].nome, "") != 0) {
				strcpy(txt, highscore[i].nome);
				strcat(txt, "\t");
				strcat(txt, pontuacao);
			}
			// al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2, 300 + i * 100,ALLEGRO_ALIGN_CENTRE, highscore[i].nome);
			// al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2 + 100, 300 + i * 100,ALLEGRO_ALIGN_CENTRE, pontuacao);
			al_draw_text(font_small, al_map_rgb(255,255,255), WIDTH/2 + 100, 300 + i * 100,ALLEGRO_ALIGN_CENTRE, txt);
		}
	} 
	al_flip_display();

	// ENQUANTO NÃO FOR PRESSIONADO O ENTER O JOGADOR CONTINUA FORA DO GAME
	while(concluido < 2){
		while (!al_is_event_queue_empty(fila_eventos)){
		        ALLEGRO_EVENT evento;
		        al_wait_for_event(fila_eventos, &evento);
		        if (evento.type == ALLEGRO_EVENT_KEY_CHAR){
		    		if(evento.keyboard.keycode == 'C'){// O caracter 'C' significa que o Enter foi Apertado
		            	concluido += 1;
			        }
		    }
		}
	}
	return 0;
}


int ranking() {

	printf("Ranking OK!\n");

	// Connection statusConnection;
	clientMsg package;
	serverHighscore recvHighscore;

	package.gameOption = WANT_HIGHSCORE;
	// statusConnection.response = 2;
	
	sendMsgToServer(&package, sizeof(clientMsg));

	printf("Sending Request\n");

	// recvMsgFromServer(&clearBuffer, WAIT_FOR_IT);

	recvMsgFromServer(&recvHighscore, WAIT_FOR_IT);
	
	
	// 	// if(msgFromServer != NO_MESSAGE) {
	printf("Highscore received\n");

	printData(recvHighscore.score, 5);

	// printf("highscore name: %s\n", recvHighscore.score.nome);

	telaRanking(recvHighscore.score, recvHighscore.fileLength);
	return 0;
}
