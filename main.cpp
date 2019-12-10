//
//  Created by Joshua Jacobs for 10/31/19 Programming Assignment.
//  Copyright © 2019 Joshua Jacobs. All rights reserved.
//

//                                                                  PSEUDOCODE:
//Player draws two cards
//Computer 1 & 2 is dealt two cards
//Deck is dealt one card face up, one face down.
//Bets are payed back at 1.5 ratio

/*Player Rules:
Player determines how much they can bet (Start with $500)
 1st Move: Player can SPLIT, DOUBLE, Hit, or Stand
 2nd Move+: Player can only Hit or Stand.
 2nd Move+ Exception: Player can also SPLIT or DOUBLE their new hands
 Player loses if they get go over 21
 Player wins if they are the closest to 21 when all other bots stand
 SPLIT ON ACE EXCEPTION: Only allowed to draw one more card, No more splitting or doubling allowed
 If double aces and is stood on, prompts the user to choose the value 2 or 12
*/

/*Deck Rules:
 Deck hits until hand val >= 17
 One of the values is hidden and is revealed once all other players stand
*/

/*
 Comp 1 Rules: Always hits until 17<
 Comp 2 Rules: Always hits until 18<
 */

//TODO: FIX money earned after split                                          Mostly Fixed
//See splitHand comments

//TODO: Blackjack check is broken (True Blackjack, 21 works though)           Very Minor Problem

//TODO: If player busts Jacks, Queens, and Kings revert back to >10           Very Minor Problem
//Not a huge issue since it doesn't actually change gameplay, just a weird display issue
//Only occurs when facecard is the busting card, sometimes? Hard to repeat?

//TODO: If both computers are out out, programs errors out on player turn     Minor Problem
//Technically it's supposed to end the program anyways at the same exact point
//just way more gracefully

#include <ctime>
#include <iostream>
#include <vector>
#include "Card.h"
#include "Deck.h"

using namespace std;

int valueOfHand(vector<Card>&, int);
int checkForDoubleAces(vector<Card>&, int);
int assignValueOfDoubleAces(int);
bool checkForSplitAvailibility(vector<vector<Card>>&, vector<Card>&, int);
vector<vector<Card>> splitHand(vector<vector<Card>>&, vector<Card>&, Deck&, vector<int>&, vector<int>&, int);
vector<int> collectBets(vector<int>&);
vector<int> insurance(vector<int>&, vector<int>&);
bool canDouble(vector<vector<Card>>&, vector<Card>&, vector<int>&, vector<int>&, int);
void doubleBet (vector<int>&, int, vector<int>&);
vector<Card> hit(vector<Card>&, Deck&);
bool deckTurn(vector<Card>&, vector<int>&, vector<int>&);
int playerTurn(vector<vector<Card>>&, Deck&, vector<int>&, vector<int>&, short int, vector<int>&);
int compOneTurn(vector<vector<Card>>&, Deck&, vector<int>&, vector<int>&, short int, vector<int>&);
int compTwoTurn(vector<vector<Card>>&, Deck&, vector<int>&, vector<int>&, short int, vector<int>&);
int deckTurnTwo(vector<Card>&, Deck&);
void displayTable();
void declareBanks(vector<int>&);
bool isBlackjack(vector<vector<Card>>&, int);
void calculatePayOut(vector<vector<Card>>, vector<int>&, vector<int>&, bool);
bool playAgainPrompt();
void clearRound (vector<Card>&, vector<vector<Card>>&, vector<vector<Card>>&, vector<vector<Card>>&, Deck&, vector<int>&, vector<int>&, vector<int>&, vector<int>&);
int globalCardDraw(bool, int j = 0);
void shuffleTheDeck(Deck&);
vector<int> calculateNumberOfWins(vector<int>, vector<int>, vector<int>, int);
void terminateGame();
bool canPlay(vector<int>, int);
void staticBoolPlayers(int);


