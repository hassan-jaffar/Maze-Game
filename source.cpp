#include<iostream>
#include <fstream>
//library to use keyboard keys
#include <conio.h>
//library to generate random numbers
#include <cstdlib>
#include <time.h>
using namespace std;
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77


template <class type>		//stack class template based
class Stack {
private:
	struct Node {
	public:
		type data;
		Node* next;
		Node* prev;
		Node(type x, Node* p = 0, Node* n = 0)
		{
			data = x;
			prev = p;
			next = n;
		}
	};
	Node* head;
	Node* tail;
	int size;
public:
	Stack()
	{
		head = new Node(0);
		tail = new Node(0);
		head->next = tail;
		tail->prev = head;
		size = 0;
	}
	~Stack()
	{
		while (!isEmpty())
		{
			pop();
		}
	}
	void push(type& val)
	{
		tail->prev->next = new Node(val, tail->prev, tail);
		tail->prev = tail->prev->next;
		size++;
		//cout << "Item added\n";
	}
	void pop()
	{
		if (!isEmpty())
		{
			Node* temp;
			temp = tail->prev;
			tail->prev = tail->prev->prev;
			tail->prev->next = tail;
			delete temp;
			size--;
			//cout << "Item removed\n";
		}
		else
			cout << "Stack Empty\n";
	}
	void print()
	{
		for (Node* temp = head->next; temp != tail; temp = temp->next)
		{
			if (temp->next != tail)
				cout << temp->data << " -> ";
			else
				cout << temp->data;
		}
		cout << endl;
	}
	bool isEmpty()
	{
		if (size == 0)
			return true;
		else
			return false;
	}
	int getSize()
	{
		return size;
	}
	type& rear()
	{
		return head->next->data;
	}
	type& front()
	{
		return tail->prev->data;
	}
};


class Coordinates {
private:
	int roomNo;
	int x;
	int y;
public:
	Coordinates(int a = 0, int b = 0)
	{
		x = a;
		y = b;
		roomNo = 0;		//sets room no 0 by default. only sets it when needed
	}
	~Coordinates()		//nothing to deallocate in coordinates
	{

	}
	void print()
	{
		cout << "\tRoom #" << roomNo << ": (" << x << ", " << y << ")\t";
	}
	void setCoordinates(int a, int b)
	{
		x = a;
		y = b;
	}
	//getters
	int getX()
	{
		return x;
	}
	int getY()
	{
		return y;
	}
	int getRoomNo()
	{
		return roomNo;
	}
	void setRoomNo(int a)		//set room number only when needed
	{
		roomNo = a;
	}
	bool operator!=(Coordinates& rhs)		//comparison to check if stack got 2 same coordinates consecutively
	{
		if (x == rhs.getX() && y == rhs.getY() && roomNo == rhs.getRoomNo())
			return false;
		else
			return true;
	}
	friend ostream& operator<<(ostream& out, Coordinates& obj);
};
ostream& operator<<(ostream& out, Coordinates& obj)		//overlaoded ostream operator
{
	obj.print();
	return out;
}

