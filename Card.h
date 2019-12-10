#pragma once
#ifndef card_h
class Card
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
#endif
