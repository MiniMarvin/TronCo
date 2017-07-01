/**
 * OBS: O X é a variação vertical e o Y é a variação horizontal!!!
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "libTronco/tronco.h"
#include "libSocket/server.h"

#include <time.h>

#define RESTART_GAME

#define UP    0
#define DOWN  1
#define LEFT  2
#define RIGHT 3

void delay(unsigned int mseconds);
void setPlayers(player set[MAXCLIENTS], player to[MAXCLIENTS]);
int cleanMatrix(char matrix[SIZEX][SIZEY], int player);
void printMatrix(char matrix[SIZEX][SIZEY]);

int main(int argc, char **argv){
    printf("SERVER_RUNNING");

    bool sair = false;
    int i, j; 

    player players[MAXCLIENTS];

    clientMsg clientPackage;
    serverMsg serverPackage;

    struct msg_ret_t retorno;


    //// direções iniciais
    serverPackage.dir[0] = DOWN;
    serverPackage.dir[1] = DOWN;
    serverPackage.dir[2] = LEFT;
    serverPackage.dir[3] = RIGHT;


    serverInit(MAXCLIENTS);
   
   int n_connections = 0; //número de clientes conectados
   int received_ids[4];


   while(!sair){
	//aceitar novas conexões até encher a sala
	while(n_connections < 4){
		int id = acceptConnection();
		if(id != NO_CONNECTION){
		  printf("Alguem se Conectou com ID %d\n", id);
		  n_connections++;
		}
	}

	//receber mensagens dos clientes
        retorno = recvMsg(&clientPackage);
        if(retorno.status == MESSAGE_OK){
            int id = retorno.client_id;
	    received_ids[id] = 1;
 	    //atualizar a direção do cliente
            serverPackage.dir[id] = clientPackage.dir;
	    //ver se cliente morreu
            if(clientPackage.status = 0){
            	disconnectClient(id);
	    }
        }
	
	//verificar se todos os clientes atualizaram o servidor
	int count = 0;
	for(i = 0; i < 4; i++){
		count = count + received_ids[i];
	}

	//quando todos os clientes atualizarem o servidor
	if (count == 4){
		//atualizar todos eles ao mesmo tempo;
		for(int id = 0; id < 4; id++){
			serverPackage.id = i;
			sendMsgToClient(&serverPackage, sizeof(serverPackage), id);	
			received_ids[id] = 0;
		}	
	}
    }
   return 0;
}

void delay(unsigned int mseconds){
    clock_t goal = mseconds + clock();
    while (goal > clock());
}


