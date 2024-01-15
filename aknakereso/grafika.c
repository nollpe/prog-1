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
#include "debugmalloc.h"

#include "grafika.h"


/* a program minden grafikus megjelen�t�s�t ez a file tartalmazza*/

/* ablak letrehozasa infoC, en ehez hozza nem erek*/
/*                          konkretan fel oraval kesobb hozzaertem...*/
//letrehoz egy SDL ablakot
void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow(felirat, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, SDL_WINDOW_RESIZABLE);
    if (window == NULL)
    {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL)
    {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);

    *pwindow = window;
    *prenderer = renderer;
}


extern int gombok_jobb_szele_a_nehezsegi_szinteknel[4];
extern char nehezsegi_szintek_nevei[4][13];

//az ablak tetej�n l�v� v�laszthat� neh�zs�gi szinteket �rja ki
void nehezsegi_szintek_kiirasa(SDL_Renderer *renderer)
{
    int ide_irhato=10;
    for(int i=beginner; i<=custom; i++)
    {
        TTF_Init();
        TTF_Font *font = TTF_OpenFont("arial.ttf", 12);
        if (!font)
        {
            SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
            exit(1);
        }
        /*  !InfoC!  kicsit atalakitva*/
        SDL_Color kek = {0, 0, 185};
        SDL_Surface *felirat;
        SDL_Texture *felirat_t;
        SDL_Rect hova = { 0, 0, 0, 0 };

        felirat = TTF_RenderUTF8_Blended(font, nehezsegi_szintek_nevei[i], kek);
        felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);
        hova.x = ide_irhato;

        ide_irhato+=felirat->w+20;
        gombok_jobb_szele_a_nehezsegi_szinteknel[i]=ide_irhato-10;// a felirat 10 pixeles k�rnyezet�ben �rz�keli ha r� kattint a user

        hova.y = 10;
        hova.w = felirat->w;
        hova.h = felirat->h;
        SDL_RenderCopy(renderer, felirat_t, NULL, &hova);
        SDL_FreeSurface(felirat);
        SDL_DestroyTexture(felirat_t);
    }
}


//ha vesztett:szomr�, ha nyer:mosolygos fejet rajzol ki, a magyar nyelvtant sz�nd�kosan nem betart� nn�vvel rendelkez� k�pek seg�ts�g�vel
void mosi_szomi_fely_kirjaz(SDL_Renderer* renderer,ablak_meretei ablak,bool vesztett_e)
{
    SDL_Texture* mosifely;

                                    // k�l�n k�p ha vesztett/ nyert
    mosifely=IMG_LoadTexture(renderer,(vesztett_e)?"szomifely.png":"mosifely.png");

    SDL_Rect forrasfelulet= {0,0,idomero_magassaga,idomero_magassaga};
    SDL_Rect celfelulet= {ablak.szelesseg/2-idomero_magassaga/2,menu_merete,idomero_magassaga,idomero_magassaga};
    SDL_RenderCopy(renderer,mosifely,&forrasfelulet,&celfelulet);

    SDL_DestroyTexture(mosifely);
}


//ki�rja a v�lt sz�m�t a nem bez�szl�zott bomb�knak
void bombak_szamanak_kirajzolasa(SDL_Renderer* renderer,jatek_adatai jatek)
{
    int ennyi=jatek.bombak_szama-jatek.zaszlozott_mezok_szama;
    char megmaradt[3];
    itoa(ennyi,megmaradt,10);
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("arial.ttf", 30);
    if (!font)
    {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }
    SDL_Color peros= {185,0,0};
    SDL_Surface *felirat;
    SDL_Texture *felirat_t;
    SDL_Rect hova = { 0, 0, 0, 0 };

    felirat = TTF_RenderUTF8_Blended(font, megmaradt, peros);
    felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);

    boxRGBA(renderer,0,menu_merete,felirat->w+20,idomero_magassaga+menu_merete-1,255,255,255,255);

    hova.x = 10;
    hova.y = menu_merete+5;
    hova.w = felirat->w;
    hova.h = felirat->h;
    SDL_RenderCopy(renderer, felirat_t, NULL, &hova);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
    SDL_RenderPresent(renderer);

}


