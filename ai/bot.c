/**
 * Bot para jogar TronCo, será executado com um algorítmo de busca pelo maior
 * caminho para um ponto que fica próximo ao bot, fazendo uma estrutura do tipo
 * 
 * [1][2][3]
 * [4][p][5]
 * [6][7][8]
 * 
 * Escolhendo um desses blocos para ser o bloco de destino uma vez, até que
 * não haja mais um caminho possível para aquele lugar, uma vez que a cada
 * movimento o mapa muda deve ser recomputado o caminho até aquele local, visto
 * que não há caminho deve reestabelecer o destino e prosseguir. Uma vez que
 * o personagem tenha morrido deve parar de computar para evitar de ficar
 * processando sem necessidade na máquina do servidor.
 * 
 * O algorítmo será:
 * - Pega do servidor o mapa atual
 * - Verifica se há um caminho disponível
 * - Se sim busca o maior caminho em um determinado tempo de execução, tendo
 * passado o tempo a busca é cancelada e o maior até aquele momento é utilizado.
 * - Move o personagem de forma a seguir o caminho.
 * - Re
 * 
 **/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../lib/client.h"

// Mapa utilizado para testar os algorítmos de decisão de caminho.
const char testmap[][5] = {	{'a','0','0','B','0'},
							{'a','0','0','b','b'},
							{'a','A','0','0','0'},
							{'0','0','0','0','0'},
							{'0','0','0','0','0'}};

// Protótipos de funções
void pegaMapa(char** map, int map_size);
void buscaCaminho(char** map, int map_size, char player);
void move();

/**
 * @brief      Recebe e processa as funções referentes ao bot do jogo TronCo.
 *
 * @param[in]  argc  The argc
 * @param      argv  The argv - o primeiro argumento é o tamanho do mapa.
 *
 * @return     retorna 0 informando ao SO que o programa foi finalizado.
 */
int main(int argc, char const *argv[]) {
	
	int map_size = 0, i, j;
	char** map;
	
	
	// Recebe como argumento o tamanho do mapa.
	if(argc >= 2) {
	    map_size = atoi(argv[1]);
	}
	
	//aloca memória para o mapa
	map = (char**) malloc(sizeof(char*)*map_size);
	for (i = 0; i < map_size; i++) {
	    map[i] = (char*) malloc(sizeof(char)*map_size);
	}
	
	// Obtém o mapa atual do servidor.
	pegaMapa(map, map_size);
	
	for(i = 0; i < map_size; i++) {
    	for(j = 0; j < map_size; j++) {
    		printf("%c ", map[i][j]);
    	}
    	printf("\n");
    }
	
	// Realiza a busca no grafo pelo caminho a ser seguido.
	buscaCaminho(map, map_size);
	
	// Controla a movimentação do personagem.
	move();
	
	return 0;
}

/**
 * @brief       Define qual o caminho que o bot necessita percorrer.
 *  			Obs: o primeiro algorítmo vai ser simplesmente um random que define pra qual
 *				lado o player fantasma deve correr para ter certeza que o bot vai funcionar.
 * 
 * @param[In]   map         O mapa a ser processado.
 * @param[In]   map_size    O tamanho do mapa.
 * @param[In]   player      A letra do player a buscar.
 */ 
void buscaCaminho(char** map, int map_size, char player) {
	srand(time(NULL));
    int rand_move, pos_x, pos_y;
    
    for(i = 0; i < map_size; i++) {
    	for(j = 0; j < map_size; j++) {
    		if( map[i][j] == player ) {
    			pos_x = i;
    			pos_y = y;
    		}
    	}
    }
    
    //encontra um local ao redor do player onde o objetivo possa ser posto.
    do {
    	rand_move = rand()%8;
    }
    while( board_pos(map, pos_x, pos_y, rand_move) != '0' );
    
    //encontra o caminho para o objetivo.
    
}

/**
 * @brief	Encontra o valor do caractere para uma determinada posição que rodeia
 *			a posição do player.
 * 
 * @return	Retorna o valor do caractere em relação aquela posição no mapa
 */
char board_pos(char** map, int pos_x, int pos_y, int rand_move) {
	char val;
	switch(rand_move) {
		case 0:
			val = map[pos_x - 1][pos_y - 1];
			break;
		case 1:
			val = map[pos_x][pos_y - 1];
			break;
		case 2:
			val = map[pos_x + 1][pos_y - 1];
			break;
		case 3:
			val = map[pos_x - 1][pos_y];
			break;
		case 4:
			val = map[pos_x + 1][pos_y];
			break;
		case 5:
			val = map[pos_x - 1][pos_y + 1];
			break;
		case 6:
			val = map[pos_x][pos_y + 1];
			break;
		case 7:
			val = map[pos_x + 1][pos_y + 1];
			break;
		
	}
	return val;
}

/**
 * @brief
 * 
 * @param[In]   map         O endereço onde o mapa deverá ser armazenado
 * @param[In]   map_size    O tamanho do mapa.
 */
void pegaMapa(char** map, int map_size) {
    int i, j;
    
    for(i = 0; i < map_size; i++) {
    	for(j = 0; j < map_size; j++) {
    		map[i][j] = testmap[i][j];
    	}
    }
    
}

void move(){
	
}