#include "Game.h"
#include "player.h"

Texture tEmpty, tShip, tHit, tMiss, tSink;
Texture blackBgr;

bool AIPlay(player &p, player &opponent, Sprite** tab_own, Sprite** tab_spr, ComputerAlgorithm &AI, RenderWindow& window, Text& com)
{
	ship *tmp;
	tmp = AI.ShootOpponent();
	window.clear(Color(32, 29, 130, 255));
	DrawFields(p, tab_own, true, window, cfg::first_pg_x, cfg::first_pg_y, cfg::scale, false);
	DrawFields(opponent, tab_spr, SHIP_VISIBILITY, window, cfg::second_pg_x, cfg::second_pg_y, cfg::scale, true);
	com.setString("BOT");
	window.draw(com);
	window.display();
	while (tmp != nullptr)
	{
		if (tmp->RefShipLength() == 0)
		{
			if (p.CountShips() == 0)
			{
				return true;
			}
		}
		tmp = AI.ShootOpponent();
		window.clear(Color(32, 29, 130, 255));
		DrawFields(p, tab_own, true, window, cfg::first_pg_x, cfg::first_pg_y, cfg::scale, false);
		DrawFields(opponent, tab_spr, SHIP_VISIBILITY, window, cfg::second_pg_x, cfg::second_pg_y, cfg::scale, true);
		com.setString("BOT");
		window.draw(com);
		window.display();
	}
	return false;
}

void ClearArea(ship& s, field** tab)
{
	int x, y,length;
	DIRECTION dir;
	x = s.getX();
	y = s.getY();
	length = s.getFirstLength();
	dir = (DIRECTION)s.tmpdir;

	switch (dir)
	{
	case UP:
		for (int i = 0; i < length; i++)
		{
			tab[y - i][x].refState() = SINK;
			if (x - 1 >= 0)
				tab[y - i][x - 1].refState() = MISS;
			if (x + 1 < 10)
				tab[y - i][x + 1].refState() = MISS;

		}
		if (y - length >= 0)
			tab[y - length][x] = MISS;
		if (y + 1 < 10)
			tab[y + 1][x] = MISS;
		break;
	case LEFT:
		for (int i = 0; i < length; i++)
		{
			tab[y][x-i].refState() = SINK;
			if (y - 1 >= 0)
				tab[y - 1][x - i].refState() = MISS;
			if (y + 1 < 10)
				tab[y + 1][x - i].refState() = MISS;

		}
		if (x - length >= 0)
			tab[y][x - length] = MISS;
		if (x + 1 < 10)
			tab[y][x + 1] = MISS;
		break;
	case DOWN:
		for (int i = 0; i < length; i++)
		{
			tab[y + i][x].refState() = SINK;
			if (x - 1 >= 0)
				tab[y + i][x - 1].refState() = MISS;
			if (x + 1 < 10)
				tab[y + i][x + 1].refState() = MISS;

		}
		if (y + length < 10)
			tab[y + length][x] = MISS;
		if (y - 1 >= 0)
			tab[y - 1][x] = MISS;
		break;
	case RIGHT:
		for (int i = 0; i < length; i++)
		{
			tab[y][x + i].refState() = SINK;
			if (y - 1 >= 0)
				tab[y - 1][x + i].refState() = MISS;
			if (y + 1 < 10)
				tab[y + 1][x + i].refState() = MISS;

		}
		if (x + length < 10)
			tab[y][x + length] = MISS;
		if (x - 1 >= 0)
			tab[y][x - 1] = MISS;
		break;
	case NONE:
		cout << "SOMETHING WENT WRONG";
		break;
	default:
		break;
	}
}

void LoadTexture(Texture& t, string filename)
{
	if (!t.loadFromFile(filename))
	{
		MessageBox(NULL, "Cannot load a texture!", "ERROR", NULL);
	}
}