int main()
{
    Deck deck;
    
    vector<Card> playerHand;
    vector<Card> dealerHand;
    vector<Card> compOneHand;
    vector<Card> compTwoHand;
    vector<vector<Card>> playerHands;
    vector<vector<Card>> compOneHands;
    vector<vector<Card>> compTwoHands;
    vector<int> bets;
    vector<int> money;//player is 0
    vector<int> pointsOne;
    vector<int> pointsTwo;
    vector<int> pointsP;
    short int valueOne;
    short int valueTwo;
    short int valueP;
    short int deckVal;
    short int currentHandIteration = 0;
    bool endGame = false;
    bool oneBlackjack = false;
    bool twoBlackjack = false;
    bool pBlackjack = false;
    bool playAgain = true;
    vector<int> numberOfWins;

    //Gives each player $250
    for (int i = 0; i < 3; i++) {
        money.push_back(250);
    }
    
    deck.shuffle();
    
    while (playAgain == true) {
        cout << "Cards drawn: " << globalCardDraw(false, 1) << endl;
        bets = collectBets(money);
    
        for (int i=0; i<3; i++) {
            cout << "Player[" << i << "] Bet: $" << bets.at(i) << endl;
            cout << "Player[" << i << "] Money: $" << money.at(i) << endl;
        }

        for(int winAssignment = 0; winAssignment < 3; winAssignment++) {
            numberOfWins.push_back(0);
        }
        cout << endl;
    
        playerHand = deck.dealCards(2);
        dealerHand = deck.dealCards(2);
        compOneHand = deck.dealCards(2);
        compTwoHand = deck.dealCards(2);
    
        playerHands.push_back(playerHand);
        compOneHands.push_back(compOneHand);
        compTwoHands.push_back(compTwoHand);
    
        cout << "Dealer up card: ";
        dealerHand.at(0).display();
        cout << endl;
    
        while(endGame == false) {
            endGame = deckTurn(dealerHand, bets, money);
            if (endGame == true)
                break;
        compOne:
            if (canPlay(money, 1) == false) {
                cout << "Computer One can no longer play!" << endl;
                pointsOne.push_back(0);
                staticBoolPlayers(1);
                goto compTwo;
            }
            valueOne = compOneTurn(compOneHands, deck, bets, money, currentHandIteration, money);
            if (valueOne == 21) {
                if (compOneHands.at(currentHandIteration).size() == 2) {
                    oneBlackjack = true;
                }
            }
            
            if (currentHandIteration + 1 < compOneHands.size()) {
                pointsOne.push_back(valueOne);
                currentHandIteration++;
                goto compOne;
            } else {
                pointsOne.push_back(valueOne);
                currentHandIteration = 0;
            }
        compTwo:
            if (canPlay(money, 2) == false) {
                cout << "Computer Two can no longer play!" << endl;
                pointsTwo.push_back(0);
                staticBoolPlayers(2);
                goto playerTurn;
            }
            valueTwo = compTwoTurn(compTwoHands, deck, bets, money, currentHandIteration, money);
            if (valueTwo == 21) {
                if (compTwoHands.at(currentHandIteration).size() == 2) {
                    twoBlackjack = true;
                }
            }
            if (currentHandIteration + 1 < compTwoHands.size()) {
                pointsTwo.push_back(valueTwo);
                currentHandIteration++;
                goto compTwo;
            } else {
                pointsTwo.push_back(valueTwo);
                currentHandIteration = 0;
            }
        playerTurn:
            cout << "YOUR TURN\n";
            valueP = playerTurn(playerHands, deck, bets, money, currentHandIteration, money);
            if (valueP == 21) {
                if (playerHands.at(currentHandIteration).size() == 2) {
                    pBlackjack = true;
                }
            }
            if (currentHandIteration + 1 < playerHands.size()) {
                pointsP.push_back(valueP);
                currentHandIteration++;
                goto playerTurn;
            } else {
                pointsP.push_back(valueP);
                currentHandIteration = 0;
            }
            for (int loops = 0; loops < pointsP.size(); loops++) {
                cout << pointsP.at(loops) << "    ";
            }
            cout << "\nYour turn is over\n";
            deckVal = deckTurnTwo(dealerHand, deck);
            numberOfWins = calculateNumberOfWins(pointsOne, pointsTwo, pointsP, deckVal);
            
            endGame = true;
        }
        for (int payout = 0; payout < 3; payout++) {
            if (payout == 0) {
                if (pBlackjack == true) {
                    cout << "YOU WON: $" << bets.at(0) * 2.5 << endl;
                    money.at(0) = money.at(0) + bets.at(0) * 2.5;
                    cout << "NEW BANK: $" << money.at(0) << endl;
                }
                else if (numberOfWins.at(0) > 0) {
                    cout << "YOU WON: $" << bets.at(0) * (2 + (numberOfWins.at(0) - 1)) << endl;
                    money.at(0) = money.at(0) + bets.at(0) * (2 + (numberOfWins.at(0) - 1));
                }
                else
                    cout << "You lost your bet of $" << bets.at(0) << endl;
            }
            else if (payout == 1) {
                cout << "BEFORE BANK: " << money.at(1) << " AND BET OF: " << bets.at(1) << endl;
                if (oneBlackjack == true) {
                    cout << "Computer One Won: $" << bets.at(1) * 2.5 << endl;
                    money.at(1) = money.at(1) + bets.at(1) * 2.5;
                    cout << "Computer One Bank: $" << money.at(1) << endl;
                }
                else if (numberOfWins.at(1) > 0) {
                    cout << "Computer One Won: $" << bets.at(1) * (2 + (numberOfWins.at(1) - 1)) << endl;
                    money.at(1) = money.at(1) + bets.at(1) * (2 + (numberOfWins.at(1) - 1));
                }
                else
                    cout << "Computer One lost $" << bets.at(1) << endl;
            }
            else if (payout == 2) {
                cout << "BEFORE BANK: " << money.at(2) << " AND BET OF: " << bets.at(2) << endl;
                if (twoBlackjack == true) {
                    cout << "Computer Two Won: $" << bets.at(2) * 2.5 << endl;
                    money.at(2) = money.at(2) + bets.at(2) * 2.5;
                    cout << "Computer Two Bank: $" << money.at(2) << endl;
                }
                else if (numberOfWins.at(2) > 0) {
                    cout << "Computer Two won: $" << bets.at(2) * (2 + (numberOfWins.at(2) - 1)) << endl;
                    money.at(2) = money.at(2) + bets.at(2) * (2 + (numberOfWins.at(2) - 1));
                }
                else
                    cout << "Computer Two lost $" << bets.at(2) << endl;
            }
        }
    
        declareBanks(money);
        playAgain = playAgainPrompt();
        if (playAgain == true) {
            clearRound(dealerHand, playerHands, compOneHands, compTwoHands, deck, pointsP, pointsOne, pointsTwo, numberOfWins);
            endGame = false;
        }
    }
    return 0;
}




