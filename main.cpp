/*
   
	Various Card games implemented in C++
	 this is a practice project
	 
	 Included Features:
	 
	 -> Xeri/Ξερη (Normal & Custom Version)
	 -> Blackjack/21
	 -> Blackjack/Crazy 8s/Pagat, Basically Uno with Playing cards (rules will be given)
	 -> AI Oppoment with varying difficulties

*/

#include <iostream>
//Math Lib
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <bits/stdc++.h>
#include <vector>
#include <map>

using namespace std;
char Suits[4] = {'H','D','S','C'};
enum AIDifficulty {
	Easy = 6, // Basic with Some memory
	Medium = 12, // a lil bit advanced with Some memory
	Hard = 20, // Uses advanced tactics and techniques (some of mine too)
	Impossible = 52 // Cheats to win
	
	//Memory usage amount is based on the gametype
	// Point is the AI has memory that is used in order to win games (Card counting,calculating odds etc etc)
	
	//Try Imposssible mode if you want to have fun
};
map<char, string> SuitIcons = {
    {'H', u8"♥"},
    {'D', u8"♦"},
    {'S', u8"♠"},
    {'C', u8"♣"}
};
int parseValue(char c) {
	switch (c) { //So i can get the values normally
		case 'A': return 0;
		case 'J': return 10;
		case 'Q': return 11;
		case 'K': return 12;
		default:  return c - '2' + 1; //ascii tricks
	}
}
string displayValue(int value) {
	switch (value) {
		case 0:  return "A";
		case 9:  return "10";
		case 10: return "J";
		case 11: return "Q";
		case 12: return "K";
		default: return string(1, char(value + '1'));
	}
}

static string Input(string msg,string _ = ""){
	cout << msg;
	while (!(cin >> _)) {  // Keep asking until the user enters a valid number
		cout << "Invalid input. Try again: ";
		cin.clear(); // Reset input errors
		cin.ignore(10000, '\n'); // Remove bad input
	}
	return _;
}
class Card{
	public:
		char suit;
		int value;
		//0->A
		//9 -> 10
		//10 -> J
		//11 -> Q
		//12->K
		Card(){}
		Card(char s,int v){
			suit = s;
			value = v;
		}
		bool operator==(const Card& other) const{
			return suit == other.suit && value == other.value;
		}
};

class Player{
	public:
		int points= 0;
		int wins = 0;
		vector<Card> hand;
		vector<Card> bin;
		vector<Card> bin2; //-> for misc purposes (in Xeri you need 2 discard piles for your cards to count them,
						   //in this case i use this so i dont confuse the Xeri cards with the Point cards
		Player(){
			points = 0;
			wins = 0;
		}
		
};
class Game{
	public:
		vector<Card> deck;
		vector<Card> bin;
		vector<Card> table;
		//AI Stuff
		vector<Card> AIMemory; //A Vector that contains the Cards the AI "remembers" in order to calculate posssible winning odds eetc etc
		int AIMemoryLimit; //A Limit since AIMemory is a vector
		enum AIDifficulty Diff; //Difficulty of the AI
		
		void printDeck(vector<Card> d){
			vector<vector<string>> asciiCards;
			for (Card& c : d){
				string val = displayValue(c.value);
				if (val.size() == 1) val += " "; // pad single-char values so 10 aligns
				string s = SuitIcons[c.suit];

				vector<string> lines(5);
				lines[0] = " _____ ";
				lines[1] = "|" + val + "   |";
				lines[2] = "|  " + s + "  |";
				lines[3] = "|"+s+v+" "+ val + "|";
				lines[4] = " ----- ";

				asciiCards.push_back(lines);
			}

			for (int i = 0; i < 5; i++){ // 5 lines per card
				for (auto& cardLines : asciiCards){
					cout << cardLines[i] << " ";
				}
				cout << "\n";
			}
		}
		void shuffleDeck(){
			//Mersenne Twister 32-bit rand num gen
			static mt19937 rng(random_device{}());
			shuffle(begin(deck), end(deck), rng);
		}

		void setDeck(){
			for (int i = 0; i < 4; i++){
				for (int j = 0; j < 13; j++){
					deck.push_back(Card(Suits[i], j));
				}
			}
		}		
		void dealCard(Player& plr){
			plr.hand.push_back(deck.back());
			deck.pop_back();
		}
		void setTable(int n){ //n is the number of cards
			for(int i = 0; i < n;i++){
				table.push_back(deck.back());
				deck.pop_back();
			}
		}
		void CountCard(Carc c){
			if(1 < c.value < 5){
				return 1;
			}else if (9 < c.value < 12 || c.value == 0){
				return -1;
			}else{
				return 0;
			}
		}
		void AddToMemory(Card c,int limit){
			if (AIMemory.size() <= AIMemoryLimit){
				AIMemory.push_back(c);
			}else{
				AIMemory.pop_back();
				
			}
		}
		void AIPlay(Player clanker,int GT,enum AIDifficulty diff){
			AIMemoryLimit = diff;
			vector<Card> &clankerHand = clanker.hand;
			vector<Card> &clankerbin = clanker.bin;
			vector<Card> &clankerbin2 = clanker.bin2;
			
			int CardCountScore = 0; //Arbitary Score used in Card counting for the game Blackjack/21
			//Turns out this score could also be used to "predict" the rest of the cards left in the deck
			//and be used in other games too,like Xeri,where it dictates if you should risk throwing J/Q/K or A to force
			//the opposition to play a number card and possibly get a Xeri or Collect all the cards with a J
			
			
			if(GT== 0 ) { // Xeri Game
				switch(diff){
					case Easy:
						for (int i =0;i < clankerHand.size(); i++){
						
						}
						break;
					case Medium:
						break;
					case Hard:
						break;
					case Impossible:
						break;
					default:
					
						break;
				}
			}
		}
		
};