void DrawFields(player& p, Sprite** tab, bool visibility, RenderWindow& window, int x, int y, double scale, bool canSelect, bool online, field** tab_enemy)
{
	Vector2f mouse(Mouse::getPosition(window));
	STATUS s;
	for (int i = 0; i < SIZE_OF_PG; i++)
		for (int j = 0; j < SIZE_OF_PG; j++)
		{
			if (!online)
				s = p.WhatState(j, i);
			else
				s = tab_enemy[i][j].refState();
			Sprite& tmp = tab[i][j];
			if (tmp.getGlobalBounds().contains(mouse) && canSelect && p.NotShot(j,i) && !online)
				tmp.setTexture(blackBgr);
			else if(online && tmp.getGlobalBounds().contains(mouse) && canSelect && s == EMPTY)
				tmp.setTexture(blackBgr);
			else
			{
				switch (s)
				{
				case EMPTY:
					tmp.setTexture(tEmpty);
					break;
				case SHIP:
					if (visibility)
						tmp.setTexture(tShip);
					else
						tmp.setTexture(tEmpty);
					break;
				case MISS:
					tmp.setTexture(tMiss);
					break;
				case HIT:
					tmp.setTexture(tHit);
					break;
				case SINK:
					tmp.setTexture(tSink);
					break;
				default:
					MessageBox(NULL, "Wrong field!", "Error", NULL);
					break;
				}
			}
			tmp.setScale(scale, scale);
			tmp.setPosition(x + scale*100*j, y + scale*100*i);
			window.draw(tmp);
		}
}

Game::Game()
{
	state = END;
	if (!font.loadFromFile("font.ttf"))
	{
		MessageBox(NULL, "Cannot find font file!", "ERROR", NULL);
		return;
	}
	state = MENU;
}

bool Game::Shoot(player& p, RenderWindow& window, Sprite** tab, Event& event, Text& t)
{
	Vector2f mouse(Mouse::getPosition(window));
	ship* tmp;
	for (int i = 0; i < SIZE_OF_PG; i++)
		for (int j = 0; j < SIZE_OF_PG; j++)
			if (tab[i][j].getGlobalBounds().contains(mouse) && event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && p.NotShot(j, i))
			{
				tmp = p.Shoot(i, j);
				if (tmp == nullptr)
					return false;
				if (tmp->RefShipLength() == 0)
				{
					if (p.CountShips() == 0)
					{
						state = GAME_OVER;
						t.setString("TU AS GAGNE");
						return true;
					}
					t.setString("TU A COULER L'ENNEMIE, C'EST ENCORE A TOI");
				}
				else
					t.setString("TU A TOUCHER L'ENNEMIE, C'EST ENCORE A TOI");
				return true;
			}
	return true;
}

void Game::menu()
{
	RenderWindow window(VideoMode(cfg::WINDOW_WIDTH, cfg::WINDOW_HEIGHT, 32), "BATAILLE NAVALE", Style::None);
	Text title("BATAILLE NAVALE", font, 100);
	title.setStyle(Text::Bold);
	title.setPosition(cfg::WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 30);

	const int nOptions = 3;
	Text options[nOptions];
	string str[] = { "Singleplayer","Multiplayer Local", "QUITTER" };

	for (int i = 0; i < nOptions; i++)
	{
		options[i].setFont(font);
		options[i].setCharacterSize(50);
		options[i].setString(str[i]);
		options[i].setPosition(cfg::WINDOW_WIDTH / 2 - options[i].getLocalBounds().width / 2, 200 + i * 100);
		options[i].setColor(Color::White);
	}
	Clock clock;
	while (state == MENU)
	{
		Event event;
		Vector2f mouse(Mouse::getPosition(window));
		Time elapsed = clock.restart();

		if (elapsed < milliseconds(1000 / cfg::fps)) // permet d'optimiser le programe et demander trop au proc
			sleep(milliseconds(1000 / cfg::fps) - elapsed);

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				state = END;

			// MENU OPTIONS
			else if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left
				&& options[nOptions - 1].getGlobalBounds().contains(mouse))
				state = END;
			else if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left
				&& options[0].getGlobalBounds().contains(mouse))
			{
				state = GAME;
			}
			else if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left
				&& options[1].getGlobalBounds().contains(mouse))
				state = GAME_MULTI;
		}

		window.clear(Color(32,29,130,255));
		window.draw(title);

		for (int i = 0; i < nOptions; i++)
		{
			if (options[i].getGlobalBounds().contains(mouse))
				options[i].setColor(Color::Black);
			else
				options[i].setColor(Color::White);
			window.draw(options[i]);
		}
		window.display();
	}
}

