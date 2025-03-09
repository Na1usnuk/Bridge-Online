#include "core.hpp"

int main()
{	
	std::vector<Player> v;
	v.emplace_back("p1");
	v.emplace_back("p2");
	v.emplace_back("p3");

	Session ses(v);

	ses.Start();

	ses.End();
}
