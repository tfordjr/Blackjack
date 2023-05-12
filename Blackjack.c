#include<stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

int evaluate (char array[], int size){
    int value = 0;
    bool aceFlag = 0;
    for (int i = 0; i < size; i++) {
        if (array[i]== 'K' || array[i]== 'Q' || array[i]== 'J' || array[i]== 'T' ){     // TEN CASE
            value += 10;
        } else if(array[i]== 'A'){          // ACE CASE
                aceFlag = 1;
                value++;
        } else {                // Number Case
            value += (array[i] - 48);
        }
    }
    if (aceFlag == 1 && value < 12){
        value += 10;
    }

    return value;
}

int main(){
    char deck[] = "A23456789TJQK";
    printf("Welcome to BlackJack in C\n");
    printf("On your action you may...\n1. Hit    2. Stand    3. Double Down    4. Split\n");



    bool handOver = 0;      //variable initialization
    int handSize = 2, handValue = 0;
    char dealersHand[10] = "";
    char yourHand[10] = "";

    srand(time(NULL));
    int r = rand() % 13;        //hand initialization
    yourHand[0] = deck[r];
    r = rand() % 13;
    yourHand[1] = deck[r];
    r = rand() % 13;
    dealersHand[0] = deck[r];
    handValue = evaluate(yourHand, handSize);

    printf("Dealer's Hand: %s\tYour Hand:%s\t\tHand Value: %d\t",dealersHand, yourHand, handValue);

    while (!handOver) {
        int choice;
        printf("...");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                r = rand() % 13;
                yourHand[handSize] = deck[r];
                handSize++;
                handValue = evaluate(yourHand, handSize);
                printf("Dealer's Hand: %s\tYour Hand:%s\t\tHand Value: %d\t",dealersHand, yourHand, handValue);
                if(handValue>20)
                    handOver = 1;
                break;
            case 2:
                printf("StAnD dOwN\n");
                handOver = 1;
                break;
            case 3:
                r = rand() % 13;
                printf("Your Hand:%c DOUBLED!!!!", deck[r]);

                break;
            case 4:
                printf("Splat\n");
                break;
            default:
                printf("Whoops don't press that\n");
                break;
        }
    }

    if (handValue>21) {
        printf("BUSTED!");
    } //else {

    //}




    return 0;
}
