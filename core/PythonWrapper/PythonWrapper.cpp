#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "../BridgeCore.h"


using namespace boost::python;

typedef std::vector<Player> playerList;
typedef std::vector<Card> cardList;

BOOST_PYTHON_MODULE(BridgeGame)
{
	class_<playerList>("playerList")
		.def(vector_indexing_suite<playerList>());

	class_<cardList>("cardList")
		.def(vector_indexing_suite<cardList>());

	class_<Player>("Player", init<std::string, int>())
		.def("nick", &Player::nick)
		.def("points", &Player::points)
		.def("addPoints", &Player::addPoints)
		;

	class_<Card>("Card", init<std::string>())
		.def("num", &Card::num)
		.def("suit", &Card::suit)
		.def("raw", &Card::raw)
		.def("repr", &Card::repr)
		;

	class_<Session, boost::noncopyable>("BridgeSession")
		.def("StartGame", &Session::StartGame)
		.def("AddPlayer", &Session::AddPlayer)
		.def("CurrentTurn", &Session::CurrentTurn)
		.def("OnTopCard", &Session::OnTopCard)
		.def("ListOfPlayers", &Session::ListOfPlayers)
		.def("GetPlayerCards", &Session::GetPlayerCards)
		.def("PlayerPickCard", &Session::PlayerPickCard)
		.def("EndTurn", &Session::EndTurn)
		.def("Turn", &Session::Turn)
		.def("IsGameStarted", &Session::IsGameStarted)
		.def("PlayersCount", &Session::PlayersCount)
		.def("IsPlayerPlaying", &Session::IsPlayerPlaying)
		.def("PlayerCardsCount", &Session::PlayerCardsCount)
		.def("IsGameEnded", &Session::IsGameEnded)
		.def("EndGame", &Session::EndGame)
		;
}
