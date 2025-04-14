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

	Session(gameptr_t game) : m_game(std::move(game)), m_is_game_started(false) { }

	Session() : Session(std::make_unique<ClassicalBridge>()) { }

	virtual ~Session() = default;

	void AddPlayer(const std::string& nick) {  m_plist.emplace_back(nick); }

	plist_t ListOfPlayers() const { return m_plist; }

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

	void StartGame() { if (!m_is_game_started) { m_currentTurn.push_back(m_game->Start(m_plist.size())); m_is_game_started = true; } }

	card_t OnTopCard() 
	{ 
		if(!m_currentTurn.size())
			return m_game->HeapTopCard(); 
		return m_currentTurn.back();
	}

	player_t CurrentTurn() { return m_plist[m_game->CurrentTurn()]; }

	cardcontainer_t GetPlayerCards(const std::string& player)
	{
		int id;
		for (int i = 0; i < m_plist.size(); i++)
			if (m_plist[i].nick() == player)
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

	bool IsGameStarted() const { return m_is_game_started; }

	int PlayersCount() const { return m_plist.size(); }

	int PlayerCardsCount(const std::string& nick) 
	{ 
		if(m_is_game_started)
			return GetPlayerCards(nick).size();
		return 0;
	}

	bool IsPlayerPlaying(const std::string& str) const 
	{ 
		for (auto p : m_plist)
			if (p.nick() == str)
				return true;
		return false;
	}

	bool IsGameEnded()
	{
		return false;
	}


private:

	cardcontainer_t m_currentTurn;
	gameptr_t m_game;
	plist_t m_plist;
	bool m_is_game_started;

};



