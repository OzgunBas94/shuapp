//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ozgun Bas
//Student number: b8037359

#include "Users.h"
#include "DatabaseManager.h"
using namespace std;
//_____________UserBase_____________

using Username = string;

UserBase::UserBase(const Username& username, const string& password, const string& email)
	: m_username(username)
	, m_password(password)
	, m_email(email)
{}

UserBase::~UserBase() {}

const string UserBase::get_username() const {
	return m_username;
}

const string UserBase::get_password() const {
	return m_password;
}
void UserBase::set_password(const string& val) {
	m_password = val;
}

const string UserBase::get_email() const {
	return m_email;
}

void UserBase::set_email(const string& val) {
	m_email = val;
}
//_____________AdminUser_____________
void AdminUser::add_user() {
	string username;
	string password;
	string email;
	string userType;
	string age;

		cout << "Add new user." << endl;
		cout << "Username: ";
		cin >> username;
		while (!(DatabaseManager::instance().find_user(username) == nullptr)) {
			cout << "This username is already taken. Please try again." << endl;
			cout << "Username: ";
			cin >> username;
		}
		cout << "Password: ";
		cin >> password;
		cout << "Email: ";
		cin >> email;
		while (DatabaseManager::instance().find_email(email)) {
			cout << "This email is already taken. Please try again!: ";
			cout << "Email: ";
			cin >> email;
		}
		cout << "Age: ";
		cin >> age;
		cout << "Usertype (admin/player): ";
		cin >> userType;
		while (! (userType != "admin" || userType != "Admin"
			||  userType != "player" || userType != "Player")) {
			cout << " This usertype is not valid. Please try again!";
			cin >> userType;
		}

		DatabaseManager::instance().store_user_data(username, password, email, userType, stoi(age));
	}

void AdminUser::add_game() {
		string title, description, ageRating, gamestudio;
		double price;

		cout << "Add new game." << endl;
		cout << "Title: ";
		cin.ignore();
		getline(cin, title);
		cout << "Gamestudio: ";
		cin >> gamestudio;
		auto pGamestudio = DatabaseManager::instance().find_user(gamestudio);
		if (pGamestudio != nullptr) {
			cout << "Description: ";
			cin.ignore();
			getline(cin, description);
			cout << "Price: ";
			cin >> price;
			cout << "Age rating: ";
			cin >> ageRating;
			DatabaseManager::instance().store_game_data(title, description, price, stoi(ageRating),pGamestudio->get_username());
			cout << endl << "You have added the game '" << title << "' successfully!" << endl << endl;
		}else {
			cout << "This gamestudio does not exist!\n";
		}
}

void AdminUser::list_all_users() const
{
	auto userVisitorLambda = [](const UserBase& rUser) {
		cout << rUser.get_username() << " " << rUser.get_email() << " " << "\n";
	};

	DatabaseManager::instance().visit_users(userVisitorLambda);
}

void AdminUser::delete_game(){
	cout << "ID of the game you want to delete: ";
	string gameId;
	cin >> gameId;

	DatabaseManager::instance().remove_game(gameId);
}


void AdminUser::modify_game(Game*& game, const int option, const int gameId) {

	string newGameDesc;
	string newGamePrice;

	if (option == 1) {
		cout << "New description: ";
		cin.ignore();
		getline(cin, newGameDesc);
		game->set_description(newGameDesc);
		DatabaseManager::instance().modify_game(game, newGameDesc, "", "");
		cout << "You have changed the description successfully!" << endl;
	}
	else {
		cout << "New price: " ;
		cin >> newGamePrice;
		double newPrice = stod(newGamePrice);
		game->set_price(newPrice);
		DatabaseManager::instance().modify_game(game,"", newGamePrice,"");
		cout << "You have changed the description successfully!" << endl;
	}
}

const map<Game::GameId, Game*> PlayerUser::get_user_map() const {
	return m_usersGames;
}

 list<string>PlayerUser::get_recorded_list()  {
	return m_recordedData;
}

