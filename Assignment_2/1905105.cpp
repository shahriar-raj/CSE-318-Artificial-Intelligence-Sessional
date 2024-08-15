#include <bits/stdc++.h>
#include "windows.h"
using namespace std;

#define INF 99999

int w1, w2, w3, w4, depth1, heur1;
int w21, w22, w23, w24, depth2, heur2;

class Board
{
public:
    int pits[2][7];
    int add_move;
    int captured;
    int turn;
    bool choice;
    Board();
    void printBoard();
    void gamePlay(int);
    int getSum1();
    int getSum2();
    int heuristics(int);
    bool gameover();
    pair<int, int> minimax_algo_with_pruning(int, int, int, int, int, int, int, bool);
};

Board::Board()
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            pits[i][j] = 4;
        }
    }
    pits[0][6] = 0;
    pits[1][6] = 0;
    add_move = 0;
    captured = 0;
    choice = false;
}

int Board ::getSum1()
{
    int sum = 0;
    for (int i = 0; i < 6; i++)
    {
        sum += pits[0][i];
    }
    return sum;
}

int Board ::getSum2()
{
    int sum = 0;
    for (int i = 0; i < 6; i++)
    {
        sum += pits[1][i];
    }
    return sum;
}

int Board::heuristics(int h)
{
    if (h == 1)
    {
        if (choice)
        {
            return pits[0][6] - pits[1][6];
        }
        return pits[1][6] - pits[0][6];
    }
    else if (h == 2)
    {
        if (choice)
        {
            return w21 * heuristics(1) + w22 * (getSum1() - getSum2());
        }
        return w1 * heuristics(1) + w2 * (getSum2() - getSum1());
    }
    else if (h == 3)
    {
        if (choice)
        {
            return heuristics(2) + w23 * add_move;
        }
        return heuristics(2) + w3 * add_move;
    }
    else if (h == 4)
    {
        if (choice)
        {
            return heuristics(3) + w24 * captured;
        }
        return heuristics(3) + w4 * captured;
    }
    else
    {
        if (choice)
        {
            return heuristics(3) + w24 * captured;
        }
        return heuristics(3) + w4 * captured;
    }
}

bool Board::gameover()
{
    if (getSum2() == 0)
    {
        pits[0][6] += getSum1();
        for (int i = 0; i < 6; i++)
            pits[0][i] = 0;
        return true;
    }
    else if (getSum1() == 0)
    {
        pits[1][6] += getSum2();
        for (int i = 0; i < 6; i++)
            pits[1][i] = 0;
        return true;
    }
    return false;
}

pair<int, int> Board ::minimax_algo_with_pruning(int d, int alpha, int beta, int t, int c, int a_m, int heur, bool ch)
{
    if (!d)
    {
        // cout << "Heuristic: " << heuristics(heur) << "Choice: " << choice << endl;
        return make_pair(heuristics(heur), 0);
    }
    if (gameover())
    {
        return make_pair(heuristics(heur), 0);
    }
    int index = -1;
    if (turn)
    {
        int best;
        if (choice)
            best = INF;
        else
            best = -INF;
        // Recur for possible 6 children
        for (int child = 0; child < 6; child++)
        {
            // cout << "Turn 1 Choice : " << choice << "  Child: " << child << endl;
            if (pits[turn][child] == 0)
            {
                continue;
            }
            int temp_pits[2][7];
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 7; j++)
                {
                    temp_pits[i][j] = pits[i][j];
                }
            }
            int temp_a_m = add_move;
            int temp_captured = captured;
            int temp_turn = turn;
            // cout << "DEPTH (P): " << d << endl;
            // printBoard();
            gamePlay(child + 1);
            int val = minimax_algo_with_pruning(d - 1, alpha, beta, turn, c, a_m, heur, choice).first;
            // cout << "DEPTH (A): " << d << endl;
            // printBoard();
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 7; j++)
                {
                    pits[i][j] = temp_pits[i][j];
                }
            }
            add_move = temp_a_m;
            captured = temp_captured;
            turn = temp_turn;
            if ((val > best && !choice) || (val < best && choice))
            {
                best = val;
                index = child;
            }
            if (choice)
                beta = min(beta, best);
            else
                alpha = max(alpha, best);

            // Alpha Beta Pruning
            if (beta <= alpha)
            {
                break;
            }
        }
        return make_pair(best, index);
    }
    else
    {
        int best;
        if (choice)
            best = -INF;
        else
            best = INF;
        // Recur for possible 6 children
        for (int child = 0; child < 6; child++)
        {
            // cout << "Turn 0 Choice : " << choice << "  Child: " << child << endl;
            if (pits[turn][child] == 0)
            {
                continue;
            }
            int temp_pits[2][7];
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 7; j++)
                {
                    temp_pits[i][j] = pits[i][j];
                }
            }
            int temp_a_m = add_move;
            int temp_captured = captured;
            int temp_turn = turn;
            // cout << "DEPTH (P):  " << d << endl;
            // printBoard();
            gamePlay(child + 1);
            int val = minimax_algo_with_pruning(d - 1, alpha, beta, turn, c, a_m, heur, choice).first;
            // cout << "DEPTH (A): " << d << endl;
            // printBoard();
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 7; j++)
                {
                    pits[i][j] = temp_pits[i][j];
                }
            }
            add_move = temp_a_m;
            captured = temp_captured;
            turn = temp_turn;
            if ((val < best && !choice) || (choice && val > best))
            {
                best = val;
                index = child;
            }

            if (choice)
                alpha = max(alpha, best);
            else
                beta = min(beta, best);

            // Alpha Beta Pruning
            if (beta <= alpha)
            {
                break;
            }
        }
        return make_pair(best, index);
    }
}

