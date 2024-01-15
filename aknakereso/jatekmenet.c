#include "jatekmenet.h"

extern int gombok_jobb_szele_a_nehezsegi_szinteknel[4];
extern char nehezsegi_szintek_nevei[4][13];
extern SDL_TimerID id;

/* ez a fuggveny hivodik meg az idozito altal.  !InfoC!
 * betesz a feldolgozando esemenyek koze (push) egy felhasznaloi esemenyt */
//az ido meresehez kell
Uint32 idozit(Uint32 ms, void *param)
{
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;   /* ujabb varakozas */
}

//osszegzi az osszes olyan fuggvenyt ami kell a jatek ujrakezdesehez szukseges fuggvenyeket igy csak ezt kell meghivni es mindent megold
//a tombok free elese nincs benne mert akkor nem lehetne meghívni a program elején, úgyhogy azt külön kell csinálni
void jatek_ujrakezdese(jatek_adatai* jatek,ablak_meretei* ablak,bool restart)
{
    if(!restart)
    {
        palya_adatainak_bekerese(jatek);
        palya_tombjeinek_lefoglalasa(jatek);
    }
    ures_palya_feltoltese(jatek);
    bombak_generalasa(jatek);
    szamok_helyrerakasa(jatek);
    ablak_mereteinek_kiszamolasa(*jatek, ablak);
    jatek->idomero=-1;
    jatek->zaszlozott_mezok_szama=0;

}

//a kattintott koordinatakat alakitja tombindexeke
void tomb_koordinatakra(koordinatak* kattintott)
{
    kattintott->y-=(menu_merete+idomero_magassaga);
    kattintott->y/=mezo_szelessege_magassaga;
    kattintott->x/=mezo_szelessege_magassaga;
}

//ha jatek nehezsegen akar valtozatni, megnézzük melyikre
void menure_kattintott(jatek_adatai* jatek,koordinatak kattintott,ablak_meretei* ablak,SDL_Window* window)
{
    for(int i=beginner; i<=custom; i++)
    {
        if(kattintott.x<gombok_jobb_szele_a_nehezsegi_szinteknel[i])
        {
            jatek->nehezsegi_szint=i;
            palya_tombjeinek_falszabaditasa(jatek);
            jatek_ujrakezdese(jatek, ablak,false);

            SDL_SetWindowSize(window,ablak->szelesseg,ablak->magassag);//ablak átméretezése
            jatek->vesztett_e=false;
            break;
        }
    }
}

//ha a jatekos mezore kattintott bal egergombbal
void mezore_kattintott(koordinatak kattintott,jatek_adatai* jatek,SDL_Renderer* renderer,ablak_meretei ablak)
{
    if(kattintott.x>jatek->palya_szelessege*mezo_szelessege_magassaga || kattintott.y>jatek->palya_magassaga*mezo_szelessege_magassaga+idomero_magassaga+menu_merete)
    {
        return;//erre azert van szukseg mert lehet hogy az ablak nagyobb mint a palya es ha melle kattint is erzekelne valamit a kovetkezo sorban mert igazabol egy 1d tomb a palya
    }
    tomb_koordinatakra(&kattintott);
    //ha az elsõ kattintás: el kell indítati az idõzítõt
    if(jatek->idomero==-1 && !jatek->nyert)
    {
        id=SDL_AddTimer(1000, idozit, NULL);
        jatek->idomero=0;
    }


    //ha zaszlóra nyom nem csinál semmit
    if(jatek->palya[kattintott.y][kattintott.x].zaszlo||jatek->palya[kattintott.y][kattintott.x].lathato_e)
    {
        return;
    }

    //bombát talált e el
    else if(jatek->palya[kattintott.y][kattintott.x].bombak_helyei)
    {
        SDL_RemoveTimer(id);//leallitja az idozitot
        jatek->vesztett_e=true;
        vesztett_kirajzolas(*jatek,renderer,ablak);
        system("start https://www.youtube.com/watch?v=oEGO-PrQ3_4");

    }

    //láthatóvá tesszük a megfelelõ mezõket
    else if(!jatek->palya[kattintott.y][kattintott.x].lathato_e)
    {
        lathatosag_terjedese(kattintott,jatek);
    }
}

//magat magyarazza
bool nyert_e(jatek_adatai jatek)
{
    for(int y=0; y<jatek.palya_magassaga; y++)
    {
        for(int x=0; x<jatek.palya_szelessege; x++)
        {
            //ha bármenyik nem bomba hely nincs felderítve akkor nem nyer a játékos
            if(!jatek.palya[y][x].lathato_e && !jatek.palya[y][x].bombak_helyei)
            {
                return false;
            }
        }
    }
    return true;
}

void ha_nyert(jatek_adatai* jatek,SDL_Renderer* renderer,ablak_meretei ablak)
{
    //le kell állítani az idõmérõt
    SDL_RemoveTimer(id);

    //csak akkor foglalkozunk a rekordal ha elõre megadott nehézségi szinteken játszik a játékos
    if(jatek->nehezsegi_szint!=custom)
    {
        if(jatek->rekord>jatek->idomero)
        {
            kirajzolas(*jatek,renderer,ablak);
            jatek->rekord=jatek->idomero;
            uj_rekord_ablak(*jatek);
            uj_rekord(*jatek);
        }
    }
}

//ha a jatekos be vagy ki zaszlozott egy mezot
void zaszlozott(koordinatak kattintott, jatek_adatai* jatek)
{
    tomb_koordinatakra(&kattintott);//nem kell a mainben levo kattintottat valtoztetni mert ugysincs semmi masra hasznalva mar
    // láthazó mezõket nem zászlózhat, és akkor sem ha nyert vagy vesztett
    if(!jatek->palya[kattintott.y][kattintott.x].lathato_e && !jatek->nyert && !jatek->vesztett_e)
    {
        jatek->palya[kattintott.y][kattintott.x].zaszlo^=true;

        //hogy tudjam számolni a vélhetõ "veszélyes bombákat"
        if(jatek->palya[kattintott.y][kattintott.x].zaszlo)
        {
            jatek->zaszlozott_mezok_szama++;
        }
        else
        {
            jatek->zaszlozott_mezok_szama--;
        }
    }
}
