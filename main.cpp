#include "core/BridgeCore.h"
#include <iostream>
#include <cstdlib>
#include <array>


#define print(x) std::cout<<x<<std::endl

std::pair<Session::card_t, bool> ConstructCard(const std::string & str)
{
	if (str.size() != 2)
	{
		print("Invalid card input.");
		return { Session::card_t(0,0), false};
	}
	char s_sym;
	switch (str.back())
	{
	case 'd':
	{
		s_sym = Card::Suit::BUBEN;
		break;
	}
	case 'h':
	{
		s_sym = Card::Suit::CHERVA;
		break;
	}
	case 's':
	{
		s_sym = Card::Suit::PIKA;
		break;
	}
	case 'c':
	{
		s_sym = Card::Suit::HRESTA;
		break;
	}
	}

	char s_num;

	switch (str.front())
	{
	case 'A':
	{
		s_num = 14;
		break;
	}
	case 'K':
	{
		s_num = 13;
		break;
	}
	case 'Q':
	{
		s_num = 12;
		break;
	}
	case 'J':
	{
		s_num = 11;
		break;
	}
	case '1':
	{
		s_num = 10;
		break;
	}
	default:
		s_num = str.front() - '0';
		break;
	}
	Session::card_t constructed(s_num, s_sym);

	std::cout << "CONSTRUCTED CARD: " << constructed << std::endl;

	return { constructed, true };
}

void CLIError(std::string str)
{
	//std::system("cls");
	std::cout << "\n\nError: " << str << std::endl;
	//std::system("pause");
	return;
}

void CLIBridge()
{
	Session s;

	s.AddPlayer("Nazik");
	s.AddPlayer("Maks");

	s.StartGame();


	print("Console Bridge game.");
	print("Currently you can play only with yourself.\n\n\n");
	//std::system("pause");

	bool gameShouldEnd = false;

	do
	{
		//std::system("cls");
		auto currentTurn = s.CurrentTurn();

		std::cout << "\t\tOn Heap: " << s.OnTopCard() << '\n';
		std::cout << "\n\n\nPlayer: " << currentTurn.nick() 
			  << "\nPoints: " << currentTurn.points() 
			  << "\n\nCards: ";

		for (const auto& c : s.GetPlayerCards(currentTurn.nick()))
			std::cout << c << ' ';
		std::cout << std::endl;

		std::cout << "\nInput card name (s - pika, c - hresta, d - buben, h - cherva)\n zero (0) to pick\n e for turn end\n\n Your input: ";
		std::string str;
		std::cin >> str;

		if (str == "0")
		{
			if (!s.PlayerPickCard())
				CLIError("You can`t take card");
			continue;
		}

		if (str == "e")
		{
			if (!s.EndTurn()) 
				CLIError("Invalid Turn");
			else if(s.IsGameEnded())
			{
				s.EndGame();
				s.CalculateAllPoints();
				std::cout << "\nGame End!\n\nContinue Playing? [y/n]:";
				std::string inp;
				std::cin >> inp;
				if(inp == "y")
					s.StartGame();
				else
					gameShouldEnd == true;

			}
			continue;
		}

		auto res = ConstructCard(str);

		if (!res.second)
		{
			CLIError("Invalid card input.");
			continue;
		}

		s.Turn(res.first);


		//gameShouldEnd = true;
	} while (!gameShouldEnd);

}


int main()
{

	CLIBridge();



}
