#include "palya_elokeszitese.h"
#include "valtozotipusok.h"
#include "debugmalloc.h"


//ha a j�t�kos custom p�ly�t akar fel�ll�tani akkor annak az adatait ez a f�ggv�ny k�ri be
void custom_beker(jatek_adatai* jatek)
{
    SDL_Window *custom_window;
    SDL_Renderer *custom_renderer;
    sdl_init("adatok",custom_ablak_szelessege,custom_ablak_magassaga, &custom_window, &custom_renderer);

    //max sz=50, m=30 bombak: sz*m-1 (mert azt mondtam)
    int tempek[3]={jatek->palya_szelessege,jatek->palya_magassaga,jatek->bombak_szama};
    int maxok[3]={50,30,tempek[0]*tempek[1]-1};
    bool quit=false;
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
            /*leokezta-e*/
            if(abs(kattintott.x-(custom_ablak_szelessege-ok_gomb_terulete/2))<15 &&abs(kattintott.y-custom_ablak_magassaga/2)<15)
            {
                jatek->bombak_szama=tempek[custom_bombak_szama_index];
                jatek->palya_magassaga=tempek[custom_magassag_index];
                jatek->palya_szelessege=tempek[custom_szelesseg_index];
                quit=true;
            }
            //valami mas jelentosre kattint
            //oof azert ez sem fog szepsegversenyt nyerni gec
            else if(abs((kattintott.x%100)-50)<15&&abs(kattintott.y-custom_ablak_magassaga/2)<betumeret*2)
            {
                int ezt_valtoztatta=kattintott.x/100;

                if(tempek[ezt_valtoztatta]<maxok[ezt_valtoztatta]&&kattintott.y<custom_ablak_magassaga/2-5)//kb 30 tol lesz 45 ig a gomb a masik meg 55 tol 70 ig
                {
                    tempek[ezt_valtoztatta]++;
                }
                else if(tempek[ezt_valtoztatta]>1&&kattintott.y>custom_ablak_magassaga/2+5)
                {
                    tempek[ezt_valtoztatta]--;
                }

            }
            maxok[2]=tempek[0]*tempek[1]-1;
            if(tempek[custom_bombak_szama_index]>maxok[custom_bombak_szama_index])
            {
                tempek[custom_bombak_szama_index]=maxok[custom_bombak_szama_index];
            }
            break;
        case(SDL_QUIT):
            quit=true;
            continue;
            break;
        default:
            break;
        }

        custom_kirajz(tempek,custom_renderer);
        SDL_RenderPresent(custom_renderer);
    }
    SDL_DestroyWindow(custom_window);
}

//k�l�nb�z� neh�zs�gi szintek p�ly�j�nak az adatait k�ri be: vagy txt b�l, vagy a felhaszn�l�t�l
void palya_adatainak_bekerese(jatek_adatai* jatek)
{
    if(jatek->nehezsegi_szint==custom)
    {
        custom_beker(jatek);
    }
    else
    {
        //a custom.txt helyett mejd bekerem az adatokat csak ezt elobb csinaltam mint hogy kitalaltam volna, hogy hogyan akarom bekerni
        char nehezsegi_szintek_nevei[4][17]={"Beginner.txt","Intermediate.txt","Expert.txt","Custom.txt"};
        FILE* inp;
        inp=fopen(nehezsegi_szintek_nevei[jatek->nehezsegi_szint],"r");
        fscanf(inp,"%d",&jatek->palya_szelessege);
        fscanf(inp,"%d",&jatek->palya_magassaga);
        fscanf(inp,"%d",&jatek->bombak_szama);
        if(jatek->nehezsegi_szint!=custom)
        {
            fscanf(inp,"%d",&jatek->rekord);
        }
        fclose(inp);
    }
}

