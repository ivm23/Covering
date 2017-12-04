#include <iostream>
#include <fstream>
#include <vector>
#include <Windows.h>

using namespace std;

const int WHITE = 15;
const int RED = 4;
const int BACKGROUND = 0 << 4;
const int NUMBER_OF_COLOR = 14;
const int NONEPLAN = 0;

ifstream in("map.txt");
ofstream out("map.txt");

int width = 0, height = 0;
int countOfAgents = 0;
long long countOfIteration = 0;
vector<vector<int>> planMap;


HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

class Agent {

private:
	int color;
	int x, y;
	int planX = NONEPLAN, planY = NONEPLAN;
public:
	Agent(int c, int coordX, int coordY) : color(c), x(coordX), y(coordY) {}

	~Agent() {};

	int getColor() { return color; }
	pair<int, int> getCoordinates() { return make_pair(x, y); }

	void printAgent() {
		SetConsoleTextAttribute(hConsole, (WORD)(BACKGROUND | color % NUMBER_OF_COLOR + 1));
		cout << color << ' ' << '(' << x << ", " << y << ')' << endl;
		SetConsoleTextAttribute(hConsole, (WORD)(BACKGROUND | WHITE));
	}

	void getNewCoordinates(vector<vector<int>> & planMap) {
		if (planX == 0 && planY == 0) {
			int step = 1;
			bool cellSelected = false;
			while (!cellSelected && (x + step < height || x - step >= 0 || y + step < width || y - step >= 0)) {
				for (int ix = 0; ix <= step; ++ix) {
					for (int iy = step; iy >= 0; --iy) {
						if (!cellSelected && x + ix < height && y + iy < width && planMap[x + ix][y + iy] == 0) {
							planMap[x + ix][y + iy] = color;
							planX = x + ix;
							planY = y + iy;
							cellSelected = true;
						}
						if (!cellSelected && x - ix >= 0 && y + iy < width && planMap[x - ix][y + iy] == 0) {
							planMap[x - ix][y + iy] = color;
							planX = x - ix;
							planY = y + iy;
							cellSelected = true;
						}
						if (!cellSelected && x + ix < height && y - iy >= 0 && planMap[x + ix][y - iy] == 0) {
							planMap[x + ix][y - iy] = color;
							planX = x + ix;
							planY = y - iy;
							cellSelected = true;
						}
						if (!cellSelected && x - ix >= 0 && y - iy >= 0 && planMap[x - ix][y - iy] == 0) {
							planMap[x - ix][y - iy] = color;
							planX = x - ix;
							planY = y - iy;
							cellSelected = true;
						}
					}
				}
				++step;
			}
		}

	}

	pair<int, int> makeStep() {
		if (x == planX || y == planY) {
			y = planY;
			x = planX;
			planX = planY = 0;
		}
		else
			x = planX;

		return make_pair(x, y);
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
	planMap.resize(height);

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			map[i].push_back(0);
			planMap[i].push_back(0);
		}
	}
}

void const printMap(vector<vector<int>> & map) {
	for (auto h = map.begin(); h != map.end(); ++h) {
		for (auto w = (*h).begin(); w != (*h).end(); ++w) {
			if (*w != 0) {
				SetConsoleTextAttribute(hConsole, (WORD)(BACKGROUND | (*w) % NUMBER_OF_COLOR + 1));
			}
			
				printf("%2.0d", *w);
			//cout << *w;
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
			++countOfIteration;

			(*agent).getNewCoordinates(planMap);

			auto newCoordinates = (*agent).makeStep();
			if (map[newCoordinates.first][newCoordinates.second] == 0)
				map[newCoordinates.first][newCoordinates.second] = (*agent).getColor(), --countOfCells;
			printMap(map);
			//	Sleep(100);
		}
	}

	cout << "Number of iterations: " << countOfIteration << endl;
	cout << "Map after covering: " << endl;
	printMap(map);
	system("Pause");
	return 0;
}