#include <iostream>
#include "Card.h"

using namespace std;

Card::Card(char s,int v)
{
	suit=s;
	value=v;
}

char Card::getSuit()
{
	return suit;
}

int Card::getVal()
{
	return value;
}

void Card::display()
{
	if(value<11&&value>1)
		cout<<value<<suit;
    else if(value==11)
		cout<<"J"<<suit;
	else if(value==12)
		cout<<"Q"<<suit;
	else if(value==13)
		cout<<"K"<<suit;
	else
		cout<<"A"<<suit;
}


//***********************************************************************
//Do Not Modify the code in between these comments
//***********************************************************************
/*class Card
 {
 public:
 Card(char suit,int value);
 char getSuit();
 int getVal();
 void display();
 
 private:
 char suit;
 int value;
 };
 
 Card::Card(char s,int v)
 {
 suit=s;
 value=v;
 }
 
 char Card::getSuit()
 {
 return suit;
 }
 
 int Card::getVal()
 {
 return value;
 }
 
 void Card::display()
 {
 if(value<11&&value>1)
 cout<<value<<suit<<endl;
 else if(value==11)
 cout<<"J"<<suit<<endl;
 else if(value==12)
 cout<<"Q"<<suit<<endl;
 else if(value==13)
 cout<<"K"<<suit<<endl;
 else
 cout<<"A"<<suit<<endl;
 }
 
 class Deck
 {
 public:
 Deck();
 int getSize();
 void shuffle();
 void display();
 vector<Card> dealCards(int numCards);
 
 private:
 vector<Card> deck;
 Card switcher;
 };
 
 Deck::Deck()
 :switcher(3,1)
 {
 for(int i=1;i<14;i++)
 {
 for(int j=0;j<4;j++)
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
 Card c('D', i);
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
 for(int i=0;i<deck.size();i++)
 {
 int random=rand()%deck.size();
 switcher=deck[i];
 deck[i]=deck[random];
 deck[random]=switcher;
 }
 }
 
 void Deck::display()
 {
 for(int i=0;i<deck.size();i++)
 {
 deck[i].display();
 }
 }
 
 vector<Card> Deck::dealCards(int numCards)
 {
 vector<Card> cardsDealt;
 for(int i=0;i<numCards;i++)
 {
 cardsDealt.push_back(deck[deck.size()-1]);
 deck.pop_back();
 }
 return cardsDealt;
 }
 */
//***********************************************************************
