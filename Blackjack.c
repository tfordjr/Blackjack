#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
//#include "...\cards.c"

void split(char card, char dealersHand[], const char deck[], int *chips, int *wager);
bool play (char dealersHand[], char yourHand[], int dealersHandSize, int *yourHandSize, const char deck[], int *chips, int *wager, bool alreadySplit);

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
    } if (aceFlag == 1 && value < 12)
        value += 10;

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
    printf("Dealer's Hand: %s  (Value: %d)\t\tYour Hand:%s  (Value: %d)", dealersHand,
           evaluate(dealersHand, dealersHandSize), yourHand, evaluate(yourHand, yourHandSize));
}

void whoWon(char dealersHand[], char yourHand[], int dealersHandSize, int yourHandSize, int* chips, const int* wager) {
    int handValue = evaluate(yourHand, yourHandSize);
    int dealersHandValue = evaluate(dealersHand, dealersHandSize);

    if (dealersHandValue > 21) {                      // WINNER EVALUATION
        printf("Dealer has busted!!!");
        *chips += 2 * *wager;
    } else if (dealersHandValue > handValue) {
        printf("The house always wins...");
    } else if (dealersHandValue < handValue) {
        printf("You won!!");
        *chips += 2 * *wager;
    } else if (dealersHandValue == handValue) {
        printf("Push!!");
        *chips += *wager;
    }
}

bool insurance(const char deck[], char hand[], int *handSize, int *chips, const int *wager, char *card){
    int insuranceBet = (*wager/2);
    printf("\nDealer is showing an Ace! Would you like to purchase insurance for %d chips? Enter 0 for no or 1 for yes", insuranceBet);
    bool insurance = inputValidate(0, 1);

    if(insurance)                                                            // insurance Bet deducted from chips
        *chips -= insuranceBet;

    printf("\nDealer is checking his hidden card...");                // Dealer shows if blackjack
    int r = rand() % 13;
    if(r > 8){
        printf("\nDealer has blackjack! ");
        hand[1] = deck[r];
        *handSize += 1;
        if(insurance){
            printf("You've lost %d chips, but won %d chips off your insurance bet!", (insuranceBet * 2), (insuranceBet * 2));
            *chips += (insuranceBet * 3);
        }
        return 1;
    } else {
        printf("\nDealer does not have blackjack! Play continues...\n");
        *card = deck[r];
        return 0;
    }
}

void split(char card, char dealersHand[], const char deck[], int *chips, int *wager) {
    char hands[2][10] = {"", ""};
    hands[0][0] = card;
    hands[1][0] = card;
    int handSize[] = {1, 1}, dealersHandSize = 1;
    bool busted[] = {0, 0};

    for (int i = 0; i < 2; ++i) {
        printf("\nHand %d:\n", i + 1);
        hit(deck, hands[i], &handSize[i]);
        displayHands(dealersHand, hands[i], dealersHandSize, handSize[i]);
        busted[i] = play(dealersHand, hands[i], dealersHandSize, &handSize[i], deck, chips, wager, 1);
    }

                   // DEALER'S HAND LOOP
    while (evaluate(dealersHand, dealersHandSize) < 17) {
        hit(deck, dealersHand, &dealersHandSize);
    }

    for (int i = 0; i < 2; ++i) {                  // DISPLAY DEALERS HAND AND CALCULATE WHO WON FOR EACH HAND
        printf("\nHand %d: \n", i+1);
        sleep(1);
        displayHands(dealersHand, hands[i], dealersHandSize, handSize[i]);
        printf("\n");
        if (busted[i]){
            printf("BUSTED!!");
        }else{
            whoWon(dealersHand, hands[i], dealersHandSize, handSize[i], chips, wager);
        }
    }
}

