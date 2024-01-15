#include "lathatosag_terjedese.h"
#include "debugmalloc.h"

void lathatosag_terjedese(koordinatak a,jatek_adatai* jatek)
{
    //a z�szl�kat a felder�tett ter�letr�l kivessz�k
    if(jatek->palya[a.y][a.x].zaszlo)
    {
        jatek->palya[a.y][a.x].zaszlo=false;
        jatek->zaszlozott_mezok_szama--;
    }
    //ha 0 a hely �rt�ke akkor a mellette l�v�k�n biztos nincs bomba teh�t megh�vjuk a f�ggv�nyt azokra is
    if(jatek->palya[a.y][a.x].palya==0&&!jatek->palya[a.y][a.x].lathato_e)
    {
        jatek->palya[a.y][a.x].lathato_e=true;
        for(int i=-1; i<2; i++)
        {   /*igazabol nem problema, hogy megnez egy olyan mezot amit mar megnezett, mert akkor csak szimplan nem tortenik semmi */
            for(int j=-1; j<2; j++)
            {
                koordinatak temp= {a.x+i,a.y+j};
                if(temp.x<jatek->palya_szelessege&&temp.x>=0&&temp.y<jatek->palya_magassaga&&temp.y>=0)
                {
                    lathatosag_terjedese(temp,jatek);
                }
            }
        }

    }
    //ha a mez� �rt�ka nagyobb mint 0 akkor csak l�that�v� kell tenni
    else if(jatek->palya[a.y][a.x].palya>0)
    {
        jatek->palya[a.y][a.x].lathato_e=true;
    }
}