void Game::Play(player& p, player& opponent, string s)
{
	RenderWindow window(VideoMode(cfg::game_window_width, cfg::game_window_height, 32), "BATAILLE NAVALE", Style::None);
	ComputerAlgorithm ai(&p);
	LoadTexture(tEmpty, "image_empty.png");
	LoadTexture(tShip, "image_ship.png");
	LoadTexture(tHit, "image_hit.png");
	LoadTexture(tMiss, "image_miss.png");
	LoadTexture(tSink, "image_sink.png");
	LoadTexture(blackBgr, "image_select.png");
	Text title_me("VOUS",font, 50);
	Text title_opp("ENNEMIE", font, 50);
	Text back_button("RETOUR", font, 30);
	Text communicate("Votre tour", font, 50);
	title_me.setStyle(Text::Bold);
	title_opp.setStyle(Text::Bold);
	back_button.setStyle(Text::Bold);
	title_me.setPosition((cfg::first_pg_x + 500*cfg::scale) - title_me.getGlobalBounds().width / 2, cfg::first_pg_y + 1000*cfg::scale);
	title_opp.setPosition((cfg::second_pg_x + 500 * cfg::scale) - title_opp.getGlobalBounds().width / 2, cfg::second_pg_y + 1000 * cfg::scale);
	back_button.setPosition(cfg::game_window_width - cfg::scale * 100 - back_button.getGlobalBounds().width, cfg::scale * 50);
	communicate.setPosition(cfg::scale * 100, cfg::scale * 50);
	Sprite** tab_spr = new Sprite*[SIZE_OF_PG];
	Sprite** tab_own = new Sprite*[SIZE_OF_PG];
	for (int i = 0; i < SIZE_OF_PG; i++)
	{
		tab_spr[i] = new Sprite[SIZE_OF_PG];
		tab_own[i] = new Sprite[SIZE_OF_PG];
	}

	Clock clock;
	while (window.isOpen())
	{
		Time elapsed = clock.restart();
		if (elapsed < milliseconds(1000 / cfg::fps))  // permet d'optimiser le programe et demander trop au proc
			sleep(milliseconds(1000 / cfg::fps) - elapsed);
		Vector2f mouse(Mouse::getPosition(window));
		Event event;
		if (back_button.getGlobalBounds().contains(mouse))
			back_button.setColor(Color::Black);
		else
			back_button.setColor(Color::White);

		while (window.pollEvent(event))
		{
			if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left && back_button.getGlobalBounds().contains(mouse))
			{
				if(state == GAME_OVER || IDYES)
				{
					state = MENU;
					window.close();
				}
				continue;
			}
			if (event.type == Event::Closed)
			{
				state = MENU;
				window.close();
			}
			if (state == GAME && !Shoot(opponent, window, tab_spr, event, communicate))
			{
				communicate.setString("BOT");
				window.clear(Color(32, 29, 130, 255));
				window.draw(back_button);
				window.draw(title_me);
				window.draw(title_opp);
				window.draw(communicate);
				DrawFields(p, tab_own, true, window, cfg::first_pg_x, cfg::first_pg_y, cfg::scale, false);
				DrawFields(opponent, tab_spr, (state == GAME_OVER), window, cfg::second_pg_x, cfg::second_pg_y, cfg::scale, state != GAME_OVER);
				window.display();
				if (AIPlay(p, opponent, tab_own, tab_spr, ai, window, communicate))
				{
					state = GAME_OVER;
					communicate.setString("LE BOT A GAGNE");
					
				}
				else
				communicate.setString("Votre tour");
			}

			if (state == GAME_MULTI && !Shoot(opponent, window, tab_spr, event, communicate))
			{
				return;
			}
			
		}
		window.clear(Color(32, 29, 130, 255));
		window.draw(back_button);
		window.draw(title_me);
		window.draw(title_opp);
		window.draw(communicate);

		DrawFields(p, tab_own, (state != GAME_MULTI || cfg::my_ship_visibility) , window, cfg::first_pg_x, cfg::first_pg_y, cfg::scale, false);
		DrawFields(opponent, tab_spr,(state == GAME_OVER || SHIP_VISIBILITY), window, cfg::second_pg_x, cfg::second_pg_y, cfg::scale, state != GAME_OVER);
		window.display();
	}
}

// GAME CLASS' METHODS

void Game::hotSeat()
{
	player p1, p2;
	p1.SetShips();
	p2.SetShips();
	while(1)
	{
		Play(p1, p2, "Player 1");
		if (state != GAME_MULTI)
			break;

		Play(p2, p1, "Player 2");
		if (state != GAME_MULTI)
			break;
	}
}