//TAKES THE INITIAL VALUE OF THE HAND. Only time this should be redone is if a hand has been split recently
int valueOfHand (vector<Card> &hand, int playerNum) {
    int value = 0;
    
    //***** Checks to see if the hand is initially double aces. Checks only once for each hand.
    for (int i = 0; i < hand.size(); i++) {
        if (hand.at(i).getVal() > 10) {
            value += 10;
        }
        else
            value += hand.at(i).getVal();
    }
    return value;
}

int checkForDoubleAces(vector<Card> &hand, int playerNum) {
    int val = 0;
    if (hand.at(0).getVal() == 1) {
        if (hand.at(1).getVal() == 1) {
            if (hand.size() == 2) {
                val = assignValueOfDoubleAces(playerNum);
            }
        }
    }
    
    return val;
}
//Has player or computer assign a value to a hand of double aces
int assignValueOfDoubleAces(int playerNum) {
    short int chosenVal = 2;
    int r;
    
    srand((int)time(0));
    
    //Player Input
    if (playerNum == 1) {
        do {
            cout << "You have a starting hand of double aces! Incredible! Choose starting hand value of 2 or 12: ";
            cin >> chosenVal;
        } while((chosenVal != 2) && (chosenVal !=12));
    }
    //Deck always assigns highest value to double ace
    else if (playerNum == 0) {
        chosenVal = 12;
    }
    
    //Computer randomly chooses hand value
    else {
        r = rand() % 2;
        if (r == 1)
            chosenVal = 2;
        else
            chosenVal = 12;
    }
    
    return chosenVal;
}

