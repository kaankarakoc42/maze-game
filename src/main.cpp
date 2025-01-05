#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <windows.h>

#include <set>
#include <unordered_map>
#include <map>
#include <utility>
#include <stack>

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

int DecisionPointCount = 0;

void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;  
    coord.Y = y;  

    SetConsoleCursorPosition(hConsole, coord);
}

