#include <stdio.h>
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
    if (aceFlag == 1 && value < 12)
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
    printf("\nDealer's Hand: %s  (Value: %d)\t\tYour Hand:%s  (Value: %d)", dealersHand,
           evaluate(dealersHand, dealersHandSize), yourHand, evaluate(yourHand, yourHandSize));
}

void displaySplitHands(char dealersHand[], char hand1[], char hand2[], int dealersHandSize, int hand1Size, int hand2Size) {
    printf("\nDealer's Hand: %s  (Value: %d)\t\tHand 1:%s  (Value: %d)\t\tHand 2:%s  (Value: %d)",
           dealersHand, evaluate(dealersHand, dealersHandSize),
           hand1, evaluate(hand1, hand1Size), hand2, evaluate(hand2, hand2Size));
}

void whoWon(char dealersHand[], char yourHand[], int dealersHandSize, int yourHandSize, int* chips, int* wager) {
    int handValue = evaluate(yourHand, yourHandSize);
    int dealersHandValue = evaluate(dealersHand, dealersHandSize);

    if (dealersHandValue > 21) {                      // WINNER EVALUATION
        printf("Dealer has busted!!!");
        *chips += 2 * *wager;
    } else if (dealersHandValue > handValue) {
        printf("The house always wins...");
    } else if (dealersHandValue < handValue) {
        printf("You win!!");
        *chips += 2 * *wager;
    } else if (dealersHandValue == handValue) {
        printf("Push!!");
        *chips += *wager;
    }
}

bool insurance(const char deck[], char hand[], int *handSize, int *chips, const int *wager, bool *dealerBlackjack){
    int insuranceBet = (*wager/2);
    printf("\nDealer is showing an Ace! Would you like to purchase insurance for %d?", insuranceBet);
    bool insurance = inputValidate(0, 1);

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

void split(char card, char dealersHand[], const char deck[], int *chips, int *wager){
    char hands[2][10];
    hands[0][0] = card;
    hands[1][0] = card;
    int handSize[] = {1,1}, dealersHandSize = 1;
    bool handOver, busted[] = {0, 0};

    for (int i = 0; i < 2; ++i) {
        printf("\nHand %d:", i+1);
        handOver = 0;

        while (!handOver) {            // Hand loops, ends when player's hand is complete or busted
            int choice = inputValidate(1, 3);
            switch (choice) {
                case 1:                     // HIT!
                    hit(deck, hands[i], &handSize[i]);
                    displayHands(dealersHand, hands[i], dealersHandSize, handSize[i]);
                    if (evaluate(hands[i], handSize[i]) > 20)
                        handOver = 1;
                    break;
                case 2:                     // STAND!
                    printf("StAnD dOwN");
                    handOver = 1;
                    break;
                case 3:                     // DOUBLE DOWN!
                    hit(deck, hands[i], &handSize[i]);
                    displayHands(dealersHand, hands[i], dealersHandSize, handSize[i]);
                    *chips -= *wager;
                    *wager = *wager * 2;
                    handOver = 1;
                    break;
                default:                     // UNUSED DEFAULT
                    printf("\nWhoops don't press that");
                    break;
            }
            if (evaluate(hands[i], handSize[i]) > 21) {
                printf("\tBUSTED HAND %d!!", i+1);
                busted[i] = 1;
                handOver = 1;
            }
        }
    }                                                                 // END OF SPLIT HANDS USER PLAY LOOPS!!!
    while (evaluate(dealersHand, dealersHandSize) < 17) {          // DEALER'S HAND LOOP
        hit(deck, dealersHand, &dealersHandSize);
        displaySplitHands(dealersHand, hands[0], hands[1], dealersHandSize, handSize[0], handSize[1]);
    }
    for (int i = 0; i < 2; ++i) {                        // BOTH HAND RESULTS
        printf("\nHand %d: ", i+1);
        if (busted[i]){
            printf("BUSTED!!");
        }else{
            whoWon(dealersHand, hands[i], dealersHandSize, handSize[i], chips, wager);
        }
    }
}


int main(){
    const char deck[] = "A23456789TJQK";                // deck initialization
    printf("\nWelcome to BlackJack in C");
    printf("\nOn your action you may:\n1. Hit    2. Stand    3. Double Down    4. Split    5. Quit");

    bool gameOver = 0;
    int chips = 100;

    while(!gameOver) {           // Main game loop

        bool handOver = 0, busted = 0, dealerBlackjack = 0, blackjack = 0, splitted = 0;      //variable initialization
        int yourHandSize = 0, dealersHandSize = 0, wager, choice;
        char yourHand[10] = "", dealersHand[10] = "";

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
                    printf("DOUBLED! You're in for %d!!!", wager*2);
                    hit(deck, yourHand, &yourHandSize);
                    displayHands(dealersHand, yourHand, dealersHandSize, yourHandSize);
                    chips -= wager;
                    wager *= 2;
                    handOver = 1;
                    break;
                case 4:                     // SPLIT!
                    if(yourHand[0] == yourHand[1]){
                        printf("\nSplat");
                        splitted = 1;
                        split(yourHand[0], dealersHand, deck, &chips, &wager);
                        handOver = 1;
                        chips -= wager;
                    } else {
                        printf("\nCannot split cards");
                    }
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

        if(!busted && !dealerBlackjack && !blackjack && !splitted) {                            // DEALER'S HAND LOOP
            while (evaluate(dealersHand, dealersHandSize) < 17) {
                hit(deck, dealersHand, &dealersHandSize);
                displayHands(dealersHand, yourHand, dealersHandSize, yourHandSize);
            }
            printf("\n");
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