#include <iostream>
#include <vector>
#include <conio.h>
#include <Windows.h>

using namespace std;

const int HEIGHT = 10;
const int WIDTH = 23;

bool GAMEOVER;

enum eDirection {
    STOP = 0,
    UP,
    DOWN,
    LEFT,
    RIGHT,
};
eDirection direction;

void gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, pos);
}

class Tail {
public:
    vector<pair<int, int>> segments;

    void AddSegment(int x, int y) {
        segments.push_back({ x, y });
    }

    void Move(int newHeadX, int newHeadY) {
        if (!segments.empty()) {
            for (size_t i = segments.size() - 1; i > 0; --i) {
                segments[i] = segments[i - 1];
            }
            segments[0] = { newHeadX, newHeadY };
        }
    }

    void Draw() const {
        for (const auto& segment : segments) {
            gotoxy(segment.first, segment.second);
            cout << 'o';
        }
    }

    bool CheckCollision(int x, int y) const {
        for (const auto& segment : segments) {
            if (segment.first == x && segment.second == y) {
                return true;
            }
        }
        return false;
    }
};

int snake_length = 1;
bool gameStarted = false;

void Start(int* fruitX, int* fruitY, int* snakeX, int* snakeY) {
    GAMEOVER = false;
    direction = STOP;

    *snakeX = WIDTH / 2;
    *snakeY = HEIGHT / 2;

    srand(time(0));
    *fruitX = (rand() % (WIDTH - 2)) + 1;
    *fruitY = (rand() % (HEIGHT - 2)) + 1;
}

void DrawBorders() {
    for (int i = 0; i < WIDTH; i++) {
        cout << "-";
    }
    cout << endl;

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0 || j == WIDTH - 1) {
                cout << "|";
            }
            else {
                cout << " ";
            }
        }
        cout << endl;
    }

    for (int i = 0; i < WIDTH; i++) {
        cout << "-";
    }
    cout << endl;
}

void DisplayStartMessage() {
    gotoxy(WIDTH / 3, HEIGHT / 2);
    cout << "Start game\n| by pressing W A S D";
}

void Gameboard(int* fruitX, int* fruitY, int* snakeX, int* snakeY, const Tail& tail) {
    if (gameStarted) {

        gotoxy(*snakeX, *snakeY);
        cout << 'O';

        gotoxy(*fruitX, *fruitY);
        cout << '*';

        tail.Draw();
    }

    gotoxy(0, HEIGHT + 1);


    gotoxy(0, HEIGHT + 2);
    cout << "Score: " << snake_length - 1 << endl;
}

void ClearPosition(int x, int y) {
    gotoxy(x, y);
    cout << ' ';
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'W':
        case 'w':
            if (direction != DOWN)
                direction = UP;
            break;
        case 'A':
        case 'a':
            if (direction != RIGHT)
                direction = LEFT;
            break;
        case 'S':
        case 's':
            if (direction != UP)
                direction = DOWN;
            break;
        case 'D':
        case 'd':
            if (direction != LEFT)
                direction = RIGHT;
            break;
        default:
            break;
        }
        if (!gameStarted) {
            gameStarted = true;
            gotoxy(WIDTH / 3, HEIGHT / 2);
            cout << "              \n|                     ";
        }
    }
}

void Movement(int* snakeX, int* snakeY, Tail& tail) {
    int prevX = *snakeX;
    int prevY = *snakeY;

    switch (direction) {
    case UP:
        (*snakeY)--;
        break;
    case DOWN:
        (*snakeY)++;
        break;
    case LEFT:
        (*snakeX)--;
        break;
    case RIGHT:
        (*snakeX)++;
        break;
    default:
        break;
    }

    tail.Move(prevX, prevY);
}

void Obstacle(int* snakeX, int* snakeY, int* fruitX, int* fruitY, Tail& tail) {
    if (*snakeX <= 0 || *snakeX >= WIDTH - 1 || *snakeY <= 0 || *snakeY >= HEIGHT) {
        GAMEOVER = true;
    }

    if (*snakeX == *fruitX && *snakeY == *fruitY) {
        snake_length += 1;
        *fruitX = (rand() % (WIDTH - 2)) + 1;
        *fruitY = (rand() % (HEIGHT - 2)) + 1;
        tail.AddSegment(*fruitX, *fruitY);
    }

    if (tail.CheckCollision(*snakeX, *snakeY)) {
        GAMEOVER = true;
    }
}

int main() {
    int fruitX, fruitY, snakeX, snakeY;
    Tail tail;
    Start(&fruitX, &fruitY, &snakeX, &snakeY);
    DrawBorders();

    DisplayStartMessage();
    gotoxy(0, HEIGHT + 2);
    cout << "Score: 0";

    while (!GAMEOVER) {
        if (gameStarted) {
            ClearPosition(snakeX, snakeY);
            for (const auto& segment : tail.segments) {
                ClearPosition(segment.first, segment.second);
            }
        }
        Input();
        if (gameStarted) {
            Movement(&snakeX, &snakeY, tail);
            Obstacle(&snakeX, &snakeY, &fruitX, &fruitY, tail);
        }
        Gameboard(&fruitX, &fruitY, &snakeX, &snakeY, tail);
        Sleep(100);
    }
    system("CLS");
    gotoxy(WIDTH / 3, HEIGHT / 2);
    cout << "GAME OVER!\n Your score is: " << snake_length - 1;
    gotoxy(WIDTH, HEIGHT + 1);

    return 0;
}
