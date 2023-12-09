#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <queue>

using namespace std;

class card
{
public:
    //Constructor
    card(const string& value, const string& suit) : cardValue(value), cardSuit(suit)
    {

    }

    // Copy Constructor
    card(const card& copy) : cardValue(copy.cardValue), cardSuit(copy.cardSuit) {}

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

    // Overloaded Assignment Operator. Operator= must be memeber fucntion because it modifies state of the object
    card& operator=(const card& other) {
        if (this != &other) {  // Check for self-assignment
            cardValue = other.cardValue;
            cardSuit = other.cardSuit;
        }
        return *this;
    }

 
    

    friend ostream& operator<<(ostream& os, const card& card)
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
        vector<string> suits = { "Clubs", "Diamonds", "Hearts", "Spades" };
        vector<string> values = { "Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King" };

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
                    current->next = newNode;
                }
            }
        }
    }
    ~deck()
    {
        node* current = head;
        while (current)
        {
            node* next = current->next;
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
            current = current->next;
        }
        return os;
    }


    void shuffle()
    {
        srand(static_cast<unsigned int>(time(nullptr)));
        vector<card> shuffledDeck;
        node* current = head;
        while (current)
        {
            shuffledDeck.push_back(current->data);
            current = current->next;
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

    //Deal Function that deals card from top of deck and removes that card from the deck
    card deal() {
        if (head) {                     //Checks that head =! nullPtr (check that deck isnt empty)
            card topCard = head->data;  //topCard = the data from the head ptr
            node* temp = head;          //node ptr temp points to same card that head is pointing to
            head = head->next;          //Head will now be whatever the next card is in the deck
            delete temp;                //Remove the card that we dealt
            return topCard;
        }
        else {
            // Handle the case when the deck is empty
            throw runtime_error("Empty Deck");
        }
    }

    //replace function that puts card at bottom of deck
    void replace(const card& newCard) {
        node* newNode = new node(newCard);

        if (!head) {            //Check if deck is empty
            head = newNode;     //set head to be newNode, becomes only element in list
        }
        else {
            node* current = head;           //Start at head
            while (current->next) {         //run while loop while the next value of the current node is not a nullPointer (only occurs for last item in list)
                current = current->next;    //Move through the linked list by setting current node to the next element of the previous node
            }

            current->next = newNode;        //Once we get the last node in linked list, replace the nullPointer in the next element of the node with the newNode, (puts newCard at end of deck)
        }
    }


private:
    node* head;
};

void playFlip(deck& gameDeck) {
    int playerScore = 0;  // Initialize player's score to 0

    cout << "Initial deck:" << endl;
    cout << gameDeck << endl;

    // Shuffle the deck three times
    gameDeck.shuffle();
    gameDeck.shuffle();
    gameDeck.shuffle();
 
    queue<card> top24Cards;  // Create a queue to store the top 24 cards

    cout << "Top 24 cards (face-down):" << endl;
    for (int i = 0; i < 24; i++) {
        card topCard = gameDeck.deal();
        top24Cards.push(topCard);  // Enqueue the top card
        cout << "Card " << i + 1 << ": " << "Face-Down" << endl;
    }

    cout << "Remaining deck:" << endl;
    cout << gameDeck << endl;

    char choice;
    bool endGame = false;

    cout << "Start flipping cards (Y/N): ";
    cin >> choice;

    while (choice == 'Y' || choice == 'y') {
        if (top24Cards.empty()) {
            endGame = true;
            break;
        }

        card flippedCard = top24Cards.front();  // Get the front card in the queue
        top24Cards.pop();  // Remove the front card
        gameDeck.replace(flippedCard);

        cout << "Flipped Card: " << flippedCard << endl;

        //Score Calculation

        if (flippedCard.getValue() == "Ace") {
            playerScore += 10;
        }
        else if (flippedCard.getValue() == "King" || flippedCard.getValue() == "Queen" || flippedCard.getValue() == "Jack") {
            playerScore += 5;
        }
        else if (flippedCard.getValue() == "8" || flippedCard.getValue() == "9" || flippedCard.getValue() == "10") {
            // No change in score
        }
        else if (flippedCard.getValue() == "7") {
            playerScore /= 2;
        }
        else {
            playerScore = 0;
        }

        if (flippedCard.getSuit() == "Hearts") {
            playerScore += 1;
        }


        cout << "Player's Current Score: " << playerScore << endl;
        cout << "Continue flipping cards (Y/N): ";
        cin >> choice;
        if (choice == 'n' || choice =='N')
        {
            endGame = true;
        }
    }


    if (endGame) {
        cout << "Game Ended. Player's Final Score: " << playerScore << endl;
    }
}

int main()
{
    //Part B
    char response;
    cout << "Do you want to play Flip [Y/N]" << endl;
    cin >> response;

    if (response == 'Y' || response == 'y') {
        deck MyDeck;
        playFlip(MyDeck);
    }

    return 0;
}