void AdminUser::view_statistics() {
	string username, game, date, time, length;
	auto allGames = DatabaseManager::instance().get_games_map();
	list<Game> mostPurchasedGamesList;
	auto it3 = allGames.begin();
	auto mostPurchased = it3;

	cout << "Which statistic you want to view" << endl;
	cout << "(1) Records of all purchases" << endl;
	cout << "(2) Time each player spends in each game" << endl;
	cout << "(3) Most purchased game" << endl;
	cout << "(4) Avarage game price" << endl;

	char option;
	cin >> option;

	map<UserBase::Username, UserBase*> myMap = DatabaseManager::instance().get_map();
	//map<string, int> myMap = DatabaseManager::instance().get_map();
	switch (option) {
	case '1':
		if (!myMap.empty()) {
			for (map<UserBase::Username, UserBase*>::const_iterator it = myMap.begin(); it != myMap.end(); ++it) {
				if (it->second->get_user_type() == UserTypeId::kPlayerUser) {
					PlayerUser* pUser = dynamic_cast<PlayerUser*>(it->second);
					if (!pUser->get_user_map().empty()) {
						for (auto it: pUser->get_user_map()) {
								cout << pUser->get_username() << " has bought " << it.second->get_title() << " " << it.second->get_game_id() << " " << pUser->get_date_of_bought_game() << endl;

						}
					}

				}

			}
		}break;

	case '2': 
		
		if (!myMap.empty()) {
			for (map<UserBase::Username, UserBase*>::iterator it = myMap.begin(); it != myMap.end(); ++it) {
				if (it->second->get_user_type() == UserTypeId::kPlayerUser) {
					PlayerUser* pUser = dynamic_cast<PlayerUser*>(it->second);
					list <string> recordedList = pUser->get_recorded_list();
					
						list<string>::const_iterator it2 = recordedList.begin();
						if (recordedList.size()>0) {
							cout << pUser->get_username();
						while (it2 != recordedList.end()) {
							{
								game = *it2;
								date = *(++it2);
								time = *(++it2);
								length = *(++it2);
								cout << " played " << game << " on " << date << " at " << time << " for " << length << " secconds" << endl;
								++it2;
							}
						}
					}

				}

			}
		} break;
	case '3': 
		int tmp;
		int gameId;
		tmp = it3->second.get_gameCounter();
		++it3;
			while (it3 != allGames.end()) {
				if (tmp <= it3->second.get_gameCounter()) {
					if (tmp < it3->second.get_gameCounter()) {
						tmp = it3->second.get_gameCounter();
						mostPurchasedGamesList.clear();
						mostPurchased = it3;
					}
					else {
						mostPurchasedGamesList.push_back(it3->second);
					}
				}
				++it3;
			}
			if (mostPurchasedGamesList.empty()) {
				cout << endl << "The most purchased game is: " << mostPurchased->second.get_title() << "("
					<< mostPurchased->second.get_gameCounter() << "x)" << endl;
			} else {
				cout << endl << "The most purchased games are with (" << mostPurchased->second.get_gameCounter()<< "x):"
					<< endl << mostPurchased->second.get_title() << endl;
				for (auto it : mostPurchasedGamesList) {
					cout << "- " << it.get_title() << endl;
				}
				cout << endl;
			} break;
	
	case '4':
		double avarageGamePrice;
		double gamePrice = 0.0;
		for (auto it : allGames) {
			gamePrice += it.second.get_price();
		}
		avarageGamePrice = gamePrice / allGames.size();
		cout << "The avarage game price of the games are " << avarageGamePrice << " \x9C" << endl;
		break;

	//default:  cout << "INAVLID OPTION\n"; break;
	}

}

//_____________PlayerUser_____________

PlayerUser::PlayerUser(const Username& username, const string& password, const string& email, const int age, const double fund)
	: UserBase(username, password, email),  age(age), accountFunds(fund) {}

const UserTypeId PlayerUser::get_user_type() const {
	return UserTypeId::kPlayerUser;
}

double PlayerUser::get_available_funds() const {
	return accountFunds;
}

void PlayerUser::set_date_purchased_game(const string& dateOfGame) {
	date = dateOfGame;
}

const string PlayerUser::get_date_of_bought_game() const {
	return date;
}

void PlayerUser::set_date_of_playing_game(const string& dateOfPlayingGame) {
	dateOfPlay = dateOfPlayingGame;
}