//ki�rja mennyi ideje kezdte a j�t�kos a j�t�kot
void ido_kirajzolasa(SDL_Renderer* renderer,jatek_adatai jatek,ablak_meretei ablak)
{
    char ido[3];//az eredetiben is csak 3 ig irja (asszem) � akinek az nem eleg az mit akar?
    if(jatek.idomero==-1)
    {
        jatek.idomero=0;
    }
    itoa(jatek.idomero,ido,10);
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("arial.ttf", 30);
    if (!font)
    {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }
    SDL_Color peros= {185,0,0};
    SDL_Surface *felirat;
    SDL_Texture *felirat_t;
    SDL_Rect hova = { 0, 0, 0, 0 };


    felirat = TTF_RenderUTF8_Blended(font, ido, peros);
    felirat_t = SDL_CreateTextureFromSurface(renderer, felirat);

    boxRGBA(renderer,ablak.szelesseg-felirat->w-30,menu_merete,ablak.szelesseg,idomero_magassaga+menu_merete-1,255,255,255,255);

    hova.x = ablak.szelesseg-felirat->w-20;
    hova.y = menu_merete+5;
    hova.w = felirat->w;
    hova.h = felirat->h;
    SDL_RenderCopy(renderer, felirat_t, NULL, &hova);
    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);
    SDL_RenderPresent(renderer);
}


//az eg�sz j�tokot megjelen�ti grafikusan
void kirajzolas(jatek_adatai jatek, SDL_Renderer* renderer,ablak_meretei ablak)
{
    //h�tt�r legyen csak feh�r
    boxRGBA(renderer,0,0,ablak.szelesseg,ablak.magassag,255,255,255,255);

    //egyes�vel a mez�k megjelen�t�se
    for(int y=0; y<jatek.palya_magassaga; y++)
    {
        for(int x=0; x<jatek.palya_szelessege; x++)
        {

            //https://github.com/vilbeyli/Minesweeper/tree/master/Assets/Textures
            // a felhasznalt kepek innen vannak, nem en csinaltam oket
            SDL_Texture* aktualis_kep;
            if(jatek.palya[y][x].zaszlo)
            {
                aktualis_kep=IMG_LoadTexture(renderer,"flag.png");
            }
            else if(jatek.palya[y][x].lathato_e)
            {
                // bizonyos sz�m k�p�nek neve
                char filenev[7]="n.png";
                filenev[0]='0'+jatek.palya[y][x].palya;

                aktualis_kep=IMG_LoadTexture(renderer,filenev);
            }
            else
            {
                aktualis_kep=IMG_LoadTexture(renderer,"default.png");
            }
            if (aktualis_kep == NULL)
            {
                SDL_Log("Nem nyithato meg a kepfajl: %s %d %d", IMG_GetError(),y,x);
                exit(1);
            }
            SDL_Rect forrasfelulet= {0,0,mezo_szelessege_magassaga,mezo_szelessege_magassaga};
            SDL_Rect celfelulet= {mezo_szelessege_magassaga*x,menu_merete+idomero_magassaga+mezo_szelessege_magassaga*y,mezo_szelessege_magassaga,mezo_szelessege_magassaga};
            SDL_RenderCopy(renderer,aktualis_kep,&forrasfelulet,&celfelulet);
            SDL_DestroyTexture(aktualis_kep);
            //itt nem foglalkozunk azzal, ha vesztett a jatekos , az egy kulon fuggveny
        }

    }
    mosi_szomi_fely_kirjaz(renderer,ablak,false);
    nehezsegi_szintek_kiirasa(renderer);
    ido_kirajzolasa(renderer,jatek,ablak);
    bombak_szamanak_kirajzolasa(renderer,jatek);
    SDL_RenderPresent(renderer);
}


//kisz�molja az SDL ablak sz�less�g�t, magass�g�t
void ablak_mereteinek_kiszamolasa(jatek_adatai jatek,ablak_meretei* ablak)
{
    ablak->magassag=mezo_szelessege_magassaga*jatek.palya_magassaga+menu_merete+idomero_magassaga;

    if(mezo_szelessege_magassaga*jatek.palya_szelessege>gombok_jobb_szele_a_nehezsegi_szinteknel[custom])
    {
        ablak->szelesseg=mezo_szelessege_magassaga*jatek.palya_szelessege;
    }
    else
    {
        ablak->szelesseg=gombok_jobb_szele_a_nehezsegi_szinteknel[custom];
    }
}


