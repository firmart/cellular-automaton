#ifndef CALC_H
#define CLAC_H
int count_neighbor(int map[][NB_BLOCK_HEIGHT], int x, int y) ;
void rule_apply(int map[][NB_BLOCK_HEIGHT],int pre_map[][NB_BLOCK_HEIGHT],SqList born, SqList surv);
int List_exist (SqList* l, char c);
int population(int map[][NB_BLOCK_HEIGHT]);
#endif //CALC_H
