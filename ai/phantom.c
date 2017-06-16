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
#include "../v2.0/libTronco/tronco.h"
#include "../v2.0/libTronco/allegroUsefull.h"

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
void pegaMapa(char** map, int map_size);
int move(char** map, int size, mapPos pos);
// mapPos findPlayer(char** map, int size, int player, mapPos old_pos);
void findPlayer(char** map, int map_size, int player, mapPos *old_pos);
// Exibe uma tela 
ALLEGRO_DISPLAY *display = NULL;

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
	
	int map_size = 0, i, j, jogador = 0;
	int direcao; // Direção de movimento do bot
	char** map;
	mapPos pos = {-1,-1};

	srand(time(NULL));

	
	// Recebe como argumento o tamanho do mapa.
	// if(argc >= 2) {
	//     map_size = atoi(argv[1]);
	// }
	 
	map_size = SIZE;

	//aloca memória para o mapa
	map = (char**) malloc(sizeof(char*)*map_size);
	for (i = 0; i < map_size; i++) {
	    map[i] = (char*) malloc(sizeof(char)*map_size);
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
	/*******************************************************************/

	// Comunica com o servidor, calcula o passo e envia o movimento para
	// o server.
	while(1) {
		// Obtém o mapa atual do servidor.
		pegaMapa(map, map_size);
		
		/**
		 * Comunica com o servidor e carrega o allegro.
		 */
		recvMsgFromServer(&serverPackage, DONT_WAIT);
		printaMatriz(serverPackage.matriz);

		// Encontra a posição do player 0
		// pos = findPlayer(map, map_size, jogador, pos);
		findPlayer(map, map_size, jogador, &pos);
		printf("ingame: %d | %d\n", pos.x, pos.y);

		// Controla a movimentação do personagem.
		// direcao = move(map, map_size, pos);
		do
		{
			direcao = rand()%4;
		} while (direcao == 2);
		
		clientPackage.dir = direcao; // Atualiza para o caso do valor ser inconsistente.

		for (int i = 0; i < 10000; ++i);

		sendMsgToServer(&clientPackage, sizeof(clientPackage));

		al_rest(0.01);
		al_flip_display();
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
void pegaMapa(char** map, int map_size) {
    int i, j;
    serverMsg serverPackage;

    //recebendo a mensagem do servidor
    //experimentem trocar WAIT_FOR_IT por DONT_WAIT...
    recvMsgFromServer(&serverPackage, DONT_WAIT);


    for(i = 0; i < map_size; i++) {
    	for(j = 0; j < map_size; j++) {
    		// map[i][j] = testmap[i][j];
    		map[i][j] = serverPackage.matriz[i][j];
    	}
    }
    
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
int move(char** map, int map_size, mapPos pos) {
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
				if(pos.y <= map_size) {
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
				if(pos.x <= map_size) {
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
void findPlayer(char** map, int map_size, int player, mapPos *old_pos) {
	// mapPos pos;
	int i, j;

	// Checa se o bot inicializou agora
	// if(old_pos.x == -1 || old_pos.y == -1) {
		// Encontra a posição de um determinado player
		for (i = 0; i < map_size; ++i) {
			for (j = 0; j < map_size; ++j) {
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