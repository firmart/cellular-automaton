//#ifndef SQ_LIST_H
//#define SQ_LIST_H

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
typedef int Status;

#define LIST_INIT_SIZE 50
#define LIST_INCREMENT 1

typedef char ElemType;
typedef struct {
    ElemType* elem;
    int length;
    int listsize;
} SqList;


void Init_SqList(SqList* L) ;
void DestroyList(SqList* L);
void ClearList(SqList* L) ;
Status ListEmpty(SqList L);
int ListLength(SqList L) ;
Status GetElem(SqList L, int i, ElemType* e) ;
int LocateElem(SqList L, ElemType e, int compare(const ElemType,const ElemType));
Status PriorElem(SqList L, int cur_e, ElemType* e);
Status NextElem(SqList L, int cur_e, ElemType* e);
Status ListInsert (SqList* L, int i, ElemType e);
Status ListDelete (SqList* L, int i, ElemType* e);
void ListIter(SqList L, void fun(ElemType));
void ListUnion(SqList* L1, SqList L2);
void ListMergeAsc(SqList L1,SqList L2,SqList* L3);
void ListUnionAsc(SqList L1,SqList L2,SqList* L3);

//#endif //SQ_LIST_H
