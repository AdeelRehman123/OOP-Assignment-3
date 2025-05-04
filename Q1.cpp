#include <iostream>
#include <fstream>
#include <string>
using namespace std;

template <class T1, class T2>
class Pair
{
public:
	T1 first;
	T2 second;
	Pair(T1 f, T2 s) : first(f), second(s) {}
	void display()
	{
		cout << "Pair: (" << first << ", " << second << ")" << endl;
	}
};

class User
{
protected:
	int userID;
	string name;
	bool paymentStatus;

public:
	User(int id = 0, string n = "") : userID(id), name(n), paymentStatus(false) {}
	virtual void login() = 0;
	virtual void makePayment() = 0;
	bool checkPayment() const { return paymentStatus; }
	int getUserID() const { return userID; }
	string getName() const { return name; }
};

class Student : public User
{
public:
	Student(int id = 0, string n = "") : User(id, n) {}
	void login() override
	{
		cout << "Student " << name << " logged in using password." << endl;
	}
	void makePayment() override
	{
		if (paymentStatus)
		{
			cout << name << " has already made payment.\n";
		}
		else
		{
			cout << name << " has made semester payment.\n";
			paymentStatus = true;
		}
	}
};

class Teacher : public User
{
public:
	Teacher(int id = 0, string n = "") : User(id, n) {}
	void login() override
	{
		cout << "Teacher " << name << " logged in using OTP." << endl;
	}
	void makePayment() override
	{
		if (paymentStatus)
		{
			cout << name << " has already made payment.\n";
		}
		else
		{
			cout << name << " has made monthly payment.\n";
			paymentStatus = true;
		}
	}
};

class Staff : public User
{
public:
	Staff(int id = 0, string n = "") : User(id, n) {}
	void login() override
	{
		cout << "Staff " << name << " logged in using staff code." << endl;
	}
	void makePayment() override
	{
		if (paymentStatus)
		{
			cout << name << " has already made payment.\n";
		}
		else
		{
			cout << name << " has made monthly payment.\n";
			paymentStatus = true;
		}
	}
};
class Admin : public User
{
public:
	Admin(int id = 0, string n = "") : User(id, n) {}
	void login() override
	{
		cout << "Admin " << name << " logged in using security token." << endl;
	}
	void makePayment() override
	{
		cout << "Admin doesn't need to make payments.\n";
	}
};

class Route
{
private:
	int routeID;
	string startLocation, endLocation;

public:
	Route(int id = 0, string start = "", string end = "") : routeID(id), startLocation(start), endLocation(end) {}
	void displayRoute() const
	{
		cout << "Route: " << startLocation << " to " << endLocation << endl;
	}
	int getRouteID() const { return routeID; }
	string getStartLocation() const { return startLocation; }
	string getEndLocation() const { return endLocation; }
};

class Bus
{
private:
	int busID;
	Route route;
	int seatsAvailable;
	User *usersOnBoard[30];
	int userCount;
	int routeID;

public:
	Bus(int id = 0, Route r = Route()) : busID(id), route(r), seatsAvailable(30), userCount(0) {}
	void addUser(User *user)
	{
		if (userCount < 30)
		{
			usersOnBoard[userCount++] = user;
			seatsAvailable--;
			cout << user->getName() << " boarded Bus " << busID << endl;
		}
		else
		{
			cout << "No seats available!" << endl;
		}
	}
	void displayBusDetails() const
	{
		cout << "Bus ID: " << busID << ", Seats: " << seatsAvailable << endl;
		route.displayRoute();
		cout << "Passengers: ";
		for (int i = 0; i < userCount; i++)
			cout << usersOnBoard[i]->getName() << " ";
		cout << endl;
	}
	int getBusID() const { return busID; }
	int getRouteID() const { return routeID; }
};