//Function checks if the hand in play can be split.
bool checkForSplitAvailibility(vector<vector<Card>> &hands, vector<Card> &hand, int playerNum) {
    string s;
    int r;
    bool split = false;
    int consecutiveAces = 0;
    
    srand((int)time(0));
    
    //Checks if card value is the same in slot one and two AND checks if hand is size of 2
    if ((hand.at(0).getVal() == hand.at(1).getVal()) && hand.size() == 2) {
        for (int i = 0; i < hands.size(); i++) {
            if (hands.at(i).at(1).getVal() == 1){
                consecutiveAces++;
            } else {
                consecutiveAces--;
            }
        }
        if (consecutiveAces == 2) {
            split = false;
        }
        else if (playerNum == 1) {
            split = true;
        }
        //Computer players randomly choose if they want to split.
        else if (playerNum == 2 || playerNum == 3) {
            r = rand() % 2;
            if (r == 1) {
                split = true;
            }
        }
    }
    return split;
}

//Returns a new set of hands
vector<vector<Card>> splitHand (vector<vector<Card>> &hands, vector<Card> &hand, Deck &deck, vector<int> &bank, vector<int> &bets, int playerNum) {
    vector<Card> handNew;
    
    //handNew is set to be the same as the passed hand
    handNew = hand;
    Card lazyCoderCard = handNew.at(1);
    
    //Delete last element
    handNew.pop_back();
    handNew = hit(handNew, deck);
    
    //Delete last element of passed hand
    hand.pop_back();
    
    //LazyCoderCard is an easy way for me to copy a past card that will be deleted
    hand.at(0) = lazyCoderCard;
    hand = hit(hand, deck);
    
    //Assigned handNew to hands
    hands.push_back(handNew);
    
    cout << "Your current hand has: ";
    hand.at(0).display();
    cout << "     ";
    hand.at(0).display();
    cout << endl;
    cout << "Your new hand has these cards: ";
    handNew.at(0).display();
    cout << "     ";
    handNew.at(1).display();
    cout << endl;
    
    //Foreseable problem: Doubling on one of these hands will actually double both.
    bank.at(playerNum) -= bets.at(playerNum);
    bets.at(playerNum) *= 2;
    return hands;
}

//Function collects bets from all the players
vector<int> collectBets(vector<int> &bank) {
    vector<int>collectedBets;
    int bet;
    
    //Randomize
    srand((int)time(0));
    

    for (int i = 0; i < 3; i++) {
        //Collects bet info from player. Game ends if the player has <$10
        if (i == 0) {
            if (bank.at(0) == 0) {
                cout << "You ran out of money! Game over!" << endl;
                terminateGame();
            }
            else if (bank.at(0) < 10) {
                cout << "You do not have the minimum bet for this table! Game over!" << endl;
                terminateGame();
            }
            do {
                cout << "How much would you like to bet? Maximum Bet is: $" << bank.at(0) << ". Minimum of $10. ";
                cin >> bet;
            } while (bet < 10 || bet > bank.at(0));
            bank.at(i) -= bet;
        }
        else {
            if (bank.at(i) < 10) {
                bet = 0;
            }
            else {
                //Bots randomly bet
                bet = (rand() % bank.at(i)-10)/3 + 10;
                bank.at(i) -= bet;
            }
        }
        collectedBets.push_back(bet);
    }
    return collectedBets;
}

