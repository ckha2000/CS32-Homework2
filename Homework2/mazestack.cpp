//
//  mazestack.cpp
//  Homework2
//
//  Created by christopher kha on 2/3/19.
//  Copyright © 2019 christopher kha. All rights reserved.
//

#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>

#include <stack>
using namespace std;

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};


// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec){
    stack<Coord> s;
    Coord start(sr, sc);
    s.push(start);
    
    maze[sr][sc] = '^';                                     // '^' --> discovered spot
    
    //int i = 0;
    while(!s.empty()){
        Coord curr = s.top();
        s.pop();
        if(curr.r() == er && curr.c() == ec)
            return true;
        
        if((curr.r()+1) < nRows && maze[curr.r()+1][curr.c()] == '.'){            // south
            maze[curr.r()+1][curr.c()] = '^';
            Coord temp(curr.r()+1, curr.c());
            s.push(temp);
            
        }
        if((curr.c()-1) >= 0 && maze[curr.r()][curr.c()-1] == '.'){      // west
            maze[curr.r()][curr.c()-1] = '^';
            Coord temp(curr.r(), curr.c()-1);
            s.push(temp);
                
        }
        if((curr.r()-1) >= 0 && maze[curr.r()-1][curr.c()] == '.'){      // north
            maze[curr.r()-1][curr.c()] = '^';
            Coord temp(curr.r()-1, curr.c());
            s.push(temp);
            
        }
        if((curr.c()+1) < nCols && maze[curr.r()][curr.c()+1] == '.'){      // east
            maze[curr.r()][curr.c()+1] = '^';
            Coord temp(curr.r(), curr.c()+1);
            s.push(temp);
        }
        // cerr << i << "\t\t\t ( " << curr.r() << ", " << curr.c() << " )" << endl;
        //i++;
    }
    return false;
}


int main()
{
    string maze[10] = {
        "XXXXXXXXXX",
        "X....X...X",
        "X.XX.XX..X",
        "XXX....X.X",
        "X.XXX.XXXX",
        "X.X...X..X",
        "X...X.X..X",
        "XXXXX.X.XX",
        "X........X",
        "XXXXXXXXXX"
    };

    if (pathExists(maze, 10,10, 3,5, 8,8))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}


