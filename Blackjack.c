#include<stdio.h>
#include <time.h> //time function
#include <stdlib.h> //random number generator functions
#include <string.h>
#include <stdbool.h>

#define MAX 9
#define MAX_CARDS 52
#define MAX_RANKS 13
#define MAX_SUITS 4
#define COLS 3 //number of columns to display in output

struct card{
    char *rank;
    char suit[MAX];
};
typedef struct card Card;

char *ranks[MAX_RANKS] = {"Ace", "Two", "Three", "Four", "Five", "Six", "Seven",
                          "Eight", "Nine", "Ten", "Jack", "Queen", "King"};
char suits[MAX_SUITS][MAX] = {"Clubs", "Diamonds", "Hearts", "Spades"};

void initialize(Card []);
void shuffle(Card []);

int main(){

    Card deck[MAX_CARDS] = {"",""};
    initialize(deck);
    shuffle(deck);
    printf("Welcome to BlackJack in C\n");
    bool handOver, gameOver = 0;

    //while(!gameOver) {
    while (!handOver) {
        int choice;
        printf("Your action! You may...\n1. Hit    2. Stand    3. Double Down    4. Split\nWhat is your choice? ");
        scanf("%d", &choice);


        switch (choice) {
            case 1:
                printf("%5s of %-12s\n", deck[0].rank, deck[0].suit);
                break;
            case 2:
                printf("StAnD dOwN\n");
                break;
            case 3:
                printf("%5s of %-12s DOUBLED!!\n", deck[0].rank, deck[0].suit);
                break;
            case 4:
                printf("Splat\n");
                break;
            default:
                printf("Whoops don't press that\n");
                break;
        }
    }
    //}

    return 0;
}


void initialize(Card deck[]){
    int i = 0;
    for(i=0; i<MAX_CARDS; i++){
        deck[i].rank = ranks[i%MAX_RANKS];
        strncpy(deck[i].suit, suits[i/MAX_RANKS], MAX);
    }
}


void shuffle(Card deck[]){
    printf("\nshuffling deck ... \n");
    int swapper = 0; //index of card to be swapped
    int i = 0; //counter
    Card temp = {"", ""}; //temp holding place for swap
    srand(time(NULL)); //seed the random numbers with current time
    for(i=0;i<MAX_CARDS;i++){
        //generate a pseudo-random number from 0 to 51
        swapper = rand() % MAX_CARDS;
        //swap current card with da swapper
        temp = deck[i];
        deck[i] = deck[swapper];
        deck[swapper] = temp;
    }
}