bool play (char dealersHand[], char yourHand[], int dealersHandSize, int *yourHandSize, const char deck[], int *chips, int *wager, bool alreadySplit) {

    bool handOver = 0;
    int choice;

    while (!handOver) {            // Hand loop, ends when player's hand is complete or busted
        choice = inputValidate(1, 5);
        switch (choice) {
            case 1:                     // HIT!
                hit(deck, yourHand, yourHandSize);
                displayHands(dealersHand, yourHand, dealersHandSize, *yourHandSize);
                if (evaluate(yourHand, *yourHandSize) > 20)
                    handOver = 1;
                break;
            case 2:                     // STAND!
                printf("StAnD dOwN");
                handOver = 1;
                break;
            case 3:                     // DOUBLE DOWN!
                if (*chips >= *wager && *yourHandSize == 2) {
                    printf("\tDOUBLED! You're in for %d!!!\n", *wager*2);
                    hit(deck, yourHand, yourHandSize);
                    displayHands(dealersHand, yourHand, dealersHandSize, *yourHandSize);
                    *chips -= *wager;
                    *wager *= 2;
                    handOver = 1;
                } else {
                    printf("You cannot double down");
                }
                break;
            case 4:                     // SPLIT!
                if(yourHand[0] == yourHand[1] && *chips >= *wager && alreadySplit == 0){
                    printf("\tSplit! You're in for %d on each hand!", *wager);
                    split(yourHand[0], dealersHand, deck, chips, wager);
                    *chips -= *wager;
                    return 1;             // RETURN BUSTED IF SPLIT SO WE SKIP DEALER'S TURN FOR SPLIT HANDS AND HANDLE THAT IN SPLIT FUNCTION
                } else {
                    printf("Cannot split cards");
                }
                break;
            case 5:                     // QUIT!
                printf("\nThanks for playing!");
                exit(0);
            default:                     // UNUSED DEFAULT
                printf("\nWhoops don't press that");
                handOver = 1;
        }

        if (evaluate(yourHand, *yourHandSize) > 20) {
            if (evaluate(yourHand, *yourHandSize) > 21) {
                printf("\n\tBUSTED!!");
                return 1;
            } else {
                printf("\t21!!!");
            }
        }
    }
    return 0;
}

int main(){
    const char deck[] = "A23456789TJQK";                // one time game data initialization
    printf("\nWelcome to BlackJack in C");
    printf("\nOn your action you may:\n1. Hit    2. Stand    3. Double Down    4. Split    5. Quit");
    srand(time(NULL));
    bool gameOver = 0;
    int chips = 100;

    while(!gameOver) {           // Main game loop

        bool dealerBlackjack = 0, blackjack = 0, busted;      //variable initialization
        int yourHandSize = 0, dealersHandSize = 0, wager;
        char yourHand[10] = "", dealersHand[10] = "", dealerCard = 'L';

        hit(deck, yourHand, &yourHandSize);           // Initialize hands
        hit(deck, yourHand, &yourHandSize);
        hit(deck, dealersHand, &dealersHandSize);

        printf("\n\tYou now have %d chips...\nNew Hand!!\tWhat is your wager? ", chips);   // What's your wager?
        wager = inputValidate(2, chips);
        chips -= wager;
        displayHands(dealersHand, yourHand, dealersHandSize, yourHandSize);

        if (evaluate(yourHand, yourHandSize) == 21) {   // PLAYER BLACKJACK??
            printf("\nBlackJack!!! Payout is 3/2! You've won %d off your %d wager!", ((3 * wager)/2), wager);
            chips += (5 * wager)/2;
            blackjack = 1;
        }

        if (dealersHand[0] == 'A' && !blackjack) {          // INSURANCE???
            dealerBlackjack = insurance(deck, dealersHand, &dealersHandSize, &chips, &wager, &dealerCard);
            displayHands(dealersHand, yourHand, dealersHandSize, yourHandSize);
        }

                                                        // MAIN PLAY LOOP
        if (!dealerBlackjack && !blackjack) {         // If either player has blackjack, skip play loop and dealer hand loop
            busted = play(dealersHand, yourHand, dealersHandSize, &yourHandSize, deck, &chips, &wager, 0);

            if(!busted) {                       // DEALER'S HAND LOOP
                if(dealerCard != 'L'){
                    dealersHand[dealersHandSize] = dealerCard;           // Tends to dealer's 'hidden card'
                    dealersHandSize += 1;
                    printf("\n");
                    displayHands(dealersHand, yourHand, dealersHandSize, yourHandSize);
                }
                while (evaluate(dealersHand, dealersHandSize) < 17) {        // Hit until 17
                    sleep(1);
                    hit(deck, dealersHand, &dealersHandSize);
                    printf("\n");
                    displayHands(dealersHand, yourHand, dealersHandSize, yourHandSize);
                }
                printf("\n\t");
                whoWon(dealersHand, yourHand, dealersHandSize, yourHandSize, &chips, &wager);    // Who won?
            }
        }

        if (chips <= 1){                                        // OUT OF CHIPS?
            printf("\nYou now have %d chips", chips);
            printf("\nYou lost all your chips! Better luck next time!");
            gameOver = 1;
        }
    }
    return 0;
}