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
const int DOWN = -1;
const int UP = 1;
const int VERTICAL_MOVE = 0;

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

	~Agent() {
		color = -1; 
	};

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
		if (x < planX) ++x; else
			if (x > planX) --x; else
				if (y < planY) ++y; else
					if (y > planY) --y;
					
		if (planX == x && planY == y) {
			planX = 0;
			planY = 0;
	}
		return make_pair(x, y);
	}

	pair<int, int> makeStep(int & moveHorisont) {
	
		if (moveHorisont == 0) {
			y = y + 1;
			return make_pair(x, y);
		}
		if (moveHorisont == UP) {
			if (x + UP < height) {
				x = x + UP;
				return make_pair(x, y);
			}
			moveHorisont = DOWN;
			y = y + 1;
			if (y == width) {
				x = 0;
				y = 0;
				return make_pair(0, 0);
			}
			return make_pair(x, y);
		}
		if (moveHorisont == DOWN) {
			if (x + DOWN >= 0) {
				x = x + DOWN;
				return make_pair(x, y);
			}
			moveHorisont = UP;
			y = y + 1;
			if (y == width) {
				x = 0;
				y = 0;
				return make_pair(0, 0);
			}
			return make_pair(x, y);
		} 
		x = 0;
		y = 0;
		return make_pair(0, 0);
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
			
			printf("%1.0d", *w);
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

void clearMapAfterDelete(vector<vector<int>> & map) {
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j)
			if (planMap[i][j] != 0 && map[i][j] != planMap[i][j]) planMap[i][j] = 0;
	}

}

int main() {

	getInputData();

	long long countOfCells = width * height;
	 
	vector<Agent> agents;
	vector<vector<int>> map;

	makeAgents(agents);
	printAgents(agents);

	makeMap(map);
//	printMap(map);

	map[0][0] = (*agents.begin()).getColor();
	planMap[0][0] = (*agents.begin()).getColor();

	countOfIteration = 1;
	--countOfCells;

	// пока не все покрыли
	int flagDeleteAgent = 3;
	while (countOfCells != 0) {
		++countOfIteration;
		for (auto agent = agents.begin(); agent != agents.end(); ++agent) {
		/*	if (countOfCells < 20 && flagDeleteAgent > 0) {
				(*agent).~Agent(); flagDeleteAgent--;
				clearMapAfterDelete(map);
			}*/
			if ((*agent).getColor() != -1) {
				

				(*agent).getNewCoordinates(planMap);

				auto newCoordinates = (*agent).makeStep();
				if (map[newCoordinates.first][newCoordinates.second] == 0) {
					map[newCoordinates.first][newCoordinates.second] = (*agent).getColor();
					planMap[newCoordinates.first][newCoordinates.second] = (*agent).getColor();
					--countOfCells;
				}
		//		printMap(map);
			}
			//	Sleep(100);
		}
	}
	
	cout << "Number of iterations: " << countOfIteration << endl;
	cout << "Map after covering: " << endl;
	printMap(map);

	////////////////////////////SECOND IDEA/////////////////////////////////

	cout << "Second idea\n";

	countOfCells = width * height;
	agents.clear();

	makeAgents(agents);
	printAgents(agents);
	
	map.clear();

	makeMap(map);
//	printMap(map);

	map[0][0] = (*agents.begin()).getColor();
	planMap[0][0] = (*agents.begin()).getColor();
	
	--countOfCells;
	countOfIteration = 1;

	int step = width / countOfAgents;
	int err = width % countOfAgents;
	vector<int> moveHorison;
	moveHorison.resize(countOfAgents + 1);

	moveHorison[(*agents.begin()).getColor()] = UP;

	while (countOfCells != 0) {
		++countOfIteration;
		for (auto agent = agents.begin(); agent != agents.end(); ++agent) {
			if (err != 0) {
				if (moveHorison[(*agent).getColor()] == VERTICAL_MOVE && (*agent).getCoordinates().second == ((*agent).getColor() - 1) * (step + 1)) {
					--err;
					moveHorison[(*agent).getColor()] = UP;
				}
			}
			else
					if (moveHorison[(*agent).getColor()] == VERTICAL_MOVE && (*agent).getCoordinates().second == ((*agent).getColor() -1 ) * step) {
						moveHorison[(*agent).getColor()] = UP;
					}

				auto newCoordinates = (*agent).makeStep(moveHorison[(*agent).getColor()]);

				if (map[newCoordinates.first][newCoordinates.second] == 0) {
					map[newCoordinates.first][newCoordinates.second] = (*agent).getColor();
					//planMap[newCoordinates.first][newCoordinates.second] = (*agent).getColor();
					--countOfCells;
				}
			
		}
		//printMap(map);
	}

	cout << "Number of iterations: " << countOfIteration << endl;
	cout << "Final map: \n";
	printMap(map);

	system("Pause");
	return 0;
}