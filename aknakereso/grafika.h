#ifndef GRAFIKA_H_INCLUDED
#define GRAFIKA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <string.h>

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include "valtozotipusok.h"
#include "debugmalloc.h"

/* a program minden grafikus megjelenítését ez a file tartalmazza*/

/* ablak letrehozasa infoC, en ehez hozza nem erek*/
/*                          konkretan fel oraval kesobb hozzaertem...*/
//letrehoz egy SDL ablakot
void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer);


//az ablak tetején lévő választható nehézségi szinteket írja ki
void nehezsegi_szintek_kiirasa(SDL_Renderer *renderer);


//ha vesztett:szomrú, ha nyer:mosolygos fejet rajzol ki, a magyar nyelvtant szándékosan nem betartó nnévvel rendelkező képek segítségével
void mosi_szomi_fely_kirjaz(SDL_Renderer* renderer,ablak_meretei ablak,bool vesztett_e);


//kiírja a vélt számát a nem bezászlózott bombáknak
void bombak_szamanak_kirajzolasa(SDL_Renderer* renderer,jatek_adatai jatek);


//kiírja mennyi ideje kezdte a játékos a játékot
void ido_kirajzolasa(SDL_Renderer* renderer,jatek_adatai jatek,ablak_meretei ablak);


//az egész játokot megjeleníti grafikusan
void kirajzolas(jatek_adatai jatek, SDL_Renderer* renderer,ablak_meretei ablak);


//kiszámolja az SDL ablak szélességét, magasságát
void ablak_mereteinek_kiszamolasa(jatek_adatai jatek,ablak_meretei* ablak);


//ha a jatékos vesztett akkor megjelenítünk minden mezőt és úgy rajzoljuk ki a pályát
void vesztett_kirajzolas(jatek_adatai jatek, SDL_Renderer* renderer,ablak_meretei ablak);


enum custom_ablak_tulajdonsagainak_adatainak_pls_kill_me_nem_tudtam_mire_vallalkozok_az_allithatoval_XD
{
    custom_ablak_szelessege=400,custom_ablak_magassaga=100,kiirando_dolgok_szama=3,
    ok_gomb_terulete=100,haromszog_szelessege=20,
    betumeret=10
};


//a custom nehézségi szint adatainak bekéréséhez szükséges ablakot jeleníti meg
void custom_kirajz(int tempek[],SDL_Renderer* custom_renderer);


//ha új rekordot állít fel a játékos, akkor ezt egy új ablakon kiírja neki
void uj_rekord_ablak(jatek_adatai jatek);

#endif // GRAFIKA_H_INCLUDED
