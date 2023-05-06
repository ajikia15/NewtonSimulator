// MORE (serious) OOP
#include <assert.h>
#include <ncurses.h>
#include <unistd.h>

#include <iostream>
#include <vector>
#include <list>
#include <set>

using namespace std;

#define MAIN_COLOR 1
#define DANG_COLOR 2
#define STAT_COLOR 3
#define BUCK_COLOR 4

#define GAMEOVER_TIME 2 // s
#define TIME_FRAME 5    // ms
#define MIN_DELAY(w) (50 / (w))
#define MAX_DELAY(w) (50 * (w))
#define CRT_DELAY 600 // every ... frames new one MBOX

#define FIELD(w) ((w)*5)

#define PEN_MIN -100
#define PEN_MAX 100

#define BUCKET_WIDTH 2
#define BUCKET_MAX 10

#define PEN(wi, hi) (((wi) + 1) * ((hi) + 1))
#define RA(min, max) ((min) + rand() % ((max) - (min) + 1)) // random value from set {min,min+1,...,max}

// static box
class BOX
{
protected: // protected beacuse of providing an access to the derived class (!)
    int xpos, ypos;
    int width, height;
    int color, bgcolor;
    WINDOW *win;

public:
    BOX() {}
    BOX(WINDOW *wn, int x, int y, int w, int h, int c, int b) { Init(wn, x, y, w, h, c, b); }
    void Init(WINDOW *wn, int x, int y, int w, int h, int c, int b)
    {
        win = wn;
        xpos = x;
        ypos = y;
        width = w; // width and height are actual width/height - 1
        height = h;
        color = c;
        bgcolor = b;
    }
    void Show()
    {
        wattron(win, COLOR_PAIR(color));
        mvwaddch(win, ypos, xpos, ACS_ULCORNER);
        for (int i = 1; i < height; i++)
            mvwaddch(win, ypos + i, xpos, ACS_VLINE);

        mvwaddch(win, ypos + height, xpos, ACS_LLCORNER);
        for (int i = 1; i < width; i++)
            mvwaddch(win, ypos, xpos + i, ACS_HLINE);

        mvwaddch(win, ypos, xpos + width, ACS_URCORNER);
        for (int i = 1; i < height; i++)
            mvwaddch(win, ypos + i, xpos + width, ACS_VLINE);

        mvwaddch(win, ypos + height, xpos + width, ACS_LRCORNER);
        for (int i = 1; i < width; i++)
            mvwaddch(win, ypos + height, xpos + i, ACS_HLINE);

        for (int i = 1; i <= width - 1; i++)
            for (int j = 1; j <= height - 1; j++)
                mvwprintw(win, ypos + j, xpos + i, "%c", ' ');
        //		wrefresh(win);
    }
    void Hide()
    {
        wattron(win, COLOR_PAIR(bgcolor));
        for (int i = 0; i <= width; i++)
            for (int j = 0; j <= height; j++)
                mvwprintw(win, ypos + j, xpos + i, "%c", ' ');
        //		wrefresh(win);
    }
    int getX() { return xpos; }
    int getY() { return ypos; }
    int getH() { return height; }
    int getW() { return width; }
};

// moving box
class MBOX : public BOX
{
private:
    int delay; // clock * TIME_FRAME
public:
    MBOX(WINDOW *wn, int x, int y, int w, int h, int c, int b, int f) { Init(wn, x, y, w, h, c, b, f); }
    void Init(WINDOW *wn, int x, int y, int w, int h, int c, int b, int f)
    {
        ((BOX *)(this))->Init(wn, x, y, w, h, c, b);
        delay = f;
    }

    void Move() { ypos += 1; }
    int getDelay() { return delay; }
    int Check()
    {
        if (ypos + height == getmaxy(win) - 1)
            return 1; // OK
        return 0;
    }
};

class BUCKET : public BOX
{
public:
    BUCKET() {}
    void Init(WINDOW *wn, int x, int y, int w, int h, int c, int b)
    {
        ((BOX *)(this))->Init(wn, x, y, w, h, c, b);
    }
    // 0: do not touch (nothing), 1 - collect (good), -1: collision (wrong)
    int CollisionDetect(BOX &box)
    {
        if (ypos == box.getY() + box.getH() + 1) // touching!
        {
            if (xpos <= box.getX() && (xpos + width >= box.getX() + box.getW()))
                return 1; // catch it!
            else if (((xpos >= box.getX()) && (xpos <= box.getX() + box.getW())) || ((xpos + width >= box.getX()) && (xpos + width <= box.getX() + box.getW())))
                return -1; // collision
        }
        return 0;
    }
    int getWidth() { return width; }
    int Move(char ch)
    {
        if (ch == ',') // LEFT
        {
            Hide(); // OK
            xpos -= 1;
            if (xpos < 0)
                xpos = 0;
            return 1;
        }
        else if (ch == '.') // RIGHT
        {
            Hide(); // OK
            xpos += 1;
            if (xpos > getmaxx(win) - width - 1)
                xpos = getmaxx(win) - width - 1;
            return 1;
        }
        return 0;
    }
    // 0 - nothing; 1 - change, 2 - won, 3 - lost
    int UpdateWidth(int pen)
    {
        if (pen < PEN_MIN)
        {
            width++;
            if (width == BUCKET_MAX)
                return 2;
            else
                return 1;
        } // won
        else if (pen > PEN_MAX)
        {
            width--;
            if (width == 0)
                return 3;
            else
                return 1;
        } // lost
        return 0;
    }
};

