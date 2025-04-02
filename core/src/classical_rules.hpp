#include "rules_base.hpp"


class ClassicalRules : public rules_base
{
	public:

		void DeckInit() override;

	private:


};



void ClassicalRules::DeckInit() 
{
	for(int i = 0; i <= 14; ++i)
		for(int ii = 0; i <= 4; ++ii)
			m_deck.emplace(i, ii);
}
