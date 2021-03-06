//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ozgun Bas
//Student number: b8037359
#pragma once

#include <string>
using namespace std;

class Game
{
public:
	using GameId = int;

	Game(const GameId& gameId, const string& title, const string& desc,  double& price, const int ageRating, const string& studio, const int version)
		: m_gameId(gameId)
		, m_title(title)
		, m_description(desc)
		, m_price(price)
		, m_ageRating(ageRating)
		, m_studio(studio)
		, m_version(version)
	{}

	virtual ~Game() {}

	const Game::GameId get_game_id() const { return m_gameId; }

	const string get_title() const { return m_title; }

	const string get_description() const { return m_description; }
	
	const int get_ageRating() const { return m_ageRating; }

	const double get_price() const { return m_price; }

	const string get_studio() const { return m_studio; }

	const int get_version() const { return m_version; }

	const int get_gameCounter() const { return gameCounter; }

	void set_title(const string& val) { m_title = val; }

	void set_description(const string& val) { m_description = val; }

	void set_price(const double val) { m_price = val; }

	void set_ageRating(const int val) { m_ageRating = val; }

	void set_studio(const int val) { m_studio = val; }

	void set_new_version(const int val) { m_version = val; }

	void increment_gameCounter() {	++gameCounter;}

private:
	const GameId m_gameId;	// Numerical identifier for a game  (unique key).
	string m_title; // The game title.
	string m_description; // Some information about the game.
	double m_price; //The price of the game.
	int m_ageRating;
	int gameCounter = 0; 
	string m_studio;
	int m_version;
};



