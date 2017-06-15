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