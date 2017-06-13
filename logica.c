#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lib/client.h"

#define size 40

void limpar_tela();

int menu();
int jogo();
void delay(unsigned int mseconds);

int main(){
    int running = 1;
    int estado = 0; //0 = menu //1 = jogar

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

int menu(){
        int cursor = 0; 
        char read;
        //Executa enquanto o usuário escolher
        while(1){ 
            printf("\n\n");
            if(cursor == 0) printf("->");
            printf("            JOGAR\n\n");
            if(cursor == 1) printf("->");
            printf("            RANKING\n\n");
            if(cursor == 2) printf("->");
            printf("            QUIT\n\n");
            
            
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

int jogo(){
    int myX = 0;
    int myY = 0;
    
    int running = 1;
    
    int dir = 0; //0 = direita; 1 = baixo; 2 = esquerda; 3 = cima 
    int tabuleiro [size][size];
    
    //preencher matriz
    int i = 0;
    for(i; i < size; i++){
        int j = 0;
        for(j; j < size; j++){
            tabuleiro[i][j] = ' ';
        }
    }
    
    while(running == 1){
        tabuleiro[myX][myY] = 'X';
        
        //imprimir matriz
        int i = 0;
        for(i; i < size; i++){
            int j = 0;
            for(j; j < size; j++){
                printf("%c ", tabuleiro[j][i]);
            }
            printf("\n");
        }    
        
        //atualizar meu X e Y
        switch(dir){
            case 0: // direita
                myX++;
                break;
                
            case 1: // baixo
                myY++;
                break;
            
            case 2: // esquerda
                myX--;
                break;
            
            case 3: // cima
                myY--;
                break;
        }
        
        
        //ler os valor
        char read;
        read = getch();
        //printf("EU RECEBI: %c", read);
        switch(read){
            case 's': // direita
                    dir = 1;
                    break;
            case 'a': // baixo
                    dir = 2;
                    break;
                    
            case 'w': // esquerda
                    dir = 3;
                    break;
            
            case 'd': // cima
                    dir = 0;
                    break;
                    
        }
        
        //dar um delay
        delay(10000);   
        
        //limpar tela
        limpar_tela();
    }
    
    return 0;
}

//ferramentas
void limpar_tela(){
    printf("\e[1;1H\e[2J");
}

void delay(unsigned int mseconds){
    clock_t goal = mseconds + clock();
    while (goal > clock());
}