//ha a jat�kos vesztett akkor megjelen�t�nk minden mez�t �s �gy rajzoljuk ki a p�ly�t
void vesztett_kirajzolas(jatek_adatai jatek, SDL_Renderer* renderer,ablak_meretei ablak)
{
    boxRGBA(renderer,0,0,ablak.szelesseg,ablak.magassag,255,255,255,255);

    for(int y=0; y<jatek.palya_magassaga; y++)
    {
        for(int x =0; x<jatek.palya_szelessege; x++)
        {
            SDL_Texture* aktualis_kep;
            if(jatek.palya[y][x].bombak_helyei)
            {
                aktualis_kep=IMG_LoadTexture(renderer,"mine.png");
            }
            else
            {
                char filenev[7]="n.png";
                filenev[0]=jatek.palya[y][x].palya+'0';
                aktualis_kep=IMG_LoadTexture(renderer,filenev);
            }
            SDL_Rect innen= {0,0,mezo_szelessege_magassaga,mezo_szelessege_magassaga};
            SDL_Rect ide= {x*mezo_szelessege_magassaga,y*mezo_szelessege_magassaga+menu_merete+idomero_magassaga,mezo_szelessege_magassaga,mezo_szelessege_magassaga};

            SDL_RenderCopy(renderer,aktualis_kep,&innen,&ide);
            SDL_DestroyTexture(aktualis_kep);
        }
    }
    mosi_szomi_fely_kirjaz(renderer,ablak,true);
    nehezsegi_szintek_kiirasa(renderer);
    ido_kirajzolasa(renderer,jatek,ablak);
    bombak_szamanak_kirajzolasa(renderer,jatek);
    SDL_RenderPresent(renderer);
}


//a custom neh�zs�gi szint adatainak bek�r�s�hez sz�ks�ges ablakot jelen�ti meg
void custom_kirajz(int tempek[],SDL_Renderer* custom_renderer)
{
    char szovegek_a_custom_ablakaban[kiirando_dolgok_szama][13]={"szelesseg","magassag","bombak szama"};
    boxRGBA(custom_renderer,0,0,custom_ablak_szelessege,custom_ablak_magassaga,255,255,255,255);
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("arial.ttf", betumeret);
    if (!font)
    {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }

    SDL_Surface *felirat;
    SDL_Texture *felirat_t;
    SDL_Color fekete = {0, 0, 0};
    SDL_Rect hova = { 0, 0, 0, 0 };

    for(int i=0;i<kiirando_dolgok_szama;i++)//magikus szam geci, am nem vok ideges
    {

        /* szelesseg, magassa, bombak szama szoveg kiirasa*/
        felirat = TTF_RenderUTF8_Blended(font, szovegek_a_custom_ablakaban[i], fekete);
        felirat_t = SDL_CreateTextureFromSurface(custom_renderer, felirat);
        hova.x = (i+0.5)*((custom_ablak_szelessege-ok_gomb_terulete)/kiirando_dolgok_szama)-felirat->w/2;
        hova.y = 1;
        hova.w = felirat->w;
        hova.h = felirat->h;
        SDL_RenderCopy(custom_renderer, felirat_t, NULL, &hova);

        /*szelesseg, magassa, bombak szama szamok kiirasa*/
        char temp_szam[5];
        itoa(tempek[i],temp_szam,10);

        /*szelesseg, magassa, bombak szama szamok kiirasa*/
        felirat = TTF_RenderUTF8_Blended(font, temp_szam, fekete);
        felirat_t = SDL_CreateTextureFromSurface(custom_renderer, felirat);
        hova.x = (i+0.5)*((custom_ablak_szelessege-ok_gomb_terulete)/kiirando_dolgok_szama)-felirat->w/2;
        hova.y = custom_ablak_magassaga/2-felirat->h/2;
        hova.w = felirat->w;
        hova.h = felirat->h;
        SDL_RenderCopy(custom_renderer, felirat_t, NULL, &hova);

        /* + - kiirasa*/
        felirat = TTF_RenderUTF8_Blended(font, "+", fekete);
        felirat_t = SDL_CreateTextureFromSurface(custom_renderer, felirat);
        hova.x = (i+0.5)*((custom_ablak_szelessege-ok_gomb_terulete)/kiirando_dolgok_szama)-felirat->w/2;
        hova.y = custom_ablak_magassaga/2-2*betumeret;
        hova.w = felirat->w;
        hova.h = felirat->h;
        SDL_RenderCopy(custom_renderer, felirat_t, NULL, &hova);

        felirat = TTF_RenderUTF8_Blended(font, "-", fekete);
        felirat_t = SDL_CreateTextureFromSurface(custom_renderer, felirat);
        hova.x = (i+0.5)*((custom_ablak_szelessege-ok_gomb_terulete)/kiirando_dolgok_szama)-felirat->w/2;
        hova.y = custom_ablak_magassaga/2+betumeret;
        hova.w = felirat->w;
        hova.h = felirat->h;
        SDL_RenderCopy(custom_renderer, felirat_t, NULL, &hova);



    }
    felirat = TTF_RenderUTF8_Blended(font, "ok", fekete);
    felirat_t = SDL_CreateTextureFromSurface(custom_renderer, felirat);
    hova.x = (custom_ablak_szelessege-ok_gomb_terulete/2)-felirat->w/2;
    hova.y = custom_ablak_magassaga/2-felirat->h/2;
    hova.w = felirat->w;
    hova.h = felirat->h;
    SDL_RenderCopy(custom_renderer, felirat_t, NULL, &hova);

    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);

}


