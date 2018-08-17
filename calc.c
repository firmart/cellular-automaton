#include "cons.h"
#include "sq_List.h"

int count_neighbor(int map[][NB_BLOCK_HEIGHT], int x, int y) {
    int counter = 0, i, j, k;
    int neighbor[3][3] = {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}};

    //0 mean that the cell won't be count
    if (x == 0) for (k = 0; k < 3; k++) {
            neighbor[0][k] = 0;
        }

    if (y == 0) for (k = 0; k < 3; k++) {
            neighbor[k][0] = 0;
        }

    if (x == NB_BLOCK_WIDTH - 1) for (k = 0; k < 3; k++) {
            neighbor[2][k] = 0;
        }

    if (y == NB_BLOCK_HEIGHT - 1) for (k = 0; k < 3; k++) {
            neighbor[k][2] = 0;
        }

    for (i = x - 1; i <= x + 1; i++) {
        for (j = y - 1; j <= y + 1; j++) {
            if (neighbor[i - x + 1][j - y + 1] && map[i][j] == RED1) {
                counter++;
            }
        }
    }

    return counter;
}

int List_exist (SqList *l, char c) {
    int i;

    for (i = 0; i < l->length; i++) if (c == l->elem[i]) {
            return 1;
        }

    return 0;
}

void rule_apply(int map[][NB_BLOCK_HEIGHT], int pre_map[][NB_BLOCK_HEIGHT], SqList born, SqList surv) {
    int i, j, h;
    int flag = 1;

    for (i = 0; i < NB_BLOCK_WIDTH; i++) {
        for (j = 0; j < NB_BLOCK_WIDTH; j++) {
            flag = 1;

            for (h = 0; h < born.length && flag; h++) {
                if (count_neighbor(map, i, j) == ((int)born.elem[h] - 48) && map[i][j] != RED1) {
                    pre_map[i][j] = RED1;
                    flag = 0;
                }
            }

            for (h = 0; h < surv.length && flag; h++) {
                if (count_neighbor(map, i, j) == ((int)surv.elem[h] - 48) && map[i][j] == RED1) {
                    pre_map[i][j] = RED1;
                    flag = 0;
                }
            }

            if (flag) {
                switch (map[i][j]) {
#if DEGR == 1

                    case RED1:
                        pre_map[i][j] = RED2;
                        break;

                    case RED2:
                        pre_map[i][j] = RED3;
                        break;

                    case RED3:
                        pre_map[i][j] = RED4;
                        break;

                    case RED4:
                        pre_map[i][j] = BR5;
                        break;

                    case BR5:
                        pre_map[i][j] = BR6;
                        break;

                    case BR6:
                        pre_map[i][j] = BR7;
                        break;

                    case BR7:
                        pre_map[i][j] = BR8;
                        break;

                    case BR8:
                        pre_map[i][j] = BLUE9;
                        break;

                    case BLUE9:
                        pre_map[i][j] = BLUE10;
                        break;

                    case BLUE10:
                        pre_map[i][j] = BLUE11;
                        break;
#endif

                    default:
                        pre_map[i][j] = WHITE;
                }
            }
        }
    }
}

int population(int map[][NB_BLOCK_WIDTH]) {
    int i, j, pop = 0;

    for (i = 0; i < NB_BLOCK_HEIGHT; i++) {
        for (j = 0; j < NB_BLOCK_WIDTH; j++) {
            if (map[i][j] == RED1) {
                pop++;
            }
        }
    }

    return pop;
}