void PlayerUser::set_time_of_playing(const string& timeOfPlaying) {
	time = timeOfPlaying;
}

const string PlayerUser::get_time_of_playing() const {
	return time;
}

void PlayerUser::set_length_of_playing(string lengthOfPlaying) {

	length = lengthOfPlaying;
}

const string PlayerUser::get_length_of_playing() const {
	return length;
}

const int PlayerUser::get_age_of_player() const {
	return age;
}

const PlayerUser::UserGames& PlayerUser::get_game_map() const {
	return m_usersGames;
}

void PlayerUser::add_game_to_map(const Game::GameId& id, Game* pGame) {
	m_usersGames.insert(make_pair(pGame->get_game_id(), pGame));
}

void PlayerUser::list_my_games() {

	cout << "My games:" << endl;
	//UserGames m_usersGamesMap = get_user_map();
	if (!m_usersGames.empty()) {
		for (auto it : m_usersGames) {
			auto pGame = DatabaseManager::instance().find_game(it.first);
			cout << "ID: " << pGame->get_game_id() << " TITLE: " << pGame->get_title() << " DESCRIPTION: " << pGame->get_description() << endl;
		}
	}
	else {
		cout << "map is empty" << endl;
	}
}

void PlayerUser::list_games_by_ageRating() {
	int age;
	cout << "Until which age rating do you want to list the games? " << endl;
	cout << "Age rating: ";
	cin >> age;

	DatabaseManager::instance().list_games_by_age_rating(age);

}

void PlayerUser::search_game_by_title() {
	string title;
	cout << "Which game do you want to search?" << endl;
	cout << "Title: ";
	cin.ignore();
	getline(cin, title);

	auto pGame = DatabaseManager::instance().find_game_with_title(title);
	if (pGame == nullptr) {
		cout << "This game is not existing! Please try again" << endl << endl;
	}
	else {
		cout << "Title: " << pGame->get_title() << " Description: " << pGame->get_description() << " Price: " << pGame->get_price() << endl << endl;
	}
}

void PlayerUser::add_funds() {
	cout << "Add funds: ";
	string fund;
	cin >> fund;
	this->accountFunds += stod(fund);
	double get = get_available_funds();
	cout << "FUNDS: " << get << endl;
	DatabaseManager::instance().modify_user(get_username(), accountFunds);
	cout << "You successfully added " << fund << " in your wallet! \n Current wallet: " << accountFunds << endl;
}

void PlayerUser::withdraw_funds(const double val) {
	accountFunds -= val;
}
void PlayerUser::buy_game() {
	Game* pGame = nullptr;
	double userFunds = 0;
	double gamePrice = 0;
	string id;

	cout << "Enter the ID of the game: ";
	cin >> id;

	if (id != "") {
		pGame = DatabaseManager::instance().find_game(stoi(id));
	}
	if (this->get_age_of_player() >= pGame->get_ageRating()) {
		if (pGame != nullptr) {
			gamePrice = pGame->get_price();
		}
		userFunds = this->get_available_funds();
		if (userFunds >= gamePrice) {
			this->withdraw_funds(gamePrice);
			DatabaseManager::instance().modify_user(this->get_username(), this->get_available_funds());
			add_game_to_map(pGame->get_game_id(), pGame);
			DatabaseManager::instance().store_purchased_game(this, pGame);
			cout << "You successfully bought the game - " << pGame->get_title() << " - " << endl;
		}
		else {
			cout << "You don't have enough money to buy this game!" << endl << endl;
		}
	}else {
			cout << "You are to young to buy this game." << endl;
		}
	}

void PlayerUser::add_recorded_data( string game, string date, string time, string length) {
	m_recordedData.push_back(game);
	m_recordedData.push_back(date);
	m_recordedData.push_back(time);
	m_recordedData.push_back(length);
}	