class TransportationSystem
{
private:
	User *users[100];
	Bus buses[20];
	Route routes[20];
	int userCount, busCount, routeCount;

public:
	void loadUsersFromFile()
	{
		ifstream file("users.bin", ios::binary);
		if (!file)
		{
			cout << "No previous user data found. Starting fresh.\n";
			return;
		}

		file.read((char *)&userCount, sizeof(userCount));
		for (int i = 0; i < userCount; i++)
		{
			int id;
			string name;
			bool paid;
			int type;

			file.read((char *)&id, sizeof(id));

			size_t len;
			file.read((char *)&len, sizeof(len));
			name.resize(len);
			file.read(&name[0], len);

			file.read((char *)&paid, sizeof(paid));
			file.read((char *)&type, sizeof(type));

			if (type == 1)
				users[i] = new Student(id, name);
			else if (type == 2)
				users[i] = new Teacher(id, name);
			else if (type == 3)
				users[i] = new Staff(id, name);
			else if (type == 4)
				users[i] = new Admin(id, name);

			if (paid)
				users[i]->makePayment();
		}

		file.close();
	}
	void loadRoutesFromFile()
	{
		ifstream file("routes.bin", ios::binary);
		if (!file)
			return;
		file.read((char *)&routeCount, sizeof(routeCount));
		for (int i = 0; i < routeCount; i++)
		{
			int id;
			string start, end;
			size_t len;
			file.read((char *)&id, sizeof(id));
			file.read((char *)&len, sizeof(len));
			start.resize(len);
			file.read(&start[0], len);
			file.read((char *)&len, sizeof(len));
			end.resize(len);
			file.read(&end[0], len);
			routes[i] = Route(id, start, end);
		}
		file.close();
	}

	TransportationSystem() : userCount(0), busCount(0), routeCount(0)
	{
		loadUsersFromFile();
		loadRoutesFromFile();
		loadBusesFromFile();
	}
	void saveBusesToFile()
	{
		ofstream file("buses.bin", ios::binary);
		if (!file)
		{
			cout << "File opening error!" << endl;
			return;
		}
		file.write((char *)&busCount, sizeof(busCount));
		for (int i = 0; i < busCount; i++)
		{
			int id = buses[i].getBusID();
			int routeID = buses[i].getRouteID();
			file.write((char *)&id, sizeof(id));
			file.write((char *)&routeID, sizeof(routeID));
		}
		file.close();
	}

	void loadBusesFromFile()
	{
		ifstream file("buses.bin", ios::binary);
		if (!file)
		{
			cout << "No previous bus data found.\n";
			return;
		}
		file.read((char *)&busCount, sizeof(busCount));
		for (int i = 0; i < busCount; i++)
		{
			int id, routeID;
			file.read((char *)&id, sizeof(id));
			file.read((char *)&routeID, sizeof(routeID));

			bool routeFound = false;
			for (int j = 0; j < routeCount; j++)
			{
				if (routes[j].getRouteID() == routeID)
				{
					buses[i] = Bus(id, routes[j]);
					routeFound = true;
					break;
				}
			}
			if (!routeFound)
			{
				cout << "Route ID " << routeID << " for Bus ID " << id << " not found. Skipping this bus.\n";
				busCount--;
				i--;
			}
		}
		file.close();
	}

	void saveRoutesToFile()
	{
		ofstream file("routes.bin", ios::binary);
		if (!file)
		{
			cout << "File opening error!" << endl;
			return;
		}
		file.write((char *)&routeCount, sizeof(routeCount));
		for (int i = 0; i < routeCount; i++)
		{
			int id = routes[i].getRouteID();
			string start = routes[i].getStartLocation();
			string end = routes[i].getEndLocation();
			size_t len = start.size();
			file.write((char *)&id, sizeof(id));
			file.write((char *)&len, sizeof(len));
			file.write(start.c_str(), len);
			len = end.size();
			file.write((char *)&len, sizeof(len));
			file.write(end.c_str(), len);
		}
		file.close();
	}
	void registerUser()
	{
		int id, choice;
		string name;
		cout << "Register as: 1. Student 2. Teacher 3. Staff 4. Admin\nChoice: ";
		cin >> choice;
		cout << "Enter ID: ";
		cin >> id;
		cout << "Enter Name: ";
		cin.ignore();
		getline(cin, name);

		try
		{
			for (int i = 0; i < userCount; i++)
			{
				if (users[i]->getUserID() == id)
					throw "Duplicate ID detected!";
			}
			switch (choice)
			{
			case 1:
				users[userCount++] = new Student(id, name);
				break;
			case 2:
				users[userCount++] = new Teacher(id, name);
				break;
			case 3:
				users[userCount++] = new Staff(id, name);
				break;
			case 4:
				users[userCount++] = new Admin(id, name);
				break;
			default:
				throw "Invalid Choice!";
			}
			cout << name << " registered successfully.\n";
			saveUsersToFile();
		}
		catch (const char *msg)
		{
			cout << "Error: " << msg << endl;
		}
	}

