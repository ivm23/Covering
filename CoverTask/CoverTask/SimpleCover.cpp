#include <iostream>
#include <fstream>
#include <vector>
#include <Windows.h>

using namespace std;

const int WHITE = 15;
const int RED = 4;
const int BACKGROUND = 0 << 4;
const int NUMBER_OF_COLOR = 14;

ifstream in("map.txt");
ofstream out("map.txt");

int width = 0, height = 0;
int countOfAgents = 0;
long long countOfIteration = 0;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

class Agent {

private:
	int color;
	int x, y;
public:
	Agent(int c, int coordX, int coordY) : color(c), x(coordX), y(coordY) {}

	~Agent() {};

	int getColor() { return color;  }
	pair<int, int> getCoordinates() { return make_pair(x,y); }

	void printAgent() {
		SetConsoleTextAttribute(hConsole, (WORD)(BACKGROUND | color % NUMBER_OF_COLOR + 1));
		cout << color << ' ' << '(' << x << ", " << y << ')' << endl;
		SetConsoleTextAttribute(hConsole, (WORD)(BACKGROUND | WHITE));
	}

	pair<int, int> const getNewCoordinates(vector<vector<int>> & map) {
		if (x + 1 < height && map[x + 1][y] == 0) {
			x = x + 1; 
			return make_pair(x, y);
		}

		if (x - 1 >= 0 && map[x - 1][y] == 0) {
			x = x - 1;
			return make_pair(x, y);
		}
		if (y + 1 < width && map[x][y + 1] == 0) {
			y = y + 1;
			return make_pair(x, y);
		}
		if (y - 1 >= 0 && map[x][y - 1] == 0) {
			y = y - 1;
			return make_pair(x, y);
		}

		// если нет свободных
		if (x + 1 < height) {
			x = x + 1;
			return make_pair(x, y);
		}

		if (x - 1 >= 0) {
			x = x - 1;
			return make_pair(x, y);
		}
		if (y + 1 < width) {
			y = y + 1;
			return make_pair(x, y);
		}
		if (y - 1 >= 0) {
			y = y - 1;
			return make_pair(x, y);
		}
	}
};

void makeAgents(vector<Agent> & agents) {
	for (int i = 0; i < countOfAgents; ++i) {
		agents.push_back(Agent(i + 1, 0, 0));
	}
}

void const printAgents(vector<Agent> & agents) {
	for (auto agent = agents.begin(); agent != agents.end(); ++agent) {
		(*agent).printAgent();
	}
}

void makeMap(vector<vector<int>> & map) {
	map.resize(height);

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j)
			map[i].push_back(0);
	}
}

void const printMap(vector<vector<int>> & map) {
	for (auto h = map.begin(); h != map.end(); ++h) {
		for (auto w = (*h).begin(); w != (*h).end(); ++w) {
			if (*w != 0) {
				SetConsoleTextAttribute(hConsole, (WORD)(BACKGROUND | (*w) % NUMBER_OF_COLOR + 1));
			}
		//	printf("%1.0f ", w);
			cout << *w;
			SetConsoleTextAttribute(hConsole, (WORD)(BACKGROUND | WHITE));
			
		}
		cout << endl;
	}
}
void getInputData() {
	cout << "width = ";
	SetConsoleTextAttribute(hConsole, (WORD)(BACKGROUND | RED));
	cin >> width;
	SetConsoleTextAttribute(hConsole, (WORD)(BACKGROUND | WHITE));
	cout << "height = ";
	SetConsoleTextAttribute(hConsole, (WORD)(BACKGROUND | RED));
	cin >> height;
	SetConsoleTextAttribute(hConsole, (WORD)(BACKGROUND | WHITE));
	cout << "count of agents = ";
	SetConsoleTextAttribute(hConsole, (WORD)(BACKGROUND | RED));
	cin >> countOfAgents;
	SetConsoleTextAttribute(hConsole, (WORD)(BACKGROUND | WHITE));
}



int main() {

	getInputData();

	long long countOfCells = width * height;

	vector<Agent> agents;
	vector<vector<int>> map;
	
	makeAgents(agents);
	printAgents(agents);

	makeMap(map);
	printMap(map);

	map[0][0] = (*agents.begin()).getColor();

	countOfIteration = 1;
	--countOfCells;
	
	// пока не все покрыли
	while (countOfCells != 0) {
		for (auto agent = agents.begin(); agent != agents.end(); ++agent) {
			auto newCoordinates = (*agent).getNewCoordinates(map);
			++countOfIteration;
			if (map[newCoordinates.first][newCoordinates.second] == 0)
				map[newCoordinates.first][newCoordinates.second] = (*agent).getColor(), --countOfCells;
			printMap(map);
			Sleep(100);
		}
	}

	cout << countOfIteration << endl;
	printMap(map);
	system("Pause");
	return 0;
}