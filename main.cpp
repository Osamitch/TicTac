#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <limits>
#include <cmath>
#include <iostream>
void drawGameField(char*, const int&);
void gameloop(char*);
float insighttree(char*, char, char, bool, float, const int&, int);
bool checkinsightwin(char, char*,const int&);

int main()
{
    char fieldmatrix[] = {'_', '_', '_', '_', '_', '_', '_', '_', '_','_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_'};

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
    auto width = 5;
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
                    while (p - (char *)matrix < 25)
                    {
                        *p = '_';
                        ++p;
                    }
                    std::cout << " 3, 4 or 5 game field?\n";
                    int ans_i=0;
                    std::cin >> ans_i;
                    if (ans_i>=3 && ans_i<=5)
                    {
                        width=ans_i;
                    }
                    else
                    {
                        std::cout << "Invalid input, try again\n";
                        break;
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
                    drawGameField(matrix, width);
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
                if (x >= 0 && x < width && y >= 0 && y < width && matrix[x*width+y] == '_')
                {
                    matrix[x*width+y] = symb;

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
                std::cout << symb << " AI turn, can take time";
                int max_x = 0;
                int max_y = 0;
                for (int x = 0; x < width; ++x)
                {
                    for (int y = 0; y < width; ++y)
                    {
                        if (matrix[x*width+y] == '_')
                        {
                            matrix[x*width+y] = symb;
                            auto max_weight = 1.f;
                            auto win = insighttree((char*)matrix, symb, enemysymb, false, max_weight, width, 6);
                            matrix[x*width+y] = '_';
                            std::cout<< ".";
                            if (win > max_win)
                            {
                                max_win = win;
                                max_x = x;
                                max_y = y;
                            }
                        }
                    }
                }
                std::cout<<'\n';
                matrix[max_x*width + max_y] = symb;
                state = GameState::CheckWin;
                break;
            }
            case GameState::CheckWin:
            {
                drawGameField(matrix, width);
                getchar();
                if(checkinsightwin(playSymbols[turnSymbol], matrix, width))
                {
                    std::cout << playSymbols[turnSymbol] <<" wins!\n";
                    state = GameState::Initial;
                }
                else
                {
                    bool is_draw = true;
                    for (int i = 0; i < width*width; ++i)
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

void drawGameField(char* matrix, const int &width)
{
    for (int y = width-1; y >= 0; --y)
    {
        for (int x = 0; x < width; ++x)
        {
            std::cout << "|" << matrix[x*width+y];
        }

        std::cout << "|\n";
    }
}

float insighttree(char* matrix, char symb, char enemysymb, bool isMyturn, float weight, const int& width, int depth)
{
    if(checkinsightwin(symb, matrix, width))
    {
        return weight;
    }
    else if(checkinsightwin(enemysymb, matrix, width))
    {
        return -weight;
    }
    else
    {
        bool is_draw = true;
        for (int i = 0; i < width*width; ++i)
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
    --depth;
    if (depth<=0)
    {
        return ret;
    }
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < width; ++y)
        {
            if (matrix[x*width+y] == '_')
            {
                char inmatrix[25];
                std::copy(matrix, matrix+width*width, inmatrix);
                if (isMyturn)
                    inmatrix[x*width+y] = symb;
                else
                    inmatrix[x*width+y] = enemysymb;
                
                ret += insighttree(inmatrix, symb, enemysymb, !isMyturn, weight / 2.f, width, depth);
            }
        }
    }
    return ret;// * weight;
}

bool checkinsightwin(char symb, char* matrix, const int &width)
{
    bool res = false;
    bool row = true;
    bool column = true;
    for (int y = 0; y < width; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            column = column && (matrix[x*width+y] == symb);
            row = row && (matrix[y*width+x] == symb);
        }
        res = res || row || column;
        row = true;
        column = true;
    }

    bool diag1 = true;
    bool diag2 = true;
    for (int x=0; x<width; ++x)
    {

        diag1 = (diag1 && matrix[x*width +x] ==symb);
        diag2 = (diag2 && matrix[(width-1-x)*width +x]==symb);
    }
    return diag1 ||diag2 || res;
}