//ha �j rekordot �ll�t fel a j�t�kos, akkor ezt egy �j ablakon ki�rja neki
void uj_rekord_ablak(jatek_adatai jatek)
{
    SDL_Window *uj_rekord_window;
    SDL_Renderer *uj_rekord_renderer;

    ablak_meretei uj_rekord_ablak;
    uj_rekord_ablak.szelesseg=300;
    uj_rekord_ablak.magassag=400;

    sdl_init("Aknakereso",uj_rekord_ablak.szelesseg,uj_rekord_ablak.magassag, &uj_rekord_window, &uj_rekord_renderer);

    boxRGBA(uj_rekord_renderer,0,0,uj_rekord_ablak.szelesseg,uj_rekord_ablak.magassag,185,185,185,255);

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("arial.ttf", 32);
    if (!font)
    {
        SDL_Log("Nem sikerult megnyitni a fontot! %s\n", TTF_GetError());
        exit(1);
    }
    SDL_Color feliratszin = {185, 0, 170};
    SDL_Surface *felirat;
    SDL_Texture *felirat_t;
    SDL_Rect hova = { 0, 0, 0, 0 };

    //uj rekord felirat
    felirat = TTF_RenderUTF8_Blended(font, "New record:", feliratszin);
    felirat_t = SDL_CreateTextureFromSurface(uj_rekord_renderer, felirat);
    hova.x = uj_rekord_ablak.szelesseg/2 - felirat->w/2;
    hova.y = uj_rekord_ablak.magassag*2/5 - felirat->h/2;
    hova.w = felirat->w;
    hova.h = felirat->h;
    SDL_RenderCopy(uj_rekord_renderer, felirat_t, NULL, &hova);

    char rekord[15];
    itoa(jatek.rekord,rekord,10);
    strcat(rekord," seconds");

    // a felirat �jrahasznos�tva csak most maga a rekord m�sodpercekben
    felirat = TTF_RenderUTF8_Blended(font, rekord, feliratszin);
    felirat_t = SDL_CreateTextureFromSurface(uj_rekord_renderer, felirat);
    hova.x = uj_rekord_ablak.szelesseg/2 - felirat->w/2;
    hova.y = uj_rekord_ablak.magassag*3/5 - felirat->h/2;
    hova.w = felirat->w;
    hova.h = felirat->h;
    SDL_RenderCopy(uj_rekord_renderer, felirat_t, NULL, &hova);


    SDL_FreeSurface(felirat);
    SDL_DestroyTexture(felirat_t);

    SDL_RenderPresent(uj_rekord_renderer);

    SDL_Event ev;
    while (SDL_WaitEvent(&ev))
    {
        if(ev.type==SDL_MOUSEBUTTONDOWN||ev.type == SDL_QUIT)
            break;
    }
    SDL_DestroyWindow(uj_rekord_window);


}
