#ifndef VALTOZOTIPUSOK_H_INCLUDED
#define VALTOZOTIPUSOK_H_INCLUDED
#include <stdbool.h>
#include "debugmalloc.h"//ha itt benne van, mindenhol benne van
/*az összeg nem alapvető változótípust itt hozom létr, hogy elég legyen ezt include-olgatni minden más fileba*/

//a SDL ablakok szélesség, magassága
typedef struct ablak_meretei
{
    int szelesseg;
    int magassag;

}ablak_meretei;

//a programban lényegében ehhez igazodik minden a megjelenítéskor
enum ablakmeretei
{
    mezo_szelessege_magassaga=32, menu_merete=30,
    idomero_magassaga=40

};

//majd ezeknek az adatait egy txt bol fogom bekerni kiveve az egyenit
enum nehezsegi_szintek
{
    beginner=0,intermediate=1,expert=2,custom=3
};

#include <SDL2/SDL_stdinc.h>
// a pálya egy mezőjének koordinátái
typedef struct koordinatak
{
    Sint32 x;
    Sint32 y;
} koordinatak;

// a pálya egy mezőjének adatai egy structba rakva
typedef struct palya_tombok
{
    bool bombak_helyei;
    int palya;
    bool lathato_e;
    bool zaszlo;
}palya_tombok;

typedef struct jatek_adatai
{
    int palya_szelessege;//txt 1. sor ha egy elore meghatarozott palyan jatszik a jetekos
    int palya_magassaga;//txt 2. sor
    int bombak_szama;//txt 3. sor

    //tomboket bele kene rakni megegy structba es azt lefoglalni

    //tombok amikkel kezelem a palya adatait
    palya_tombok** palya;

    //ezekben vannak elmentve az adatok, ezekre nem referalok sehol csak az ezekbe mutato pointertomboket hasznalom
    palya_tombok* lefoglal;

    int nehezsegi_szint;
    int idomero;// a jetekos idejet meri az adott jatekban
    int rekord; //masodpercekben és csak akkor ha elõre meghatározott nehézségi szinten játszott
                //tarolja az adott nehezsegi szintre a jatekos rekordjat
    int zaszlozott_mezok_szama;
    bool vesztett_e;
    bool nyert;

}jatek_adatai;

#endif // VALTOZOTIPUSOK_H_INCLUDED
