#pragma once

#include <random>
#include <vector>

constexpr char SUIT_PIKA_SYMBOL = 'p';
constexpr char SUIT_BUBEN_SYMBOL = 'b';
constexpr char SUIT_HRESTA_SYMBOL = 'x';
constexpr char SUIT_CHERVA_SYMBOL = 'c';


constexpr unsigned char SUIT_PIKA   = 1;
constexpr unsigned char SUIT_BUBEN  = 2;
constexpr unsigned char SUIT_HRESTA = 3;
constexpr unsigned char SUIT_CHERVA = 4;
constexpr unsigned char SUIT_NONE   = 0;


struct Card
{
	Card(const unsigned char& n, const unsigned char& suit) : m_data(0)
	{
		m_data |= (n << 4);
		m_data |= suit;
	}
	explicit Card(const unsigned char& card_data_bits) : m_data(card_data_bits) { }

	Card() = default;
	Card(const Card&) = default;
	Card& operator=(const Card&) = default;
	~Card() = default;


	int num() const { return m_data >> 4; }
	char suit() const
	{
		switch(suit_bin())
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
	unsigned char suit_bin() const { return m_data ^ (num() << 4); }
	void swap(Card& oth)
	{
		Card tmp(m_data);
		m_data = oth.m_data;
		oth.m_data = tmp.m_data;
	}

	private: unsigned char m_data;
};

#include <iostream>

std::ostream& operator<<(std::ostream& os, const Card& c)
{
	os << c.num() << c.suit();
	return os;
}

bool operator==(const Card& lhs, const Card& rhs)
{
	return (lhs.num() == rhs.num() and lhs.suit_bin() == rhs.suit_bin());
}

template <unsigned char N>
class CardDeck
{
	public:

		using size_t = unsigned char;
		using card_t = Card;

	public:

		CardDeck() = default;
		~CardDeck() = default;


		const card_t& operator[](const size_t& n) const
		{
			return m_deck[n];
		}

		card_t& operator[](const size_t& n) 
		{
			return const_cast<card_t&>(static_cast<const CardDeck&>(*this)[n]);
		}

		card_t get()
		{
			return m_deck[--m_size];
		}

		card_t top() const
		{
			return m_deck[m_size - 1];
		}

		const card_t* const get(size_t count)
		{
			m_size -= count;
			return &m_deck[m_size];
		}

		void clear() 
		{
			m_size = 0;
		}

		bool empty() const
		{
			return m_size == 0;
		}

		bool full() const
		{
			return m_size == m_capacity;
		}

		int size() const
		{
			return m_size;
		}

		size_t capacity() const
		{
			return m_capacity;
		}

		void shuffle() 
		{
			if(empty()) m_size = m_capacity;
			static std::default_random_engine re;
			std::uniform_int_distribution<size_t> uid;
			using param_t = std::uniform_int_distribution<size_t>::param_type;

			for(size_t i = m_size - 1; i > 0; i--)
				m_deck[i].swap(m_deck[uid(re, param_t(0, i))]);
		}

		void push(const card_t& card)
		{
			m_deck[m_size++] = card;
		}

		void push(const std::vector<Card>& cards)
		{
			for(const Card& c : cards)
				push(c);
		}

		void ClassicalInit()
		{
			for(size_t i = 6; i <= 14; ++i)
				for(size_t ii = 1; ii <= 4; ++ii)
					push(Card(i, ii));
		}

	private:

		const static size_t m_capacity = N;

		card_t m_deck[N];
		size_t m_size = 0;
};

