#include <stack>
#include <list>

#include "card.hpp"

class rules_base
{
	public:

		using card_t = Card;
		using deck_t = std::stack<card_t, std::list<card_t> >;

	public:

		virtual void DeckInit() = 0;



	protected:

		deck_t m_deck;
		deck_t m_heap;
};



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
