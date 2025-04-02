#pragma once

#include <stdint.h>
#include <unordered_map>
#include <vector>
#include <memory>
#include <utility>
#include "player.hpp"
#include "rules.hpp"


class Session
{
public:

	using card_t = Bridge::card_t;
	using gameptr_t = std::unique_ptr<Bridge>;
	using player_t = Player;
	using cardcontainer_t = std::vector<card_t>;
	using plist_t = std::vector<player_t>;

public:

	Session(gameptr_t game) : m_game(std::move(game)) { }

	Session() : m_game(std::make_unique<ClassicalBridge>()) { }

	virtual ~Session() = default;

	void AddPlayer(const std::string& nick) { m_plist.emplace_back(nick); }

	const plist_t& ListOfPlayers() const { return m_plist; }

	void Turn(card_t card) 
	{ 
		m_currentTurn.push_back(card); 
	}

	bool EndTurn()
	{
		if (m_game->HandleTurn(m_currentTurn))
		{
			m_currentTurn.clear();
			return true;
		}
		m_currentTurn.clear();
		return false;
	}

	void StartGame() { m_game->Start(m_plist.size()); }

	card_t OnTopCard() { return m_game->HeapTopCard(); }

	player_t CurrentTurn() { return m_plist[m_game->CurrentTurn()]; }

	cardcontainer_t GetPlayerCards(player_t player)
	{
		int id;
		for (int i = 0; i < m_plist.size(); i++)
			if (m_plist[i] == player)
				id = i;

		auto c = m_game->GetPlayerCards(id);
		if(m_currentTurn.size())
			for (int i = 0; i < c.size(); ++i)
				for (int ii = 0; ii < m_currentTurn.size(); ++ii)
					if (c[i] == m_currentTurn[ii])
						c.erase(c.begin() + i);
		return c;
	}

	bool PlayerPickCard()
	{
		if (!m_game->CanPlayerTakeCard())
			return false;

		if (!m_game->HandleResponce())
		{
			m_game->PlayerTakeCards(1);
			m_game->SetCardTakenFlag();
		}
		return true;
	}


private:

	cardcontainer_t m_currentTurn;
	gameptr_t m_game;
	plist_t m_plist;

};



