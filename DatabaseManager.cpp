//C++ Boot Camp - Task 2 - 2018-19 
//Name: Ozgun Bas
//Student number: b8037359

#include "DatabaseManager.h"

DatabaseManager::DatabaseManager()
{

}

DatabaseManager::~DatabaseManager()
{
	// We must delete users since we have take ownership.
	for (auto it : m_users)
	{
		if (it.second) delete it.second;
	}
}

DatabaseManager& DatabaseManager::instance()
{
	static DatabaseManager s_instance;
	return s_instance;
}


void DatabaseManager::load_data()
{
	
	load_user_list();
	load_game_list();
	load_user_game();
	load_recorded_game_data();

}

void DatabaseManager::load_user_list() {
	string username, pw, email, userType, funds;
	double fundsInDouble;
	ifstream file("listUsers.txt");
	if (file.is_open()) {

		while (file >> username >> pw >> email >> userType >> funds) {
			if (userType == "admin" || userType == "Admin") {
				add_user(new AdminUser(username, pw, email));
			}
			else {
				fundsInDouble = stod(funds);
				add_user(new PlayerUser(username, pw, email, fundsInDouble));
			}

		}
		file.close();
	}
	else {
		cout << "Could not open the file: " << endl;
	}
}


void DatabaseManager::load_game_list() {

	ifstream gameFile;
	string readFile;
	string gameTitle;
	string gameDesc;
	double gamePrice;
	int gameId = 0;

	gameFile.open("listGames.txt");

	if (gameFile.is_open()) {
		while (getline(gameFile, readFile)) {

			gameId = stoi(get_token(readFile));
			gameTitle = get_token(readFile);
			gameDesc = get_token(readFile);
			gamePrice = stod(get_token(readFile));

			add_game(Game(gameId, gameTitle, gameDesc, gamePrice));
		}

		this->idCount = gameId;
		gameFile.close();
	}
	else {
		cout << "Could not open this file!";
	}
}
void DatabaseManager::load_user_game() {
	string readFile;
	string username;
	string gameId;
	string date;
	ifstream file("listUserGames.txt", ios::app);

	if (file.is_open()) {
		if(!(file.peek()== ifstream::traits_type::eof())){
			while (getline(file, readFile)) {
				username = get_token(readFile);
				auto pUser = dynamic_cast<PlayerUser*>(find_user(username));
				while ((gameId = get_token(readFile)) != "") {
					date = get_token(readFile);
					pUser->set_date_of_bought_game(date);
					Game::GameId id = stoi(gameId);
					Game* pGame = find_game(stoi(gameId));
					pUser->add_game_to_map(id,pGame);

				}
			}
		}
		file.close();
	}
	else {
		cerr << "Couldn't open the file!";
	}
}

void DatabaseManager::load_recorded_game_data() {

}

void DatabaseManager::store_user_data(string user, string pw, string mail, string userType)
{
	ofstream outfile("listUsers.txt", ios_base::app);

	string username =user;
	string password = pw;
	string email = mail;
	string typeOfUser = userType;
	string funds = "0.0";
	double fundsInDouble;
		if (outfile.is_open()){
			outfile << username << " " << password << " " << email << " " << typeOfUser << " " << funds << endl;
			outfile.close();
			if (typeOfUser == "admin" || typeOfUser == "Admin") {
				add_user(new AdminUser(username, password, email));
			}
			else {
				fundsInDouble = stod(funds);
				add_user(new PlayerUser(username, password, email, fundsInDouble));
				//add_user(new PlayerUser(username, password, email));
			}
		}
		else {
			cout << "This Textfile does not exist!";
		}
	}


void DatabaseManager::store_game_data(string& title, string& description, double price){
	ofstream outFile;
	outFile.open("listGames.txt", ios::app);

	string gameTitle = title;
	string gameDesc = description;
	int id = ++(this->idCount);
	string gameId = to_string(id);
	double gamePrice = price;

	if (!outFile) {
		cout << "Textfile doesn't exist!";
	}
	else {
		outFile << id << ',' << gameTitle << ',' << gameDesc << ',' << to_string(gamePrice) << "," << endl;
		outFile.close();
		add_game(Game(id, gameTitle, gameDesc, gamePrice));
		cout << "You added the game: " << title << endl;
	}

}

