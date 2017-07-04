#include "AllegroCore.h"


bool coreInit()
{
    //modules and add-ons initialization
	if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return false;
    }

    if (!al_init_image_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
        return false;
    }

    al_init_font_addon();
    // if (!al_init_font_addon())
    // {
    //     fprintf(stderr, "Falha ao inicializar add-on allegro_font.\n");
    //     return false;
    // }

    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_ttf.\n");
        return false;
    }

    if (!al_init_primitives_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_primitives.\n");
        return false;
    }


    eventsQueue = al_create_event_queue();
    if (!eventsQueue)
    {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        return false;
    }

 	

    return true;
}

bool windowInit(int W, int H, char title[])
{
    //window and event queue creation
    main_window = al_create_display(W, H);
    if (!main_window)
    {
        fprintf(stderr, "Falha ao criar janela.\n");
        return false;
    }
    al_set_window_title(main_window, title);

    //registra janela na fila de eventos
    al_register_event_source(eventsQueue, al_get_display_event_source(main_window));

    return true;
}

bool inputInit()
{
	/*------------------------------MOUSE--------------------------------*/
	//Inicializa Mouse
	if (!al_install_mouse())
    {
        fprintf(stderr, "Falha ao inicializar o mouse.\n");
        al_destroy_display(main_window);
        return false;
    }

    // Atribui o cursor padrão do sistema para ser usado
    if (!al_set_system_mouse_cursor(main_window, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
    {
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
        al_destroy_display(main_window);
        return false;
    }
    /*------------------------------MOUSE--------------------------------*/

    /*------------------------------TECLADO------------------------------*/
    if (!al_install_keyboard())
    {
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
        return false;
    }

    /*------------------------------TECLADO------------------------------*/


    //Registra mouse e teclado na fila de eventos
    al_register_event_source(eventsQueue, al_get_mouse_event_source());
    al_register_event_source(eventsQueue, al_get_keyboard_event_source());

    return true;
}

bool fontInit()
{
    /*------------------------------FONTE--------------------------------*/
    start = al_load_font("Resources/Fonts/pressStart.ttf", 16, 0);
    if (!start)
    {
        fprintf(stderr, "Falha ao carregar \"pressStart.ttf\".\n");
        return false;
    }

    ubuntu = al_load_font("Resources/Fonts/Ubuntu-R.ttf", 32, 0);
    if (!ubuntu)
    {
        fprintf(stderr, "Falha ao carregar \"Ubuntu-R.ttf\".\n");
        return false;
    }

    return true;
}

void allegroEnd()
{
    al_destroy_display(main_window);
    al_destroy_event_queue(eventsQueue);
}



void startTimer()
{
    startingTime = al_get_time();
}

double getTimer()
{
    return al_get_time() - startingTime;
}

void FPSLimit()
{
	if (getTimer() < 1.0/FPS)
    {
        al_rest((1.0 / FPS) - getTimer());
    }
}


bool loadGraphics()
{
    hscs = al_load_bitmap("Resources/Etc/World.png");
    if (!hscs){
        fprintf(stderr, "Falha carregando world.png\n");
        return false;
    }

    objects = al_load_bitmap("Resources/Tilesets/objects.png");
    if (!objects){
        fprintf(stderr, "Falha carregando objects.png\n");
        return false;
    }

    return true;
}
