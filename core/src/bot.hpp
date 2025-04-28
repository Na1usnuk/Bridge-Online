
#include "card.hpp"
#include "player.hpp"
#include <vector>


class TurnTree
{
	using card_t = Card;

	struct Node 
	{
		card_t val;
		std::vector<*Node> childs;
	};

	public:

		TurnTree() : m_Root(

		TurnTree(const card_t rootCard) : m_Depth(1)
		{
		}



		int Depth() const { return m_Depth; }


	private:

	int m_Depth
	Node* m_Root;

}



class Bot : public Player
{
	public:

		using card_set_t = std::vector<Card>;

		Bot() = default;

		
		std::vector<Card> GetTurn(const Card heapCard, const card_set_t& myCards)
		{
			std::vector<card_set_t> possibleTurns;
			
			card_set_t possibleFirsCards = PossibleFirstCards(heapCard, myCards);

			for(const auto c : possibleFirstCards)
			{

			}

		}

	private:

		std::vector<card_set_t> ExistingTurns(const card_set_t& firstCards, const card_set_t& cards)
		{
			std::vector<card_set_t> possibleTurns; 

			for
		}


		card_set_t PossibleCards(const Card topCard, const card_set_t& cards)
		{
			card_set_t possibles;

			for(const auto c : cards)
			{
				if(c.num() == 9 ||
				   c.num() == topCard.num() ||

			}
		}

		card_set_t PossibleFirstCards(const Card topCard, const std::vector<Cards> cards)
		{
			std::vector<Card> possibleFirsCards;

			for(const auto c : myCards)
				if(c.num() == topCard.num() ||
				   c.suit() == topCard().suit() ||
				   c.num() == Card::VALET)
					possibleFirtCards.push_back(c);

			return possibleFirsCards;
		}


	private:
		
};