class Room {
private:
	int rows;
	int cols;
	Coordinates startPoint;
	char** map;
	bool isTreasure;		//to store if this room has treasure or not. helps in gasper functionality
public:
	Room(int r = 0, int c = 0)
	{
		rows = r;
		cols = c;
		startPoint = Coordinates(0, 0);
		map = new char* [rows];
		for (int i = 0; i < rows; i++)
			map[i] = new char[cols];
		isTreasure = false;
	}
	~Room()		//deallocate map
	{
		for (int i = 0; i < rows; i++)
			delete map[i];
		delete map;
	}
	bool getIsTreasure()
	{
		return isTreasure;
	}
	void loadRoom(const char* fileName)
	{
		fstream file1;
		file1.open(fileName, ios::in);
		if (file1.is_open())
		{
			inputData(file1);
		}
		else
		{
			cout << "error opening file \n";
		}
		file1.close();
	}
	fstream& inputData(fstream& in)			//every file reading its data from its own file
	{
		in >> rows;
		in.ignore();
		in >> cols;
		in.ignore();
		int x = 0, y = 0;
		in >> x;
		in.ignore();
		in >> y;
		in.ignore();
		startPoint.setCoordinates(x, y);
		map = new char* [rows];
		for (int i = 0; i < rows; i++)
		{
			map[i] = new char[cols];
			for (int j = 0; j < cols; j++)
			{
				in >> map[i][j];
				if (map[i][j] == 'T')
					isTreasure = true;
				//in.ignore();
			}
		}

		return in;
	}
	void printRoom()		//printing all the rooms before teleporting user to any room so he has an idea of all the rooms
	{
		cout << "Rows: " << rows << " Cols: " << cols << endl << "Starting Point: ";
		startPoint.print();
		cout << endl;
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
				cout << map[i][j] << " ";
			cout << endl;
		}
		cout << endl << endl;
	}
	int printingCall(Stack<Coordinates>& stack, int roomNo)		//function for iterative method
	{
		int c = 0;
		int x = startPoint.getX(), y = startPoint.getY();
		printRoomMap(x, y);
		while (1)
		{
			c = 0;
			c = _getch();		//getting input from keyboard
			Coordinates temp;
			int a = 0, b = 0;
			switch (c) {
			case KEY_UP:
				if (x > 0)
					a = -1;
				break;
			case KEY_DOWN:
				if (x < rows - 1)
					a = 1;
				break;
			case KEY_LEFT:
				if (y > 0)
					b = -1;
				break;
			case KEY_RIGHT:
				if (y < cols - 1)
					b = 1;
				break;
			case 'q':
			case 'Q':
				system("cls");
				cout << "\n\n\t\tQuit. Game Ended\n\n\n";
				system("pause");
				return 0;
			default:
				break;
			}								//all possible cases dealt with seperately after taking key input
			if (map[x + a][y + b] == 'X')
			{
				printRoomMap(x, y);
			}
			else if (map[x + a][y + b] == '0')
			{
				x = x + a;
				y = y + b;
				printRoomMap(x, y);
				temp.setCoordinates(x, y);
				temp.setRoomNo(roomNo);
				if (temp != stack.front())		//checks if no movement has been made. only adds coordinates on movement
					stack.push(temp);

			}
			else if (map[x + a][y + b] == 'D')
			{
				x = x + a;
				y = y + b;
				printRoomMap(x, y);
				temp.setCoordinates(x, y);
				temp.setRoomNo(roomNo);
				if (temp != stack.front())
					stack.push(temp);
				system("cls");
				cout << "\n\nUser eaten up by Devil. Game Ended\n\n\n";
				return 0;
			}
			else if (map[x + a][y + b] == 'T')
			{
				x = x + a;
				y = y + b;
				printRoomMap(x, y);
				temp.setCoordinates(x, y);
				temp.setRoomNo(roomNo);
				if (temp != stack.front())
					stack.push(temp);
				system("cls");
				cout << "\n\nUser found the Treasure. Game Ended\n\n\n";
				return 1;
			}
			else if (map[x + a][y + b] == 'P')
			{
				x = x + a;
				y = y + b;
				printRoomMap(x, y);
				temp.setCoordinates(x, y);
				temp.setRoomNo(roomNo);
				if (temp != stack.front())
					stack.push(temp);
				system("cls");
				cout << "\n\nUser found the Portal\n\n\n";
				return 2;
			}
			else if (map[x + a][y + b] == 'G')
			{
				x = x + a;
				y = y + b;
				printRoomMap(x, y);
				temp.setCoordinates(x, y);
				temp.setRoomNo(roomNo);
				if (temp != stack.front())
					stack.push(temp);
				system("cls");
				cout << "\n\nUser found the Gasper\n\n\n";
				return 3;
			}
		}
	}
	void printRoomMap(int r, int c) {
		system("cls");

		for (int i = -1; i < rows + 1; i++) {
			for (int j = -1; j < cols + 1; j++)
			{
				if (i == -1 || i == rows || j == -1 || j == cols)		//each element in room got its unique symbol in game
					cout << " " << char(197) << " ";
				else
				{
					if (i == r && j == c && map[i][j] != 'X') cout << " " << char(1) << " ";
					else
					{
						if (map[i][j] == 'X')
							cout << " " << char(197) << " ";
						else if (map[i][j] == '0')
							cout << " " << ' ' << " ";
						else if (map[i][j] == 'D')
							cout << " " << '#' << " ";
						else if (map[i][j] == 'G')
							cout << " " << '~' << " ";
						else if (map[i][j] == 'P')
							cout << " " << char(177) << " ";
						else if (map[i][j] == 'T')
							cout << " " << char(219) << " ";
					}
				}
			}
			cout << endl << endl;
		}
	}
	int getStartX()
	{
		return startPoint.getX();
	}
	int getStartY()
	{
		return startPoint.getY();
	}
	int getRows()
	{
		return rows;
	}
	int getCols()
	{
		return cols;
	}
	char**& getMap()
	{
		return map;
	}
};