//ha �j rekordot �ll�t fel a j�tkos azt elmenti a program, de csak az el�re fel�ll�tott neh�zs�gi szinteken
void uj_rekord(jatek_adatai jatek)
{
    char nehezsegi_szintek_nevei[3][17]={"Beginner.txt","Intermediate.txt","Expert.txt"};
    FILE* out;
    out=fopen(nehezsegi_szintek_nevei[jatek.nehezsegi_szint],"w");
    fprintf(out,"%d\n",jatek.palya_szelessege);
    fprintf(out,"%d\n",jatek.palya_magassaga);
    fprintf(out,"%d\n",jatek.bombak_szama);
    fprintf(out,"%d\n",jatek.rekord);
    fclose(out);
}

// a p�lya adatainak t�rol�s�hoz a mem�ria lefoglal�sa
void palya_tombjeinek_lefoglalasa(jatek_adatai* jatek)
{
    //az adatokat t�rol� t�mb
    jatek->lefoglal= (palya_tombok*) malloc(jatek->palya_magassaga*jatek->palya_szelessege*sizeof(palya_tombok));

    //a t�rol� t�mb�n bel�lre mutat� pointerek t�mbje -> 2 dimenzi�s t�mb
    jatek->palya = (palya_tombok**) malloc(jatek->palya_magassaga * sizeof(palya_tombok*));

    //pointert�mb elemeinek �rt�kad�sa
    for (int y = 0; y < jatek->palya_magassaga; ++y)
    {
        jatek->palya[y] = (jatek->lefoglal+y*jatek->palya_szelessege);
    }
}

// a p�lya adatainak t�rol�s�hoz haszn�lt mem�ria felszabad�t�sa
void palya_tombjeinek_falszabaditasa(jatek_adatai* jatek)
{
    free(jatek->palya);

    free(jatek->lefoglal);
}

// a p�lya adatainak t�rol�s�hoz a mem�ria felt�lt�se az alaphelyzettel
void ures_palya_feltoltese(jatek_adatai* jatek)
{
    for(int y=0; y<jatek->palya_magassaga; y++)
    {
        for(int x=0; x<jatek->palya_szelessege; x++)
        {
            jatek->palya[y][x].bombak_helyei=false;
            jatek->palya[y][x].lathato_e=false;
            jatek->palya[y][x].zaszlo=false;
        }
    }
}

//a p�ly�n l�v� bomb�kat rakja le
void bombak_generalasa(jatek_adatai* jatek)
{
    for(int i=0; i<jatek->bombak_szama; i++)
    {
        koordinatak random;
        random.x=rand()%jatek->palya_szelessege;
        random.y=rand()%jatek->palya_magassaga;
        if(jatek->palya[random.y][random.x].bombak_helyei)
        {
            i--;
        }
        else
        {
            jatek->palya[random.y][random.x].bombak_helyei=true;
        }
    }
}

//meghat�rozza, hogy mennyi bomba van egy adott mez� k�r�l
int bombak_szama_korulotte(koordinatak k,jatek_adatai* jatek)
{
    int temp_bombak_szama=0;
    for(int i=-1; i<=1; i++)
    {
        if(!(k.y+i>=0 && k.y+i<jatek->palya_magassaga))// a palyarol ne menjunk ki
        {
            continue;
        }
        for(int j=-1; j<=1; j++)
        {
            if(!(k.x+j>=0 && k.x+j<jatek->palya_szelessege))
            {
                continue;
            }
            if(jatek->palya[k.y+i][k.x+j].bombak_helyei)
            {
                temp_bombak_szama++;
            }

        }
    }
    return temp_bombak_szama;
}

//megn�zi, hogy egy adott mez�re milyen sz�mot kell rakni: h�ny bomba van k�r�l�tte (�s azt be is �rja)
void szamok_helyrerakasa(jatek_adatai* jatek)
{
    for(int y=0; y<jatek->palya_magassaga; y++)
    {
        for(int x=0; x<jatek->palya_szelessege; x++)
        {
            koordinatak temp= {x,y};
            jatek->palya[y][x].palya=bombak_szama_korulotte(temp,jatek);
        }
    }
}

