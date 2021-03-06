/**
 * Bot para jogar TronCo, será utilizado um algorítmo de teoria dos jogos
 * denominado de MiniMax. Esse algorítmo é capaz de gerar uma árvore de
 * possibilidades que ficam presentes após os próximos possíveis movimentos
 * e em seguida avaliar quais deles tem maior chance de ganhar o jogo.
 * Referências: 
 * - http://www.sifflez.org/misc/tronbot/index.html
 * - http://neverstopbuilding.com/minimax
 * 
 * Uma vez que o Tronco tem o tempo de atualização de cerca de 10ms, o
 * algorítmo precisa rodar nesse intervalo de tempo, que é 0.01s, então
 * é necessário fazer otimização, mesmo que por cache. Uma forma de
 * fazer isso é:
 * - Constrói a árvore e processa.
 * - Recebe o mapa do servidor.
 * - Compara o mapa com as folhas (Compara só os quadrados próximos a 
 * posição de movimentação de cada personagem, porque assim economiza
 * uma boa quantidade de ciclos de máquina e corta todo o reprocesso 
 * da construção da árvore e das evaluações de posição). Número de 
 * comparações será: 3*numero_de_players.
 * - Move o ramo correto para a base da árvore e desaloca os demais.
 * - Processa novamente a partir desse mapa.
 * 
 * Para montar o mapa, será feita uma busca pela posição de cada um
 * dos Players, fazendo com que elas sejam registradas num vetor para
 * que seja então reutilizada posteriormente para evitar busca.
 * 
 * Idealmente deve haver uma thread de comunicação e uma thread de
 * processamento, pois, a thread de comunicação aguarda os dados do
 * servidor e a de processamento continua gerando árvores continuamente
 * 
 * Ideia: Phantom Bot necessita ser um único arquivo com argumento
 * sendo o número de bots (n), pois, assim é possível utilizar n+1 
 * threads de processamento otimizando o tempo de processamento, pois,
 * como a árvore é imutável para o jogo, então o Phantom Client iria
 * gerar a árvore de busca e iria podá-la de acordo com a resposta
 * atual do server, por meio de uma 
 * 
 * IDEIA: por o phantom bot recebendo o IP do server, então cada 
 * client poderia adicionar seu próprio phantom bot.
 * 
 * OBS: necessário modificar o server caso seja implementada a abordagem
 * de multithreading para cada bot, pois, cada client só consegue controlar
 * um único bot.
 * 
 * Uma vez que tenha sido encontrado um ramo que ganha instantaneamente
 * ele para de processar aquele ramo e aquele player e começa a processar
 * os demais.
 * 
 **/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../lib/client.h"
#include "../v0.07(Direcao Atualizada)/libTronco/tronco.h"
#include "../v0.07(Direcao Atualizada)/libTronco/allegroUsefull.h"

// #define SIZE 50

typedef struct{
	int x;
	int y;
} mapPos;

// Mapa utilizado para testar os algorítmos de decisão de caminho.
const char testmap[][5] = {	{'a','0','0','B','0'},
							{'a','0','0','b','b'},
							{'a','A','0','0','0'},
							{'0','0','0','0','0'},
							{'0','0','0','0','0'}};

// Protótipos de funções
void pegaMapa(char** map, int map_size_x, int map_size_y);
int move(char** map, int map_size_x, int map_size_y, mapPos pos);
// mapPos findPlayer(char** map, int size, int player, mapPos old_pos);
void findPlayer(char** map, int map_size_x, int map_size_y, int player, mapPos *old_pos);
char getVal(char** map, int map_size_x, int map_size_y, mapPos pos, int val);
int random_move(char** map, int map_size_x, int map_size_y, mapPos pos);


// Exibe uma tela 
ALLEGRO_DISPLAY *display = NULL;

// Tilesheet
ALLEGRO_BITMAP *imagem = NULL;

// Evento de captura do teclado
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;

/**
 * @brief      Recebe e processa as funções referentes ao bot do jogo TronCo.
 *
 * @param[in]  argc  The argc
 * @param      argv  The argv - o primeiro argumento é o tamanho do mapa.
 *
 * @return     retorna 0 informando ao SO que o programa foi finalizado.
 */
