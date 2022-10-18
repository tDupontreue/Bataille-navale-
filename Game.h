#pragma once
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <Windows.h>
#include "Config.h"
#include "player.h"

using namespace std;
using namespace sf;

class Connector
{
public:
	bool myTurn;
	bool isServer;
	bool isConnected;
	bool connecting;
	bool receiving;
	bool dataReceived;
	bool won;
	bool shipHit;
	bool shipSunk;
	int x, y;
	ship enemy_ship;
	Text& text;
};

class Game
{
	Font font;
	enum GameState {MENU,GAME,GAME_OVER,GAME_MULTI,GAME_ONLINE,END};
	GameState state;

public:
	Game();
	void runGame()
	{

		while (state != END)
		{
			switch (state)
			{
			case MENU:
				menu();
				break;
			case GAME:
				singlePlayer();
				break;
			case GAME_MULTI:
				hotSeat();
				break;
			case GAME_OVER:
				state = END;
				break;
			}
		}

	}
	void Play(player&, player&, string s);
	void singlePlayer()
	{
		player p, comp;
		ComputerAlgorithm ai(&p);
		p.SetShips();
		comp.SetShips();
		Play(p, comp, "player1");

		return;
	}
	bool Shoot(player& p, RenderWindow& window, Sprite** tab, Event& event, Text& t);
	void menu();
	void hotSeat();
};

void DrawFields(player& p, Sprite** tab, bool visibility, RenderWindow& window, int x, int y, double scale, bool canSelect, bool online = false, field** tab_enemy = nullptr);