//Function allows the player to put down an insurance bet that the dealer has a blackjack, bots always insure
vector<int> insurance(vector<int> &collectedBets, vector<int> &bank) {
    int maximumBet;
    int bet;
    vector<int> insuranceBet;
    for (int i = 0; i<3; i++) {
        maximumBet = collectedBets.at(i) / 2;
        if (maximumBet > bank.at(i)) {
            maximumBet = bank.at(i);
        }
        if (i == 0) {
            cout << "The flipped card for the dealer is an ace. You may put a bet that the next card has a value of 10.\n";
            cout << "The maximum you may bet is equal to half of your original bet, and pays out at a 2:1 rate.\n";
            do {
                cout << "How much would you like to put down for Insurance? ($0 - $" << maximumBet << ").";
                cin >> bet;
                if (bet < 0)
                    bet = 0;
                insuranceBet.push_back(bet);
                bank.at(i) -= bet;
            } while (bet > maximumBet);
        }
        else if (i == 1) {
            bet = maximumBet / 2;
            insuranceBet.push_back(bet);
            bank.at(i) -= bet;
        }
        else if (i == 2) {
            if (maximumBet >= (bank.at(i)/2)) {
                bet = maximumBet;
                insuranceBet.push_back(bet);
                bank.at(i) -= bet;
            }
        }
    }
    return insuranceBet;
}

//Checks to see if hand can be doubled
bool canDouble(vector<vector<Card>> &hands, vector<Card> &hand, vector<int> &betsMade, vector<int> &bank, int playerNum){
    bool doubleAvailable = false;
    int consecutiveAces = 0;
   
    //Checks if hand size is 2, and also if the player can afford to double their bet
    if (hand.size() == 2) {
        for (int i = 0; i < hands.size(); i++) {
            if (hands.at(i).at(1).getVal() == 1){
                consecutiveAces++;
            } else {
                consecutiveAces--;
            }
        }
        if (consecutiveAces == 2) {
            doubleAvailable = false;
        }
        else if ((betsMade.at(playerNum - 1) * 2) < bank.at(playerNum - 1)) {
            doubleAvailable = true;
        }
    }
    return doubleAvailable;
}

//Function just doubles the bet
void doubleBet (vector<int> &betsMade, int playerNum, vector<int> &bank) {
    int value = betsMade.at(playerNum);
    betsMade.at(playerNum) = value * 2;
    bank.at(playerNum) -= value;
}

//Function draws a card and adds it to the hand; also increases globalCardDraw counter
vector<Card> hit(vector<Card> &hand, Deck &drawDeck) {
    vector<Card> tempHand;
    tempHand = drawDeck.dealCards(1);
    globalCardDraw(false);
    
    //More memory efficient to combine vectors
    hand.reserve(hand.size() + tempHand.size());
    hand.insert(hand.end(), tempHand.begin(), tempHand.end());
    return hand;
}

//Dealers turn draws card and might end the game early with a blackjack.
bool deckTurn(vector<Card> &hand, vector<int> &bets, vector<int> &banks){
    int value = valueOfHand(hand, 0);
    vector<int> insurances;
    if (hand.at(0).getVal() == 1) {
        insurances = insurance(bets, banks);
        if (hand.at(1).getVal() >=10) {
            cout << "Dealer's Hand! : ";
            hand.at(0).display();
            cout << "     ";
            hand.at(1).display();
            cout << endl;
            value += 10;
            cout << "DEALER revealed a BLACKJACK!" << endl;
            for (int i = 0; i < insurances.size(); i++) {
                banks.at(i) += (insurances.at(i) * 3);
            }
            cout << "Insurances have been payed out." << endl;
            return true;
        }
    }
    return false;
}

//Dealer hits until >=17
int deckTurnTwo(vector<Card> &hand, Deck &drawDeck){
    int value = valueOfHand(hand, 0);
    while (value < 17) {
        hit(hand, drawDeck);
        value = valueOfHand(hand, 0);
    }
    if (value > 21) {
        value = 0;
        cout << endl << "Dealer busted." << endl;
    }
    else
        cout << "YOU MUST DEFEAT THE DEALER'S " << value << endl;
    return value;
}

