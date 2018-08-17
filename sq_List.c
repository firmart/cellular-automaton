#include <stdio.h>
#include <stdlib.h>
#include "sq_List.h"

static int equality(const ElemType a,const ElemType b);
/*
* (+) random access
* (-) insert/delete element require to move a lot of elements.
*
*/

void Init_SqList(SqList* L) {
    L->elem = (ElemType*)malloc(LIST_INIT_SIZE * sizeof(ElemType));
    if(!L->elem) exit(OVERFLOW);
    L->length = 0;
    L->listsize = LIST_INIT_SIZE;
}

void DestroyList(SqList* L) {
    free(L->elem);
    L->elem = NULL;
    L->length = 0;
    L->listsize = 0;
}

void ClearList(SqList* L) {
    L->length = 0;
}

Status ListEmpty(SqList L) {
    if(L.length == 0) return TRUE;
    else return FALSE;
}

int ListLength(SqList L) {
    return L.length;
}

Status GetElem(SqList L, int i, ElemType* e) {
    if(i<1 || i > L.length) return(ERROR);
    *e = L.elem[i-1];
    return (OK);
}

int LocateElem(SqList L, ElemType e, int compare(const ElemType,const ElemType)) {
    int i;
    for(i=0; i < L.length; i++)  if(compare(L.elem[i],e)) return i+1;
    return 0;
}

Status PriorElem(SqList L, int cur_e, ElemType* e){
	int i;
	for(i=1; i < L.length; i++) {
		if (L.elem[i] == cur_e) {
			*e = L.elem[i-1];
			return OK;
		}
	}
	return INFEASIBLE;
}

Status NextElem(SqList L, int cur_e, ElemType* e){
	int i;
	for(i=0; i < L.length-1; i++) {
		if (L.elem[i] == cur_e) {
			*e = L.elem[i+1];
			return OK;
		}
	}
	return INFEASIBLE;
}

Status ListInsert (SqList* L, int i, ElemType e){
	ElemType* newbase;
	int j;
	if(i<1 || i > L->length+1) return(ERROR);
	if (L->length >= L->listsize){
		newbase = (ElemType*) realloc(L->elem,(L->listsize+LIST_INCREMENT)*sizeof(ElemType));
		if (!newbase) exit(OVERFLOW);
		L->elem = newbase;
		L->listsize += LIST_INCREMENT;
	}
	for (j=L->length;j>=i-1;j--) L->elem[j+1] = L->elem[j];
	L->elem[i-1] = e;
	L->length++;
	return OK;
}

Status ListDelete (SqList* L, int i, ElemType* e){
	int j;
	if(i<1 || i > L->length) return(ERROR);
	*e = L->elem[i-1];
	for(j=i-1;j < L->length;j++) L->elem[j] = L->elem[j+1];
	L->length--;
	return OK;
}

void ListIter(SqList L, void fun(ElemType)){
	int i;
	for(i=0;i<L.length;i++) fun(L.elem[i]);
}


/** \brief Give the union of 2 unsorted sequences lists
 *
 * \param Sequence List 1
 * \param Sequence List 2
 * \return L1 = L1 Union L2
 * Time Complexity O(L1_length * L2_length)
 * It's suggested to sort the lists( eg. with qsort O(nlogn)), and use the ListUnionAsc function.
 */
void ListUnion(SqList* L1, SqList L2){
    int i;
    ElemType e;
    for (i=1;i <= L2.length;i++){
        GetElem(L2,i,&e);
        if(!LocateElem(*L1,e,equality)) ListInsert(L1,L1->length+1,e);
    }
}

static int equality(const ElemType a,const ElemType b){
    return a==b;
}
/** \brief Give the merge list of 2 ascending sorted sequences lists
 *
 * \param Ascending sorted Sequence List 1
 * \param Ascending sorted Sequence List 2
 * \return L3 the merge List with an ascending sort
 * Time Complexity O(L1_length + L2_length).
 *
 */

void ListMergeAsc(SqList L1,SqList L2,SqList* L3){
    int i=0,j=0,k=0;
    L3->listsize = L3->length = L1.length + L2.length;
    L3->elem = (ElemType*)malloc(L3->listsize * sizeof(ElemType));
    if (!L3->elem) exit(OVERFLOW);
    while(i <= L1.length-1 && j <= L2.length-1){
        if (L1.elem[i] <= L2.elem[j]) L3->elem[k++] = L1.elem[i++];
        else L3->elem[k++] = L2.elem[j++];
    }
    while (i <= L1.length-1) L3->elem[k++] = L1.elem[i++];
    while (j <= L2.length-1) L3->elem[k++] = L2.elem[j++];
}

/** \brief Give the union of 2 ascending sorted sequences lists
 *
 * \param Ascending sorted Sequence List 1
 * \param Ascending sorted Sequence List 2
 * \return L3 = L1 Union L2
 * Time Complexity O(L1_length + L2_length)
 */

void ListUnionAsc(SqList L1,SqList L2,SqList* L3){
    int i=0,j=0,k=0;
    L3->listsize = L3->length = L1.length + L2.length;
    L3->elem = (ElemType*)malloc(L3->listsize * sizeof(ElemType));
    if (!L3->elem) exit(OVERFLOW);
    while(i <= L1.length-1 && j <= L2.length-1){
        if (L1.elem[i] < L2.elem[j]) L3->elem[k++] = L1.elem[i++];
        else if (L1.elem[i] == L2.elem[j]) {
            L3->elem[k++] = L1.elem[i++];
            j++;
            L3->length--;
        }
        else L3->elem[k++] = L2.elem[j++];

    }
    while (i <= L1.length-1) L3->elem[k++] = L1.elem[i++];
    while (j <= L2.length-1) L3->elem[k++] = L2.elem[j++];
}
