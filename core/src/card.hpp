#pragma once

#include <sstream>


class Card
{
public:

	enum Suit : unsigned char
	{
		PIKA   = 1,
		BUBEN  = 2,
		HRESTA = 3,
		CHERVA = 4,
		NONE   = 0
	};

	enum Num
	{
		Joker = 1,
		Valet = 11,
		Dama = 12,
		King = 13,
		Tyz = 14
	};

	using byte = unsigned char;
	using num_t = char;
	using cbyte = const byte;
	using byter = byte&;
	using cbyter = const byte&;

public:

	Card(num_t n, num_t suit) : m_data(0)
	{
		init(n, suit);
	}

	Card(const std::string& card_str) : m_data(0)
	{
		num_t suit, n;

		if (card_str.find("spades") != std::string::npos)
			suit = Suit::PIKA;
		else if (card_str.find("diams") != std::string::npos)
			suit = Suit::BUBEN;
		else if (card_str.find("clubs") != std::string::npos)
			suit = Suit::HRESTA;
		else if (card_str.find("hearts") != std::string::npos)
			suit = Suit::CHERVA;

		switch (card_str.front())
		{
		case '1':
		{
			n = 10;
			break;
		}
		case 'J':
		{
			n = Num::Valet;
			break;
		}
		case 'Q':
		{
			n = Num::Dama;
			break;
		}
		case 'K':
		{
			n = Num::King;
			break;
		}
		case 'A':
		{
			n = Num::Tyz;
			break;
		}
		default:
			n = (card_str.front() - '0');
		}
		init(n, suit);
	}

	void init(num_t n, num_t suit)
	{
		m_data |= (n << 4);
		m_data |= suit;
	}

	explicit Card(byte card_data_bits) : m_data(card_data_bits) { }

	Card() : m_data(0) {}
	Card(const Card&) noexcept = default;
	Card(Card&&) noexcept = default;
	Card& operator=(const Card&) noexcept = default;
	~Card() = default;


	int num() const noexcept { return m_data >> 4; }

	byte suit() const noexcept { return m_data ^ (num() << 4); }

	void swap(Card&) noexcept;

	std::string repr() const
	{
		std::ostringstream oss;

		switch (num())
		{
		case Card::Num::Tyz:
		{
			oss << 'A';
			break;
		}
		case Card::Num::King:
		{
			oss << 'K';
			break;
		}
		case Card::Num::Dama:
		{
			oss << 'Q';
			break;
		}
		case Card::Num::Valet:
		{
			oss << 'J';
			break;
		}
		case 10:
		{
			oss << "10";
			break;
		}
		default:
			oss << char(num() + '0');
			break;
		}

		switch (suit())
		{
		case Card::Suit::BUBEN:
		{
			oss << 'd';
			break;
		}
		case Card::Suit::CHERVA:
		{
			oss << 'h';
			break;
		}
		case Card::Suit::HRESTA:
		{
			oss << 'c';
			break;
		}
		case Card::Suit::PIKA:
		{
			oss << 's';
			break;
		}
		}

		return oss.str();
	}

	byte raw() const { return m_data; }

	operator bool() { return m_data; }

private: 

	byte m_data;
};


void Card::swap(Card& oth) noexcept
{
	Card tmp(m_data);
	m_data = oth.m_data;
	oth.m_data = tmp.m_data;
}


bool operator==(const Card& lhs, const Card& rhs) noexcept
{
	return (lhs.num() == rhs.num() && lhs.suit() == rhs.suit());
}

bool operator!=(const Card& lhs, const Card& rhs)
{
	return !(lhs == rhs);
}


namespace std
{
	void swap(Card& lhs, Card& rhs) noexcept
	{
		lhs.swap(rhs);
	}
}





#include <iostream>
#include <cassert>
std::ostream& operator<<(std::ostream& os, Card c)
{
	char s_sym;

	switch (c.suit())
	{
	case Card::Suit::BUBEN:
	{
		s_sym = 4;
		break;
	}
	case Card::Suit::CHERVA:
	{
		s_sym = 3;
		break;
	}
	case Card::Suit::HRESTA:
	{
		s_sym = 5;
		break;
	}
	case Card::Suit::PIKA:
	{
		s_sym = 6;
		break;
	}
	default:
		s_sym = 0;
	}

	char s_num;

	switch (c.num())
	{
	case Card::Num::Tyz:
	{
		s_num = 'A';
		break;
	}
	case Card::Num::King:
	{
		s_num = 'K';
		break;
	}
	case Card::Num::Dama:
	{
		s_num = 'Q';
		break;
	}
	case Card::Num::Valet:
	{
		s_num = 'J';
		break;
	}
	case 10:
	{
		s_num = '1';
		break;
	}
	default:
		s_num = c.num() + '0';
		break;
	}

	os << s_num << s_sym;
	return os;
}
