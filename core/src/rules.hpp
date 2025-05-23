#pragma once

#include <stack>
#include <list>
#include <random>
#include <utility>
#include <iostream>
#include <algorithm>
#include <cassert>

#include "card.hpp"
//#include "bridge_exceptions.hpp"

class Bridge
{
	public:

		using bridge_exception = std::exception;
		using card_t = Card;
		using cardcontainer_t = std::vector<card_t>;
		using deck_t = cardcontainer_t;
		using plist_t = std::vector<cardcontainer_t>;
		using num_t = short;
		using unum_t = unsigned short;

	public:

		Bridge() : m_turn(0), m_cardTaken(0) { }

		virtual ~Bridge() = default;

		virtual card_t Start(num_t num_of_players, num_t num_of_cards = 5) = 0;

		void PlayerTakeCards(num_t, num_t);

		void PlayerTakeCards(num_t);

		virtual bool HandleTurn(const cardcontainer_t&) = 0;

		virtual bool HandleResponce() = 0;

		virtual int Looser() 
		{
			for (int i = 0; i < m_plist.size(); ++i)
				if (m_plist[i].size() < 0)
					return i;
			return -1;
		}

		card_t HeapTopCard() 
		{
			if(m_hot.size())
				return m_hot.back(); 
			if(m_heap.size())
				return m_heap.back();
			return card_t(0, 0);
		}

		card_t HotCard() { if (!IsHotCardsResponced()) return m_hot.back(); return card_t(); }

		num_t CurrentTurn() { return m_turn; }

		const cardcontainer_t& GetPlayerCards(num_t pid)
		{
			return m_plist[pid];
		}

		const cardcontainer_t& GetPlayerCards()
		{
			return GetPlayerCards(m_turn);
		}

		virtual bool CanPlayerTakeCard() = 0;

		virtual unum_t DeckCapacity() noexcept = 0;

		void SetCardTakenFlag()
		{
			m_cardTaken = true;
		}

		bool IsGameEnded() 
		{
			int currentlyPlaying = 0;
			for (auto p : m_plist)
				if (p.size())
					++currentlyPlaying;
			return currentlyPlaying == 1;
		}

		virtual unum_t CalculatePoints(const card_t&) noexcept;

		void ClearAll()
		{
			for (auto& c : m_plist)
				c.clear();
			m_deck.clear();
			m_heap.clear();
			m_hot.clear();
			m_cardTaken = false;
		}

	protected:

		void RemovePlayerCards(const cardcontainer_t& cards);

		virtual void DeckInit() noexcept = 0;

		card_t GetCard() noexcept
		{ 
			card_t c = m_deck.back(); 
			m_deck.pop_back(); 
			return c; 
		}

		virtual void ShuffleDeck() noexcept;

		int Turn()
		{
			return m_turn;
		}

		void IncrementTurn()
		{
			if (++m_turn == m_plist.size())
				m_turn = 0;
			m_cardTaken = false;
			if (!GetPlayerCards().size() && !IsGameEnded())
				IncrementTurn();
		}

		void IncrementTurn(num_t n)
		{
			while(--n)
				IncrementTurn();
		}

		bool IsHotCardsResponced() { return m_hot.size() == 0; }


	protected:

		plist_t m_plist;
		deck_t m_deck;
		deck_t m_heap;
		deck_t m_hot;
		num_t m_turn;
		bool m_cardTaken;
};


class ClassicalBridge : public Bridge
{
public:

	card_t Start(num_t num_of_players, num_t num_of_cards = 5) override;

	void DeckInit() noexcept override;

	unum_t DeckCapacity() noexcept override { return 36; }

	bool HandleTurn(const cardcontainer_t&) override;

	bool HandleResponce() override;

	bool CanPlayerTakeCard() override;

	void ClearResponceQueue();

	bool PlayerHave(num_t turn, card_t card);

	bool PlayerHave(card_t card);

	bool PlayerHave(const cardcontainer_t& cards);

};

bool ClassicalBridge::PlayerHave(const cardcontainer_t& cards)
{
	for (card_t c : cards)
		if (!PlayerHave(c))
			return false;
	return true;
}

bool ClassicalBridge::PlayerHave(num_t turn, card_t card)
{
	for (card_t c : m_plist[turn])
		if (card == c)
			return true;
	return false;
}

bool ClassicalBridge::PlayerHave(card_t card)
{
	return PlayerHave(m_turn, card);
}

void ClassicalBridge::ClearResponceQueue()
{
	while(m_hot.size())
	{
		m_heap.push_back(m_hot.back());
		m_hot.pop_back();
	}
}

bool ClassicalBridge::CanPlayerTakeCard()
{
	if (HeapTopCard().num() == 6 || HotCard().num() == 7 || HotCard().num() == 8)
		return true;
	if (!m_cardTaken)
	{
		m_cardTaken = true;
		return true;
	}
	return false;
}

