#include <vector>
#include <ctime>
#include "Deck.h"


using namespace std;


Deck::Deck()
	:switcher(3, 1)
{
	for (int i = 1; i < 14; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (j == 0)
			{
				Card c('C', i);
				deck.push_back(c);
			}
			else if (j == 1)
			{
				Card c('D', i);
				deck.push_back(c);
			}
			else if (j == 2)
			{
				Card c('H', i);
				deck.push_back(c);
			}
			else {
				Card c('S', i);
				deck.push_back(c);
			}
		}
	}
}

void Deck::clearDeck() {
    deck.clear();
}

void Deck::newDeck() {
    for (int i = 1; i < 14; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (j == 0)
            {
                Card c('C', i);
                deck.push_back(c);
            }
            else if (j == 1)
            {
                Card c('D', i);
                deck.push_back(c);
            }
            else if (j == 2)
            {
                Card c('H', i);
                deck.push_back(c);
            }
            else {
                Card c('S', i);
                deck.push_back(c);
            }
        }
    }
}

int Deck::getSize()
{
	return deck.size();
}

void Deck::shuffle()
{
	srand(time(0));
	for (int i = 0; i < deck.size(); i++)
	{
		int random = rand() % deck.size();
		switcher = deck[i];
		deck[i] = deck[random];
		deck[random] = switcher;
	}
}

void Deck::display()
{
	for (int i = 0; i < deck.size(); i++)
	{
		deck[i].display();
	}
}

vector<Card> Deck::dealCards(int numCards)
{
	vector<Card> cardsDealt;
	for (int i = 0; i < numCards; i++)
	{
		cardsDealt.push_back(deck[deck.size() - 1]);
		deck.pop_back();
	}
	return cardsDealt;
}
