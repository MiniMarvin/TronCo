#include <stdio.h>
#include <stdlib.h>
#include "lib/client.h"


void limpar_tela();

int menu();
int jogo();


int main(){
    int running = 1;
    int estado = 0; //0 = menu //1 = jogar
    //int cursor = 0; //posição do cursor
    
    while (running == 1){
        printf("ESTADO = %d\n", estado);
        switch(estado){
            case 0:
                estado = menu();
                break;
            
            case 1:
                printf("COMEÇAR O JOGO\n");
                estado = jogo();
                break;
            
            case 3:
                running = 0; //quit -> finaliza o jogo
                break;
        }

    }
}

void limpar_tela(){
    printf("\e[1;1H\e[2J");
}

int menu(){
        int cursor = 0; 
        
        while(1){ //eternamente
            printf("\n\n");
            if(cursor == 0) printf("->");
            printf("            JOGAR\n\n");
            if(cursor == 1) printf("->");
            printf("            RANKING\n\n");
            if(cursor == 2) printf("->");
            printf("            QUIT\n\n");
            
            char read;
            read = getch();
            //printf("EU RECEBI: %c", read);
            switch(read){
                case 's':
                    cursor++;
                    cursor = cursor%3;
                    printf("%d", cursor);
                    break;
                case 'w':
                    cursor--;
                    if(cursor == -1) cursor = 2;
                    cursor = cursor%3;
                    printf("%d", cursor);
                    break;
                    
                case ' ':
                    return cursor + 1;//retorna o estado
            }
            
            
            
            limpar_tela();
        }
}

int  jogo(){
    printf("O JOGO\n");
    return 0;
}