	void makePayment()
	{
		int id;
		cout << "Enter User ID: ";
		cin >> id;
		for (int i = 0; i < userCount; i++)
		{
			if (users[i]->getUserID() == id)
			{
				users[i]->makePayment();
				saveUsersToFile();
				return;
			}
		}
		cout << "User not found!" << endl;
	}

	void addRoute()
	{
		int id;
		string start, end;
		cout << "Enter Route ID: ";
		cin >> id;
		cout << "Start Location: ";
		cin.ignore();
		getline(cin, start);
		cout << "End Location: ";
		getline(cin, end);
		routes[routeCount++] = Route(id, start, end);
		cout << "Route added." << endl;
		saveRoutesToFile();
	}

	void addBus()
	{
		int busID, routeID;
		cout << "Bus ID: ";
		cin >> busID;
		cout << "Route ID: ";
		cin >> routeID;
		for (int i = 0; i < routeCount; i++)
		{
			if (routes[i].getRouteID() == routeID)
			{
				buses[busCount++] = Bus(busID, routes[i]);
				cout << "Bus added." << endl;
				saveBusesToFile();
				return;
			}
		}
		cout << "Route not found!" << endl;
	}

	void assignUserToBus()
	{
		int userID, busID;
		cout << "User ID: ";
		cin >> userID;
		cout << "Bus ID: ";
		cin >> busID;
		for (int i = 0; i < userCount; i++)
		{
			if (users[i]->getUserID() == userID)
			{
				if (!users[i]->checkPayment())
				{
					cout << "Payment pending!" << endl;
					return;
				}
				for (int j = 0; j < busCount; j++)
				{
					if (buses[j].getBusID() == busID)
					{
						buses[j].addUser(users[i]);
						return;
					}
				}
				cout << "Bus not found!" << endl;
				return;
			}
		}
		cout << "User not found!" << endl;
	}

	void displayAllBuses()
	{
		for (int i = 0; i < busCount; i++)
			buses[i].displayBusDetails();
	}

	void saveUsersToFile()
	{
		ofstream file("users.bin", ios::binary);
		if (!file)
		{
			cout << "File opening error!" << endl;
			return;
		}
		file.write((char *)&userCount, sizeof(userCount));
		for (int i = 0; i < userCount; i++)
		{
			int id = users[i]->getUserID();
			string name = users[i]->getName();
			bool paid = users[i]->checkPayment();
			int type;
			if (dynamic_cast<Student *>(users[i]))
				type = 1;
			else if (dynamic_cast<Teacher *>(users[i]))
				type = 2;
			else if (dynamic_cast<Staff *>(users[i]))
				type = 3;
			else if (dynamic_cast<Admin *>(users[i]))
				type = 4;

			file.write((char *)&id, sizeof(id));
			size_t len = name.size();
			file.write((char *)&len, sizeof(len));
			file.write(name.c_str(), len);
			file.write((char *)&paid, sizeof(paid));
			file.write((char *)&type, sizeof(type));
		}
		file.close();
	}
};

int main()
{
	TransportationSystem ts;
	//loginUser();      Login user logic
	int choice;
	while (true)
	{
		cout << "\n1. Register User\n2. Make Payment\n3. Add Route\n4. Add Bus\n5. Assign User to Bus\n6. Display All Buses\n7. Exit\nChoice: ";
		cin >> choice;
		switch (choice)
		{
		case 1:
			ts.registerUser();
			break;
		case 2:
			ts.makePayment();
			break;
		case 3:
			ts.addRoute();
			break;
		case 4:
			ts.addBus();
			break;
		case 5:
			ts.assignUserToBus();
			break;
		case 6:
			ts.displayAllBuses();
			break;
		case 7:
			return 0;
		default:
			cout << "Invalid choice!" << endl;
		}
	}
}
