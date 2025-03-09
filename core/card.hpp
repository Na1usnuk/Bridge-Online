#pragma once


constexpr char SUIT_PIKA_SYMBOL = 'p';
constexpr char SUIT_BUBEN_SYMBOL = 'b';
constexpr char SUIT_HRESTA_SYMBOL = 'x';
constexpr char SUIT_CHERVA_SYMBOL = 'c';


constexpr unsigned char SUIT_PIKA   = 1;
constexpr unsigned char SUIT_BUBEN  = 2;
constexpr unsigned char SUIT_HRESTA = 3;
constexpr unsigned char SUIT_CHERVA = 4;
constexpr unsigned char SUIT_NONE   = 0;


class Card
{
public:

	using byte = unsigned char;
	using cbyte = const byte;
	using byter = byte&;
	using cbyter = const byte&;

public:

	Card(cbyter, cbyter);

	explicit Card(cbyter card_data_bits) : m_data(card_data_bits) { }

	Card() = default;
	Card(const Card&) = default;
	Card(Card&&) noexcept = default;
	Card& operator=(const Card&) = default;
	~Card() = default;


	int num() const { return m_data >> 4; }

	char suit() const;

	unsigned char suit_bin() const { return m_data ^ (num() << 4); }

	void swap(Card&) noexcept;

private: 

	unsigned char m_data;
};


void Card::swap(Card& oth) noexcept
{
	Card tmp(m_data);
	m_data = oth.m_data;
	oth.m_data = tmp.m_data;
}


Card::Card(cbyter n, cbyter suit) : m_data(0)
{
	m_data |= (n << 4);
	m_data |= suit;
}


char Card::suit() const
{
	switch (suit_bin())
	{
	case SUIT_PIKA:
		return SUIT_PIKA_SYMBOL;
	case SUIT_BUBEN:
		return SUIT_BUBEN_SYMBOL;
	case SUIT_HRESTA:
		return SUIT_HRESTA_SYMBOL;
	case SUIT_CHERVA:
		return SUIT_CHERVA_SYMBOL;
	default:
		return '\0';
	}
	return '\0';
}


bool operator==(const Card& lhs, const Card& rhs)
{
	return (lhs.num() == rhs.num() and lhs.suit_bin() == rhs.suit_bin());
}


namespace std
{
	void swap(Card& lhs, Card& rhs)
	{
		lhs.swap(rhs);
	}
}


#ifndef NDEBUG
#include <iostream>

std::ostream& operator<<(std::ostream& os, const Card& c)
{
	os << c.num() << c.suit();
	return os;
}
#endif



