#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
using namespace std;


//The reson why we should use a 1D array is it easy to delete
struct GamePlay {
    int size; //size of board
    int* board; //convert 2D board into 1D board size nxn
    int score;
    string playerName;
    stack<vector<int>> undoStack; // Lưu trữ trạng thái cho undo
    stack<vector<int>> redoStack; // Lưu trữ trạng thái cho redo
    stack<int> scoreUndoStack;
    stack<int> scoreRedoStack;
};

void initBoard(GamePlay& game) {
    srand(time(0));
    game.board = new int[game.size * game.size]();
    int x1 = rand() % game.size, y1 = rand() % game.size;
    int x2 = rand() % game.size, y2 = rand() % game.size;
    while (x1 == x2 && y1 == y2) {
        x2 = rand() % game.size;
        y2 = rand() % game.size;
    }
    game.board[x1 * game.size + y1] = (rand() % 2 + 1) * 2;
    game.board[x2 * game.size + y2] = (rand() % 2 + 1) * 2;
}

void printBoard(const GamePlay& game) {
    cout << "Player: " << game.playerName << endl;
    cout << "Score: " << game.score << endl;
    for (int i = 0; i < game.size; ++i) {
        for (int j = 0; j < game.size; ++j) {
            cout << setw(4) << game.board[i * game.size + j] << " ";
        }
        cout << endl;
    }
}

void saveState(GamePlay& game) {
    vector<int> currentState(game.board, game.board + game.size * game.size);
    game.undoStack.push(currentState);
    game.scoreUndoStack.push(game.score);
}

void restoreState(GamePlay& game, bool isUndo) {
    if (isUndo) {
        if (!game.undoStack.empty()) {
            vector<int> previousState = game.undoStack.top();
            copy(previousState.begin(), previousState.end(), game.board);
            game.undoStack.pop();
            game.score = game.scoreUndoStack.top();
            game.scoreUndoStack.pop();
        }
    }
    else {
        if (!game.redoStack.empty()) {
            vector<int> previousState = game.redoStack.top();
            copy(previousState.begin(), previousState.end(), game.board);
            game.redoStack.pop();
            game.score = game.scoreRedoStack.top();
            game.scoreRedoStack.pop();
        }
    }
}

void addRandom(GamePlay& game) {
    vector<int> emptyCells;
    for (int i = 0; i < game.size * game.size; ++i) {
        if (game.board[i] == 0) {
            emptyCells.push_back(i);
        }
    }
    if (!emptyCells.empty()) {
        int idx = rand() % emptyCells.size();
        game.board[emptyCells[idx]] = (rand() % 2 + 1) * 2;
    }
}

bool moveAndMerge(GamePlay& game, int direction) {
    bool moved = false;
    switch (direction) {
    case 'w':
        for (int j = 0; j < game.size; ++j) {
            for (int i = 1; i < game.size; ++i) {
                if (game.board[i * game.size + j] != 0) {
                    int k = i;
                    while (k > 0 && game.board[(k - 1) * game.size + j] == 0) {
                        game.board[(k - 1) * game.size + j] = game.board[k * game.size + j];
                        game.board[k * game.size + j] = 0;
                        k--;
                        moved = true;
                    }
                    if (k > 0 && game.board[(k - 1) * game.size + j] == game.board[k * game.size + j]) {
                        game.board[(k - 1) * game.size + j] *= 2;
                        game.score += game.board[(k - 1) * game.size + j];
                        game.board[k * game.size + j] = 0;
                        moved = true;
                    }
                }
            }
        }
        break;
    case 's':
        for (int j = 0; j < game.size; ++j) {
            for (int i = game.size - 2; i >= 0; --i) {
                if (game.board[i * game.size + j] != 0) {
                    int k = i;
                    while (k < game.size - 1 && game.board[(k + 1) * game.size + j] == 0) {
                        game.board[(k + 1) * game.size + j] = game.board[k * game.size + j];
                        game.board[k * game.size + j] = 0;
                        k++;
                        moved = true;
                    }
                    if (k < game.size - 1 && game.board[(k + 1) * game.size + j] == game.board[k * game.size + j]) {
                        game.board[(k + 1) * game.size + j] *= 2;
                        game.score += game.board[(k + 1) * game.size + j];
                        game.board[k * game.size + j] = 0;
                        moved = true;
                    }
                }
            }
        }
        break;
    case 'a':
        for (int i = 0; i < game.size; ++i) {
            for (int j = 1; j < game.size; ++j) {
                if (game.board[i * game.size + j] != 0) {
                    int k = j;
                    while (k > 0 && game.board[i * game.size + (k - 1)] == 0) {
                        game.board[i * game.size + (k - 1)] = game.board[i * game.size + k];
                        game.board[i * game.size + k] = 0;
                        k--;
                        moved = true;
                    }
                    if (k > 0 && game.board[i * game.size + (k - 1)] == game.board[i * game.size + k]) {
                        game.board[i * game.size + (k - 1)] *= 2;
                        game.score += game.board[i * game.size + (k - 1)];
                        game.board[i * game.size + k] = 0;
                        moved = true;
                    }
                }
            }
        }
        break;
    case 'd':
        for (int i = 0; i < game.size; ++i) {
            for (int j = game.size - 2; j >= 0; --j) {
                if (game.board[i * game.size + j] != 0) {
                    int k = j;
                    while (k < game.size - 1 && game.board[i * game.size + (k + 1)] == 0) {
                        game.board[i * game.size + (k + 1)] = game.board[i * game.size + k];
                        game.board[i * game.size + k] = 0;
                        k++;
                        moved = true;
                    }
                    if (k < game.size - 1 && game.board[i * game.size + (k + 1)] == game.board[i * game.size + k]) {
                        game.board[i * game.size + (k + 1)] *= 2;
                        game.score += game.board[i * game.size + (k + 1)];
                        game.board[i * game.size + k] = 0;
                        moved = true;
                    }
                }
            }
        }
        break;
    }
    return moved;
}

