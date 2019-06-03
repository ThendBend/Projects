#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <unordered_map>
#include <random>
#include <ctime>
#include <iterator>
#include <Windows.h>
#include <conio.h>
#include <cstdlib>

using namespace std;

class Game
{
public:
	Game();
	void gChar();//done
	void gOptions();//done
	void fMonsters();//done
	void pExit();//done
	void gShop();//done
	void pEquip();
	void pInventory();//done
	void gInventory();//done
	void saveGame();//done
	void loadGame();//done
	string getInv();//done
	string getEquip();//done
	long long multiplyZero(long long num, long long num2);//done
	void newGame();//done
	void pStart();//done
private:
	long long myGold;
	long long myExp;
	long long saveCnt = 0;
	long long myLevel;
	long long monsCnt = 0;
	long long oldmCnt = 0;
	long long myDmg;
	long long monsDmg;
	long long maxHealth;
	long long curHealth;
	long long maxLevel;
	long long expNext;
	float myAtk;
	float monsAtk;
	long long monsHealth;
	long long curmonsHealth;
	long long myDefense;
	static const int numWeapons = 3;
	static const int numMons = 5;
	static const int numItems = 6;
	char myChoice;
	char fChoice;
	char charChoice;
	int itemCnt = 1;
	int wepEquip = numItems/2;
	string myWeapons[numWeapons]{ "Bow","Wand","Sword" };
	string myPick;
	string nameChar;
	string monsSen[numMons]{ "You enter a public bathroom only to find a Giant Cockroach drinking from the water of a broken sink with a large hole in the wall.","You travel to the desert to meet with the desert prince. On the way there a Giant Poisonous Scorpion which appears to be hiding under the sand pushes it way out of the sand and faces you.","Your college friends invite you to a party. You enter the house and think maybe you are too early but it turns out to be a haunted house and you find all your friends dead and an Angry Ghost staring right through your soul.","It is Christmas Eve! You go outside on the way to meet up with friends and take a picture with Santa. You get confused thinking is it Halloween? A Frozen Undead appeared to ruin your Christmas Eve.","You are late to work. You wake up, dress up and get in the car. Afraid you might get fired you speed up,crash and die. Your soul leaves your body, on the way to Heaven you get excited but a Hell Dragon stops you, ruins your excitement and says 'You shall not enter Heaven unless you defeat me'." };
	string monsMsg[numMons]{ "The Giant Cockroach slashed you with its antennae!","The Poisonous Scorpion grabed you with its pincers, squeezed and let go!","The Angry ghost swirled towards you and hit you CLAP!","The Frozen Undead cut you with its frozen ice nails!","The Hell Dragon took a sip from the hell fire and blew it with a strong surge towards you!" };
	string allMons[numMons]{ "Giant Cockroach","Giant Venomous Scorpion","Angry Ghost","Frozen Undead","Hell Dragon" };
	string mainOptions[5]{ "Inventory","Shop","Monsters","Save game","Main menu" };
	int myInv[numItems]{ 0,0,0,0,0,0 };
	int invEquip[numItems]{ 0,0,0,0,0,0 };
	map<string, int> itemStats;
	string allItems[numItems]{ "Lusty Gloves","Lusty Boots","Lusty Chest","Heavens Gloves","Heavens Boots","Heavens Chest" };
	string charList[3]{ "Archer","Mage","Warrior" };
	string charMsg[3]{ "You shot your arrow and it landed on its legs!","You casted a spell and the monster shivered!","You swung your sword at the monster and cut it!" };
	string charcritMsg[3]{ "CRITICAL HIT!\nYou shot your arrow and it landed perfectly on the monsters head!","CRITICAL HIT!\nYou casted a powerful spell which caused the ground to shake and the monster shrieked!","CRITICAL HIT!\nYou spun your sword and the monster takes multiple cuts!" };
	string charmissMsg[3]{ "You shot your arrow but it missed!","You tried to cast a spell but failed!","You swung your sword but the monster dodged it!" };
	map<string, long long> priceShop;
	map <string, long long> priceInv;
	vector <string> oldShop{ "Lusty Gloves","Lusty Boots","Lusty Chest" };
	vector <string> saveOrder{ "Save file","Save file", "Save file", "Save file" };
	vector <string> saveOrdertmp{ "Save file","Save file", "Save file", "Save file" };
	vector <string> loadOrder{ "Load file","Load file", "Load file", "Load file" };
};