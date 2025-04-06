#pragma once

#include <stdint.h>
#include <unordered_set>
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

	card_t OnTopCard() 
	{ 
		if(!m_currentTurn.size())
			return m_game->HeapTopCard(); 
		return m_currentTurn.back();
	}

	player_t CurrentTurn() { return m_plist[m_game->CurrentTurn()]; }

	cardcontainer_t GetPlayerCards(player_t player)
	{
		int id;
		for (int i = 0; i < m_plist.size(); i++)
			if (m_plist[i] == player)
				id = i;

		

		auto c = m_game->GetPlayerCards(id);

		if(m_currentTurn.size())
		{
			
			cardcontainer_t tmp;
			tmp.reserve(c.size());
			for (card_t card : c)
				if (std::find(m_currentTurn.begin(), m_currentTurn.end(), card) == m_currentTurn.end())
					tmp.push_back(card);

			using std::swap;
			swap(tmp, c);
		}

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

	bool IsGameEnded()
	{
		for (const auto& p : ListOfPlayers())
			if (GetPlayerCards(p).size() == 0)
				return true;
		return false;
	}


private:

	cardcontainer_t m_currentTurn;
	gameptr_t m_game;
	plist_t m_plist;

};