int main(int argc, char const *argv[]) {
	
	int map_size_x = 0, map_size_y = 0, i, j, jogador = 0;
	int direcao; // Direção de movimento do bot
	char** map;
	int server_dir[4] = {0};
	mapPos pos = {-1,-1};

	srand(time(NULL));

	
	// Recebe como argumento o tamanho do mapa.
	// if(argc >= 2) {
	//     map_size = atoi(argv[1]);
	// }
	 
	map_size_x = SIZEX;
	map_size_y = SIZEY;

	//aloca memória para o mapa
	map = (char**) malloc(sizeof(char*)*map_size_x);
	for (i = 0; i < map_size_x; i++) {
	    map[i] = (char*) malloc(sizeof(char)*map_size_y);
	}

	/*********************Conexão com o servidor************************/
	clientMsg clientPackage;
	serverMsg serverPackage;

	//Esse IP irá nos conectar a "nós mesmos", apenas para efeito de testes.
	char ServerIP[30]={"127.0.0.1"};
	connectToServer(ServerIP);
	/*******************************************************************/
	
	/**************************Allegro**********************************/
	if (!inicializar(&display, &fila_eventos)) {
		return -1;
	}

	al_clear_to_color(al_map_rgb(10,50,30));
	al_flip_display();

	imagem = al_load_bitmap("./Resources/Tilesets/rastro_novo.png");
	/*******************************************************************/

	// Comunica com o servidor, calcula o passo e envia o movimento para
	// o server.
	while(1) {
		// Obtém o mapa atual do servidor.
		pegaMapa(map, map_size_x, map_size_y);
		
		/**
		 * Comunica com o servidor e carrega o allegro.
		 */
		recvMsgFromServer(&serverPackage, DONT_WAIT);

		// Atualiza qual player é o que está sendo usado
		jogador = serverPackage.client_id;
		printf("jogador: %d | pack: %d\n", jogador, serverPackage.client_id);

		printaMatriz(serverPackage.matriz, imagem, serverPackage.dir);

		

		// Encontra a posição do player 0
		// pos = findPlayer(map, map_size, jogador, pos);
		findPlayer(map, map_size_x, map_size_y, jogador, &pos);
		printf("ingame: %d | %d\n", pos.x, pos.y);

		// Controla a movimentação do personagem.
		direcao = move(map, map_size_x, map_size_y, pos);
		do
		{
			direcao = rand()%4;
		} while (direcao == 2);
		
		// direcao = random_move(map, map_size_x, map_size_y, pos);

		clientPackage.dir = direcao; // Atualiza para o caso do valor ser inconsistente.

		// for (int i = 0; i < 10000; ++i);

		sendMsgToServer(&clientPackage, sizeof(clientPackage));


		printaMatriz(serverPackage.matriz, imagem, serverPackage.dir);
		al_flip_display();
		al_rest(1);
		// al_rest(0.01);
		// al_flip_display();
		// delay(100);
	}
	
	
	return 0;
}


/**
 * @brief       Obtém o mapa do servidor.
 * 
 * @param[In]   map         O endereço onde o mapa deverá ser armazenado
 * @param[In]   map_size    O tamanho do mapa.
 */
void pegaMapa(char** map, int map_size_x, int map_size_y) {
    int i, j;
    serverMsg serverPackage;

    //recebendo a mensagem do servidor
    //experimentem trocar WAIT_FOR_IT por DONT_WAIT...
    recvMsgFromServer(&serverPackage, DONT_WAIT);


    for(i = 0; i < map_size_x; i++) {
    	for(j = 0; j < map_size_y; j++) {
    		// map[i][j] = testmap[i][j];
    		map[i][j] = serverPackage.matriz[i][j];
    	}
    }
    
}

/**
 * @brief      Pega o valor em char da posição da matriz em relação a uma posição
 *
 * @param      map       The map
 * @param[in]  map_size  The map size
 * @param[in]  pos       The position
 * @param[in]  val       The value
 *
 * @return     The value.
 */
char getVal(char** map, int map_size_x, int map_size_y, mapPos pos, int val) {
	char character = 0;
	switch(val) {
		case 0:
			if(pos.x > 0)
				character = map[pos.x - 1][pos.y];
			break;
		case 1:
			if(pos.x <= map_size_x)
				character = map[pos.x + 1][pos.y];
			break;
		case 2:
			if(pos.y > 0)
				character = map[pos.x][pos.y - 1];
			break;
		case 3:
			if(pos.y <= map_size_y)
				character = map[pos.x][pos.y + 1];
			break;
	}

	return character;
}

