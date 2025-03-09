#include <stack>
#include <list>
#include <random>

#include "card.hpp"

class rules_base
{
	public:

		using card_t = Card;
		using deck_t = std::stack<card_t, std::list<card_t> >;

	public:

		virtual void DeckInit() = 0;

		virtual void ShuffleDeck();

		virtual unsigned int CalculatePoints(const card_t&) = 0;


	protected:

		deck_t m_deck;
		deck_t m_heap;
};


void rules_base::ShuffleDeck()
{
	
	static std::default_random_engine re;
	std::uniform_int_distribution<size_t> uid;
	using param_t = std::uniform_int_distribution<size_t>::param_type;
	using std::swap;
	for (int i = m_deck.size() - 1; i > 0; i--)
		swap(m_deck[i], m_deck[uid(re, param_t(0, i))]);
}


unsigned int Calculate(const Card& card)
{
	switch(card.num())
	{
		case 14: 
			return 15;
		case 13:
		case 12:
			return 10;
		case 11:
			return 25;
	}
	return 0;
}
