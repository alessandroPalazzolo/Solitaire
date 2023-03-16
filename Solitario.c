#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#define L 50
#define N 40

typedef struct link{
    char value[N/2];
    struct link* next;
    struct link* prev;
} card;

static card* head = NULL;
static card* tail = NULL;

int fileRead(FILE*,char**);
void deckRand(char**);
void llist(char**);
void showGame(char*[][4],int);
void useCard(int*,char*[][4],card*);
void player(card*,char*[][4],int*,int*);
void leftPlayer(card*,char*[][4],int*);

//MAIN

int main(){
    //start file reading
    char* deck[N];
    FILE* fp = fopen("cards.txt","r");
    if (fileRead(fp,deck)!=40){
        fprintf(stderr,"error reading deck!\n");
        exit(EXIT_FAILURE);
    }
    fclose(fp);
    //end file reading
    
    srand((unsigned int)time(NULL));
    
    char* game[2][4] = {{"/♤","/♧","/♡","/♢"},
                        {"/♤","/♧","/♡","/♢"}};
    
    deckRand(deck);
    card* target = NULL;
    card* remaining = NULL;
    int nelDeck = N, i=1, j;
    int usedCards;
    int spottedCards;
    
    while (i) {
        i=0;
        usedCards=0;
        spottedCards=0;
        printf("\n- DECK START (%d CARDS) -\n",nelDeck);
        showGame(game,2);
        target = head;
        remaining = tail;
        player(target,game,&usedCards,&spottedCards);
        int nelLeft = nelDeck-spottedCards;
        for (j=0; j<nelLeft; j++) {
            printf("\ncard: %s\n",remaining->value);
            leftPlayer(remaining,game,&usedCards);
            remaining = remaining->prev;
        }
        nelDeck-=usedCards;
        i=usedCards;
    }
    
    printf("\nEnd of Game (%d cards remaining):\n",nelDeck);
    showGame(game,2);
    printf("\n");
}

//FILEREAD

int fileRead(FILE* pt,char** arr){
    int k=0;

    if (pt==NULL) {
        fprintf(stderr,"error n.%d: %s\n",errno,strerror(errno));
        exit(EXIT_FAILURE);
    }
    while (!feof(pt)) {
        arr[k] = (char*)malloc(N/2);
        fscanf(pt,"%s",arr[k]);
        k++;
    }

    return(k-1);
}

//DECKRAND

void deckRand(char** arr){
    int i;
    char temp[N/2];
    
    for (i=0; i<N; i++) {
        int x = rand()%39;
        if(x==i)
            continue;
        strcpy(temp,arr[i]);
        free(arr[i]);
        arr[i] = (char*)malloc(sizeof(arr[x]));
        strcpy(arr[i],arr[x]);
        free(arr[x]);
        arr[x] = (char*)malloc(sizeof(temp));
        strcpy(arr[x],temp);
    }
    
    llist(arr);
}

//LLIST

void llist(char** arr){
    int i;
    for (i=0; i<N; i++) {
        card* new = NULL;
        new = (card*)malloc(sizeof(card));
        strcpy(new->value,arr[i]);
        free(arr[i]);
        if (head==NULL)
            head = new;
        else {
            tail->next = new;
            new->prev = tail;
        }
        tail = new;
    }
    // card* print = head;
    // for (i=0; i<N; i++) {
    //     printf("%s\n",print->value);
    //     print = print->next;
    // }
}

//SHOWGAME

void showGame(char* bidArr[][4],int nrows){
    printf("\n        1    2    3    4\n");
    printf("\n 1   | %s | %s | %s | %s |\n",bidArr[0][0],bidArr[0][1],bidArr[0][2],bidArr[0][3]);
    printf("     ---------------------\n");
    printf(" 2   | %s | %s | %s | %s |\n",bidArr[1][0],bidArr[1][1],bidArr[1][2],bidArr[1][3]);
}

//USECARD

void useCard(int* coord,char* matrix[][4],card* x){
    matrix[coord[0]-1][coord[1]-1] = (char*)malloc(sizeof(x->value));
    strcpy(matrix[coord[0]-1][coord[1]-1],x->value);
}

//PLAYER

void player(card* target,char* game[][4],int* uCards,int* sCards){
    card* basecard = NULL;
    int choice;
    int coord[2];
    
    while (target!=tail&&target!=tail->prev&&basecard!=tail){
        basecard = target->next->next;
        target = basecard;
        
        printf("\ncard: %s\n",basecard->value);
        printf("\nuse(1)...continue(2): ");
        scanf("%d",&choice);
        
        while (choice==1) {
            printf("coordinates (ex. 3 1 for 3rd column - 1st row):");
            scanf("%d%d",&coord[0],&coord[1]);
            
            useCard(coord,game,target);
            (*uCards)++;
            
            if (target->next!=NULL&&target!=head)
                target->next->prev = target->prev;
            if (target->prev!=NULL&&target!=tail)
                target->prev->next = target->next;
            
            if (target==tail){
                target->prev->next = NULL;
                tail = tail->prev;
                basecard = tail;
            }
            if (target==head) {
                target->next->prev = NULL;
                head = head->next;
            }
            
            showGame(game,2);
            
            if (target->prev!=NULL)
                target = target->prev;
                
            printf("\nsub card: %s\n",target->value);
            printf("\nuse(1)...continue(2): ");
            scanf("%d",&choice);
        }
        
        target = basecard->next;
        (*sCards)+=3;
    }
}

//LEFTPLAYER

void leftPlayer(card* target,char* game[][4],int* uCards){
    int choice;
    printf("\nuse(1)...continue(2): ");
    scanf("%d",&choice);
    if (choice==1) {
        int coord[2];
        printf("coordinates:");
        scanf("%d%d",&coord[0],&coord[1]);
        useCard(coord,game,target);
        (*uCards)++;
        
        if (target->next!=NULL&&target!=head)
            target->next->prev = target->prev;
        if (target->prev!=NULL&&target!=tail)
            target->prev->next = target->next;
        
        if (target==tail){
            target->prev->next = NULL;
            tail = tail->prev;
        }
        if (target==head) {
            target->next->prev = NULL;
            head = head->next;
        }
        
        showGame(game,2);
    }
}
