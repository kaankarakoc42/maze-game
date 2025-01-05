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

class Point{
     public:
     int x;
     int y;
     Point(int x,int y) : x(x),y(y){}

    // Optionally, override equality for comparison (useful for sets or maps)
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    // Optional: to use Point in unordered_map or unordered_set
    struct HashFunction {
        size_t operator()(const Point& p) const {
            return hash<int>()(p.x) ^ (hash<int>()(p.y) << 1);
        }
    };
};
