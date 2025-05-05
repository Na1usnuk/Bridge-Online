def choose_suit_callback():
    "Function that is called when needed to choose suit. Needs to return suit"
    return Card.Suit.NONE


class Card:

    class Suit:
        "Unicode symbols for card suits"
        NONE   = chr(0)
        Diams  = chr(9830) 
        Spades = chr(9824)
        Clubs  = chr(9827)
        Hearts = chr(9829)

    class Num:
        Jack  = 'J'
        Queen = 'Q'
        King  = 'K'
        Ace   = 'A'

    Suits = [Suit.Diams, Suit.Spades, Suit.Clubs, Suit.Hearts]
    Nums  = [6, 7, 8, 9, 10, Num.Jack, Num.Queen, Num.King, Num.Ace]

    def __init__(self, num, suit, rule):
        assert callable(rule)

        self.responsed = False
        self.num = num
        self.suit = suit
        # Simple wrapper for rule function
        self.rule = lambda c, ses: rule(self, c, ses)

    def __str__(self):
        return f'{self.num}{self.suit}'

    def __repr__(self):
        return self.__str__()


class ClassicalRules:
   """
   Rules for classical bridge, every function take current card as first parameter and next card as second. 
   Return boolean true if next card is valid turn, false otherwise.
   Some rules change state of session, so functions also take session as third parameter.
   """

   def calculate(cards):
       "Function that calculates points. Takes cards and return points as intager. Return Value can be negative"
       points = 0
       for c in cards:
           if c.num == 10 or c.num == Card.Num.Queen or c.num == Card.Num.King:
               points += 10
           elif c.num == Card.Num.Jack:
               points += 25
           elif c.num == Card.Num.Ace:
               points += 15
       return points

   def first_move(lhs, rhs, ses):
       """
       It happens, that first card in move have slightly different rule.
       lhs is last card in previous move and rhs is first card in current turn.
       """
       return (lhs.num == rhs.num) or (lhs.suit == rhs.suit) or (rhs.num == Card.Num.Jack)

   def basic(lhs, rhs, ses):
       "Basic rule for cards 10, king and queen"
       return lhs.num == rhs.num

   def for_jack(lhs, rhs, ses):
       suit = choose_suit_callback()
       if suit == Card.Suit.NONE:
           suit = rhs.suit
       rhs.suit = suit
       return True

   def for_6(lhs, rhs, ses):
       ses.can_take_card = True
       return ClassicalRules.first_move(lhs, rhs, ses)

   def for_7(lhs, rhs, ses):
       if lhs.responsed:
           return ClassicalRules.basic(lhs, rhs, ses)
      
       pass

   def for_8(lhs, rhs, ses):
       pass

   def for_9(lhs, rhs, ses):
       return True

   def for_ace(lhs, rhs, ses):
       pass

   # Ordered rules for every card
   List = [for_6, for_7, for_8, for_9, basic, for_jack, basic, basic, for_ace]



import itertools
from random import shuffle as random_shuffle
from sys import int_info

class Deck:

    deck = []

    def __init__(self, rules=ClassicalRules()):
        self.first_move_rule = rules.first_move
        self.calculate_func  = rules.calculate
        for (n, s), r in zip(itertools.product(Card.Nums, Card.Suits), [r for r in rules.List for _ in Card.Suits]):
            self.deck.append(Card(n, s, r))

    def __getitem__(self, index):
        return self.deck[index]

    def __str__(self):
        return str([r for r in self.deck])

    def __repr__(self):
        return self.__str__()

    def __len__(self):
        return len(self.deck)

    def shuffle(self):
        random_shuffle(self.deck)


class Player:
    def __init__(self, nick, points=0):
        self.nick = nick
        self.points = points
        self.cards = []

    def give_cards(self, cards):
        assert isinstance(cards, list)
        self.cards.extend(cards)



class Session:

    def __init__(self):
        self.current_turn = 0     # Id of player that is currently turning(?)
        self.int_turn = []        # Intermidiate cards that is thrown but not validated yet
        self.plist = []           # List of players
        self.deck = Deck()
        self.heap = []            # Heap of thrown cards
        self.can_take_card = True # Is player can take card from deck


    def add_player(self, nick, points=0):
        self.plist.append(Player(nick, points))

    def start(self):
        self.deck.shuffle()
        for p in self.plist:
            p.give_cards(self.deck[:5])
            self.deck = self.deck[5:]
        self.int_turn.append(self.plist[self.current_turn].cards.pop())

    def top_card(self):
        if(len(self.int_turn)):
            return self.int_turn[-1]
        return self.heap[-1]

    def turn(self, card):
        self.int_turn.append(card)

    def end_turn(self):
        # Check if player even have cards
        if not self.int_turn in self.plist[self.current_turn].cards:
            return False

        # Previous player last card
        prev = self.top_card()

        # Validate first card
        if not self.deck.first_move_rule(prev, self.int_turn[0], self):
            return False
        
        prev = self.int_turn[0]
        for c in self.int_turn[1:]:
            if not prev.rule(c, self):
                return False
            prev = c
        self.deck.extend(self.int_turn)
        int_turn = []
        return True




if __name__ == '__main__':
    #Mini test
    ses = Session()
    ses.add_player('Nazik')
    ses.add_player('Kolya')

    print('Deck size: {}'.format(len(ses.deck)))
    print(ses.deck)

    ses.start()

    print('Deck size: {}'.format(len(ses.deck)))
    print(ses.deck)

    print(f'Top card: {ses.top_card()}')
    for p in ses.plist:
        print(f'{p.nick}`s cards: {p.cards}')



    



