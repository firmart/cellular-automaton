#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL/SDL.h>

#include "sq_List.h"
#include "cons.h"
#include "calc.h"


void Blit_map(SDL_Surface *screen, SDL_Surface *color[], int map[][NB_BLOCK_HEIGHT]);
int List_exist (SqList *l, char c);
void copy (int map[][NB_BLOCK_HEIGHT], int dest_map[][NB_BLOCK_HEIGHT]);
void map_init(int map[][NB_BLOCK_HEIGHT], int d);
void pop_copy(int pop[], int map[][NB_BLOCK_HEIGHT]);
void Blit_pop(SDL_Surface *screen, int pop[], SDL_Surface *v);
int pop_min(int pop[]);
int pop_max(int pop[]);



int main ( int argc, char **argv ) {


#ifdef _WIN32
    freopen("CON", "w", stdout);
    freopen("CON", "r", stdin);
    freopen("CON", "w", stderr);
#endif // _WIN32


    //Declaration
    SDL_Surface *screen = NULL;
    SDL_Rect position;
    SDL_Event event;
    //Declarations of counter
    int i, j;

    //rand init
    srand(time(NULL));

    //Windows init
    SDL_Init(SDL_INIT_VIDEO);
#if PRINT_POP==1
    screen = SDL_SetVideoMode(WINDOWS_WIDTH, WINDOWS_HEIGHT + POP_STAT_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
#else
    screen = SDL_SetVideoMode(WINDOWS_WIDTH, WINDOWS_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
#endif
    SDL_WM_SetCaption("Cellular Automaton", NULL);
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
    SDL_Flip(screen);


    SDL_Surface *color[12] = {NULL};
    color[WHITE] = SDL_CreateRGBSurface(SDL_HWSURFACE, BLOCK_SIZE, BLOCK_SIZE, 32, 0, 0, 0, 0);
    SDL_FillRect(color[WHITE], NULL, SDL_MapRGB(color[WHITE]->format, 255, 255, 255));
    //Surface

    for (i = 0; i < BLUE11; i++) {
        color[i + 1] = SDL_CreateRGBSurface(SDL_HWSURFACE, BLOCK_SIZE, BLOCK_SIZE, 32, 0, 0, 0, 0);
        SDL_FillRect(color[i + 1], NULL, SDL_MapRGB(color[i + 1]->format, \
                     DEGR_SRC_COLOR_R - i * ((DEGR_SRC_COLOR_R - DEGR_DEST_COLOR_R) / 10), \
                     DEGR_SRC_COLOR_G - i * ((DEGR_SRC_COLOR_G - DEGR_DEST_COLOR_G) / 10), \
                     DEGR_SRC_COLOR_B - i * ((DEGR_SRC_COLOR_B - DEGR_DEST_COLOR_B) / 10)));
    }//degrade

    //map init
    SDL_EnableKeyRepeat(5, 100);
    int map [NB_BLOCK_WIDTH][NB_BLOCK_HEIGHT] = {{0, 0}}, pre_map [NB_BLOCK_WIDTH][NB_BLOCK_HEIGHT] = {{0, 0}};
    //map_init(map,2);
    /*
    map[10][10]=RED1;
    map[11][11]=RED1;
    map[11][13]=RED1;
    map[12][12]=RED1;
    map[9][11]=RED1;
    */
    map_init(map, 50);

#if PRINT_POP==1
    int pop[NB_BLOCK_WIDTH] = {0};
#endif //PRINT_POP

    SDL_Surface *value = NULL;
    value = SDL_CreateRGBSurface(SDL_HWSURFACE, BLOCK_SIZE, BLOCK_SIZE, 32, 0, 0, 0, 0);
    SDL_FillRect(value, NULL, SDL_MapRGB(value->format, 0, 0, 0));

#if PRINT_POP==1
    Blit_pop(screen, pop, value);
#endif //PRINT_POP

    position.x = 0;
    position.y = WINDOWS_HEIGHT;

#if PRINT_POP==1
    SDL_Surface *pop_blank = NULL;
    pop_blank = SDL_CreateRGBSurface(SDL_HWSURFACE, WINDOWS_WIDTH, POP_STAT_HEIGHT, 32, 0, 0, 0, 0);
    SDL_FillRect(pop_blank, NULL, SDL_MapRGB(pop_blank->format, 255, 255, 255));
#endif //PRINT_POP



    Blit_map(screen, color, map);
    SqList src, born, surv;
    Init_SqList(&src);
    Init_SqList(&born);
    Init_SqList(&surv);
    FILE *fp = fopen("rules.txt", "r");
    fgets(src.elem, 18, fp);
    i = 0;
    j = 1;

    while (src.elem[i] != '\\' && src.elem[i] != '/') {
        if (!List_exist(&born, src.elem[i])) {
            ListInsert(&born, j++, src.elem[i]);
        }

        i++;
    }

    j = 1;
    i++;

    while (src.elem[i] != '\n' && src.elem[i] != ' ' && i < 18) {
        if (!List_exist(&surv, src.elem[i])) {
            ListInsert(&surv, j++, src.elem[i]);
        }

        i++;
    }

    fclose(fp);

    int continuer = 1;

    while (continuer) {
        SDL_WaitEvent(&event);

        switch (event.type) {
            case SDL_QUIT:
                continuer = 0;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        continuer = 0;
                        break;

                    case SDLK_p:
                        rule_apply(map, pre_map, born, surv);
                        Blit_map(screen, color, pre_map);
#if PRINT_POP==1
                        pop_copy(pop, map);
                        SDL_BlitSurface(pop_blank, NULL, screen, &position);
#endif //PRINT_POP
                        SDL_Flip(screen);
#if PRINT_POP==1
                        Blit_pop(screen, pop, value);
#endif //PRINT_POP
                        copy(pre_map, map);
                        break;

                    default:
                        break;
                }

            default:
                break;
        }
    }

    for (i = 0; i < 12; i++) {
        SDL_FreeSurface(color[i]);
    }

    SDL_Quit();
    return 0;
}



void Blit_map(SDL_Surface *screen, SDL_Surface *color[], int map[][NB_BLOCK_HEIGHT]) {
    int i, j;
    SDL_Rect position;

    for (i = 0; i < NB_BLOCK_WIDTH; i++) {
        for (j = 0; j < NB_BLOCK_HEIGHT; j++) {
            position.x = i * BLOCK_SIZE;
            position.y = j * BLOCK_SIZE;

            switch (map[i][j]) {
                case WHITE:
                    SDL_BlitSurface(color[WHITE], NULL, screen, &position);
                    break;

                case RED1:
                    SDL_BlitSurface(color[RED1], NULL, screen, &position);
                    break;

                case RED2:
                    SDL_BlitSurface(color[RED2], NULL, screen, &position);
                    break;

                case RED3:
                    SDL_BlitSurface(color[RED3], NULL, screen, &position);
                    break;

                case RED4:
                    SDL_BlitSurface(color[RED4], NULL, screen, &position);
                    break;

                case BR5:
                    SDL_BlitSurface(color[BR5], NULL, screen, &position);
                    break;

                case BR6:
                    SDL_BlitSurface(color[BR6], NULL, screen, &position);
                    break;

                case BR7:
                    SDL_BlitSurface(color[BR7], NULL, screen, &position);
                    break;

                case BR8:
                    SDL_BlitSurface(color[BR8], NULL, screen, &position);
                    break;

                case BLUE9:
                    SDL_BlitSurface(color[BLUE9], NULL, screen, &position);
                    break;

                case BLUE10:
                    SDL_BlitSurface(color[BLUE10], NULL, screen, &position);
                    break;

                case BLUE11:
                    SDL_BlitSurface(color[BLUE11], NULL, screen, &position);
                    break;

                default :
                    SDL_BlitSurface(color[WHITE], NULL, screen, &position);
                    break;
            }
        }
    }

    SDL_Flip(screen);
}



void copy (int map[][NB_BLOCK_HEIGHT], int dest_map[][NB_BLOCK_HEIGHT]) {
    int i, j;

    for (i = 0; i < NB_BLOCK_WIDTH; i++) {
        for (j = 0; j < NB_BLOCK_HEIGHT; j++) {
            dest_map[i][j] = map[i][j];
        }
    }
}

void map_init(int map[][NB_BLOCK_HEIGHT], int d) {
    int i, j;

    for (i = 0; i < NB_BLOCK_WIDTH; i++) {
        for (j = 0; j < NB_BLOCK_HEIGHT; j++) {
            map[i][j] = rand() % d;

            if (map[i][j] > RED1) {
                map[i][j] = WHITE;
            }
        }
    }
}

#if PRINT_POP==1

void Blit_pop(SDL_Surface *screen, int pop[], SDL_Surface *v) {
    SDL_Rect position;
    int i, min = pop_min(pop), max = pop_max(pop);
    int h = 0;

    for (i = 0; i < NB_BLOCK_WIDTH; i++) {
        //printf("%d\n",h);
        h = (max != 0 && max != min) ? (pop[i] * POP_STAT_HEIGHT) / max : POP_STAT_HEIGHT / 2;
        position.y = (WINDOWS_HEIGHT + POP_STAT_HEIGHT - 1) - h;
        position.x = i * BLOCK_SIZE;
        SDL_BlitSurface(v, NULL, screen, &position);
    }

    SDL_Flip(screen);
}

int pop_min(int pop[]) {
    int i, min = pop[0];

    for (i = 0; i < NB_BLOCK_WIDTH; i++) {
        if (pop[i] < min) {
            min = pop[i];
        }
    }

    return min;
}

int pop_max(int pop[]) {
    int i, max = pop[0];

    for (i = 0; i < NB_BLOCK_WIDTH; i++) {
        if (pop[i] > max) {
            max = pop[i];
        }
    }

    return max;
}

void pop_copy(int pop[], int map[][NB_BLOCK_HEIGHT]) {
    int i;

    for (i = 0; i < NB_BLOCK_WIDTH - 1; i++) {
        pop[i] = pop[i + 1];
    }

    pop[NB_BLOCK_WIDTH - 1] = population(map);
}

#endif //PRINT_POP
