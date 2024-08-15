#include <bits/stdc++.h>
#include <windows.h>

using namespace std;

class Board
{

public:
    int k, empty_i, empty_j, move, h, f;
    vector<vector<int>> values;
    Board(int);
    Board(void);
    void Input(void);
    void Output(void);
    void setMove(int);
    int getMove(void);
    void setEmptyI(int);
    int getEmptyI(void);
    void setEmptyJ(int);
    int getEmptyJ(void);
    int H_distance(void);
    int M_distance(void);
    bool Sol();
    Board(const Board &obj);
};

Board ::Board(int k1)
{
    move = 0;
    k = k1;
    for (int j = 0; j < k; j++)
    {
        vector<int> v1;
        values.push_back(v1);
    }
}

Board ::Board()
{
}

void Board ::Input()
{
    int value;
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            cin >> value;
            values[i].push_back(value);
            if (value == 0)
            {
                empty_i = i;
                empty_j = j;
            }
        }
    }
}

void Board ::Output()
{
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            cout << values[i][j] << " ";
        }
        cout << endl;
    }
}

void Board ::setMove(int a)
{
    move = a;
}

int Board ::getMove()
{
    return move;
}

void Board ::setEmptyI(int a)
{
    empty_i = a;
}

int Board ::getEmptyI()
{
    return empty_i;
}

void Board ::setEmptyJ(int a)
{
    empty_j = a;
}

int Board ::getEmptyJ()
{
    return empty_j;
}

int Board ::H_distance()
{
    int H_d = 0;
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            if (values[i][j] != (i * k + j + 1) && values[i][j] != 0)
                H_d += 1;
        }
    }
    return H_d;
}

int Board ::M_distance()
{
    int M_d = 0;
    int M_i, M_j;
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            if (values[i][j] == 0)
                continue;
            M_i = (values[i][j] - 1) / k;
            M_j = (values[i][j] - 1) % k;
            M_d += abs(M_i - i) + abs(M_j - j);
        }
    }
    return M_d;
}

Board ::Board(const Board &obj)
{
    f = obj.f;
    h = obj.h;
    k = obj.k;
    empty_i = obj.empty_i;
    empty_j = obj.empty_j;
    move = obj.move;
    values = obj.values;
}

bool Board ::Sol()
{
    int N = k * k;
    int *tem = new int[N];
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            tem[i * k + j] = values[i][j];
        }
    }
    int inv_count = 0;
    for (int i = 0; i < (N - 1); i++)
    {
        for (int j = i + 1; j < N; j++)
        {
            if (tem[i] && tem[j] && tem[i] > tem[j])
            {
                inv_count++;
            }
        }
    }
    if (k % 2 != 0)
    {
        if (inv_count % 2 == 0)
        {
            return true;
        }
    }
    else
    {
        int S = (k - empty_i) % 2;
        int Q = inv_count % 2;
        if ((S == 0 && Q != 0) || (S != 0 && Q == 0))
        {
            return true;
        }
    }
    return false;
}

