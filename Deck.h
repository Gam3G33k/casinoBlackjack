#pragma once
#ifndef deck_h

#include <iostream>
#include <vector>
#include "Card.h"

using namespace std;

class Deck
{
public:
    void clearDeck();
    void newDeck();
	Deck();
	int getSize();
	void shuffle();
	void display();
	vector<Card> dealCards(int numCards);

private:
	vector<Card> deck;
	Card switcher;
};
#endif