class Xeri: public Game{
	public:
		int CG[5] = {1,10,20,50,100}; //CG == "Custom Game"
		//CG[0] -> Points for every Number Card
		//CG[1] -> Points for every A/J/Q/K/10/2 of Clubs Card
		//CG[2] -> Points for every Xeri/Ξερη
		//CG[3] -> Points for every A/Q/K/10/2 of Clubs Xeri/Ξερη
		//CG[4] -> Points for every J Xeri/Ξερη
		int CGS[4] = {0,9,11,12};
		
		int SG[4] = {1,3,10,20}; //SG == "Standard Game"
		//SG[0] -> Points for every A/J/Q/K/10 Card (10 of Diamonds && 2 of Clubs gets +1 for being the "good 10")
		//SG[1] -> Points for having the most cards
		//SG[2] -> Points for every Xeri/Ξερη
		//SG[3] -> Points for every J Xeri/Ξερη
		bool cg = false; 
		int isXeri(Card c1,Card c2){
			//c1 -> card being THROWN
			bool a = (find(begin(CGS), end(CGS), c1.value) != end(CGS));
			bool b = (c1.value == 1 && c1.suit == 'C');
			if (c1.value != c2.value){
				return 0;
			}else{
				if (cg){
					if (a || b){
						return CG[3];
					}else if(c1.value != 10){
						return CG[2];
					}
					else{
						return CG[4];
					}
					
				}else{
					if (c1.value != 10){
						return SG[2];
					}else{
						return SG[3];
					}
				}
			}
		}
		int PointCount(Player plr,Player opp){
			int score = 0;
			int binSize = plr.bin.size();
			for (int i = 0;i < binSize;i++){
				Card card = plr.bin.back();
				plr.bin.pop_back();
				bool a = (find(begin(CGS), end(CGS), card.value) != end(CGS));
				bool b = (card.value == 1 && card.suit == 'C');
				bool c = (card.value == 9 && card.suit == 'D');
				if (card.value > 0 && card.value < 9){
					if (cg){ //+CG[0} for every number card from 2-9
						score += CG[0];
					}
				}else{					
					if (cg){
						if (a || b){//is Card A,J,Q,K,10,2 of clubs?
							score += CG[1]+(int)b;//+= The Score of the card +1 if 2 of clubs
						}else{
							score+= CG[0];
						}
					}else{
						if (a || b || c){ //is Card A,J,Q,K,10,10 of hearts,2 of clubs?
							score += SG[0]+(int)c+((int)b*2);//+= The Score of the card +1 if 10 of hearts or 2 of clubs
						}
					}
				}
			}
			int plrBinLen = plr.bin.size();
			int oppBinLen  = opp.bin.size();
			if (plrBinLen > oppBinLen){
				score+= SG[1];
			}
			return score;
		}
		void Game(Player chosenOne,Player slopGPT,enum AIDifficulty diff){		
			bool playerTurn = true;
			setDeck();
			shuffleDeck(); //deal each player 6 starting cards
			for (int i = 0;i < 6;i++){
				dealCard(chosenOne);
				dealCard(slopGPT);
			}
			for (int i = 0;i < 4;i++){ //deal each player 4 cards in the table,right-most is the top card
				table.push_back(deck.back());
				deck.pop_back();
			}
			while (!deck.empty()){			
				system("cls");
				cout << "Table:\n";
				printDeck(table);
				vector<Card> &plrHand = chosenOne.hand;
				if (playerTurn){
					cout << "Your Hand:\n";
					printDeck(plrHand);
					string input = Input("Choose a card to play from your hand (Suit+Value no spaces): ");
					int value = parseValue(input[1]);
					Card tempCard(input[0],value);
					auto it = find(plrHand.begin(), plrHand.end(), tempCard);
					while (it == plrHand.end()){
						input = Input("No such card found,try again: ");
						value = parseValue(input[1]);
						tempCard = Card(input[0],value);
						it = find(plrHand.begin(), plrHand.end(), tempCard);
					}				
					Card c = move(*it);
					plrHand.erase(it);
					table.push_back(c);
					int xeri = isXeri(c,table.size() > 1 ? table[table.size()-2] : c); // if cards on table,perform isXeri() else put it as a card on the table
					if ((bool)xeri){
						chosenOne.points+=xeri;
						chosenOne.bin2.push_back(c);
						for(Card& tc : table){
							chosenOne.bin.push_back(move(tc));
						}
						table.clear();
					}else{
						chosenOne.bin.push_back(c);
					}
				}else{
					
				}			
				playerTurn != playerTurn;
			}
			int plrPoints = PointCount(chosenOne,slopGPT);
			int AIPoints = PointCount(slopGPT,chosenOne);
			if (plrPoints > AIPoints){
				cout << "You won!";
			}else{
				cout << "You lost.";
			}
			//printDeck();
			// Get user input from the keyboard
		}
};

int main() {
	system("chcp 65001"); // set UTF-8 codepage
  cout << "Test Run..\n";
  Player chosenOne;
  Player slopGPT;
  Xeri g;
  g.Game(chosenOne,slopGPT,enum AIDifficulty Hard);
  return 0;
}
