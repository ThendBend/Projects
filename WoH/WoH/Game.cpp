#include "Game.h"

Game::Game()
{
	myLevel = 1;
	myGold = 0;
	myExp = 0;
	myAtk = 0.6 + (myLevel / 100);
	myDefense = 1 * myLevel * 2;
	myDmg = 30 * myLevel;
	expNext = 500 * myLevel;
	monsDmg = 25;
	monsHealth = 80;
	maxHealth = 100 * myLevel;
	maxLevel = 40;
	monsAtk = 0.5f;
	default_random_engine ranEngine;
	uniform_int_distribution<int> ranDef(10, 30);
	uniform_int_distribution<int> ranhighDef(50, 100);
	for (int c = 0; c < numItems; c++)
	{
		if (c < numItems / 2)
		{
			itemStats[allItems[c]] = ranDef(ranEngine);
		}
		else
		{
			itemStats[allItems[c]] = ranhighDef(ranEngine);
		}
	}
}
void Game::pStart()
{
	cout << "Welcome to Warriors Of Heaven!\n";
	cout << "\n1.New Game\n2.Load game\n3.Exit" << endl;
	newGame();
}
void Game::newGame()
{
	while (true)
	{
		myChoice = _getch();
		if ((myChoice - '0') <= 3 && (myChoice - '0') > 0)
		{
			switch (myChoice)
			{
			case '1': system("CLS"); gChar();
				break;
			case '2': system("CLS"); loadGame();
				break;
			case '3': system("CLS"); pExit();
				break;
			default:
				break;
			}
		}
	}
}
void Game::gChar()
{
	cout << "Please enter a character name:" << endl;
	cin >> nameChar;
	system("CLS");
	cout << "Please choose a class.\n" << endl;
	for (int c = 0; c < 3; c++)
	{
		cout << c + 1 << "." << charList[c] << endl;
	}
	while (myPick.empty())
	{
		charChoice = _getch();
		if (charChoice - '0' <= 3 && charChoice - '0' > 0)
		{
			myPick = charList[charChoice - '0' - 1];
		}
	}
	system("CLS");
	gOptions();
}
void Game::pInventory()
{
	default_random_engine ranEngine;
	uniform_int_distribution<int> ranMoney(100, 200);
	uniform_int_distribution<int> ranMoney2(500, 1000);
	int z = 0;
	int i = 0;
	for (int c = 0; c < numItems; c++)
	{
		if (c < 6)
		{
			priceInv[allItems[c]] = ranMoney(ranEngine);
		}
		else
		{
			priceInv[allItems[c]] = ranMoney2(ranEngine);
		}
		if (myInv[c] > 0)
		{
			cout << z + oldShop.size() + 1 << "." << allItems[c] << " (" << myInv[c] << ") " << priceInv[allItems[c]] << " Gold" << endl;
			z++;
		}
	}
}
void Game::gShop()
{
	if (oldmCnt != monsCnt)
	{
		oldShop.clear();
		default_random_engine ran(time(0));
		uniform_int_distribution<int> ranto(0, numItems);
		for (int c = 0; c < 3; c++)
		{
			oldShop.push_back(allItems[ranto(ran)]);
		}
		oldmCnt = monsCnt;
	}
	default_random_engine ranEngine;
	uniform_int_distribution<int> ranMoney(200, 500);
	uniform_int_distribution<int> ranMoney2(1000, 2000);

	for (int c = 0; c < numItems / 2; c++)
	{
		priceShop[allItems[c]] = ranMoney(ranEngine);
	}
	for (int c = numItems - 1; c != (numItems / 2) - 1; c--)
	{
		priceShop[allItems[c]] = ranMoney2(ranEngine);
	}
	cout << "0.Back\n\nShop:" << endl;
	for (int c = 0; c < oldShop.size(); c++)
	{
		cout << c + 1 << "." << oldShop[c] << " " << priceShop[oldShop[c]] << " Gold" << endl;
	}
	cout << "\nInventory:\n";
	pInventory();
	cout << "\nGold: " << myGold << endl;
	while (true)
	{
		myChoice = _getch();
		if (myChoice - '0' == 0)
		{
			itemCnt = 1;
			system("CLS");
			gOptions();
		}
		else if (myChoice - '0' <= oldShop.size())
		{
			if (myGold >= priceShop[oldShop[(myChoice - '0') - 1]])
			{
				for (int c = 0; c < numItems; c++)
				{
					if (oldShop[(myChoice - '0') - 1] == allItems[c])
					{
						myInv[c]++;
					}
				}
				oldShop.erase(oldShop.begin() + (myChoice - '0') - 1);
				myGold -= priceShop[oldShop[myChoice - '0' - 1]];
				system("CLS");
				gShop();
			}
			else
			{
				cout << "Not enough gold!" << endl;
			}
		}
		else if ((myChoice - '0') <=numItems+oldShop.size())
		{
			for (int c = 0; c < (myChoice - '0')-oldShop.size(); c++)
			{
				int cnt = 0;
				for (int z = 0; z < numItems; z++)
				{
					if (myInv[z] > 0)
					{
						cnt++;
					}
					if (cnt == (myChoice - '0') - oldShop.size())
					{
						auto t = allItems[z].find(" [Equiped]");
						if (t != string::npos)
						{
							allItems[z].erase(t, 10);
							for (int i = 0; i < numWeapons; i++)
							{
								auto t = (allItems[z].find(myWeapons[i]));
								if (t == string::npos)
								{
									myDmg -= itemStats[allItems[z]];
									break;
								}
								else
								{
									myDefense -= itemStats[allItems[z]];
									break;
								}
							}
						}
						myInv[z]--;
						myGold += priceInv[allItems[z]];
						cnt = 0;
						system("CLS");
						gShop();
					}
				}
			}
		}
	}
}
void Game::gInventory()
{
	int z = 0;
	for (int c = 0; c < numItems; c++)
	{
		if (c == 0)
		{
			cout << "0.Back\n\nInventory:" << endl;
		}
		if (myInv[c] > 0)
		{
			z++;
			cout << z << "." << allItems[c] << " (" << myInv[c] << ")" << endl;
		}
	}
	while (true)
	{
		myChoice = _getch();
		if (myChoice == '0')
		{
			system("CLS");
			gOptions();
		}
		else if (myChoice - '0' <= numItems&&myChoice - '0'>0)
		{
			pEquip();
		}
	}
}
void Game::pExit()
{
	MessageBox(NULL, "Thank you for playing Warriors Of Heaven!", "Thank you!", MB_OK);
	exit(0);
}
void Game::fMonsters()
{
	default_random_engine ranEngine(time(0));
	uniform_real_distribution<float> attChance(0.0f, 1.0f);
	uniform_real_distribution<float> mattChance(0.0f, 1.0f);
	uniform_int_distribution<int> critDmg(1, 6);
	uniform_int_distribution<int> nDmg(myDmg / 2, myDmg);
	uniform_int_distribution<int> mDmg(monsDmg / 2, monsDmg);
	uniform_int_distribution<int> getItems(0, numItems-1);
	int cnt = 0;
	for (int c = 0; c < numMons; c++)
	{
		if (c == 0)
		{
			cout << "0.Back\n\n" << endl;
		}
		if (myLevel >= cnt)
		{
			cout << c + 1 << "." << allMons[c] << endl << endl;
		}
		else
		{
			cout << c + 1 << ".Level " << cnt << " required\n" << endl;
		}
		cnt += 5;
	}
	while (true)
	{
		myChoice = _getch();
		if (myChoice == '0')
		{
			system("CLS");
			gOptions();
		}
		if (myChoice - '0' <= numMons && (myChoice - '0') > 0 && multiplyZero((myChoice - '0') - 1, 5) <= myLevel)
		{
			uniform_int_distribution<int> getExp(multiplyZero((myChoice - '0') - 1, 5) * 100, multiplyZero((myChoice - '0') - 1, 5) * 200);
			uniform_int_distribution<int> getGold(multiplyZero((myChoice - '0') - 1, 5) * 30, multiplyZero((myChoice - '0') - 1, 5) * 60);
			uniform_int_distribution<int> mDmg((monsDmg / 2) *multiplyZero((myChoice - '0') - 1, 5), (multiplyZero((myChoice - '0') - 1, 5))* monsDmg);
			system("CLS");
			cout << monsSen[(myChoice - '0') - 1] << "\n\n1.Fight\n2.Run Away" << endl;
			while (true)
			{
				fChoice = _getch();
				if (fChoice == '1')
				{
					curmonsHealth = 80 * multiplyZero((myChoice - '0') - 1, 5);
					monsHealth = 80 * multiplyZero((myChoice - '0') - 1, 5);
					curHealth = maxHealth;
					system("CLS");
					cout << myPick << " Health: " << curHealth << "/" << maxHealth << " VS " << allMons[(myChoice - '0') - 1] << " Health: " << curmonsHealth << "/" << monsHealth << endl;
					cout << "\n1.Attack\n2.Run Away" << endl;
					while (curHealth > 0 && curmonsHealth > 0)
					{
						fChoice = _getch();
						if (fChoice == '1')
						{
							system("CLS");
							int crit = critDmg(ranEngine);
							int ndmg = nDmg(ranEngine);
							int mdmg = mDmg(ranEngine);
							float att = attChance(ranEngine);
							float matt = mattChance(ranEngine);
							int exp = getExp(ranEngine);
							int gold = getGold(ranEngine);
							int item = getItems(ranEngine);
							cout << myPick << " Health: " << curHealth << "/" << maxHealth << " VS " << allMons[(myChoice - '0') - 1] << " Health: " << curmonsHealth << "/" << monsHealth << endl;
							cout << "..." << endl;
							Sleep(2000);
							if (crit == 6)
							{
								curmonsHealth -= ndmg * 2;
								cout << charcritMsg[(charChoice - '0') - 1] << "\nYou did " << ndmg * 2 << " Damage" << endl;
							}
							else if (att < myAtk)
							{
								curmonsHealth -= ndmg;
								cout << charMsg[(charChoice - '0') - 1] << "\nYou did " << ndmg << " Damage" << endl;
							}
							else
							{
								cout << charmissMsg[(charChoice - '0') - 1] << endl;
							}
							if (curmonsHealth < 0)
							{
								Sleep(1000);
								cout << "..." << endl;
								Sleep(2000);
								cout << "You won!" << endl;
								myExp += exp;
								myGold += gold;
								monsCnt++;
								cout << "You were awarded with " << exp << " Experience and " << gold << " Gold" << endl;
								if (att < 0.03f*multiplyZero((myChoice - '0') - 1, 5))
								{
									if (item < numItems / 2)
									{
										myInv[item]++;
										cout << "You dropped " << allItems[item] << "!" << endl;
									}
									else if (att < 0.01*multiplyZero((myChoice - '0') - 1, 5))
									{
										myInv[item]++;
										cout << "You dropped " << allItems[item] << "!" << endl;
									}
								}
								if (myChoice == '5')
								{
									cout << "\nCongratulations! You are in Heaven!" << endl;
								}
								if (myExp >= expNext)
								{
									myLevel++;
									myAtk = 0.6 + (myLevel / 100);
									myDefense = 1 * myLevel * 2;
									myDmg = 30 * myLevel;
									myExp = exp - (expNext - myExp);
									expNext = 500 * myLevel;
									maxHealth = 100 * myLevel;
									cout << "\nCongratulations you leveled up!" << endl;
								}
								cout << "\n1.Continue" << endl;
								while (true)
								{
									fChoice = _getch();
									if (fChoice == '1')
									{
										system("CLS");
										gOptions();
									}
								}
							}
							Sleep(1000);
							cout << "..." << endl;
							Sleep(2000);
							if (monsAtk < matt)
							{
								int rdmg = mdmg - myDefense;
								if (rdmg < 0)
								{
									rdmg = 0;
								}
								curHealth -= rdmg;
								cout << monsMsg[(myChoice - '0') - 1] << "\nYou took " << rdmg << " damage!" << endl;
								cout << "\n1.Attack\n2.Run Away" << endl;
							}
							else
							{
								cout << "The " << allMons[(myChoice - '0') - 1] << " attacked but missed!" << endl;
								cout << "\n1.Attack\n2.Run Away" << endl;
							}
							if (curHealth < 0)
							{
								cout << "You died!\nGAME OVER!" << endl;
								myLevel = 1;
								myGold = 0;
								myExp = 0;
								myAtk = 0.6 + (myLevel / 100);
								myDefense = 1 * myLevel * 2;
								myDmg = 30 * myLevel;
								expNext = 500 * myLevel * 3;
								monsDmg = 20;
								monsHealth = 80;
								maxHealth = 100 * myLevel;
								maxLevel = 40;
								monsAtk = 0.5f;
								cout << "n\1.Main menu" << endl;
								while (true)
								{
									fChoice = _getch();
									if (fChoice == '1')
									{
										system("CLS");
										pStart();
									}
								}
							}
						}
						else if (fChoice == '2')
						{
							system("CLS");
							fMonsters();
						}
					}
				}
				else if (fChoice == '2')
				{
					system("CLS");
					fMonsters();
				}
			}
		}
	}
}
void Game::gOptions()
{
	cout << nameChar << " " << myPick << " Level:" << myLevel << " Exp:" << myExp << "/" << expNext << " Gold:" << myGold << endl << endl;
	for (int i = 0; i<5; i++)
	{
		cout << i + 1 << "." << mainOptions[i] << endl;
	}
	while (true)
	{
		myChoice = _getch();
		if (myChoice - '0' <= 5 && myChoice - '0' > 0)
		{
			switch (myChoice)
			{
			case '1': system("CLS"); gInventory();
				break;
			case '2': system("CLS"); gShop();
				break;
			case '3': system("CLS"); fMonsters();
				break;
			case '4': system("CLS"); saveGame();
				break;
			case '5': system("CLS"); pStart();
				break;
			default:
				break;
			}
		}
	}
}
void Game::saveGame()
{
	for (int i = 0; i < 4; i++)
	{
		if (i == 0)
		{
			cout << "0.Back\n" << endl;
		}
		cout << i + 1 << "." << saveOrder[i] << endl;
	}
	while (true)
	{
		myChoice = _getch();
		if (myChoice == '0')
		{
			system("CLS");
			gOptions();
		}
		else if (myChoice - '0' <= 4 && myChoice - '0'>0)
		{
			saveOrder[(myChoice - '0') - 1] = nameChar + " Level " + to_string(myLevel);
			fstream saveFile("Save.txt", ios::in | ios::out | ios::app);
			string str = saveOrder[(myChoice - '0') - 1];
			remove_if(str.begin(), str.end(), isspace);
			if (myLevel < 9)
			{
				str.erase(str.end() - 1);
			}
			else
			{
				str.erase(str.size() - 3, 2);
			}
			saveFile << charChoice << endl << myExp << endl << myLevel << endl << myGold << endl << str << endl << (myChoice - '0') << endl << myDefense << endl << myDmg << endl << nameChar << endl << -1 << endl;
			saveFile.close();
			fstream itemFile("Items.txt", ios::in | ios::out | ios::app);
			itemFile << getInv() << endl << getEquip() << endl << (myChoice - '0') << endl;
			itemFile.close();
			cout << str << endl;
			system("CLS");
			saveGame();
		}
	}
}
void Game::loadGame()
{
	fstream loadFile("Save.txt", ios::in | ios::out | ios::app);
	int a = 0;
	int e, g, l, c, def, dmg, neg;
	string p, s, inv, eq, ch;
	char chChoice;
	int z = 0;
	for (int i = 0; i < 4; i++)
	{
		while (getline(loadFile, p))
		{
			getline(loadFile, s);
			a = stoi(s);
			if (i + 1 == a)
			{
				if (isdigit(p[p.size() - 3]))
				{
					p.insert(p.end() - 3, 1, ' ');
					p.insert(p.end() - 9, 1, ' ');
				}
				else
				{
					p.insert(p.end() - 2, 1, ' ');
					p.insert(p.end() - 8, 1, ' ');
				}
				saveOrder[i] = p;
				loadOrder[i] = p;
			}
			if (a == -1)
			{
				nameChar = p;
			}
		}
		loadFile.close();
		loadFile.open("Save.txt");
	}
	for (int b = 0; b < 4; b++)
	{
		if (b == 0)
		{
			cout << "0.Back" << endl;
		}
		cout << b + 1 << "." << loadOrder[b] << endl;
	}
	loadFile.close();
	loadFile.open("Save.txt");
	while (true)
	{
		myChoice = _getch();
		if (myChoice == '0')
		{
			saveOrder = saveOrdertmp;
			system("CLS");
			pStart();
		}
		else if (myChoice - '0' <= 4 && myChoice - '0'>0 && loadOrder[myChoice - '0' - 1] != "Load file")
		{
			while (loadFile)
			{
				loadFile >> chChoice >> e >> l >> g >> s >> c >> def >> dmg >> ch >> neg;
				if (c == (myChoice - '0'))
				{
					myDefense = def;
					myDmg = dmg;
					myPick = charList[(chChoice - '0') - 1];
					charChoice = chChoice;
					myLevel = l;
					myExp = e;
					myGold = g;
					nameChar = ch;
				}
			}
			loadFile.close();
			fstream loadItem("Items.txt", ios::in || ios::out || ios::app);
			while (loadItem)
			{
				loadItem >> inv >> eq >> c;
				if (c == (myChoice - '0'))
				{
					for (int h = 0; h < numItems; h++)
					{
						myInv[h] = inv[h] - '0';
						invEquip[h] = eq[h] - '0';
						if (invEquip[h] > 0)
						{
							allItems[h] += " [Equiped]";
						}
					}
				}
			}
			myAtk = 0.6 + (myLevel / 100);
			expNext = 500 * myLevel;
			maxHealth = 100 * myLevel;
			system("CLS");
			gOptions();
		}
	}
}
void Game::pEquip() // Add, remove items
{
	for (int c = 0; c < myChoice - '0'; c++)
	{
		int cnt = 0;
		for (int x = 0; x < numItems; x++)
		{
			if (myInv[x] > 0)//if item is there
			{
				cnt++;//find first,second ex... occurment of an item
			}
			if (cnt == myChoice - '0')// if first,second ex... item occured is equal to my choice
			{
				if (x < numItems / 2 && invEquip[x + 3] == 1)
				{
					system("CLS");
					gInventory();

				}
				else if (x >= numItems/2 && invEquip[x - 3] == 1)
				{
					system("CLS");
					gInventory();
				}
				if (invEquip[x] == 0) //if there is no item
				{
					myDefense += itemStats[allItems[x]];
					allItems[x] += " [Equiped]";
					invEquip[x]++;
					system("CLS");
					gInventory();
				}
				else if (invEquip[x]==1)//if there is item
				{
					invEquip[x]--;
					auto t = allItems[x].find(" [Equiped]");
					allItems[x].erase(t, 10);
					myDefense -= itemStats[allItems[x]];
					system("CLS");
					gInventory();
				}
				system("CLS");
				gInventory();
			}
		}
	}
}
long long Game::multiplyZero(long long num, long long num2)
{
	return num == 0 ? 1 : num * num2;
}
string Game::getInv()
{
	string inv;
	for (int i = 0; i < numItems; i++)
	{
		inv += to_string(myInv[i]);
	}
	return inv;
}
string Game::getEquip()
{
	string eq;
	for (int i = 0; i < numItems; i++)
	{
		eq += to_string(invEquip[i]);
	}
	return eq;
}
