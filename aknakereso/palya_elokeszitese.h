#ifndef PALYA_ELOKESZITESE_H_INCLUDED
#define PALYA_ELOKESZITESE_H_INCLUDED
#include "valtozotipusok.h"
#include <stdio.h>
#include <stdlib.h>
#include "debugmalloc.h"
#include "grafika.h"

enum custom_bekeres
{
    custom_szelesseg_index=0,custom_magassag_index=1,custom_bombak_szama_index=2
};

//ha a játékos custom pályát akar felállítani akkor annak az adatait ez a függvény kéri be
void custom_beker(jatek_adatai* jatek);

//különböző nehézségi szintek pályájának az adatait kéri be: vagy txt ből, vagy a felhasználótól
void palya_adatainak_bekerese(jatek_adatai* jatek);

//ha új rekordot állít fel a játkos azt elmenti a program, de csak az előre felállított nehézségi szinteken
void uj_rekord(jatek_adatai jatek);

// a pálya adatainak tárolásához a memória lefoglalása
void palya_tombjeinek_lefoglalasa(jatek_adatai* jatek);

// a pálya adatainak tárolásához használt memória felszabadítása
void palya_tombjeinek_falszabaditasa(jatek_adatai* jatek);

// a pálya adatainak tárolásához a memória feltöltése az alaphelyzettel
void ures_palya_feltoltese(jatek_adatai* jatek);

//a pályán lévő bombákat rakja le
void bombak_generalasa(jatek_adatai* jatek);

//meghatározza, hogy mennyi bomba van egy adott mező körül
int bombak_szama_korulotte(koordinatak k,jatek_adatai* jatek);

//megnézi, hogy egy adott mezőre milyen számot kell rakni: hány bomba van körülötte (és azt be is írja)
void szamok_helyrerakasa(jatek_adatai* jatek);

#endif // PALYA_ELOKESZITESE_H_INCLUDED