//Computer's Turn which automates the same options as player
int compOneTurn(vector<vector<Card>> &hands, Deck &deck, vector<int> &betMoney, vector<int> &balance, short int currentHands, vector<int> &bank) {
    int r;
    short int value = 0;
    short int startVal;
    
    srand((int)time(0));
    
    //Assigns the hand as the current run of the loop
    vector<Card> hand = hands.at(currentHands);
    startVal = checkForDoubleAces(hand, 2);
    if (startVal == 0) {
        startVal = valueOfHand(hand, 2);
    }
    value = startVal;
    cout << "Computer One has a starting hand of: ";
    hand.at(0).display();
    cout << "     ";
    hand.at(1).display();
    cout << endl;
    if (startVal < 21) {
        value = startVal;
        
        //Splits and hits if able
        if(checkForSplitAvailibility(hands, hand, 2)){
            hands = splitHand(hands, hand, deck, balance, betMoney, 1);
            hand = hit(hand, deck);
        }
        // 1 in 10 chance of doubling hand
        if (canDouble(hands, hand, betMoney, balance, 2)){
            r = rand() % 10;
            if (r == 1) {
                doubleBet(betMoney, 1, bank);
            }
        }
        
        if (hand.at(0).getVal() == 1 || hand.at(1).getVal() == 1) {
            value += 10;
        }
        
        //Keeps hitting until above or equal to 16
        if (value <= 21 && value <= 17){
            do {
                hand = hit(hand,deck);
                cout << "Their new hand is: ";
                for (int k = 0; k < hand.size(); k++) {
                    hand.at(k).display();
                    cout << "     ";
                }
                cout << endl;
                
                //Adjusts values as needed
                if (hand.at(hand.size()-1).getVal() > 10) {
                    value += 10;
                }
                else if ((hand.at(hand.size()-1).getVal() == 1) && value < 11) {
                    value += 10;
                }
                else
                    value = value + hand.at(hand.size()-1).getVal();
            } while (value <= 17);
        }
        
    }
    cout << "Computer One has a hand value of " << value << endl << endl << endl;
    return value;
}

//Computer Two's turn
int compTwoTurn(vector<vector<Card>> &hands, Deck &deck, vector<int> &betMoney, vector<int> &balance, short int currentHands, vector<int> &bank) {
    int r;
    short int value = 0;
    short int startVal;
    
    srand((int)time(0));
    
    //Assigns the hand as the current run of the loop
    vector<Card> hand = hands.at(currentHands);
    startVal = checkForDoubleAces(hand, 3);
    if (startVal == 0) {
        startVal = valueOfHand(hand, 3);
    }
    value = startVal;
    if (value < 21) {
        //Splits and hits if able
        if(checkForSplitAvailibility(hands, hand, 3)){
            hands = splitHand(hands, hand, deck, balance, betMoney, 2);
            hand = hit(hand, deck);
        }
        // 1 in 10 chance of doubling hand
        if (canDouble(hands, hand, betMoney, balance, 3)){
            r = rand() % 10;
            if (r == 1) {
                doubleBet(betMoney, 2, bank);
            }
        }
        
        if (hand.at(0).getVal() == 1 || hand.at(1).getVal() == 1) {
            value += 10;
        }
        
        //Keeps hitting until 18 or more
        if (value <= 21 && value < 18){
            do {
                hand = hit(hand,deck);
                //Adjusts values as needed
                if (hand.at(hand.size()-1).getVal() > 10) {
                    value += 10;
                }
                else if ((hand.at(hand.size()-1).getVal() == 1) && value < 11) {
                    value += 10;
                }
                else
                    value = value + hand.at(hand.size()-1).getVal();
            } while (value < 18 && value <= 21);
        }
    }
    
    for (int y = 0; y < hand.size(); y++) {
        hand.at(y).display();
        cout << "     ";
    }
    cout << endl << "Comp Two has a hand with a value of: " << value << endl << endl << endl;
    return value;
}

