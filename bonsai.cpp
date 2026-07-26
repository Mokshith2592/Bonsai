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
        napms(10);

        idx++;
    }

    idx = 0;
    for (int i=endCol-2 ;i<=endCol ;i++) {
        drawChar(row, i, base[idx] ,4);
        refresh();
        napms(10);

        idx++;
    }

    for (int i=startCol+1 ;i<endCol ;i++) {
        drawChar(row-1, i, '_' ,4);
        refresh();
        napms(10);
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
        napms(10);
    }

    return {row-3 ,mid};
}

char branchChar(double angle) {
    if (angle >= 82 && angle <= 98) return '|';
    return angle < 90 ? '/' : '\\';
}

void drawLeaves(int row, int col, int radius) {
    static const string leafChars = "*o+";
    int horizontalRadius = radius * 3;
    int verticalRadius = max(1, radius - 1);

    // Make each leaf cluster wider than it is tall.
    for (int i = 0; i < radius * 12 + 12; ++i) {
        int leafRow = row + (rand() % (2 * verticalRadius + 1) - verticalRadius);
        int leafCol = col + (rand() % (2 * horizontalRadius + 1) - horizontalRadius);

        if (leafRow > 0 && leafRow < rows - 3 && leafCol > 0 && leafCol < cols - 1) {
            int colorRoll = rand() % 10;
            // Green is common; pink and orange are occasional blossoms.
            int leafColor = colorRoll < 4 ? 1
                          : colorRoll < 8 ? 2
                          : colorRoll == 8 ? 5 : 6;
            drawChar(leafRow, leafCol, leafChars[rand() % leafChars.size()], leafColor);
        }
    }
    refresh();
}

void growTree(int row, int col, double angle, int life, int depth) {
    int branchesMade = 0;
    int branchPoints = depth == 0 ? 4 : (depth == 1 ? 1 : 0);

    for (int step = 0; step < life; ++step) {
        if (row <= 1 || col <= 2 || col >= cols - 3) {
            drawLeaves(row, col, 3);
            return;
        }
        if (step % 3 == 0) {
            angle += rand() % 17 - 8;
            angle += (90.0 - angle) * (depth == 0 ? 0.08 : 0.12);
        }
        angle = clamp(angle, 40.0, 140.0);

        char ch = branchChar(angle);
        drawChar(row, col, ch, 3);

        //Add a second character
        if (depth == 0 && step % 3 == 0 && step < life - 4) {
            int thickCol = col + (angle <= 90 ? 1 : -1);
            drawChar(row, thickCol, ch, 3);
        }

        refresh();
        napms(35);

        // Start side branches at regular points
        int nextBranchStep = (branchesMade + 1) * life / (branchPoints + 1);
        if (branchesMade < branchPoints && step >= nextBranchStep) {
            ++branchesMade;
            int branchLife = max(5, life / 2 + rand() % 3 - depth);
            double side = branchesMade % 2 == 0 ? 30.0 : -30.0;
            growTree(row - 1, col, angle + side, branchLife, depth + 1);
        }

        // Move upward and shift sideways based on the current angle.
        --row;
        if (angle < 82) {
            int stride = (depth > 0 && angle < 55) ? 2 : 1;
            if (stride == 2) drawChar(row + 1, col + 1, '_', 3);
            col += stride;
        }
        else if (angle > 98) {
            int stride = (depth > 0 && angle > 125) ? 2 : 1;
            if (stride == 2) drawChar(row + 1, col - 1, '_', 3);
            col -= stride;
        }
    }

    // Finish each branch with a cluster of leaves.
    drawLeaves(row, col, depth == 0 ? 3 : (depth == 1 ? 3 : 2));
}

int main() {
    initscr(); 
    initColors();

    getDimensions();
    auto [startRow, startCol] = drawPot();

    srand(static_cast<unsigned>(time(nullptr)));
    growTree(startRow - 1, startCol, 90, min(20, rows - 6), 0);

    refresh();
    
    napms(1000);


    getch();

    endwin();           
    return 0;
}
