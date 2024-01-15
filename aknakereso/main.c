//kérem úgy, hogy a "kisebb" szó 1 s-sel van írva.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>
#include <string.h>

#include "palya_elokeszitese.h"
#include "valtozotipusok.h"
#include "lathatosag_terjedese.h"
#include "debugmalloc.h"
#include "grafika.h"
#include "jatekmenet.h"

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>

SDL_TimerID id;

int gombok_jobb_szele_a_nehezsegi_szinteknel[4];
char nehezsegi_szintek_nevei[4][13]= {"Beginner","Intermediate","Expert","Custom"};

int main(int argc, char *argv[])
{
    srand(time(0));

    ablak_meretei ablak;
    jatek_adatai jatek;
    jatek.nehezsegi_szint=beginner;//default

    jatek_ujrakezdese(&jatek,&ablak,false);

    SDL_Window *window;
    SDL_Renderer *renderer;
    sdl_init("Aknakereso",ablak.szelesseg,ablak.magassag, &window, &renderer);

    kirajzolas(jatek,renderer,ablak);

    bool quit = false;
    jatek.vesztett_e=false;
    jatek.nyert=false;

    SDL_Event event;
    while(!quit)
    {
        koordinatak kattintott;
        SDL_WaitEvent(&event);
        switch(event.type)
        {
        case(SDL_MOUSEBUTTONUP):
            kattintott.x=event.button.x;
            kattintott.y=event.button.y;
            if(event.button.button == SDL_BUTTON_LEFT)
            {   //menuben kattintott valahova
                if(kattintott.y<menu_merete)
                {
                    menure_kattintott(&jatek,kattintott,&ablak,window);
                }
                //ha a új játékot akar kezdeni
                else if(kattintott.y<menu_merete+idomero_magassaga && abs(kattintott.x-ablak.szelesseg/2)<mezo_szelessege_magassaga)
                {
                    jatek_ujrakezdese(&jatek, &ablak,true);
                    jatek.vesztett_e=false;
                }
                //ilyenkor kattintott mezőre
                else if(kattintott.y>menu_merete+idomero_magassaga)
                {
                    mezore_kattintott(kattintott,&jatek,renderer,ablak);
                }
                //nyert e a játékos
                jatek.nyert=nyert_e(jatek);

                if(jatek.nyert)
                {
                    ha_nyert(&jatek,renderer,ablak);
                }
            }
            //jobb kattintás: be vagy ki zászlózás              csak a pályán csinál valamit
            else if(event.button.button == SDL_BUTTON_RIGHT && kattintott.y>menu_merete+idomero_magassaga)
            {
                zaszlozott(kattintott,&jatek);
            }
            break;
        case(SDL_USEREVENT):
            //ido merese
            jatek.idomero++;
            ido_kirajzolasa(renderer,jatek,ablak);
            continue;
            break;
        case(SDL_QUIT):
            quit=true;
            continue;
            break;
        default:
            continue;// hogy ne legyen minden egyes egérmozdulatnál képfrissítés
            break;

        }
        //ha vesztett nem rajzoljuk ki ameddig ujra nem kezdi
        if(!jatek.vesztett_e)
        {
            kirajzolas(jatek,renderer,ablak);
        }
    }
    SDL_Quit();
    palya_tombjeinek_falszabaditasa(&jatek);

    return 0;
}