//Player's turn
int playerTurn(vector<vector<Card>> &hands, Deck &deck, vector<int> &betMoney, vector<int> &balance, short int currentHands, vector<int> &bank) {
    short int value = 0;
    short int startVal;
    string s;
    
    cout << "Your Hand: ";
    for (int z = 0; z < 2; z++) {
        hands.at(currentHands).at(z).display();
        cout << "     ";
    }
    cout << endl;
    
    vector<Card> hand = hands.at(currentHands);
    startVal = checkForDoubleAces(hand, 1);
    if (startVal == 0) {
        startVal = valueOfHand(hand, 1);
    }
    value = startVal;
    
    if (hand.at(0).getVal() == 1 || hand.at(1).getVal() == 1) {
        value += 10;
    }
   
    cout << "You have $" << betMoney.at(0) << " in the pot, and $" << balance.at(0) << " left.\n";
    do {
        split:
        if (checkForSplitAvailibility(hands, hand, 1) == true) {
            do{
                cout << "You have the ability to split your hand. Type 'help' for explanation. Split? (y/n)";
                cin >> s;
                if (s == "help") {
                    cout << "Splitting is a mechanic used in blackjack to let you create entirely new hands if you "
                    << "have two cards with the same value\n. If you do this you will be creating a new equal size bet " <<
                    "on the new hand; and you can hit, stand, double and even split on the new hand!\n" << "If your split is " <<
                    "a double ace, you may only draw a card once for each new hand." << endl;
                }
                else if (s == "y") {
                    //I'm sorry, I got lazy, I have spent too much time on this.
                    goto splitHand;
                }
                else if (s == "n") {
                    
                }
                else {
                    cout << "Invalid." << endl;
                }
            } while ((s != "y") && (s != "n"));
        }
        else {
            cout << "Your current value is: " << value << ". Would you like to [double], [hit], or [stand]? ";
            cin >> s;
        }
    
        if (s == "split") {
            splitHand:
            hands = splitHand(hands, hand, deck, bank, betMoney, 0);
            
        }
        else if (s == "double") {
            doubleBet(betMoney, 0, bank);
            hand = hit(hand, deck);
        }
        else if (s == "hit") {
            hand = hit(hand, deck);
        }
        else if (s == "stand") {
            //Laziness
            goto endFunction;
        }
    } while (s != "split" && s != "double" && s != "hit" && s != "stand"); //While loop just used for input validation
    
    escape:
    //Value is updated with the last card
    value += hand.at(hand.size()-1).getVal();
    cout << "Your Hand: ";
    for (int l = 0; l < hand.size(); l++) {
        hand.at(l).display();
        cout << "     ";
    }
    cout << endl;
    
    //While loop that repeats as long as player does not exceed 21 and doesnt say stand
    while ((s != "stand") && value <= 21) {
        cout << "You have a current hand value of " << value << ". Would you like to [hit] or [stand]? ";
        cin >> s;
        if (s == "hit") {
            hand = hit(hand,deck);
            if (hand.at(hand.size()-1).getVal() > 10) {
                value += 10;
            }
            else if ((hand.at(hand.size()-1).getVal() == 1) && value < 11) {
                value += 10;
            }
            else
                value += hand.at(hand.size()-1).getVal();
        }
        cout << "Your New Hand: ";
        for (int a = 0; a < hand.size(); a++) {
            hand.at(a).display();
            cout << "     ";
        }
        cout << endl;
    }
    cout << "Ending Value: " << value << endl << endl << endl;
endFunction:
    return value;
};

void displayTable(){
    
}

void declareBanks(vector<int> &bank) {
    cout << "Remaining Bank Totals: " << endl << endl;
    cout << "Your Bank: $" << bank.at(0) << endl;
    cout << "Computer One Bank: $" << bank.at(1) << endl;
    cout << "Computer Two Bank: $" << bank.at(2) << endl;
}