void PlayerUser::play_game() {
	string gameId;
	int result = 0;
	string date;
	string time;
	double length;

	cout << "Which game do you want to play? \n";
	if (!m_usersGames.empty()) {
		list_my_games();
		cout << "ID: ";
		cin >> gameId;

		auto pGame = DatabaseManager::instance().find_game(stoi(gameId));
		CStopWatch stopwatch;
		stopwatch.startTimer();

		cout << "Welcome to -- " << pGame->get_title() << " --" << endl;

		while (result == 0) {
			char option;
			cout << "(q) Quit the Game" << endl;
			cin >> option;
			switch (option) {
			case 'q':

				date = DatabaseManager::instance().get_date_of_play_game();
				time = DatabaseManager::instance().getTime();
				stopwatch.stopTimer();
				length = stopwatch.getElapsedTime();

				DatabaseManager::instance().store_recorded_game_data(this, pGame, date, time, length);

				result = -1;
				break;
			default:  cout << "INAVLID OPTION\n"; break;
			}

		}
		pGame = nullptr;
	}
	else {
		cout << "Your bag is empty" << endl << endl;
	}
}
void PlayerUser::gift_a_game() {
	cout << "Which player do you want to gift?" << endl;
	string giftPlayer, giftGame;
	cin >> giftPlayer;
	PlayerUser* giftUser = dynamic_cast<PlayerUser*>(DatabaseManager::instance().find_user(giftPlayer));

		if (giftUser != nullptr) {
			auto listOfUserToGift = giftUser->get_game_map();
			cout << "Which game do you want to gift?" << endl;
			cin >> giftGame;
			auto pGame = DatabaseManager::instance().find_game_with_title(giftGame);
			if (pGame != nullptr) {
				if (giftUser->get_age_of_player() >= pGame->get_ageRating()) {
					auto ownedGames = get_game_map();
					for (map<Game::GameId, Game*> ::const_iterator it = ownedGames.begin(); it != ownedGames.end(); ++it) {
						if (it->first == pGame->get_game_id()) {
							listOfUserToGift.insert(make_pair(pGame->get_game_id(), pGame));
							DatabaseManager::instance().store_purchased_game(giftUser, pGame);
							ownedGames.erase(it);
							DatabaseManager::instance().remove_users_game(to_string(pGame->get_game_id()));
							break;
						}
					}
					cout << "You gave the game " + pGame->get_title() + " to: " + giftUser->get_username() + " as a present."<<endl;
				}else {
					cout << endl << "You can't give this game to " << giftUser->get_username() << " because " << giftUser->get_username() +
										" is " << giftUser->get_age_of_player() << " years old!" << endl;
									cout << " The age rating for" << pGame->get_title() << " is: " << pGame->get_ageRating() << "!" << endl;
				}
				pGame = nullptr;
			}else {
				cout << "Couldn't find game";
			}
			giftUser = nullptr;
		}else {
			cout << "This user doesn't exist!" << endl;
		}
	}

//_____________________________GUEST_____________________________

const UserTypeId GuestUser::get_user_type() const {
	return UserTypeId();
}
//_____________________________GAMESTUDIO_____________________________

const UserTypeId GameStudio::get_user_type() const {
	return UserTypeId::kGameStudioUser;
}

void GameStudio::set_version() {
	output_gameList();
	cout << "ID of the game to change the version: ";
	string id;
	cin >> id;
	auto pGame = DatabaseManager::instance().find_game(stoi(id));
	if (pGame != nullptr) {

		cout << "Enter a new version: ";
		string version;
		cin >> version;
		int gameVer = stoi(version);
		pGame->set_new_version(gameVer);

		DatabaseManager::instance().modify_game(pGame, "", "", version);
		cout << "UPDATED - You have changed the version to " << version << endl;
	}
	else {
		cout << "Could not find the game.";
	}

}

float const GameStudio::get_version(const string& gameId) const {
	auto pGame = DatabaseManager::instance().find_game(stoi(gameId));
	return pGame->get_version();
}

void GameStudio::add_game_to_list(const Game& rGame) {
	l_gameList.push_back(rGame);
}

const list<Game> GameStudio::get_gameList() const {
	return l_gameList;
}

const void GameStudio::output_gameList() const {
	cout << "Your games: " << endl;
	for (list<Game>::const_iterator it = l_gameList.begin(); it != l_gameList.end(); ++it) {
		cout << "ID: " << it->get_game_id() << "  Title: " << it->get_title() << "  Description: " +
			it->get_description() << "  Version: " << it->get_version() << endl;
	}
}


