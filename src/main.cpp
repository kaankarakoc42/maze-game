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

enum  Directions{ 
    Right, 
    Left, 
    Up, 
    Bottom, 
    None, 
}; 
 
Directions reverse(Directions dir){ 
          if(dir==Right) return Left; 
          if(dir==Left) return Right; 
          if(dir==Up) return Bottom; 
          if(dir==Bottom) return Up; 
 
          return None; 
} 
 
class Decision{ 
      public: 
      int x,y; 
      vector<Directions> available; 
      int usedCount; 
      int id ; 
      Decision(int x,int y,vector<Directions> available): x(x),y(y),available(available){ 
             usedCount = 0; 
 
             id = DecisionPointCount++; 
      } 
 
      void removeDir(Directions dirToRemove){ 
            std::vector<Directions> updatedAvailable; 
 
            // Loop through the original vector and copy the elements that are not the one we want to remove 
            for (const auto& dir : available) { 
                if (dir != dirToRemove) { 
                    updatedAvailable.push_back(dir); 
                } 
            } 
 
            available = updatedAvailable; 
 
      } 
 
};

class Player{
      public:
      int x;
      int y;
      int startx;
      int starty;
      int mapx;
      int mapy;
      bool running;
      bool missionSuccess;
      Directions current_direction;
      Directions past_dir;
      Directions crushed;

      stack<Decision*> decisionpoints;
      vector<Decision*> decisionpointsvec;
      vector<Decision*> solutionpointsvec;
      bool stopFlow;
      Maze& maze;
      Player(int x,int y,Maze& maze) : x(x),y(y),startx(x),starty(y),mapx(x),mapy(y),maze(maze){
        running = true;
        crushed = None;
        missionSuccess = false;
        stopFlow = false;
        past_dir = None;
      }

      void move(Directions direction){
            if(direction==Right)
            {
                if(maze.path.isInPath(x+1,y))
                {
                        x++;
                        mapx+=2;
                }
                else{
                    crushed = Right;
                }
            }
            if(direction==Left)
            {
                if(maze.path.isInPath(x-1,y))
                {
                        x--;
                        mapx-=2;
                }
                else{
                    crushed = Left;
                }
            }
            if(direction==Up)
            {
                if(maze.path.isInPath(x,y-1))
                {
                        y--;
                        mapy-=1;
                }
                else{
                    crushed = Up;
                }
            }
            if(direction==Bottom)
            {
                if(maze.path.isInPath(x,y+1))
                {
                        mapy+=1;
                        y++;
                }
                else{
                    crushed = Bottom;
                }
            }
            if(maze.detectEndPoint(x,y) && !(x==startx&&y==starty)){
               missionSuccess = true;   
            }
      }

      void setPosition(int x,int y){
           this->x = x;
           this->y = y;
           this->mapx = 2*x;
           this->mapy = y;
      }


      void handleKeyboard()
      {
            if (GetAsyncKeyState('W') & 0x8000) {  // Check if 'W' is pressed
                this->move(Up);
            }
            else if (GetAsyncKeyState('A') & 0x8000) {  // Check if 'A' is pressed
                this->move(Left);
            }
            else if (GetAsyncKeyState('S') & 0x8000) {  // Check if 'S' is pressed
                this->move(Bottom);
            }
            else if (GetAsyncKeyState('D') & 0x8000) {  // Check if 'D' is pressed
                this->move(Right);
            }
            else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {  // Check if ESC is pressed
                running = false;  // Exit the game loop
            }
      };   
     
      void handleKeyboardFlow()
      {
            if (GetAsyncKeyState('W') & 0x8000) {  // Check if 'W' is pressed
                current_direction = Up;
            }
            else if (GetAsyncKeyState('A') & 0x8000) {  // Check if 'A' is pressed
                current_direction = Left;
            }
            else if (GetAsyncKeyState('S') & 0x8000) {  // Check if 'S' is pressed
                current_direction = Bottom;
            }
            else if (GetAsyncKeyState('D') & 0x8000) {  // Check if 'D' is pressed
                current_direction = Right;
            }
            else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {  // Check if ESC is pressed
                running = false;  // Exit the game loop
            }
            
            this->move(current_direction);
      };
       
      bool isInDp()
      {
            for(Decision* p : decisionpointsvec)
            {
                  if(p->x == x && p->y ==y)
                  {
                    return true;
                  }
            }
            return false;
      }
      void autoSolve()
      {
        if(!missionSuccess){
                vector<Directions> dirs;
                if(maze.path.isInPath(x+1,y) && past_dir!= Left ){dirs.push_back(Right);}
                if(maze.path.isInPath(x-1,y) && past_dir!= Right){dirs.push_back(Left);}
                if(maze.path.isInPath(x,y-1) && past_dir!= Bottom){dirs.push_back(Up);}
                if(maze.path.isInPath(x,y+1) && past_dir!= Up){dirs.push_back(Bottom);}

                if(dirs.size()>1 && !isInDp())
                {                           

                   Decision* p = new Decision(x,y,dirs);
                   decisionpoints.push(p);
                   decisionpointsvec.push_back(p);
                   p->removeDir(dirs[0]);

                }

                if(dirs.size()>0){
                    past_dir = dirs[0];
                    this->move(dirs[0]);
                }

                else{
                    Decision* p = decisionpoints.top();
                    if(p->available.size()==0)
                    {
                        decisionpoints.pop();
                        p = decisionpoints.top();
                    }
                    setPosition(p->x,p->y);
                    past_dir = p->available[0];
                    this->move(p->available[0]);
                    p->removeDir(p->available[0]);
                    p->usedCount ++;
                    cout<<"["<<p->id<<"]"<<"teloporting to "<<p->x << " "<<p->y;
                }
        }
        else{
            while(!solutionpointsvec.empty())
            {
                solutionpointsvec.push_back(decisionpoints.top());
                decisionpoints.pop();
            }
            
        }

      };

      void renderPlayer(){
           SetConsoleTextAttribute(hConsole,FOREGROUND_GREEN);
           setCursorPosition(mapx,mapy);
           cout<<"\033[42m  ";
           SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
      }
};