#ifndef CONS_H
#define CONS_H

#define BLOCK_SIZE 2
#define NB_BLOCK_HEIGHT 250
#define NB_BLOCK_WIDTH 250
#define WINDOWS_HEIGHT BLOCK_SIZE * NB_BLOCK_HEIGHT
#define WINDOWS_WIDTH BLOCK_SIZE * NB_BLOCK_WIDTH

#define PRINT_POP 0

#if PRINT_POP==1

#define POP_STAT_HEIGHT WINDOWS_HEIGHT/8

#endif // PRINT_POP


enum { WHITE, RED1, RED2, RED3, RED4, BR5, BR6, BR7,BR8, BLUE9, BLUE10, BLUE11};
#define DEGR 0
#define DEGR_SRC_COLOR_R 230
#define DEGR_SRC_COLOR_G 30
#define DEGR_SRC_COLOR_B 0

#define DEGR_DEST_COLOR_R 0
#define DEGR_DEST_COLOR_G 230
#define DEGR_DEST_COLOR_B 240

#endif //CONS_H