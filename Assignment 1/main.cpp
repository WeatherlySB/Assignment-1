#include<iostream>
#include<vector>
#include<algorithm>


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
void displayGameState(Player player, std::vector<Room> &map, Room location);
void displayGameDone(Player player, std::vector<Room> map);
void displayIllegalMove(Player player, Room::Direction action);
Room::Direction getAction();

/* Engine Functions*/
std::vector<Room> buildMap();
void randomizeKey();
bool changeGameState(Player& player, Room::Direction action, std::vector<Room>& map);
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

		/*Display Game State*/
		clearConsole();
		displayGameState(player, map, location);

		/*Collect Player Action*/
		Room::Direction action{ getAction() };



		/*Update Game State*/
		if (!changeGameState(player, action, map))
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


//setting these as void or int temporarily, may change later

//randomly place the key in either the cell, the armory, the jailer's barrack, or the gate room.
//SETTING THIS TO VOID FOR NOW
void randomizeKey() //you will need to decide on the return type and parameters!
{

}

//output information as demonstrated in the instructions.
//You will need to deal with display changes that depend on the key.
//NOTE:: NO game variables such as the player or rooms should be modified in this function!!!
void displayGameState(Player player, std::vector<Room>& map, Room location)
{
	if (player.health == 10 && player.currentRoom == Room::Name::cell)
	{
		Room current = map.at(0);
		std::cout << "View: " << current.message << std::endl;
	}

		std::cout << " Health: " << player.health << '\n';
		std::cout << " Equip: " << '\n' << '\n';
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
	 if (player.health == 0)
	 {
		 std::cout << " BUMMER, YOU DIED . . . RIP.\n";
	 }
	 else if (player.health > 0 && player.currentRoom != Room::Name::armory && player.currentRoom != Room::Name::gate)
	 {
		 return;
	 }
	 else if (player.health > 0 && player.currentRoom == Room::Name::armory && player.currentRoom == Room::Name::gate)
	 {
		 std::cout << "YOU FOUND THE KEY AND ESCAPED! \n";
	 }
	 else
		return;
}

//output illegal move messages depending if player tries to exit without the key
//or the player tries to exit the wrong way out of a room.
void displayIllegalMove (Player player, Room::Direction action)
{

	std::cout << "Wrong direction . . . \n";
}



//ask for user input, convert char to Room::Direction
Room::Direction getAction()
{
	char move{ 'x' };
	std::cin >> move;
	Room::Direction chosenDirection;
	switch (move)
	{
	case 'w':
	case 'W':
		chosenDirection = Room::Direction::N;
		std::cout << "North\n";
		break;
	case 'a':
	case 'A':
		chosenDirection =  Room::Direction::W;
		std::cout << "West\n";
		break;
	case 's':
	case 'S':
		chosenDirection = Room::Direction::S;
		std::cout << "South\n";
		break;
	case 'd':
	case 'D':
		chosenDirection = Room::Direction::E;
		std::cout << "East\n";
		break;
	default:
		chosenDirection = Room::Direction::none;
		std::cout << "Wrong way\n";
		break;
	}
	return chosenDirection;

}




//This function is the only one that should make modifications to the player/rooms including picking up the key.
// This function should call other functions in order to display illegal moves and/or move the player to a new room.
// If the player moves to a new room, health needs to be decremented.
// Look at int main to decide on return type.
//SETTING AS INT FOR NOW
bool changeGameState(Player& player, Room::Direction action, std::vector<Room>& map)
{
	//movements with action
	Room current = map.at(0); // Start in the cell
	//simple set up
	while (player.currentRoom != Room::Name::exit)
	{
		std::cout << "view: " << current.message << std::endl;
		if (current.hasKey)
		{
			std::cout << "You have picked up the key!" << std::endl;
			break;
		}
		// Go to the armory
		else if (player.currentRoom == Room::Name::cell)
		{
			for (int i = 0; i < current.connectedRoom.size(); i++)
			{
				if (current.connectedRoom[i] == Room::Name::armory) {
					current = map[i];
					break;
				}
			}
		}
		//go to exit
		else if (player.currentRoom == Room::Name::gate)
		{
			for (int i = 0; i < current.connectedRoom.size(); i++)
			{
				if (current.connectedRoom[i] == Room::Name::exit) {
					current = map[i];
					break;
				}
			}
		}
		// Go to the jailers room
		else if (player.currentRoom == Room::Name::armory)
		{
			for (int i = 0; i < current.connectedRoom.size(); i++)
			{
				if (current.connectedRoom[i] == Room::Name::jailers)
				{
					current = map[i];
					break;
				}
			}
		}
		if (player.currentRoom == Room::Name::jailers)
		{
			for (int i = 0; i < current.connectedRoom.size(); i++)
			{
				if (current.connectedRoom[i] == Room::Name::gate) {
					current = map[i];
					break;
				}
			}
		}
		player.health--;
		system("PAUSE");
		return true;
	}
	return false;
}
	


//Check the end-of-game conditions.  i.e If the player is out
//of health or the player has reached the exit
bool gameIsNotDone(Player)
{

	return true; //FOR NOW
}