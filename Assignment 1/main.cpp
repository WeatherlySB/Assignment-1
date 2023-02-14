#include<iostream>
#include<vector>
#include<algorithm>
#include<random>

std::random_device dv;
std::default_random_engine mt(dv());


/* testing key value
if (player.hasKey = true) {
		std::cout << "TRUE key\n";
	}
	if (player.hasKey = false) {
		std::cout << "FALSE key\n";
	}
	system("PAUSE");
*/

struct Room
{
	enum class Name { cell, gate, armory, jailers, exit };
	enum class Direction { N, S, E, W, none };
	std::string message; // the message displayed when in the room.
	std::vector<Direction> doors;// the directions in which there are doors.
	std::vector<Name> connectedRoom;// the name of the room in which the corresponding door leads.
	//NOTE:: these two vectors will eventually become a std::map after the STL level.
	bool hasKey{ false }; //whether or not the player has picked up the key.
};

//struct keeps track of the room the player is in, their health, and rather or not they have picked up the key.
struct Player
{
	Room::Name currentRoom{ Room::Name::cell };
	int health{ 10 };
	bool hasKey{ false };
};
/* GUI Functions */
void clearConsole();
void pauseConsole();
void splashScreen();
void displayGameState(Player player, std::vector<Room>& map, Room location);
void displayGameDone(Player player, std::vector<Room> map);
void displayIllegalMove(Player player, Room::Direction action);
Room::Direction getAction();

/* Engine Functions*/
std::vector<Room> buildMap();
void randomizeKey(std::vector<Room>& map);
bool changeGameState(Player& player, Room::Direction action, std::vector<Room>& map, Room location);
bool gameIsNotDone(Player);

int main() {

	/*Splash Screen*/
	clearConsole();

	splashScreen();

	/*set up game*/
	std::vector<Room> map{ buildMap() };
	Player player;
	Room location;

	/*Until Game Termination Condition*/
	while (gameIsNotDone(player))
	{
		//hasKey = true here

		/*Display Game State*/
		clearConsole();
		displayGameState(player, map, location);

		//hasKey = true here

		/*Collect Player Action*/
		Room::Direction action{ getAction() };

		/*Update Game State*/
		if (!changeGameState(player, action, map, location))
		{
			displayIllegalMove(player, action);
		}
	}

	/*Display Termination Game State*/
	displayGameDone(player, map);

	return 0;

}

std::vector<Room> buildMap()
{
	std::vector<Room> map;
	map.push_back( //CELL
		{
			"A small, dark prison cell with doors South and East.",
			{Room::Direction::S, Room::Direction::E},
			{Room::Name::armory, Room::Name::gate},
			false
		});
	map.push_back( //GATE
		{
			"A large, torchlit room with doors West, South, and East. \n There is daylight entering under the door to the East.",
			{Room::Direction::W, Room::Direction::S, Room::Direction::E},
			{Room::Name::cell, Room::Name::jailers, Room::Name::exit},
			false
		});
	map.push_back( //ARMORY
		{
			"A store room with doors North and East.",
			{Room::Direction::N, Room::Direction::E},
			{Room::Name::cell, Room::Name::jailers},
			false
		});
	map.push_back( //JAILERS (JAILER's KEY)
		{
			"A jailer's barracks with doors West and North.",
			{Room::Direction::W, Room::Direction::N},
			{Room::Name::armory, Room::Name::gate},
			true
		});
	map.push_back( //EXIT (WIN!)
		{
			"YOU FOUND THE KEY AND ESCAPED!",
			{},
			{},
			false
		});
	return map;
}

void clearConsole() {
	system("cls");
}

void pauseConsole() {
	system("PAUSE");
}

