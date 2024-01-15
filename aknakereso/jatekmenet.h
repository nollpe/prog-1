#ifndef JATEKMENET_H_INCLUDED
#define JATEKMENET_H_INCLUDED
#include "palya_elokeszitese.h"
#include "valtozotipusok.h"
#include "lathatosag_terjedese.h"
#include "debugmalloc.h"
#include "grafika.h"

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <math.h>

//osszegzi az osszes olyan fuggvenyt ami kell a jatek ujrakezdesehez szukseges fuggvenyeket igy csak ezt kell meghivni es mindent megold
//a tombok free elese nincs benne mert akkor nem lehetne meghívni a program elején, úgyhogy azt külön kell csinálni
void jatek_ujrakezdese(jatek_adatai* jatek,ablak_meretei* ablak,bool restart);

//az ido meresehez kell
Uint32 idozit(Uint32 ms, void *param);

//a kattintott koordinatakat alakitja tombindexeke
void tomb_koordinatakra(koordinatak* kattintott);

//ha jatek nehezsegen akar valtozatni, megnézzük melyikre
void menure_kattintott(jatek_adatai* jatek,koordinatak kattintott,ablak_meretei* ablak,SDL_Window* window);

//ha a jatekos mezore kattintott bal egergombbal
void mezore_kattintott(koordinatak kattintott,jatek_adatai* jatek,SDL_Renderer* renderer,ablak_meretei ablak);

bool nyert_e(jatek_adatai jatek);

void ha_nyert(jatek_adatai* jatek,SDL_Renderer* renderer,ablak_meretei ablak);

//ha a jatekos be vagy ki zaszlozott egy mezot
void zaszlozott(koordinatak kattintott, jatek_adatai* jatek);


#endif // JATEKMENET_H_INCLUDED
