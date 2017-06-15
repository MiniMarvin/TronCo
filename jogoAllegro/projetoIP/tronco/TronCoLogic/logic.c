#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <stdbool.h>
 
const int LARGURA_TELA = 640;
const int ALTURA_TELA = 480;
#define cteInicio '4'  // DIREÇAO INICIAL DE CADA PLAYER.
ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_BITMAP *fundo = NULL;
ALLEGRO_FONT *fonte = NULL;
#define tam 50 // TAMANHO DA MATRIZ.
bool inicializar();
 
int main(void){
    
    bool sair = false;
    int i, j;
    char tecla = '0', teclaControl = cteInicio;
    char matriZ[TAM][TAM];
    if (!inicializar())
    {
        return -1;
    }
    
    al_draw_bitmap(fundo, 0, 0, 0);
    
    for(i=0;i<TAM;i++)
        for(j=0;j<TAM;j++)
            matriZ[i][j] = '0'
    
    
    
    while (!sair)
    {
        while(!al_is_event_queue_empty(fila_eventos))
        {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);
 
            if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                switch(evento.keyboard.keycode)
                {
                case ALLEGRO_KEY_W:
                    tecla = '1';
                    break;
                case ALLEGRO_KEY_S:
                    tecla = '2';
                    break;
                case ALLEGRO_KEY_A:
                    tecla = '3';
                    break;
                case ALLEGRO_KEY_D:
                    tecla = '4';
                    break;
                }
            }
            else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                sair = true;
            }
        }
        
        if(tecla != '0')                // Se o usuario apertou alguma tecla, a variável de controle pegará esse valor. Caso contrário, a variável de controle comandará a movimentação
        teclaControl = tecla;         // a partir da direção inicial estabelecida por cteInicio.
        
        //if (teclaControl)
        //{
            al_draw_bitmap(fundo, 0, 0, 0);
 
            switch (teclaControl)
            {
            case '1':
                // W
                
                break;
            case '2':
                // S
                
                break;
            case '3':
             // A
                
                break;
            case '4':
             // D
                
                break;
            }
 
            tecla = '0';
        //}
 
        al_flip_display();
    }
 
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
 
    return 0;
}
 



bool inicializar()
{
    if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return false;
    }
 
    al_init_font_addon();
 
    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_ttf.\n");
        return false;
    }
 
    if (!al_init_image_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
        return false;
    }
 
    if (!al_install_keyboard())
    {
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
        return false;
    }
 
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela)
    {
        fprintf(stderr, "Falha ao criar janela.\n");
        return false;
    }
 
    al_set_window_title(janela, "Utilizando o Teclado");
 
    fonte = al_load_font("comic.ttf", 72, 0);
    if (!fonte)
    {
        fprintf(stderr, "Falha ao carregar \"fonte comic.ttf\".\n");
        al_destroy_display(janela);
        return false;
    }
 
    fila_eventos = al_create_event_queue();
    if (!fila_eventos)
    {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        return false;
    }
 
    fundo = al_load_bitmap("bg.jpg");
    if (!fundo)
    {
        fprintf(stderr, "Falha ao carregar imagem de fundo.\n");
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        return false;
    }
 
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
 
    return true;
}