bool printingCallR(int& x, int& y, Stack<Coordinates>& stack, Room* rooms, int numOfRooms, int& currentRoom, bool& gasperHelp, int& roomsVisited, int visitedRoomNo[]);
//protoype needs to be written before enterRoom because this needs to be rendered in enterRoom


//can be taken as a wrapper function for the recursive function, to give it a headstart
bool enterRoom(Stack<Coordinates>& stack, Room* rooms, int numOfRooms, int& currentRoom, bool& gasperHelp, int& roomsVisited, int visitedRoomNo[])
{			//part of recursive function when user enters a new room and needs to start from its starting point
	int x = rooms[currentRoom].getStartX(), y = rooms[currentRoom].getStartY();
	rooms[currentRoom].printRoomMap(x, y);
	return printingCallR(x, y, stack, rooms, numOfRooms, currentRoom, gasperHelp, roomsVisited, visitedRoomNo);
	//gives back control to the recursive function
}

bool printingCallR(int& x, int& y, Stack<Coordinates>& stack, Room* rooms, int numOfRooms, int& currentRoom, bool& gasperHelp, int& roomsVisited, int visitedRoomNo[])	//function for recursive method
{					//almost same functionality as iterative approach except that this function does everything on itself instead of giving control to main
	int c = 0;
	while (1)
	{
		c = 0;
		c = _getch();
		Coordinates temp;
		int a = 0, b = 0;
		switch (c) {
		case KEY_UP:
			if (x > 0)
				a = -1;
			break;
		case KEY_DOWN:
			if (x < rooms[currentRoom].getRows() - 1)
				a = 1;
			break;
		case KEY_LEFT:
			if (y > 0)
				b = -1;
			break;
		case KEY_RIGHT:
			if (y < rooms[currentRoom].getCols() - 1)
				b = 1;
			break;
		case 'q':
		case 'Q':
			system("cls");
			cout << "\n\n\t\tQuit. Game Ended\n\n\n";
			system("pause");
			return false;
		default:
			break;
		}
		char** map = rooms[currentRoom].getMap();		//gets reference of the actual map
		if (map[x + a][y + b] == 'T')
		{
			x = x + a;
			y = y + b;
			rooms[currentRoom].printRoomMap(x, y);
			temp.setCoordinates(x, y);
			temp.setRoomNo(currentRoom + 1);
			if (temp != stack.front())
				stack.push(temp);
			system("cls");
			cout << "\n\nUser found the Treasure. Game Ended\n\n\n";
			cout << "Treasure found at: ";
			temp.print();
			cout << "\n\nThe user's path is as follows: ";
			stack.print();
			system("pause");
			return true;
		}
		else if (map[x + a][y + b] == '0')
		{
			x = x + a;
			y = y + b;
			rooms[currentRoom].printRoomMap(x, y);
			temp.setCoordinates(x, y);
			temp.setRoomNo(currentRoom + 1);
			if (temp != stack.front())
				stack.push(temp);
			return printingCallR(x, y, stack, rooms, numOfRooms, currentRoom, gasperHelp, roomsVisited, visitedRoomNo);
		}
		else if (map[x + a][y + b] == 'D')
		{
			x = x + a;
			y = y + b;
			rooms[currentRoom].printRoomMap(x, y);
			temp.setCoordinates(x, y);
			temp.setRoomNo(currentRoom + 1);
			if (temp != stack.front())
				stack.push(temp);
			system("cls");
			cout << "\n\nUser eaten up by Devil. Game Ended\n\n\n";
			system("pause");
			return false;
		}
		else if (map[x + a][y + b] == 'X')
		{
			rooms[currentRoom].printRoomMap(x, y);
			return printingCallR(x, y, stack, rooms, numOfRooms, currentRoom, gasperHelp, roomsVisited, visitedRoomNo);
		}
		else if (map[x + a][y + b] == 'P')
		{
			x = x + a;
			y = y + b;
			rooms[currentRoom].printRoomMap(x, y);
			temp.setCoordinates(x, y);
			temp.setRoomNo(currentRoom + 1);
			if (temp != stack.front())
				stack.push(temp);
			system("cls");
			cout << "\n\nUser found the Portal\n\n\n";
			if (gasperHelp)								//using portal in room with treasure even after gasper's help is foolish
			{
				cout << "User made a mistake by using portal even after getting gasper's help\n\n";
				cout << "User punished and thrown out of the house\n\n";
				system("pause");
				return false;
			}
			else
			{
				cout << "Lets flip a coin\n";
				system("pause");
				int toss = (rand() % 2);
				if (toss)				//1 for head and 0 for tail
				{
					int temp = 0;
					cout << "Its a head\n";
					temp = (rand() % numOfRooms);
					while (temp == currentRoom)
					{
						temp = (rand() % numOfRooms);
					}
					currentRoom = temp;
					roomsVisited++;
					visitedRoomNo[roomsVisited] = currentRoom;
					cout << "User teleported to a random room\n";
					system("pause");
					return enterRoom(stack, rooms, numOfRooms, currentRoom, gasperHelp, roomsVisited, visitedRoomNo);
				}
				else
				{
					cout << "its a tail\n";
					if (roomsVisited == 1)
					{
						cout << "User kicked out of th house. Game Ended\n\n";
						system("pause");
						return false;
					}
					else
					{
						currentRoom = visitedRoomNo[roomsVisited - 1];
						roomsVisited++;
						visitedRoomNo[roomsVisited] = currentRoom;
						system("pause");
						return enterRoom(stack, rooms, numOfRooms, currentRoom, gasperHelp, roomsVisited, visitedRoomNo);
					}
				}
			}
			system("pause");
		}
		else if (map[x + a][y + b] == 'G')
		{
			x = x + a;
			y = y + b;
			rooms[currentRoom].printRoomMap(x, y);
			temp.setCoordinates(x, y);
			temp.setRoomNo(currentRoom + 1);
			if (temp != stack.front())
				stack.push(temp);
			system("cls");
			cout << "\n\nUser found the Gasper\n\n\n";
			for (int i = 0; i < numOfRooms; i++)
			{
				if (rooms[i].getIsTreasure())			//searches for the room with treasure
				{
					if (currentRoom != i)		//case if current room has the treasure
					{
						currentRoom = i;
						roomsVisited++;
						visitedRoomNo[roomsVisited] = currentRoom;
					}
					break;
				}
			}
			cout << "Gasper teleported user to the room with treasure\n";
			gasperHelp = true;
			system("pause");
			return enterRoom(stack, rooms, numOfRooms, currentRoom, gasperHelp, roomsVisited, visitedRoomNo);
		}
	}
}

