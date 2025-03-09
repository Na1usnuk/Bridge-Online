#pragma once

#include <stdint.h>
#include <unordered_map>
#include <vector>
#include "player.hpp"
#include "card_deck.hpp"


class Session
{
	public:

		using byte = unsigned char;
		using cbyter = const byte&;
		using deck_t = CardDeck<36>;

	public:

		Session(const std::vector<Player>& pl) : m_plist(pl) { }

		void PlayerTakeCards(cbyter pid, cbyter count)
		{
			for(byte i = 0; i < count; ++i)
				m_plist[pid].TakeCard(m_deck.get());
		}

		bool HandleTurn(const std::vector<Card>& cards)
		{
			std::vector<Card> new_cards(cards.size());

			if(m_hot_cards.size())
			{
				if(cards.size())
					for(const Card& c : cards)
					{
						if(c == m_hot_cards[0])
							new_cards.push_back(c);
						else
							return false;
					}
				else
					ResponseRules(m_hot_cards.get(), m_hot_cards.size());

				HotCardsResponsed();
				m_heap.push(new_cards);
				return true;
			}

			for(byte i = 0; i < cards.size(); ++i)
				if(TurnRules(cards[i]))
					new_cards.push_back(cards[i]);
				else
					return false;
			m_heap.push(new_cards);
			return true;
		}

		void Start(cbyter player_turn_id = 0)
		{
			m_deck.ClassicalInit();
			m_deck.shuffle();
			for(byte i = 0; i < m_plist.size(); ++i)
				PlayerTakeCards(i, 5);

			m_heap.push(m_plist[player_turn_id].PutCard());
			m_pturn = player_turn_id;
		}


		void End()
		{
			for(Player& p : m_plist)
				p.CalculatePoints(&ClassicalPointsCalculation);
		}

	private:

		bool TurnRules(const Card& card)
		{
			const Card& top = m_heap.top();

			switch(top.num())
			{
				case 9:
				{
					m_heap.push(card);
					return true;
				}
				case 6:
				{
					if(card.suit_bin() == top.suit_bin() or card.num() == 11)
					{
						m_heap.push(card);
						return true;
					}
					return false;
				}
				default:
					if(card.suit_bin() == top.suit_bin() or card.num() == top.num() or card.num() == 11)
						return true;
			}
			return false;
		}

		void ResponseRules(const Card& card, cbyter count)
		{
			switch(card.num())
			{
				case 7:
					PlayerTakeCards(m_pturn++, 1 * count);
				case 8:
					PlayerTakeCards(m_pturn, 2 * count);
				case 4:
					PlayerTakeCards(m_pturn, 4 * count);
			}
		}

		static unsigned int ClassicalPointsCalculation(const Card& card)
		{
			switch(card.num())
			{
				case 14:
					return 15;
				case 13:
				case 12:
				case 10:
					return 10;
				case 11:
					return 25;
				case 5: 
					return 20;
				default: 
					return 0;
			}
			return 0;
		}

		void HotCardsResponsed()
		{
			for(byte i = 0; i < m_hot_cards.size(); i++)
				m_heap.push(m_hot_cards.get());
		}

	private:

		deck_t m_deck;
		deck_t m_heap;
		CardDeck<4> m_hot_cards;
		std::vector<Player> m_plist;
		byte m_pturn; 

};