void Board::printBoard()
{

    for (int j = 6; j > 0; j--)
    {
        cout << "\t[" << j << "|" << pits[1][j - 1] << "]";
    }
    cout << "\n[" << pits[1][6] << "]\t\t\t\t\t\t\t"
         << "[" << pits[0][6] << "]" << endl;
    for (int j = 0; j < 6; j++)
    {
        cout << "\t[" << j + 1 << "|" << pits[0][j] << "]";
    }
    cout << endl;
}

void Board ::gamePlay(int input_pit)
{
    captured = 0;
    if (!turn)
    {
        int val = pits[0][input_pit - 1];
        pits[0][input_pit - 1] = 0;
        int i = input_pit;
        while (val)
        {
            i++;
            if (i % 14 == 0)
            {
                continue;
            }
            else if (i % 14 < 8)
            {
                pits[0][i % 14 - 1]++;
                val--;
            }
            else
            {
                pits[1][i % 14 - 8]++;
                val--;
            }
        }
        if (i % 14 < 7 && pits[0][i % 14 - 1] == 1 && pits[1][6 - (i % 14)])
        {
            captured = pits[0][i % 14 - 1] + pits[1][6 - (i % 14)];
            pits[0][6] += pits[0][i % 14 - 1] + pits[1][6 - (i % 14)];
            pits[0][i % 14 - 1] = 0;
            pits[1][6 - (i % 14)] = 0;
        }
        if (i % 14 == 7)
        {
            if (choice)
                add_move++;
            turn = 0;
        }
        else
        {
            if (choice)
                add_move = 0;
            turn = 1;
        }
        // printBoard();
    }
    else
    {
        int val = pits[1][input_pit - 1];
        pits[1][input_pit - 1] = 0;
        int i = input_pit;
        while (val)
        {
            i++;
            if (i % 14 == 0)
            {
                continue;
            }
            else if (i % 14 < 8)
            {
                pits[1][i % 14 - 1]++;
            }
            else
            {
                pits[0][i % 14 - 8]++;
            }
            val--;
        }
        if (i % 14 < 7 && pits[1][i % 14 - 1] == 1 && pits[0][6 - (i % 14)])
        {
            captured = pits[1][i % 14 - 1] + pits[0][6 - (i % 14)];
            pits[1][6] += pits[1][i % 14 - 1] + pits[0][6 - (i % 14)];
            pits[1][i % 14 - 1] = 0;
            pits[0][6 - (i % 14)] = 0;
        }
        if (i % 14 == 7)
        {
            add_move++;
            turn = 1;
        }
        else
        {
            add_move = 0;
            turn = 0;
        }
        // printBoard();
    }
}

int main()
{
    Board A;
    int inp, gameMode;
    string player1, player2;
    // AI vs Human
    cout << "1. AI vs Human \n2. AI vs AI" << endl;
    cin >> gameMode;
    if (gameMode == 1)
    {
        player1 = "Human";
        player2 = "AI";
        cout << "1. Human First \n2. AI First " << endl;
        cin >> gameMode;
        if (gameMode == 1)
            A.turn = 0;
        else
            A.turn = 1;
        cout << "W1, W2, W3, W4, heur1, depth1: ";
        cin >> w1 >> w2 >> w3 >> w4 >> heur1 >> depth1;
        A.printBoard();
        while (!A.gameover())
        {
            if (A.turn == 0)
            {
                while (true)
                {
                    cout << "Human's Turn: ";
                    cin >> inp;
                    if (inp < 1 || inp > 6 || A.pits[0][inp - 1] == 0)
                    {
                    }
                    else
                    {
                        break;
                    }
                }

                A.gamePlay(inp);
            }
            else
            {
                inp = A.minimax_algo_with_pruning(depth1, -INF, INF, 1, 0, 0, heur1, false).second;
                cout << "AI's Turn: " << inp + 1 << endl;
                A.gamePlay(inp + 1);
            }
            A.printBoard();
        }
    }
    else
    {
        player1 = "AI_2";
        player2 = "AI_1";
        cout << "For AI_1 W1, W2, W3, W4, heur1, depth1: ";
        cin >> w1 >> w2 >> w3 >> w4 >> heur1 >> depth1;
        cout << "For AI_2 W1, W2, W3, W4, heur2, depth2: ";
        cin >> w21 >> w22 >> w23 >> w24 >> heur2 >> depth2;
        A.turn = 0;
        while (!A.gameover())
        {
            if (A.turn == 0)
            {
                A.choice = true;
                inp = A.minimax_algo_with_pruning(depth2, -INF, INF, 0, 0, 0, heur2, true).second;
                cout << "AI_2's Turn: " << inp + 1 << endl;
                A.gamePlay(inp + 1);
                // Sleep(1000);
                // cout << "\n\n\n\n\n\n";
            }
            else
            {
                A.choice = false;
                inp = A.minimax_algo_with_pruning(depth1, -INF, INF, 1, 0, 0, heur1, false).second;
                cout << "AI_1's Turn: " << inp + 1 << endl;
                A.gamePlay(inp + 1);
                // Sleep(1000);
            }
            A.printBoard();
        }
    }

    if (A.pits[0][6] > A.pits[1][6])
    {
        cout << player1 << " Has Won" << endl;
    }
    else if (A.pits[0][6] < A.pits[1][6])
    {
        cout << player2 << " Has Won" << endl;
    }
    else
    {
        cout << "Match Drawn" << endl;
    }
    cout << player1 << " Score: " << A.pits[0][6] << endl;
    cout << player2 << " Score: " << A.pits[1][6] << endl;
    return 0;
}