void DatabaseManager::store_bought_game(PlayerUser* rPlayer, Game* rGame) {
	ifstream file("listUserGames.txt", ios::app);
	ofstream newFile("listOfUserGames.txt", ios::app);
	string readFile;
	string username;
	string gameId;
	string timeOfPurchase;
	bool gameIsStored = false;

	string tmp;

	if (file.is_open()) {
		if (!(file.peek() == ifstream::traits_type::eof())) {

			while (getline(file, readFile)) {
				username = get_token(readFile);
				tmp = username + ",";
				while ((gameId = get_token(readFile)) != "") {
					tmp += gameId;
					tmp += ",";
					timeOfPurchase = get_token(readFile);
					tmp += timeOfPurchase;
					tmp += ",";
				}
				if (rPlayer->get_username() == username) {
					string id = to_string(rGame->get_game_id()) + ",";
					tmp += id ;

					time_t date = time(0);   // get time now
					struct tm dateTime;
					errno_t result;
					result = localtime_s(&dateTime, &date);
					timeOfPurchase = (to_string(dateTime.tm_mday )+ "/" +  to_string (dateTime.tm_mon )+ "/" + to_string(dateTime.tm_year));
					tmp += timeOfPurchase + ","; 

					tmp += '\n';
					if (newFile.is_open()) {
						newFile << tmp;
						gameIsStored = true;
					}
					else {
						cerr << "Couldn't open the file!";
					}
				}
				if(!gameIsStored){
					time_t date = time(0);   // get time now
					struct tm dateTime;
					errno_t result;
					result = localtime_s(&dateTime, &date);
					timeOfPurchase = (to_string(dateTime.tm_mday) + "/" + to_string(dateTime.tm_mon + 1) + "/" + to_string(dateTime.tm_year + 1900));

					username = rPlayer->get_username();
					gameId = to_string(rGame->get_game_id());
					tmp = username + "," + gameId + "," + timeOfPurchase + ",";
					newFile << tmp;

					gameIsStored = true;
				}
			}
		}
		else {
			time_t date = time(0);   // get time now
			struct tm dateTime;
			errno_t result;
			result = localtime_s(&dateTime, &date);
			timeOfPurchase = (to_string(dateTime.tm_mday) + "/" + to_string(dateTime.tm_mon +1) + "/" + to_string(dateTime.tm_year +1900));

			username = rPlayer->get_username();
			gameId = to_string(rGame->get_game_id());
			tmp = username + "," + gameId + "," + timeOfPurchase + "," ;
			newFile << tmp;
			gameIsStored = true;
		}
		file.close();
		newFile.close();
		remove("listUserGames.txt");
		rename("listOfUserGames.txt", "listUserGames.txt");
	}
	else {
		cerr << "Couldn't open the file!";
	}
}

void DatabaseManager::store_recorded_game_data() {

}


void DatabaseManager::add_user(UserBase* pUser)
{
	// Store the user instance in the user map, indexed by username.
	// We are taking ownership of the memory pointer to.
		// open a file in write mode.

		if (pUser)
		{
			m_users.insert(make_pair(pUser->get_username(), pUser));

		}
}


void DatabaseManager::add_game(Game& rGame)
{
	// Store the game indexed by game id.
	m_games.insert(make_pair(rGame.get_game_id(), rGame));
	
}

void DatabaseManager::remove_game(const string& gameId) {
	string id;
	ifstream gameFile;
	ofstream outfile("listGames.txt");
	string readfile;
	gameFile.open("listOfGames.txt", ios::app);

	if (gameFile.is_open()) {
		while (getline(gameFile, readfile)) {
			int position = readfile.find(',');
			id = readfile.substr(0, position);
			if (gameId != id) {
				outfile << readfile<< endl;
			}
			else {
				m_games.erase(stoi(gameId));
				cout << "Game successfully deleted! " << endl;
			}

		}
	}
	else {
		cout << "Could not open this file!";
	}
	gameFile.close();
	outfile.close();
	remove("listGames.txt");
	rename("listOfGames.txt", "listGames.txt");
}