/**
 * @brief      Movimenta o bot aleatoriamente ao longo do mapa
 *
 * @param      map       The map
 * @param[in]  map_size  The map size
 * @param[in]  pos       The position
 *
 * @return     Qual direção o bot deveria seguir.
 */
int random_move(char** map, int map_size_x, int map_size_y, mapPos pos) {
	srand(time(NULL));
	int val = 0;
	int done[4] = {0};
	int sum = 0;

	do {
		val = rand()%4; // só se movimenta para 4 lados
		sum = 0;
		done[val] = 1;
		for (int i = 0; i < 4; ++i) { // Sai do loop caso nenhuma das direções esteja livre para evitar consumo de processo na máquiuna
			sum += done[val];
		}
	}
	while(getVal(map, map_size_x, map_size_y, pos, val) != '0' && sum < 4);

	return val;
}

/**
 * @brief      Realiza uma busca com base no minimax para definir qual a melhor
 *             forma de se mover
 *             
 * @long	   
 *
 * @param      map         The map
 * @param[in]  map_size    The map size
 * @param      pos         The position of all players in the map
 * @param[in]  player_num  The number of players
 * @param[in]  bot_player  The bot player num
 *
 * @return     O lado a mover
 */
int minimax_move(char** map, int map_size_x, int map_size_y, mapPos* pos, int player_num, int bot_player) {
	int move = 0;

	// Itera em todos os 4 movimentos possíveis.
	for (int i = 0; i < 4; ++i) {
		// Move
		// Gera um score para isso.
		// Invoca a execução para o próximo player a mover
		// Execução recursiva
		
	}	

	return move;
}

/**
 * @brief      Analisa os possíveis movimentos do bot no mapa do jogo e determina o melhor.
 *
 * @param      map   O mapa
 * @param[in]  size  O tamanho do mapa
 * @param[in]  pos   A posição onde o jogador foi encontrado pela última vez.
 *
 * @return     { description_of_the_return_value }
 */
int move(char** map, int map_size_x, int map_size_y, mapPos pos) {
	int move = 0, stay = 1;

	while(stay) {
		move = rand()%4;
		
		switch(move) {
			case 0:
				if(pos.y > 0) {
					if(map[pos.y - 1][pos.x] == 0) {
						stay = 0;
					}	
				}
				break;
			case 1:
				if(pos.y <= map_size_y) {
					if(map[pos.y + 1][pos.x] == 0) {
						stay = 0;
					}	
				}
				break;
			case 2:
				if(pos.x > 0) {
					if(map[pos.y][pos.x - 1] == 0) {
						stay = 0;
					}	
				}
				break;
			case 3:
				if(pos.x <= map_size_x) {
					if(map[pos.y][pos.x + 1] == 0) {
						stay = 0;
					}	
				}
				break;
		}
	}
	

	return move;
}

// mapPos findPlayer(char** map, int map_size, int player, mapPos old_pos) {
void findPlayer(char** map, int map_size_x, int map_size_y, int player, mapPos *old_pos) {
	// mapPos pos;
	int i, j;

	// Checa se o bot inicializou agora
	// if(old_pos.x == -1 || old_pos.y == -1) {
		// Encontra a posição de um determinado player
		for (i = 0; i < map_size_x; ++i) {
			for (j = 0; j < map_size_y; ++j) {
				if(map[i][j] == 'A'+player) {
					printf("pos: %d | %d\n", i, j);
					old_pos->x = i;
					old_pos->y = j;
					// return;
				}
			}
		}
	// }

	// else{

	// 	if(map[old_pos.y][old_pos.x + 1] == 'A' + player) {
	// 		pos.x = old_pos.x+1;
	// 		pos.y = old_pos.y; 
	// 	}
	// 	else if(map[old_pos.y][old_pos.x -1] == 'A' + player) {
	// 		pos.x = old_pos.x-1;
	// 		pos.y = old_pos.y; 
	// 	}
	// 	else if(map[old_pos.y+1][old_pos.x] == 'A' + player) {
	// 		pos.x = old_pos.x;
	// 		pos.y = old_pos.y+1; 
	// 	}
	// 	else {
	// 		pos.x = old_pos.x;
	// 		pos.y = old_pos.y-1; 
	// 	}

	// }

	// return pos;
}