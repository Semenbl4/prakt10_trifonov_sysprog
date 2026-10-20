#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <cstring>

using namespace std;

const int Width = 30;
const int Height = 20;

int way;

struct Node {
    int x, y;
    Node* next;
};

struct Record {
    char name[50];
    int score;
};

struct LinkedList {
    Node* head = nullptr;
    Node* tail = nullptr;
};

LinkedList snake;
int appleX, appleY;
int score;
int snakeLength;
bool gameOver;

void freeSnake(LinkedList& list) {
    while (list.head != nullptr) {
        Node* temp = list.head;
        list.head = list.head->next;
        delete temp;
    }
    list.tail = nullptr;
}

void Setup() {
    gameOver = false;
    score = 0;
    snakeLength = 1;
    way = 0;

    freeSnake(snake);

    Node* newnode = new Node;
    newnode->x = Width / 2;
    newnode->y = Height / 2;
    newnode->next = nullptr;

    snake.head = newnode;
    snake.tail = newnode;

    appleX = rand() % Width;
    appleY = rand() % Height;
}

void Draw() {
    system("cls");

    for (int i = 0; i < Width + 2; i++) cout << "#";
    cout << endl;

    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Width; j++) {
            if (j == 0) cout << "#";

            if (snake.head->x == j && snake.head->y == i) {
                cout << "O";
            }
            else if (appleX == j && appleY == i) {
                cout << "F";
            }
            else {
                bool istail = false;
                Node* curr = snake.head->next;
                while (curr != nullptr) {
                    if (curr->x == j && curr->y == i) {
                        cout << "o";
                        istail = true;
                        break;
                    }
                    curr = curr->next;
                }
                if (!istail) cout << " ";
            }

            if (j == Width - 1) cout << "#";
        }
        cout << endl;
    }

    for (int i = 0; i < Width + 2; i++) cout << "#";
    cout << endl;

    cout << "Score: " << score << " | Нажмите 'P' для Сохранения и Выхода" << endl;
}

void SaveGame() {
    ofstream savefile("save.bin", ios::binary);
    if (!savefile) return;

    savefile.write((char*)&score, sizeof(score));
    savefile.write((char*)&way, sizeof(way));
    savefile.write((char*)&appleX, sizeof(appleX));
    savefile.write((char*)&appleY, sizeof(appleY));
    savefile.write((char*)&snakeLength, sizeof(snakeLength));

    Node* curr = snake.head;
    while (curr != nullptr) {
        savefile.write((char*)&curr->x, sizeof(curr->x));
        savefile.write((char*)&curr->y, sizeof(curr->y));
        curr = curr->next;
    }
    savefile.close();
    cout << "\nИгра успешно сохранена!" << endl;
    Sleep(1000);
}

bool LoadGame() {
    ifstream in("save.bin", ios::binary);
    if (!in) {
        cout << "Не найдено сохраненных игр" << endl;
        Sleep(1000);
        return false;
    }

    freeSnake(snake);

    in.read((char*)&score, sizeof(score));
    in.read((char*)&way, sizeof(way));
    in.read((char*)&appleX, sizeof(appleX));
    in.read((char*)&appleY, sizeof(appleY));
    in.read((char*)&snakeLength, sizeof(snakeLength));

    for (int i = 0; i < snakeLength; i++) {
        Node* newnode = new Node;
        in.read((char*)&newnode->x, sizeof(newnode->x));
        in.read((char*)&newnode->y, sizeof(newnode->y));
        newnode->next = nullptr;

        if (snake.head == nullptr) {
            snake.head = newnode;
            snake.tail = newnode;
        }
        else {
            snake.tail->next = newnode;
            snake.tail = newnode;
        }
    }

    in.close();
    gameOver = false;
    return true;
}

void Controls() {
    if (_kbhit()) {
        int ch = _getch();

        if (ch == 0 || ch == 224) {
            ch = _getch(); 
            switch (ch) {
            case 75: if (way != 2) way = 1; break;
            case 77: if (way != 1) way = 2; break;
            case 72: if (way != 4) way = 3; break;
            case 80: if (way != 3) way = 4; break;
            }
        }
        else {
            switch (ch) {
            case 'a': if (way != 2) way = 1; break; 
            case 'd': if (way != 1) way = 2; break; 
            case 'w': if (way != 4) way = 3; break; 
            case 's': if (way != 3) way = 4; break; 
            case 'p': case 'P':                     
                SaveGame();
                gameOver = true;
                break;
            }
        }
    }
}

