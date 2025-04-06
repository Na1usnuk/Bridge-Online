#include <boost\python.hpp>
#include "BridgeCore.h"
using namespace boost::python;

BOOST_PYTHON_MODULE(BridgeCore)
{
	class_<Player>("BridgePlayer")
		.def("nick", &Player::nick)
		.def("points", &Player::points)
		.def("addPoints", &Player::addPoints)
		;

	class_<Session>("BridgeSession")
		.def("StartGame", &Session::StartGame)
		.def("AddPlayer", &Session::AddPlayer)
		.def("CurrentTurn", &Session::CurrentTurn)
		.def("OnTopCard", &Session::OnTopCard)
		.def("GetPlayerCards", &Session::GetPlayerCards)
		.def("PlayerPickCard", &Session::PlayerPickCard)
		.def("EndTurn", &Session::EndTurn)
		.def("Turn", &Session::Turn)
		;
}