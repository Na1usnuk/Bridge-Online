#pragma once

#include <vector>
#include <string>
#include <initializer_list>
#include <iostream>


class Player
{
	public:

		using deck_t = CardDeck<18>;

	public:

		Player(const std::string& nick) : m_nick(nick) { }
		
		const std::string nick() const { return m_nick; }

		const Card* const ShowCards()
		{
			return &m_deck[0];
		}

		void TakeCard(const Card& card)
		{
			m_deck.push(card);
		}

		const Card PutCard(const unsigned char& card_id) 
		{
			m_deck[card_id].swap(m_deck[m_deck.size()-1]);
			return m_deck.get();
		}

		const Card PutCard()
		{
			return m_deck.get();
		}
		
		deck_t::size_t CardCount() const
		{
			return m_deck.size();
		}

		unsigned int GetPoints() const
		{
			return m_points;
		}

		void CalculatePoints(unsigned int (*calc_func)(const Card&)) 
		{
			while(!m_deck.empty())
				m_points += calc_func(m_deck.get());
			m_deck.clear();
		}

		Player() = default;
		~Player() = default;
		Player& operator=(const Player&) = default;
		Player(const Player&) = default;

	private:

		std::string m_nick;
		deck_t m_deck;
		unsigned int m_points = 0;


};


std::ostream& operator<< (std::ostream& os, const Player& p)
{	
	os << p.nick();
	return os;
}	