bool isGameOver(const GamePlay& game) {
    for (int i = 0; i < game.size; ++i) {
        for (int j = 0; j < game.size; ++j) {
            if (game.board[i * game.size + j] == 0) return false;
            if (i > 0 && game.board[i * game.size + j] == game.board[(i - 1) * game.size + j]) return false;
            if (i < game.size - 1 && game.board[i * game.size + j] == game.board[(i + 1) * game.size + j]) return false;
            if (j > 0 && game.board[i * game.size + j] == game.board[i * game.size + (j - 1)]) return false;
            if (j < game.size - 1 && game.board[i * game.size + j] == game.board[i * game.size + (j + 1)]) return false;
        }
    }
    return true;
}

void saveGame(const GamePlay& game, const string& filename) {
    ofstream file(filename, ios::binary);
    if (file.is_open()) {
        file.write((char*)&game.size, sizeof(game.size));
        file.write((char*)&game.score, sizeof(game.score));
        file.write((char*)game.board, game.size * game.size * sizeof(int));
        file.close();
        cout << "Game saved!" << endl;
    }
    else {
        cout << "Unable to save the game!" << endl;
    }
}

void loadGame(GamePlay& game, const string& filename) {
    ifstream file(filename, ios::binary);
    if (file.is_open()) {
        file.read((char*)&game.size, sizeof(game.size));
        file.read((char*)&game.score, sizeof(game.score));
        delete[] game.board;
        game.board = new int[game.size * game.size];
        file.read((char*)game.board, game.size * game.size * sizeof(int));
        file.close();
        cout << "Game loaded!" << endl;
    }
    else {
        cout << "Unable to load the game!" << endl;
    }
}

void startMenu(GamePlay& game) {
    ifstream file("playerdata.dat", ios::binary);
    if (file.is_open()) {
        getline(file, game.playerName, '\0');
        file.read((char*)&game.score, sizeof(game.score));
        file.close();
    }
    else {
        cout << "No player data found. Starting a new game." << endl;
        cout << "Enter your name: ";
        cin >> game.playerName;
    }

    cout << "Player: " << game.playerName << endl;
    cout << "Score: " << game.score << endl;

    char choice;
    cout << "1. New Game" << endl;
    cout << "2. Load Game" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == '1') {
        cout << "Enter board size (e.g., 4 for 4x4): ";
        cin >> game.size;
        initBoard(game);
    }
    else if (choice == '2') {
        loadGame(game, "2048save.dat");
    }
    else {
        cout << "Invalid choice!" << endl;
        exit(0);
    }
}

int main() {
    GamePlay game;
    startMenu(game);

    while (true) {
        printBoard(game);
        if (isGameOver(game)) {
            cout << "Game Over!" << endl;
            break;
        }
        char choice;
        cout << "Enter move (w/a/s/d to move, e to exit, u to undo, r to redo): ";
        cin >> choice;

        if (choice == 'e') {
            saveGame(game, "2048save.dat");
            ofstream file("playerdata.dat", ios::binary);
            if (file.is_open()) {
                file.write(game.playerName.c_str(), game.playerName.size() + 1);
                file.write((char*)&game.score, sizeof(game.score));
                file.close();
            }
            else {
                cout << "Unable to save player data!" << endl;
            }
            break;
        }

        if (choice == 'u') {
            if (!game.undoStack.empty()) {
                vector<int> currentState(game.board, game.board + game.size * game.size);
                game.redoStack.push(currentState);
                game.scoreRedoStack.push(game.score);
                restoreState(game, true);
            }
            else {
                cout << "No moves to undo!" << endl;
            }
            continue;
        }

        if (choice == 'r') {
            if (!game.redoStack.empty()) {
                vector<int> currentState(game.board, game.board + game.size * game.size);
                game.undoStack.push(currentState);
                game.scoreUndoStack.push(game.score);
                restoreState(game, false);
            }
            else {
                cout << "No moves to redo!" << endl;
            }
            continue;
        }

        saveState(game);
        while (!game.redoStack.empty()) game.redoStack.pop();
        while (!game.scoreRedoStack.empty()) game.scoreRedoStack.pop();

        if (moveAndMerge(game, choice)) {
            addRandom(game);
        }
        else {
            cout << "Invalid move!" << endl;
        }
    }

    delete[] game.board;
    return 0;
}