//function to read rooms from files
Room* readData(int& num)			//updates the total number of rooms in main and returns pointer to rooms
{
	int numOfRooms = 0;
	fstream file;
	Room* rooms = 0;
	file.open("hauntedhouse.txt", ios::in);
	if (file.is_open())
	{
		while (!file.eof())
		{
			if (file.eof())
				break;

			file >> numOfRooms;
			num = numOfRooms;
			char temp[100];
			rooms = new Room[numOfRooms];
			for (int i = 0; i < numOfRooms; i++)
			{
				file >> temp;
				rooms[i].loadRoom(temp);		//every room loads its own data
			}
			for (int i = 0; i < numOfRooms; i++)
			{
				rooms[i].printRoom();
			}
		}
	}
	else
	{
		cout << "error opening file \n";
	}
	file.close();

	return rooms;
}

int main()
{
	srand(time(0));
	cout << "\nWELCOME TO THE TREASURE HUNT IN A HAUNTED HOUSE\n\n";
	system("pause");
	cout << "\nDo you want to continue with iterative implementation or recursive implementation?\n\n";		//selection of type of implementation
	cout << "Enter 1 for iterative approach\n\n";
	cout << "Enter 2 for recursive approach\n\n";
	int approach = 0;
	cin >> approach;
	while (approach != 1 && approach != 2)		//check to ensure only 1 or 2 is selected
	{
		cout << "Enter either 1 or 2\n\n";
		cin >> approach;
	}

	int numOfRooms = 0;
	int currentRoom = 0;
	int resp = 0;
	int roomsVisited = 0;	//keep a count of total visited rooms
	int visitedRoomNo[100];		//keeps a record of all the rooms visited
	bool gasperHelp = false;

	Room* rooms = readData(numOfRooms);
	cout << "Number of Rooms: " << numOfRooms << endl;

	Stack<Coordinates> memory;


	if (approach == 1)			//stack method case
	{
		currentRoom = (rand() % numOfRooms);
		cout << "Room " << currentRoom + 1 << endl;
		roomsVisited++;
		visitedRoomNo[roomsVisited] = currentRoom;
		system("pause");
		system("cls");

		bool running = true;

		while (running == true)
		{
			resp = rooms[currentRoom].printingCall(memory, (currentRoom + 1));
			if (resp == 1)	//treasure
			{
				cout << "Treasure found at: ";
				memory.front().print();
				cout << "\n\nThe user's path is as follows: ";
				memory.print();
				running = false;
				system("pause");
			}
			else if (resp == 2)	//portal
			{
				if (gasperHelp)
				{
					cout << "User made a mistake by using portal even after getting gasper's help\n\n";
					cout << "User punished and thrown out of the house\n\n";
					running = false;
				}
				else
				{
					cout << "Lets flip a coin\n";
					system("pause");
					int toss = (rand() % 2);
					if (toss)
					{
						int temp = 0;
						cout << "Its a head\n";
						temp = (rand() % numOfRooms);
						while (temp == currentRoom)
						{
							temp = (rand() % numOfRooms);
						}
						currentRoom = temp;
						roomsVisited++;
						visitedRoomNo[roomsVisited] = currentRoom;
						cout << "User teleported to a random room\n";
					}
					else
					{
						cout << "its a tail\n";
						if (roomsVisited == 1)
						{
							cout << "User kicked out of th house. Game Ended\n\n";
							running = false;
						}
						else
						{
							currentRoom = visitedRoomNo[roomsVisited - 1];
							roomsVisited++;
							visitedRoomNo[roomsVisited] = currentRoom;
						}
					}
				}
				system("pause");
			}
			else if (resp == 3)	//gasper
			{
				for (int i = 0; i < numOfRooms; i++)
				{
					if (rooms[i].getIsTreasure())
					{
						if (currentRoom != i)
						{
							currentRoom = i;
							roomsVisited++;
							visitedRoomNo[roomsVisited] = currentRoom;
						}
						break;
					}
				}
				cout << "Gasper teleported user to the room with treasure\n";
				gasperHelp = true;
				system("pause");
			}
			//all other cases fall in else 
			else
				running = false;
		}
	}
	//recursive case
	else
	{
		currentRoom = (rand() % numOfRooms);
		cout << "Room " << currentRoom + 1 << endl;
		system("pause");
		roomsVisited++;
		visitedRoomNo[roomsVisited] = currentRoom;
		enterRoom(memory, rooms, numOfRooms, currentRoom, gasperHelp, roomsVisited, visitedRoomNo);		//control given to recursive function
	}

	return 0;
}