// dropping objects
class BOXES
{
private:
    vector<MBOX> V; // convenient to clean-up (removing a few elements form the list)
    WINDOW *win;
    int penalty;

public:
    BOXES() {}
    void Init(WINDOW *w)
    {
        penalty = 0;
        win = w;
    }
    void Create(int bw) // bw = bucket width
    {
        int wi = RA(1, bw);
        int hi = RA(1, FIELD(bw) / wi);
        int de = RA(MIN_DELAY(bw), MAX_DELAY(bw));
        int pos = RA(0, getmaxx(win) - 1 - wi);
        V.push_back(MBOX(win, pos, 0, wi, hi, DANG_COLOR, MAIN_COLOR, de));
    }
    void Move(int frame)
    {
        for (int i = 0; i < V.size(); i++)
            if ((frame % V[i].getDelay()) == 0)
            {
                if (V[i].Check()) // legal next move?
                {
                    V[i].Hide();
                    penalty += PEN(V[i].getW(), V[i].getH());
                    V.erase(V.begin() + i);
                }
                else
                {
                    V[i].Hide();
                    V[i].Move();
                    V[i].Show();
                }
            }
    }
    void CollisionDetect(BUCKET &bucket, int frame)
    {
        for (int i = 0; i < V.size(); i++)
            if ((frame % V[i].getDelay()) == 0) // very important!
            {
                int result = bucket.CollisionDetect(V[i]);
                if (result != 0)
                {
                    UpdatePenalty(result, i);
                    V[i].Hide();
                    V.erase(V.begin() + i);
                }
            }
    }
    void UpdatePenalty(int r, int i)
    {
        if (r > 0)
            penalty -= PEN(V[i].getW(), V[i].getH());
        if (r < 0)
            penalty += PEN(V[i].getW(), V[i].getH());
    }
    void clearPenalty() { penalty = 0; }
    int getPenalty() { return penalty; }
    int getSize() { return V.size(); }
};

class GAME
{
private:
    int frame;
    WINDOW *mainwin;
    BOXES boxes;
    BUCKET bucket;

public:
    GAME() // start the game = initialization
    {
        srand(time(NULL));
        frame = 0;

        mainwin = initscr();
        curs_set(0); // do not show the cursor
        noecho();    // do not print anything after key pressed

        start_color();
        init_pair(MAIN_COLOR, COLOR_WHITE, COLOR_BLACK);
        init_pair(DANG_COLOR, COLOR_BLACK, COLOR_RED);
        init_pair(STAT_COLOR, COLOR_WHITE, COLOR_BLUE);
        init_pair(BUCK_COLOR, COLOR_BLUE, COLOR_YELLOW);
        wattron(mainwin, COLOR_PAIR(MAIN_COLOR));
        nodelay(mainwin, TRUE); // non-blocking mode for wgetch() in playwin only (!)

        boxes.Init(mainwin);

        bucket.Init(mainwin, COLS / 2 - 2, LINES - 2, BUCKET_WIDTH, 1, BUCK_COLOR, MAIN_COLOR);
        bucket.Show();
    }
    void showTimer() { mvwprintw(mainwin, 0, 0, "Time: %.2lf", (frame * TIME_FRAME) / 1000.0); }
    void showPenalty(int penalty) { mvwprintw(mainwin, 0, COLS - 12, "Penalty: %3d", penalty); }
    void showNum(int num) { mvwprintw(mainwin, 0, COLS / 2 - 7, "Boxes: %3d", num); }
    void showGameOver(string res)
    {
        mvwprintw(mainwin, 0, COLS / 2 - 7, "Game over: %s", res.c_str());
        wrefresh(mainwin);
    }
    void ShowData()
    {
        wattron(mainwin, COLOR_PAIR(STAT_COLOR));
        showTimer();
        showPenalty(boxes.getPenalty());
        showNum(boxes.getSize());
    }
    void NextFrame()
    {
        frame++;
        wrefresh(mainwin);
        Sleep();
    }
    void Sleep() { usleep(TIME_FRAME * 1000); } // recalculate in microseconds
    void mainLoop()
    {
        string result = "Quit!";
        for (char key; key = wgetch(mainwin), key != 'q';)
        {
            // add new one (if delay passed)
            if (frame % CRT_DELAY == 0)
                boxes.Create(bucket.getW()); // add new one

            // detect collisions: bucket and boxes
            boxes.CollisionDetect(bucket, frame);

            int res = bucket.UpdateWidth(boxes.getPenalty());
            if (res == 1)
            {
                boxes.clearPenalty();
                bucket.Show();
            }
            else if (res == 2)
            {
                result = "You won!";
                break;
            }
            else if (res == 3)
            {
                result = "You lost..";
                break;
            }

            // move
            boxes.Move(frame); // move and check collisions with a  ground
            if (bucket.Move(key))
                bucket.Show();

            // update info (top screen)
            ShowData();

            NextFrame();
        }
        showGameOver(result);
        usleep(GAMEOVER_TIME * 1000000);
    }
    ~GAME() // run when destroying the object
    {
        delwin(mainwin); // remove from MEMORY - WINDOW data structure
        endwin();        // clean up
    }
};

int main()
{
    GAME Game; // start the game
    Game.mainLoop();
    return EXIT_SUCCESS;
}
