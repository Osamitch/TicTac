#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <limits>
#include <cmath>
#include <iostream>
void drawGameField(char*);
void gameloop(char*);
float insighttree(char*, char, char, bool, float);
bool checkinsightwin(char, char*);

int main()
{
    char fieldmatrix[] = {'_', '_', '_', '_', '_', '_', '_', '_', '_'};
    gameloop(fieldmatrix);

    return 0;
}

enum class GameState
{
    Initial,
    PlayerTurn,
    AITurn,
    CheckWin
};


void gameloop(char* matrix)
{
    bool players[] = { true, false };

    auto state = GameState::Initial;
    char playSymbols[] = {'X', 'O'};
    auto turnSymbol = 0;
    auto prevTurnSymbol = turnSymbol;

    while (1) //main game loop
    {
        auto symb = playSymbols[turnSymbol];
        auto enemysymb = playSymbols[(turnSymbol+1)%2];
        switch (state)
        {
            case GameState::Initial:
            {
                
                std::cout << "\n Do you want to start new game?(y/n)";
                char ans = '\n';
                std::cin >> ans;
                if (ans == 'y' || ans == 'Y')
                {
                    char *p = (char *)matrix;
                    while (p - (char *)matrix < 9)
                    {
                        *p = '_';
                        ++p;
                    }
                    std::cout <<" Welcome to the game of TicTacToe!\n X goes first!\n";
                    std::cout << " X is AI(1) or Player(2)?\n";
                    std::cin >> ans;
                    if (ans=='1')
                    {
                        players[0]=false;
                    }
                    else if(ans=='2')
                    {
                        players[0]=true;
                    }
                    else
                    {
                        std::cout << "Invalid input, try again\n";
                        break;
                    }
                    std::cout << " O is AI(1) or Player(2)?\n";
                    std::cin >> ans;
                    if (ans=='1')
                    {
                        players[1]=false;
                    }
                    else if(ans=='2')
                    {
                        players[1]=true;
                    }
                    else
                    {
                        std::cout << "Invalid input, try again";
                        break;
                    }
                    drawGameField(matrix);
                    turnSymbol = 0;
                    prevTurnSymbol = turnSymbol;
                    state = players[turnSymbol] ? GameState::PlayerTurn : GameState::AITurn;
                }
                else if (ans == '\n')
                {
                }
                else
                {
                    return;
                }
                break;
            }
            case GameState::PlayerTurn:
            {
                int x, y;
                std::cout << symb << " turn, enter x dimension:";
                std::cin >> x;
                std::cout << symb << " turn, enter y dimension:";
                std::cin >> y;
                --x;
                --y;
                if (x >= 0 && x < 3 && y >= 0 && y < 3 && matrix[x*3+y] == '_')
                {
                    matrix[x*3+y] = symb;

                    state = GameState::CheckWin;
                }
                else
                {
                    std::cout << "Invalid input, try again\n";
                }
                break;
            }
            case GameState::AITurn:
            {
                auto max_win = -std::numeric_limits<float>::infinity();
                std::cout << symb << " AI turn:\n";
                int max_x = 0;
                int max_y = 0;
                for (int x = 0; x < 3; ++x)
                {
                    for (int y = 0; y < 3; ++y)
                    {
                        if (matrix[x*3+y] == '_')
                        {
                            matrix[x*3+y] = symb;
                            auto max_weight = 1.f;
                            auto win = insighttree((char*)matrix, symb, enemysymb, false, max_weight);
                            matrix[x*3+y] = '_';
                            
                            if (win > max_win)
                            {
                                max_win = win;
                                max_x = x;
                                max_y = y;
                            }
                        }
                    }
                }
                matrix[max_x*3 + max_y] = symb;
                state = GameState::CheckWin;
                break;
            }
            case GameState::CheckWin:
            {
                drawGameField(matrix);
                getchar();
                if(checkinsightwin(playSymbols[turnSymbol], matrix))
                {
                    std::cout << playSymbols[turnSymbol] <<" wins!\n";
                    state = GameState::Initial;
                }
                else
                {
                    bool is_draw = true;
                    for (int i = 0; i < 9; ++i)
                    {
                        if (matrix[i] == '_')
                        {
                            is_draw = false;
                            break;
                        }
                    }
                    if (is_draw)
                    {
                        std::cout << "Round draw!\n";
                        state = GameState::Initial;
                    }
                    else
                    {
                        turnSymbol = (turnSymbol+1) % 2;
                        state = players[turnSymbol] ? GameState::PlayerTurn : GameState::AITurn;
                    }
                }
                break;
            }
        }
    }
}

void drawGameField(char* matrix)
{
    for (int y = 2; y >= 0; --y)
    {
        for (int x = 0; x < 3; ++x)
        {
            std::cout << "|" << matrix[x*3+y];
        }

        std::cout << "|\n";
    }
}

float insighttree(char* matrix, char symb, char enemysymb, bool isMyturn, float weight)
{
    if(checkinsightwin(symb, matrix))
    {
        return weight;
    }
    else if(checkinsightwin(enemysymb, matrix))
    {
        return -weight;
    }
    else
    {
        bool is_draw = true;
        for (int i = 0; i < 9; ++i)
        {
            if (matrix[i] == '_')
            {
                is_draw = false;
                break;
            }
        }
        if (is_draw)
        {
            return 0.f;
        }
    }

    float ret = 0.f;
    for (int x = 0; x < 3; ++x)
    {
        for (int y = 0; y < 3; ++y)
        {
            if (matrix[x*3+y] == '_')
            {
                char inmatrix[3*3];
                memcpy(inmatrix, matrix, 3 * 3);
                if (isMyturn)
                    inmatrix[x*3+y] = symb;
                else
                    inmatrix[x*3+y] = enemysymb;

                ret += insighttree(inmatrix, symb, enemysymb, !isMyturn, weight / 2.f);
            }
        }
    }
    return ret;// * weight;
}

bool checkinsightwin(char symb, char* matrix)
{
    bool res = false;
    bool diag = true;
    bool row = true;
    bool column = true;
    for (int y = 0; y < 3; ++y)
    {
        for (int x = 0; x < 3; ++x)
        {
            column = column && (matrix[x*3+y] == symb);
            row = row && (matrix[y*3+x] == symb);
        }
        res = res || row || column;
        row = true;
        column = true;
    }
    diag = diag && (matrix[1*3+1] == symb) && ((matrix[0*3+0] == symb && matrix[2*3+2] == symb) || (matrix[0*3+2] == symb && matrix[2*3+0] == symb));
    return diag || res;
}