#include <stdio.h>
#include <stdlib.h>


//os módulos gerais serão apenas uma função int
int menu(); 
int jogo();


int main(){
    int running = 1;
    
    int estado = 0; //0 = menu //1 = jogar //2 = sair

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