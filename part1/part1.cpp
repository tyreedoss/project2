#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
using namespace std;

class card
{
    public:
        //Constructor
        card(const string& value,const string& suit) : cardValue(value), cardSuit(suit) 
        {

        }

        void setValue(const string& value)
        {
            cardValue = value;
        }
        void setSuit(const string& suit)
        {
            cardSuit = suit;
        }
        string getValue() const
        {
            return cardValue;
        }
        string getSuit() const
        {
            return cardSuit;
        }

        friend ostream& operator<<(ostream& os,const card& card)
        {
            os << "Value: " << card.getValue() << ", Suit: " << card.getSuit();
            return os;
        }
    
    private:
        string cardValue;
        string cardSuit;
};

class node
{
    public:
        card data;
        node* next;
        node(const card& card) : data(card), next(nullptr)
        {
        }

};

class deck
{
    public:
       

        deck() : head(nullptr)
        {
            vector<string> suits = {"Clubs", "Diamonds", "Hearts", "Spades"};
            vector<string> values = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King"};
            
            for (const string& suit : suits)
            {
                for (const string& value : values)
                {
                    card card(value, suit);
                    node* newNode = new node(card);
                    if (head == nullptr)
                    {
                        head = newNode;
                    }
                    else
                    {
                        node* current = head;
                        while (current->next)
                        {
                            current = current->next;
                        }
                        current -> next = newNode;
                    }
                }
            }
        }
        ~deck()
        {
            node* current = head;
            while(current)
            {
                node* next = current -> next;
                delete current; 
                current = next;
            }
        }

        friend ostream& operator << (ostream& os, const deck& deck)
        {
            node* current = deck.head;
            while (current)
            {
                os << current->data << endl;
                current = current -> next;
            }
            return os;
        }

        void shuffle()
        {
            srand(static_cast<unsigned int>(time(nullptr)));
            vector<card> shuffledDeck;
            node* current = head;
            while(current) 
            {
                shuffledDeck.push_back(current->data);
                current = current -> next;
            }
            
            default_random_engine rng(static_cast<unsigned int>(time(nullptr)));
            uniform_int_distribution<int> distribution(0, shuffledDeck.size() - 1);

            for (size_t i = 0; i < shuffledDeck.size(); i++)
            {
                size_t j = distribution(rng);
                swap(shuffledDeck[i], shuffledDeck[j]);
            }

            current = head;
            for (const card& card : shuffledDeck)
            {
                current->data = card;
                current = current->next;
            }
        }

        
    private:
        node* head;
};

int main()
{
    
    deck newDeck;
    cout << "Deck: " << newDeck << endl;
    newDeck.shuffle();
    cout << "Shuffled: " << newDeck << endl;

    return 0;
}