struct Hash_Function
{
    size_t operator()(const vector<vector<int>> &vec) const
    {
        size_t hash = 0;
        for (const auto &innerVec : vec)
        {
            for (const auto &val : innerVec)
            {
                hash ^= std::hash<int>()(val) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
        }
        return hash;
    }
};

class comp
{
public:
    bool operator()(const Board &b1, const Board &b2)
    {
        return (b1.f > b2.f);
    }
};

set<vector<vector<int>>> st;
priority_queue<Board, vector<Board>, comp> pq;
unordered_map<vector<vector<int>>, vector<vector<int>>, Hash_Function> parents;

void print_moves(const vector<vector<int>> &c, int k)
{
    if (parents.count(c))
        print_moves(parents[c], k);
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < k; j++)
        {
            cout << c[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void Solve(const Board &b, int k)
{
    int c, expanded_nodes = 0;
    Board AB = b;
    cout << "1. M / 2. H" << endl;
    cin >> c;
    if (c == 1)
        AB.h = AB.M_distance();
    else
        AB.h = AB.H_distance();
    AB.f = AB.getMove() + AB.h;
    st.insert(AB.values);
    expanded_nodes++;
    pq.push(AB);
    while (true)
    {
        // cout << "YES" << endl;
        Board A(k);
        A = pq.top();
        pq.pop();
        // A.Output();
        // cout << A.f << " " << A.move << endl;
        if ((A.h == 0))
        {
            cout << "\nMinimum number of moves = " << A.getMove() << "\n"
                 << endl;
            print_moves(A.values, k);
            cout << "expanded nodes: " << expanded_nodes << endl;
            break;
        }
        else
        {
            int e_i = A.getEmptyI();
            int e_j = A.getEmptyJ();
            if (e_j != 0)
            {
                // cout << "Left" << endl;
                Board B = A;
                int temp = B.values[e_i][e_j - 1];
                B.values[e_i][e_j - 1] = B.values[e_i][e_j];
                B.values[e_i][e_j] = temp;
                if (st.find(B.values) == st.end())
                {
                    expanded_nodes++;
                    parents[B.values] = A.values;
                    // cout << "Parent:" << endl;
                    //  (B.parent)->Output();
                    B.setEmptyJ(A.getEmptyJ() - 1);
                    B.setMove(A.getMove() + 1);
                    if (c == 1)
                        B.h = B.M_distance();
                    else
                        B.h = B.H_distance();
                    B.f = B.getMove() + B.h;
                    st.insert(B.values);
                    pq.push(B);
                }
                // cout << "B:" << endl;
                // cout << B.getMove() << " " << B.M_distance() << endl;
            }
            if (e_i != 0)
            {
                // cout << "UP" << endl;
                //  Sleep(1000);
                Board B = A;
                int temp = B.values[e_i - 1][e_j];
                B.values[e_i - 1][e_j] = B.values[e_i][e_j];
                B.values[e_i][e_j] = temp;
                if (st.find(B.values) == st.end())
                {
                    expanded_nodes++;
                    parents[B.values] = A.values;
                    // cout << "Parent:" << endl;
                    //  (B.parent)->Output();
                    B.setEmptyI(A.getEmptyI() - 1);
                    B.setMove(A.getMove() + 1);
                    if (c == 1)
                        B.h = B.M_distance();
                    else
                        B.h = B.H_distance();
                    B.f = B.getMove() + B.h;
                    st.insert(B.values);
                    pq.push(B);
                }
                // cout << "B:" << endl;

                // cout << B.getMove() << " " << B.M_distance() << endl;
            }
            if (e_j != k - 1)
            {
                // cout << "Right" << endl;
                //  Sleep(1000);
                Board B = A;
                int temp = B.values[e_i][e_j + 1];
                B.values[e_i][e_j + 1] = B.values[e_i][e_j];
                B.values[e_i][e_j] = temp;
                // cout << "Parent:" << endl;
                if (st.find(B.values) == st.end())
                {
                    expanded_nodes++;
                    parents[B.values] = A.values;
                    // (B.parent)->Output();
                    B.setEmptyJ(A.getEmptyJ() + 1);
                    B.setMove(A.getMove() + 1);
                    if (c == 1)
                        B.h = B.M_distance();
                    else
                        B.h = B.H_distance();
                    B.f = B.getMove() + B.h;
                    st.insert(B.values);
                    pq.push(B);
                }
                // cout << "B:" << endl;
                // cout << B.getMove() << " " << B.M_distance() << endl;
            }
            if (e_i != k - 1)
            {
                // cout << "DOWN" << endl;
                // Sleep(1000);
                Board B = A;
                int temp = B.values[e_i + 1][e_j];
                B.values[e_i + 1][e_j] = B.values[e_i][e_j];
                B.values[e_i][e_j] = temp;
                if (st.find(B.values) == st.end())
                {
                    expanded_nodes++;
                    parents[B.values] = A.values;
                    // cout << "Parent:" << endl;
                    // (B.parent)->Output();
                    B.setEmptyI(A.getEmptyI() + 1);
                    B.setMove(A.getMove() + 1);
                    if (c == 1)
                        B.h = B.M_distance();
                    else
                        B.h = B.H_distance();
                    B.f = B.getMove() + B.h;
                    st.insert(B.values);
                    pq.push(B);
                }
            }
        }
    }
}

int main()
{
    cout << "Input" << endl;
    int k;
    cin >> k;
    Board AB(k);
    AB.Input();
    if (!AB.Sol())
    {
        cout << "Unsolvable Puzzle" << endl;
    }
    else
    {
        Solve(AB, k);
    }
    return 0;
}