void splashScreen() {
	std::cout << "DUNGEON ADVENTURE" << std::endl;
	std::cout << std::endl;
	std::cout << "Shelby Weatherly (2023)" << std::endl;
	std::cout << "CPSC 2376, Programming II, Level 1 Quest" << std::endl;
	std::cout << "UA Little Rock, Computer Science Dept." << std::endl;
	std::cout << std::endl;
	std::cout << "INSTRUCTIONS:" << std::endl;
	std::cout << std::endl;
	std::cout << "Find the key and get out of the dungeon!" << std::endl;
	std::cout << std::endl;
	std::cout << "        (North)          " << std::endl;
	std::cout << "           w             " << std::endl;
	std::cout << "           |             " << std::endl;
	std::cout << "(West) a --+-- d (East)  " << std::endl;
	std::cout << "           |             " << std::endl;
	std::cout << "           s             " << std::endl;
	std::cout << "        (South)          " << std::endl;
	std::cout << std::endl;
	pauseConsole();
}


//randomly place the key in either the cell, the armory, the jailer's barrack, or the gate room.
void randomizeKey(std::vector<Room>& map) //you will need to decide on the return type and parameters!
{
	  std::uniform_int_distribution<int>ran(0, 3);

	 int randRoom = ran(dv);
	map[randRoom].hasKey = false;

}

//output information as demonstrated in the instructions.
//You will need to deal with display changes that depend on the key.
//NOTE:: NO game variables such as the player or rooms should be modified in this function!!!
void displayGameState(Player player, std::vector<Room>& map, Room location)
{
	//hasKey = true here

	if (player.health == 10)
	{
		location = map[0];
	}
	std::cout << " View: " << map[static_cast<int>(player.currentRoom)].message << '\n';
	std::cout << " Health: " << player.health << '\n';
	std::cout << " Equip: ";
	if (player.hasKey) {
		std::cout << "Jailer's Key";
	}
	std::cout << '\n' << '\n';
	std::cout << " Select Action: \n";
	std::cout << "          (North)          " << std::endl;
	std::cout << "             w             " << std::endl;
	std::cout << "             |             " << std::endl;
	std::cout << "  (West) a --+-- d (East)  " << std::endl;
	std::cout << "             |             " << std::endl;
	std::cout << "             s             " << std::endl;
	std::cout << "          (South)          " << std::endl;



	displayGameDone(player, map);



}
//output messages depending on if the player has one or lost.
void displayGameDone(Player player, std::vector<Room> map)
{
	if (player.health <= 0)
	{
		std::cout << " BUMMER, YOU DIED . . . RIP.\n";
	}
	
	else
		return;
}

//output illegal move messages depending if player tries to exit without the key
//or the player tries to exit the wrong way out of a room.
void displayIllegalMove(Player player, Room::Direction action)
{

	if (action == Room::Direction::none)
	{
		std::cout << "Wrong direction . . . \n";
		pauseConsole();
	}

}



//ask for user input, convert char to Room::Direction
Room::Direction getAction()
{
	char move{ 'x' };
	std::cin >> move;
	switch (move)
	{
	case 'w':
	case 'W':
		return Room::Direction::N;
	case 'a':
	case 'A':
		return Room::Direction::W;
	case 's':
	case 'S':
		return Room::Direction::S;
	case 'd':
	case 'D':
		return Room::Direction::E;
	default:
		return Room::Direction::none;
	}
}


//This function is the only one that should make modifications to the player/rooms including picking up the key.
// This function should call other functions in order to display illegal moves and/or move the player to a new room.
// If the player moves to a new room, health needs to be decremented.
// Look at int main to decide on return type./
bool changeGameState(Player& player, Room::Direction action, std::vector<Room>& map, Room location)
{
	randomizeKey(map);
	int currentIndex = static_cast<int>(player.currentRoom);
	

	for (int i = 0; i < map[currentIndex].doors.size(); i++) {
		if (map[currentIndex].doors[i] == action) {
			auto connectedRoom = map[currentIndex].connectedRoom[i];
			player.health--;
			player.currentRoom = connectedRoom;
			location = map[static_cast<int>(connectedRoom)];
			if (map[currentIndex].hasKey) {
				std::cout << "You found the key, now you just have to find the exit...\n";
				player.hasKey = true;
				//map[currentIndex].hasKey = false;
				system("PAUSE");
			}

			return true;
		}
		
			else
			{
				displayIllegalMove(player, action);
			}
		}


	return false;
}




//Check the end-of-game conditions.  i.e If the player is out
//of health or the player has reached the exit
bool gameIsNotDone(Player player) //false = donee
{
	if (player.health <= 0) { return false; }


}