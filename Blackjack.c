#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

int evaluate(const char array[], int size){
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

int inputValidate(int min, int max) {
    int input = -1;
    printf("...");
    scanf("%d", &input);
    while (input < min || input > max){
        printf("\nThat value is invalid. Must use a value between %d and %d...", min, max);
        scanf("%d", &input);
    }
    return input;
}

void hit(const char deck[], char hand[], int* handSize) {
    int r = rand() % 13;
    hand[*handSize] = deck[r];
    *handSize += 1;
}

void displayHands(char dealersHand[], char yourHand[], int dealersHandSize, int yourHandSize) {
    printf("\nDealer's Hand: %s  (Value: %d)\tYour Hand:%s  (Value: %d)", dealersHand,
           evaluate(dealersHand, dealersHandSize), yourHand, evaluate(yourHand, yourHandSize));
}

void whoWon(char dealersHand[], char yourHand[], int dealersHandSize, int yourHandSize, int* chips, int* wager) {
    int handValue = evaluate(yourHand, yourHandSize);
    int dealersHandValue = evaluate(dealersHand, dealersHandSize);

    if (dealersHandValue > 21) {                      // WINNER EVALUATION
        printf("\nDealer has busted!!!");
        *chips += 2 * *wager;
    } else if (dealersHandValue > handValue) {
        printf("\nThe house always wins...");
    } else if (dealersHandValue < handValue) {
        printf("\nYou win!!");
        *chips += 2 * *wager;
    } else if (dealersHandValue == handValue) {
        printf("\nPush!!");
        *chips += *wager;
    }
}

bool insurance(const char deck[], char hand[], int *handSize, int *chips, const int *wager, bool *dealerBlackjack){
    bool insurance = inputValidate(0, 1);
    int insuranceBet = (*wager/2);
    printf("\nDealer is showing an Ace! Would you like to purchase insurance for %d?", insuranceBet);

    if(insurance)                                                            // insurance Bet deducted from chips
        *chips -= insuranceBet;

    printf("\nDealer is checking his hidden card...");                // Dealer shows if blackjack
    int r = rand() % 13;
    if(r > 8){
        printf("\nDealer has blackjack!");
        *dealerBlackjack = 1;
        hand[1] = deck[r];
        *handSize += 1;
        if(insurance){
            *chips += (insuranceBet * 3);
        }
        return 1;
    } else {
        printf("\nDealer does not have blackjack! Play continues...");
        return 0;
    }
}

int main(){
    const char deck[] = "A23456789TJQK";                // deck initialization
    printf("\nWelcome to BlackJack in C");
    printf("\nOn your action you may:\n1. Hit    2. Stand    3. Double Down    4. Split    5. Quit");

    bool gameOver = 0;
    int chips = 1000;

    while(!gameOver) {           // Main game loop

        bool handOver = 0, busted = 0, dealerBlackjack = 0, blackjack = 0;      //variable initialization
        int yourHandSize = 2, dealersHandSize = 1, wager, choice;
        char yourHand[10] = "", dealersHand[10] = "";

        srand(time(NULL));
        int r = rand() % 13;            //hand initialization
        yourHand[0] = deck[r];
        r = rand() % 13;
        yourHand[1] = deck[r];
        r = rand() % 13;
        dealersHand[0] = deck[r];

        printf("\nYou now have %d chips...\nNew Hand!!\tWhat is your wager? ", chips);
        wager = inputValidate(2, chips);
        chips -= wager;
        printf("You now have %d chips", chips);
        displayHands(dealersHand, yourHand, dealersHandSize, yourHandSize);

        if (evaluate(yourHand, yourHandSize) == 21) {   // PLAYER BLACKJACK??
            printf("\nBlackJack!!! Payout is 3/2! You've won %d off your %d wager!", ((3 * wager)/2), wager);
            chips += (5 * wager)/2;
            handOver = 1, blackjack = 1;
        }

        if (dealersHand[0] == 'A' && !blackjack) {          // INSURANCE???
            handOver = insurance(deck, dealersHand, &dealersHandSize, &chips, &wager, &dealerBlackjack);
            displayHands(dealersHand, yourHand, dealersHandSize, yourHandSize);
        }

        while (!handOver) {            // Hand loop, ends when player's hand is complete or busted
            choice = inputValidate(1, 5);
            switch (choice) {
                case 1:                     // HIT!
                    hit(deck, yourHand, &yourHandSize);
                    displayHands(dealersHand, yourHand, dealersHandSize, yourHandSize);
                    if (evaluate(yourHand, yourHandSize) > 20)
                        handOver = 1;
                    break;
                case 2:                     // STAND!
                    printf("StAnD dOwN");
                    handOver = 1;
                    break;
                case 3:                     // DOUBLE DOWN!
                    hit(deck, yourHand, &yourHandSize);
                    displayHands(dealersHand, yourHand, dealersHandSize, yourHandSize);
                    chips -= wager;
                    wager *= 2;
                    handOver = 1;
                    break;
                case 4:                     // SPLIT!
                    printf("\nSplat");
                    break;
                case 5:                     // QUIT!
                    printf("\nThanks for playing!");
                    handOver = 1, gameOver = 1;
                    break;
                default:                     // UNUSED DEFAULT
                    printf("\nWhoops don't press that");
                    break;
            }
            if (evaluate(yourHand, yourHandSize) > 21) {
                printf("\tBUSTED!!");
                handOver = 1, busted = 1;
            }
        }

        if(!busted && !dealerBlackjack && !blackjack) {                            // DEALER'S HAND LOOP
            while (evaluate(dealersHand, dealersHandSize) < 17) {
                hit(deck, dealersHand, &dealersHandSize);
                displayHands(dealersHand, yourHand, dealersHandSize, yourHandSize);
            }
            whoWon(dealersHand, yourHand, dealersHandSize, yourHandSize, &chips, &wager);
        }

        if (chips <= 1){
            printf("\nYou now have %d chips", chips);
            printf("\nYou lost all your chips!\nBetter luck next time!");
            gameOver = 1;
        }
    }
    return 0;
}
