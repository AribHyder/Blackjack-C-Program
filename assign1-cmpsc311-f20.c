////////////////////////////////////////////////////////////////////////////////
//
//  File           : cmpsc311-f16-assign1.c
//  Description    : This is the main source code for for the first assignment
//                   of CMPSC311 at Penn State University.  See the related
//                   assignment page for details.
//
//   Author        : Agha Arib Hyder
//   Last Modified : 9/7/20
//

// Include Files
#include <stdio.h>
#include <cmpsc311_util.h>

// Defines
#define NUM_CARDS 52
#define MAX_CARDS 11
#define SPADE   "\xE2\x99\xA0"
#define CLUB    "\xE2\x99\xA3"
#define HEART   "\xE2\x99\xA5"
#define DIAMOND "\xE2\x99\xA6"

// Functions

////////////////////////////////////////////////////////////////////////////////
//
// Function     : print_card
// Description  : print the card from the integer value
//
// Inputs       : card - the card to print
// Outputs      : 0 always

int print_card( int card ) {

    // CODE HERE
    char card_faces[] = "234567891JQKA";
    char *card_suits[] = { SPADE, CLUB, HEART, DIAMOND };
    int suit = card / 13, cardty = card % 13;
    if ( cardty == 8 ) {
        printf( "10%s", card_suits[suit] );
    } else {
        printf( "%c%s", card_faces[cardty], card_suits[suit] );
    }

    // Return zero
    return( 0 );
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : print_cards
// Description  : print a number of cards (no more than 13 on one line)
//
// Inputs       : cards - the array of cards to print
//                num_cards - the number of cards to print
// Outputs      : 0 always

int print_cards(int cards[], int num_cards ) {
    
    int counter = 0;

    while (counter < num_cards) {
        if ((counter % 13) == 0) {
	    printf("\n");
	}

        print_card(cards[counter]);
	counter++;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : shuffle_cards
// Description  : print a number of cards (no more than 13 on one line)
//
// Inputs       : cards - the array of cards to print
//                num_cards - the number of cards to print
// Outputs      : 0 always

int shuffle_cards( int cards[], int num_cards ) {

    int temp;

    for (int x = (num_cards - 1); x > 0; x--) {
        int y = getRandomValue(0, num_cards - 1);
	temp = cards[x];
	cards[x] = cards[y];
	cards[y] = temp;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : hand_value

// Description  : return the value of the hand
//
// Inputs       : cards - the array of cards in the hand
//                num_cards - the number of cards in the hand
// Outputs      : 0 always

int hand_value( int cards[], int num_cards ) {
    
    int counter = 0;
    int hand_value = 0;
    int card;

    //using mod 13 to reduce range to 0 - 12
    while (counter < num_cards) {       
        card = cards[counter] % 13;
        //set ace to 11 at first
        if (card == 12) {
            hand_value += 11;
        }
        //add face card values
        else if (card == 9 || card == 10 || card == 11 || card == 8) {
            hand_value += 10;
        }

        else {
            hand_value += (card + 2);
        }
        counter++;
    }
    //if hand value reaches > 12 and we have an ace, set it back to 1
    if (hand_value > 21) {
        int count = 0;
        int card1;

        while (count < num_cards) {
            card1 = cards[count] % 13;
            if ((card1 == 12) && hand_value > 21) {
                hand_value -= 10;
            }
            count++;
        }
    }
    
    return hand_value;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : sort_cards
// Description  : sort a collection of cards
//
// Inputs       : hand - the cards to sort
//                num_cards - the number of cards in the hand
// Outputs      : 0 always

int sort_cards( int hand[], int num_cards ) {
    
    int counter = 0;
    // Bubble sort algorithm
    while (counter < (num_cards-1)) {
	    for (int sort = 0; sort < num_cards - counter - 1; sort++) {
	        if (hand[sort] > hand[sort + 1]) {
	            int temp_value = hand[sort + 1];
	            hand[sort + 1] = hand[sort];
                hand[sort] = temp_value; 
	        }
        }
	    counter++;    
    }  
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : dealer_play
// Description  : dealer decision to hit or stand (hit on anything less
//                than 17)
//
// Inputs       : hand - cards dealer has
//                num_cards - the number of cards in player hand
// Outputs      : 0 = stand, 1 = hit

int dealer_play( int hand[], int num_cards ) {
    
    int dealer_hand = hand_value(hand, num_cards);

    if (dealer_hand < 17) {
        return 1;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : player_play
// Description  : player decision to hit or stand
//
// Inputs       : hand - cards player has
//                num_cards - the number of cards in player hand
//                dealer_card - the dealers face up card
// Outputs      : 0 = stand, 1 = hit

int player_play( int hand[], int num_cards, int dealer_card ) {
    // Same as dealer but hits at 17 or less instead
    int player_hand = hand_value(hand, num_cards);

    if (player_hand < 18) {
        return 1;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : play_hand
// Description  : play a hand of black jack
//
// Inputs       : deck - the whole deck of cards
//                num_cards - the number of cards in the deck
//                player_money - pointer to player amount of money
// Outputs      : 1 if player wins, 0 if dealer wins

int play_hand( int deck[], int num_cards, float *player_money ) {
    
    shuffle_cards(deck, num_cards);
    printf("---- New hand ----\n\n");

    float win = 7.5;

    int player_hand;
    int dealer_hand;

    int player[11];
    int dealer[11];
    int player_num_cards = 2;
    int dealer_num_cards = 2;
    // Give dealer and player 2 cards each
    player[0] = deck[0];
    dealer[0] = deck[1];
    player[1] = deck[2];
    dealer[1] = deck[3];

    printf("Dealer Cards: ");
    print_card(dealer[0]);
    printf(" XX\n\n");
    printf("Player Cards: ");
    print_card(player[0]);
    printf(" ");
    print_card(player[1]);
    printf("\n");

    //current deck index
    int i = 3;
    //current player hand index
    int k = 1;

    player_hand = hand_value(player, player_num_cards);
    //Blackjack case
    if (player_hand == 21) {
        *player_money = *player_money + win;
        printf("Player has Blackjack!, wins $7.50\n");
        return 1;
    }    
    //While the player wants to still be hit and has not busted, keep adding cards
    while ((player_play(player, player_num_cards, dealer[0]) == 1) && hand_value(player, player_num_cards) < 22) {
        i++;
        k++;
        player_num_cards++;
        int count = 0;

        player[k] = deck[i];
        
        printf("Player hit (%d): ", hand_value(player, player_num_cards));
        while (count < player_num_cards) {
            print_card(player[count]);
            printf(" ");
            count++;
        }
        printf("\n");
    }

    player_hand = hand_value(player, player_num_cards);
    //Check for bust
    if (player_hand > 21) {
        *player_money = *player_money - 5;
        printf("Player BUSTS ... dealer wins!\n\n");
        return 0;
    }
    //Player stands case
    int count1 = 0;
    printf("Player stands (%d): ", hand_value(player, player_num_cards));
    while (count1 < player_num_cards) {
        print_card(player[count1]);
        printf(" ");
        count1++;
    }
    printf("\n\n");
    
    //current dealer hand index
    int j = 1;
    //While dealer wants more cards and has not busted keep dealing cards
    while ((dealer_play(dealer, dealer_num_cards) == 1) && hand_value(dealer, dealer_num_cards) < 22) {
        i++;
        j++;
        dealer_num_cards++;
        int count3 = 0;

        dealer[j] = deck[i];
        
        printf("Dealer hit (%d): ", hand_value(dealer, dealer_num_cards));
        while (count3 < dealer_num_cards) {
            print_card(dealer[count3]);
            printf(" ");
            count3++;
        }
        printf("\n");

    }

    dealer_hand = hand_value(dealer, dealer_num_cards);
    //Check for bust
    if (dealer_hand > 21) {
        *player_money = *player_money + 5;
        printf("Dealer BUSTS ... player wins!\n\n");
        return 1;
    }
    //stand case
    int count2 = 0;
    printf("Dealer stands (%d): ", hand_value(dealer, dealer_num_cards));
    while (count2 < dealer_num_cards) {
        print_card(dealer[count2]);
        printf(" ");
        count2++;
    }
    printf("\n\n");
    //compare hands and see who wins
    if (player_hand > dealer_hand) {
        *player_money = *player_money + 5;
        printf("Player wins!!!\n\n");
        return 1;
    }

    *player_money = *player_money - 5;
    printf("Dealer wins!!!\n\n");

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : show_player_money_histogram
// Description  : Display a histogram of the player funds by hands
//
// Inputs       : money_rounds - the list of player money by hand number
//                last_round - the hand number where the game stopped
// Outputs      : 0 always

int show_player_money_histogram( float money_rounds[], int last_round ) {
    printf("\n\n                                            Player cash per round                                      ");
    printf("\n\t ----------------------------------------------------------------------------------------------------\n");
    //generate histogram line by line (hist money decrements by five and compares to (player's money/5)*5 so it is in same increments
    int countr = 0;
    int hist_money = 200;
    while (countr < 41) {
        printf("%d\t|", hist_money);
        if(hist_money == 0){
            //handles hist_money = 0 case
            for (int z = 0; z < 100; z++) {
                if (z <= last_round){
                    printf("X");

                }
                else{
                    printf(".");
                }
                
            }
            
            
        }
        //fills histogram for all other values
        for (int z = 0; z <100; z++) {
            if ((hist_money <= (((int)money_rounds[z]/5)*5)) && hist_money != 0) {
                printf("X");
            }
            else if((hist_money > (((int)money_rounds[z]/5)*5)) && hist_money != 0) {
                printf(".");
            }
        }
          
        printf("|\n");
        countr++;
        hist_money = hist_money - 5;
    }
    printf("\n\t");
    // printing round numbers below
    int tens = 1;
    for (int x = 0; x <= 100; x++) {
        if ((x % 10 == 0) && (x != 0)) {
            printf("%d", tens);
            tens++;
        }
        else {
            printf(" ");
        }
    }
    printf("\n\t ");
    // also printing round numbers
    int ones = 0;
    for (int y = 0; y < 100; y++) {
        ones++;
        printf("%d", (ones % 10));
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////////
//
// Function     : main
// Description  : The main function for the CMPSC311 assignment #1
//
// Inputs       : argc - the number of command line parameters
//                argv - the parameters
// Outputs      : 0 if successful test, -1 if failure

int main( int argc, char **argv ) {

    /* Local variables */
    int cmp311_deck[NUM_CARDS];  // This is the deck of cards
    int num_hands = 0;
    float player_dough = 100;
    float min_bet = 5;
    int win_count = 0;
    float dough_rounds[100];

    /* Preamble information */
    printf( "CMPSC311 - Assignment #1 - Fall 2020\n\n" );
    srand(time(NULL)) ;

    /* Step #1 - create the deck of cards */
    cmp311_deck[0] = 0;
    cmp311_deck[1] = 1;
    cmp311_deck[2] = 2;
    cmp311_deck[3] = 3;
    cmp311_deck[4] = 4;
    cmp311_deck[5] = 5;
    cmp311_deck[6] = 6;
    cmp311_deck[7] = 7;
    cmp311_deck[8] = 8;
    cmp311_deck[9] = 9;
    cmp311_deck[10] = 10;
    cmp311_deck[11] = 11;
    cmp311_deck[12] = 12;
    cmp311_deck[13] = 13;
    cmp311_deck[14] = 14;
    cmp311_deck[15] = 15;
    cmp311_deck[16] = 16;
    cmp311_deck[17] = 17;
    cmp311_deck[18] = 18;
    cmp311_deck[19] = 19;
    cmp311_deck[20] = 20;
    cmp311_deck[21] = 21;
    cmp311_deck[22] = 22;
    cmp311_deck[23] = 23;
    cmp311_deck[24] = 24;
    cmp311_deck[25] = 25;
    cmp311_deck[26] = 26;
    cmp311_deck[27] = 27;
    cmp311_deck[28] = 28;
    cmp311_deck[29] = 29;
    cmp311_deck[30] = 30;
    cmp311_deck[31] = 31;
    cmp311_deck[32] = 32;
    cmp311_deck[33] = 33;
    cmp311_deck[34] = 34;
    cmp311_deck[35] = 35;
    cmp311_deck[36] = 36;
    cmp311_deck[37] = 37;
    cmp311_deck[38] = 38;
    cmp311_deck[39] = 39;
    cmp311_deck[40] = 40;
    cmp311_deck[41] = 41;
    cmp311_deck[42] = 42;
    cmp311_deck[43] = 43;
    cmp311_deck[44] = 44;
    cmp311_deck[45] = 45;
    cmp311_deck[46] = 46;
    cmp311_deck[47] = 47;
    cmp311_deck[48] = 48;
    cmp311_deck[49] = 49;
    cmp311_deck[50] = 50;
    cmp311_deck[51] = 51;
    /* Step #2 - print the deck of cards */
    print_cards(cmp311_deck, NUM_CARDS);
    printf("\n");
    /* Step #4 - shuffle the deck */
    shuffle_cards(cmp311_deck, NUM_CARDS);
    /* Step #5 - print the shuffled deck of cards */
    print_cards(cmp311_deck, NUM_CARDS);
    printf("\n");
    /* Step #6 - sort the cards */
    sort_cards(cmp311_deck, NUM_CARDS);
    /* Step #7 - print the sorted deck of cards */
    print_cards(cmp311_deck, NUM_CARDS);
    printf("\n\n");
    /* Step #9 - deal the hands */
    
    while ((num_hands < 100) && (player_dough >= min_bet)) {
        num_hands++;
        if (play_hand(cmp311_deck, NUM_CARDS, &player_dough) == 1) {
            win_count++;
        }
        printf("After hand %d player has %.2f$ left\n", num_hands, player_dough);
        dough_rounds[num_hands - 1] = player_dough;
    }
    
    printf("Blackjack done - player won %d out of %d hands (%.2f).", win_count, num_hands, (float)num_hands);
    /* Step 10 show historgrapm */
    show_player_money_histogram(dough_rounds,num_hands);

    /* Exit the program successfully */
    printf( "\n\nCMPSC311 - Assignment #1 - Spring 2020 Complete.\n" );
    return( 0 );
}

