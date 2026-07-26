#include <bits/stdc++.h>
#include <ncurses.h>

using namespace std;

int rows, cols; 

void getDimensions() {
    getmaxyx(stdscr, rows, cols);
}

void initColors() {
    start_color();
    use_default_colors();

    init_pair(1 ,70 ,-1); //Light Green
    init_pair(2 ,34 ,-1); //Bright Green
    init_pair(3 ,130 ,-1); //Light Brown
    init_pair(4 ,244 ,-1); //Gray
    init_pair(5 ,198 ,-1); //Bright HOt Pink
    init_pair(6 ,209 ,-1); //Light Orange
}

void drawChar(int r, int c, char ch, int color) {
    attron(COLOR_PAIR(color));
    mvaddch(r,c,ch);
    attroff(COLOR_PAIR(color));
}

pair<int ,int> drawPot() {
    int startCol = 5 * (cols/16);
    int endCol = 11 * (cols/16);
    int row = rows - 1;
    
    //Base
    string base = "(_)";
    int idx = 0;

    for (int i=startCol ;i<=startCol+2 ;i++) {
        drawChar(row, i, base[idx] ,4);
        refresh();
        napms(30);

        idx++;
    }

    idx = 0;
    for (int i=endCol-2 ;i<=endCol ;i++) {
        drawChar(row, i, base[idx] ,4);
        refresh();
        napms(30);

        idx++;
    }

    for (int i=startCol+1 ;i<endCol ;i++) {
        drawChar(row-1, i, '_' ,4);
        refresh();
        napms(30);
    }

    //Left Edges
    drawChar(row-1, startCol, '\\' ,4);
    drawChar(row-2, startCol-1, '\\' ,4);
    drawChar(row-3, startCol-2, ':' ,4);

    //Right Edges
    drawChar(row-1, endCol, '/' ,4);
    drawChar(row-2, endCol+1, '/' ,4);
    drawChar(row-3, endCol+2, ':' ,4);

    // Top
    int mid = (startCol + endCol) / 2; 

    string deco = "./~~~\\.";
    int decoStart = mid - deco.length() / 2;

    for (int i = startCol-1; i<=endCol+1; i++) {
        if (i>=decoStart && i<decoStart+(int)deco.length())
            drawChar(row-3, i, deco[i-decoStart] ,3);
        else drawChar(row-3 ,i ,'_' ,2);

        refresh();
        napms(30);
    }

    return {row-3 ,mid};
}

int main() {
    initscr(); 
    initColors();

    getDimensions();
    auto [startRow, startCol] = drawPot();

    mvaddch(startRow ,startCol ,'M');

    refresh();
    
    napms(1000);


    getch();

    endwin();           
    return 0;
}