bool isBlackjack(vector<vector<Card>>&, int) {
    //todo
    return false;
}
void calculatePayOut(vector<vector<Card>>, vector<int>&, vector<int>&, bool) {
    //todo, need to copy and paste code into here, but will clean up
}

//Prompts player if they want to play again
bool playAgainPrompt() {
    string s;
    bool play = false;
    do {
        cout << "Would you like to play again? (y/n)";
        cin >> s;
    } while ((s != "y") && (s != "n"));
    
    if (s == "y")
        play = true;
    else if (s == "n")
        play = false;

    return play;
}

//Function clears all point vectors, and all hands, while also checking to see if the deck needs to be recreated/shuffled
void clearRound (vector<Card> &dealer, vector<vector<Card>> &player, vector<vector<Card>> &compOne, vector<vector<Card>> &compTwo, Deck &deck, vector<int> &pointsP, vector<int> &pointsOne, vector<int> &pointsTwo, vector<int> &wins){
    //todo clear points
    int j = globalCardDraw(false);
    dealer.clear();
    player.clear();
    compOne.clear();
    compTwo.clear();
    pointsP.clear();
    pointsOne.clear();
    pointsTwo.clear();
    wins.clear();
    if (j >= 22) {
        shuffleTheDeck(deck);
    }
    for (int i = 0; i< 8; i++) {
        globalCardDraw(false);
    }
}

//Function is effectively a global counter to say how many cards are drawn, j is defaulted and lets the function be used as a getter
int globalCardDraw(bool passed, int j) {
    static int i;
    bool restart = passed;
    if (restart == false)
        i++;
    else if (restart == true)
        i = 0;
    if (j != 0)
        i--;
    restart = false;
    return i;
}

//Function deletes remaining cards in deck, and recreates a new deck
void shuffleTheDeck(Deck &deck) {
    vector<Card> discard;
    discard = deck.dealCards(deck.getSize());
    cout << "Deck has drawn enough cards that it needs to be reshuffled first." << endl;
    deck.clearDeck();
    deck.newDeck();
    globalCardDraw(true);
}

//Determines how many hands are winning hands for each player
vector<int> calculateNumberOfWins(vector<int> one, vector<int> two, vector<int> player, int deckVal){
    vector<int> wins;
    
    for (int r = 0; r < 3; r++){
        wins.push_back(0);
    }
    for (int i = 0; i < one.size(); i++) {
        if ((one.at(i) > deckVal) && (one.at(i) <= 21)) {
            wins.at(1) = wins.at(1) + 1;
        }
    }
    for (int j = 0; j < two.size(); j++) {
        if ((two.at(j) > deckVal) && (two.at(j) <= 21)) {
            wins.at(2) = wins.at(2) + 1;
        }
    }
    for (int p = 0; p < player.size(); p++) {
        if ((player.at(p) > deckVal) && (player.at(p) <= 21)) {
            wins.at(0) = wins.at(0) + 1;
        }
    }
    return wins;
}

void terminateGame(){
    exit(0);
}

//boolean for computer players to see if they have enough money to continue playing
bool canPlay(vector<int> bank, int playerNum){
    bool play = true;
    if (bank.at(playerNum) < 10) {
        play = false;
    }
    return play;
}

//tracks which computer players are out of the game
void staticBoolPlayers (int botOut) {
    static bool botOne;
    static bool botTwo;
    int i = 0;
    
    if (i == 0) {
        botOne = true;
        botTwo = true;
        i++;
    }
    
    if (botOut == 1) {
        cout << "BOT ONE BOOL SET!" << endl;
        botOne = false;
    }
    if (botOut == 2) {
        cout << "BOT TWO BOOL SET!" << endl;
        botTwo = false;
    }
    
    if (botOne == false && botTwo == false) {
        cout << "Congrats! You won! Your opponents ran out of money!" << endl;
        terminateGame();
    }
}