void DatabaseManager::modify_game(Game*& mGame, const string& newGameDesc, const string& newGamePrice ) {
	cout << "test_modify_game" << endl;
	ifstream gameFile;
	ofstream outFile("listGames.txt");
	string id;
	string readFile;
	gameFile.open("listOfGames.txt", ios::app);

	if (gameFile.is_open()) {
		while (getline(gameFile, readFile)) {
			int position = readFile.find(',');
			id = readFile.substr(0, position);
			if (mGame->get_game_id() != stoi(id)) {
				outFile << readFile << endl;
			}
			else {
				if (!newGamePrice.empty()) {
					readFile = id + "," + mGame->get_title() + "," + mGame->get_description() + "," + newGamePrice + ",\n";

					outFile << readFile;
				}
				else if (!newGameDesc.empty()) {
					readFile = id + "," + mGame->get_title() + "," + newGameDesc + "," + to_string(mGame->get_price()) + ",\n";
					outFile << readFile;
				}

			}
		}
	}
	gameFile.close();
	outFile.close();

	remove("listGames.txt");
	rename("listOfGames.txt", "listGames.txt");

}

void DatabaseManager::modify_user(const string& username, const double newFunds) {
	string readFile, id, password, email, usertype, fund;
	ifstream file("listUsers.txt", ios::app);
	ofstream newFile("listOfUser.txt", ios::app);

	if (file.is_open()) {
		while (getline(file, readFile)) {
			istringstream stream(readFile);
			stream >> id >> password >> email >> usertype >> fund;
			if (newFile.is_open()) {
				if (id != username) {
					newFile << readFile << endl;
				}
				else {
					readFile = id + ' ' + password + ' ' + email + ' ' + usertype + ' ' + to_string(newFunds);
					newFile << readFile << endl;
				}
			}
			else {
				cerr << "Couldn't open the file!";
			}
		}
		file.close();
		newFile.close();
		remove("listUsers.txt");
		rename("listOfUser.txt", "listUsers.txt");
	}
	else {
		cerr << "Couldn't open the file!";
	}
}

UserBase* DatabaseManager::find_user(const UserBase::Username& username)
{
	auto it = m_users.find(username);
	if (it != m_users.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

Game* DatabaseManager::find_game(const Game::GameId gameid)
{
	auto it = m_games.find(gameid);
	if (it != m_games.end())
	{
		return &it->second;
	}
	else
	{
		return nullptr;
	}
}


Game* DatabaseManager::find_game_with_title(string& gameTitle) {
	string title = to_lower_string(gameTitle);
	string iteratorTitle;

	auto it = m_games.begin();
	while (it != m_games.end()) {
		iteratorTitle = it->second.get_title();
		iteratorTitle = to_lower_string(iteratorTitle);
		if (iteratorTitle == title) {
			return &it->second;
		}
		++it;
	}
	return nullptr;
}

const map<UserBase::Username, UserBase*> DatabaseManager::get_map() const {
	return m_users;
}



string DatabaseManager::get_token(string& readFile) {

	int position = readFile.find(',');
	string tmp = readFile.substr(0, position);
	readFile = readFile.substr((position + 1), readFile.length() - (position + 1));
	return tmp;
}
string DatabaseManager::to_lower_string(string& lowerString) {
	string searchLowerTitle;
	for (int i = 0; i <= lowerString.length(); ++i) {
		searchLowerTitle += tolower(lowerString[i]);
	}
	return searchLowerTitle;
}
bool DatabaseManager::find_email(const string& mail) {
	for (auto it = m_users.begin(); it != m_users.end(); ++it) {
		if ((*it).second->get_email() == mail) {
			return true;
		}
	}
	return false;
}


