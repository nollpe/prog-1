#ifndef LATHATOSAG_TERJEDESE_H_INCLUDED
#define LATHATOSAG_TERJEDESE_H_INCLUDED
#include "valtozotipusok.h"
#include "debugmalloc.h"

/*a név magát magyarázza*/
/*egy mezokoordinatait es a jatek adatait keri
*megnezi, hogy egy adott mezõ felderitesekor
*a mellette levo mezoket is fel kell e deriteni,es ha
*igen akkor meghivja ugyanezt a fuggveny arra a koordinatara*/
void lathatosag_terjedese(koordinatak a,jatek_adatai* jatek);

#endif // LATHATOSAG_TERJEDESE_H_INCLUDED
