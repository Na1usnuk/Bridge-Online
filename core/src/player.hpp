#pragma once

#include <string>


class Player
{

	using str = std::string;
	using cstr = const str;
	using cstrr = const std::string&;
	//using strv = std::string_view;
	using points_t = int;

public:

	Player(cstrr nick, points_t points = 0) : m_nick(nick), m_points(points) { }

	cstr nick() const noexcept { return m_nick; }
	points_t points() const noexcept { return m_points; }
	void addPoints(points_t points) noexcept { m_points += points; }

private:

	str m_nick;
	points_t m_points;

};


bool operator== (const Player& lhs, const Player& rhs)
{
	return (lhs.nick() == rhs.nick());
}