void Logic() {
    if (way == 0) return;

    int prevX = snake.head->x;
    int prevY = snake.head->y;
    int tempX, tempY;

    Node* curr = snake.head->next;
    while (curr != nullptr) {
        tempX = curr->x;
        tempY = curr->y;
        curr->x = prevX;
        curr->y = prevY;
        prevX = tempX;
        prevY = tempY;
        curr = curr->next;
    }

    switch (way) {
    case 1: snake.head->x--; break;
    case 2: snake.head->x++; break;
    case 3: snake.head->y--; break;
    case 4: snake.head->y++; break;
    }

    if (snake.head->x >= Width || snake.head->x < 0 || snake.head->y >= Height || snake.head->y < 0) {
        gameOver = true;
    }

    curr = snake.head->next;
    while (curr != nullptr) {
        if (curr->x == snake.head->x && curr->y == snake.head->y) {
            gameOver = true;
        }
        curr = curr->next;
    }

    if (snake.head->x == appleX && snake.head->y == appleY) {
        score += 10;
        snakeLength++;

        Node* newnode = new Node;
        newnode->x = prevX;
        newnode->y = prevY;
        newnode->next = nullptr;

        snake.tail->next = newnode;
        snake.tail = newnode;

        appleX = rand() % Width;
        appleY = rand() % Height;
    }
}

void AddRecord(int finalscore) {
    system("cls");
    cout << "Игра окончена" << endl;
    cout << "Твои очки: " << finalscore << endl;
    cout << "Введите имя: ";

    Record newrec;
    cin >> newrec.name;
    newrec.score = finalscore;

    Record records[5];
    int count = 0;

    ifstream in("records.bin", ios::binary);
    if (in) {
        while (count < 5 && in.read((char*)&records[count], sizeof(Record))) {
            count++;
        }
        in.close();
    }

    if (count < 5) {
        records[count++] = newrec;
    }
    else if (finalscore > records[4].score) {
        records[4] = newrec;
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (records[j].score < records[j + 1].score) {
                swap(records[j], records[j + 1]);
            }
        }
    }

    ofstream out("records.bin", ios::binary);
    for (int i = 0; i < count; i++) {
        out.write((char*)&records[i], sizeof(Record));
    }
    out.close();
}

void ShowRecords() {
    system("cls");
    cout << "Таблица рекордов" << endl;
    ifstream in("records.bin", ios::binary);
    if (!in) {
        cout << "Рекордов еще нет!" << endl;
    }
    else {
        Record rec;
        int place = 1;
        while (in.read((char*)&rec, sizeof(Record))) {
            cout << place++ << ". " << rec.name << " - " << rec.score << endl;
        }
        in.close();
    }
    cout << "Нажмите любую клавишу для выхода в меню...";
    int dummy = _getch();
}

void GameStart() {
    while (!gameOver) {
        Draw();
        Controls();
        Logic();
        Sleep(100);
    }
    if (way != 0 && (snake.head->x >= Width || snake.head->x < 0 || snake.head->y >= Height || snake.head->y < 0)) {
        AddRecord(score);
    }
    else {
        Node* curr = snake.head->next;
        bool bitTail = false;
        while (curr != nullptr) {
            if (curr->x == snake.head->x && curr->y == snake.head->y) {
                bitTail = true; 
                break;
            }
            curr = curr->next;
        }
        if (bitTail) AddRecord(score);
    }
    freeSnake(snake);
}

int main() {
    setlocale(LC_ALL, "Russian");
    while (true) {
        system("cls");
        cout << "Змейка" << endl;
        cout << "1. Новая игра" << endl;
        cout << "2. Загрузить игру" << endl;
        cout << "3. Таблица рекордов" << endl;
        cout << "4. Выход" << endl;
        cout << "Выберите пункт меню: ";

        char choice = _getch();
        if (choice == '1') {
            Setup();
            GameStart();
        }
        else if (choice == '2') {
            if (LoadGame()) {
                GameStart();
            }
        }
        else if (choice == '3') {
            ShowRecords();
        }
        else if (choice == '4') {
            break;
        }
    }
    return 0;
}