Bridge::card_t ClassicalBridge::Start(num_t num_of_players, num_t num_of_cards)
{
	DeckInit();
	ShuffleDeck();
	m_plist.resize(num_of_players);
	for (auto& p : m_plist)
		p.reserve(10);
	while (num_of_players--)
		PlayerTakeCards(num_of_players, num_of_cards);
	//m_heap.push_back(m_plist[turn].back());
	return m_plist[m_turn].back();
}

bool ClassicalBridge::HandleResponce()
{
	if(!IsHotCardsResponced())
	{
		if(HotCard().num() == 7)
		{
			PlayerTakeCards(m_hot.size());
			IncrementTurn();
		}
		if(HotCard().num() == 8)
			PlayerTakeCards(2 * m_hot.size());
		ClearResponceQueue();
		return true;
	}
	return false;
}

bool ClassicalBridge::HandleTurn(const cardcontainer_t& cards)
{
	if (!cards.size())
	{
		HandleResponce();
		if (!CanPlayerTakeCard())
			IncrementTurn();
		return true;
	}

	if (!PlayerHave(cards))
		return false;

	if (!IsHotCardsResponced())
	{
		for (card_t c : cards)
			if (c.num() != HotCard().num())
				return false;
		for(int i = cards.size() - 1; i >= 0; i--)
			m_hot.push_back(cards[i]);
		RemovePlayerCards(cards);
		IncrementTurn();
		return true;
	}


	if(cards.back().num() == 9 || cards.back().num() == 6)
		return false;

	
	bool needToChooseSuit = false;
	card_t prev = HeapTopCard();
	int i = 0;
	if (!prev)
	{
		prev = cards[0];
		++i;
		goto Checking;
	}

	if ((cards.front().num() != prev.num()) && (cards.front().suit() != prev.suit()) && cards.front().num() != 11)
		return false;

Checking:

	std::cout << "Checking" << std::endl;

	prev = cards.front();

	for (; i < cards.size(); i++)
	{
		std::cout << "Checking " << cards[i] << std::endl;
		switch (cards[i].num())
		{
		case 14:
		{
			if ((cards[i].num() != prev.num()) && (prev.num() != 6 && cards[i].suit() != prev.suit()) && prev.num() != 9)
				return false;

			int inc = i;
			short toSkip = 0;
			while (cards[inc++].num() == 14 && inc < cards.size())
				toSkip++; 

			toSkip %= m_plist.size() - 1;

			if (inc == cards.size() - 1)
			{
				IncrementTurn(toSkip);
				goto Accepted;
			}
			
			if (toSkip)
				return false;

			i = inc-1;

			break;
		}
		case 11:
		{
			if(prev.num() != 11) return false;
			break;
		}
		default:
		{
			if ((cards[i].num() != prev.num()) && (prev.num() == 6 && cards[i].suit() != prev.suit()) && prev.num() != 9)
				return false;
			break;
		}
		}
		prev = cards[i];
	}

Accepted:
	std::cout << "Cards ACCEPTED" << std::endl;
	for (int i = 0; i < cards.size(); i++)
		cards[i].num() == 7 || cards[i].num() == 8 ? m_hot.push_back(cards[i]) : m_heap.push_back(cards[i]);

	RemovePlayerCards(cards);
	
	IncrementTurn();

	return true;
}

void Bridge::RemovePlayerCards(const cardcontainer_t& cards)
{
	cardcontainer_t& pcards = m_plist[m_turn];
	cardcontainer_t tmp;
	tmp.reserve(cards.size());
	for (card_t card : pcards)
		if (std::find(cards.begin(), cards.end(), card) == cards.end())
			tmp.push_back(card);

	using std::swap;
	swap(tmp, pcards);
}

void Bridge::ShuffleDeck() noexcept
{
	static std::default_random_engine re;
	std::uniform_int_distribution<size_t> uid;
	using param_t = std::uniform_int_distribution<size_t>::param_type;
	using std::swap;
	for (std::size_t i = m_deck.size() - 1; i > 0; i--)
		m_deck[i].swap(m_deck[uid(re, param_t(0, i))]);
}

void ClassicalBridge::DeckInit() noexcept
{
	for (num_t i = 6; i <= 14; ++i)
		for (num_t ii = 1; ii <= 4; ++ii)
			m_deck.emplace_back(i, ii);
}

bool Bridge::CanPlayerTakeCard()
{
	return !m_cardTaken;
}

void Bridge::PlayerTakeCards(num_t player_turn, num_t num_of_cards)
{
	while(num_of_cards--)
		m_plist[player_turn].push_back(GetCard());
}

void Bridge::PlayerTakeCards(num_t num_of_cards)
{
	PlayerTakeCards(m_turn, num_of_cards);
}

Bridge::unum_t Bridge::CalculatePoints(const card_t& card) noexcept
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
}
