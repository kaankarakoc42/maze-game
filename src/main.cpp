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

class Path{ 
    public: 
    vector<Point*> points; 
 
    ~Path(){ 
        for(Point* point: points) 
        { 
            delete point; 
        } 
        points.clear(); 
    } 
 
    bool isInPath(int x,int y){ 
          for(auto&& point: points){ 
              if(point->x==x && point->y==y) 
              { 
                return true; 
              } 
          } 
          return false; 
    } 
 
    void addPoint(int x,int y) 
    { 
        this->points.push_back(new Point(x,y)); 
    } 
};
 

class Maze{
      public:
        vector<vector<char>> map;  // Vector to store the map (2D grid of chars)
        Path path;    
        int MazeHeight=-1;
        int MazeWidth=-1;
        vector<vector<Point>> detectedHorizontalLines;
        vector<vector<Point>> detectedVerticalLines;
        vector<Point*> InterceptionPoints; 
        Maze()
        {

            ifstream inputFile("harita.txt");

            if (!inputFile) {
                cerr << "Error opening file!" << endl;
            }

            string line;  

            while (getline(inputFile, line)) {
                MazeHeight++;
                MazeWidth = line.length();
                vector<char> row;  
                for (size_t i = 0; i < line.size(); i++) {
                    row.push_back(line[i]);  
                    if(line[i]==' '){
                        path.addPoint(i,map.size());
                    }
                }
                map.push_back(row); 
            }


            inputFile.close();
            //detectLines();
            /*
            cout << "\nStored Horizontal Lines as List of Points:" << endl;
            for (const auto& line : detectedHorizontalLines) {
                for (const auto& pt : line) {
                    cout << "(" << pt.x << ", " << pt.y << ") ";
                }
                cout << endl;
            }

            cout << "\nStored Vertical Lines as List of Points:" << endl;
            for (const auto& line : detectedVerticalLines) {
                for (const auto& pt : line) {
                    cout << "(" << pt.x << ", " << pt.y << ") ";
                }
                cout << endl;
            }*/
        }   

        ~Maze(){
            for(auto&& point :  InterceptionPoints)
            {
                delete point;
            }
        }
       

        void renderMaze()
        {

    string mazeBuffer = "";

    for (const auto& row : map) {
        for (char c : row) {
            if (c == '#') {
                mazeBuffer += "\033[47m  \033[0m"; 
                
            } else {
                mazeBuffer += "  ";
            }
        }
        mazeBuffer += "\n"; 
    }


    setCursorPosition(0,0);
    cout << mazeBuffer;

        }

        void showPoints(){
            for(Point* p: path.points){
                SetConsoleTextAttribute(hConsole,BACKGROUND_INTENSITY| FOREGROUND_GREEN);
                setCursorPosition(p->x,p->y);
                SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
                cout<<"*";
            }            
        }


        bool detectEndPoint(int x,int y){
              if(MazeHeight==y || y == 0 || MazeWidth == x || x==0)
              {
                 return true;
              }
              return false;
        };


};
