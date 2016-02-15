#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <ctime>
#include <set>

using namespace std;

clock_t inicio;

int res[9][9];
set<int> sudoku[9][9];

bool resolveu;
bool tempo;

void inference(set<int> v[9][9]);
bool solver(set<int> c[9][9]);

void copy_v(set<int> v1[9][9], set<int> v2[9][9])
{
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            v1[i][j] = v2[i][j];
}

bool r_redution(set<int> s[9][9], int i, int j)
{
    int val = *s[i][j].begin();
    for (int p = 0, a, b; p < 9; ++p)
    {
        a = (i/3)*3 + p/3;
        b = (j/3)*3 + p%3;
        if ( p != j && s[i][p].find(val) != s[i][p].end() )
        {
            s[i][p].erase(val);
            if(s[i][p].size() == 0)
                return false;
            if(s[i][p].size() == 1)
                if(!r_redution(s,i,p))
                    return false;
        }
        if ( p != i && s[p][j].find(val) != s[p][j].end() )
        {
            s[p][j].erase(val);
            if(s[p][j].size() == 0)
                return false;
            if(s[p][j].size() == 1)
                if(!r_redution(s,p,j))
                    return false;
        }
        if ( !(a == i && b == j) && s[a][b].find(val) != s[a][b].end() )
        {
            s[a][b].erase(val);
            if(s[a][b].size() == 0)
                return false;
            if(s[a][b].size() == 1)
                if(!r_redution(s,a,b))
                    return false;
        }
    }
    return true;
}

void inference(set<int> v[9][9])
{
    if(resolveu)
        return;
    set<pair<int, int>> pos;

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if(v[i][j].size() > 1)
                pos.insert(make_pair(i,j));
        }
    }

    int x = rand()%pos.size();
    set<pair<int,int>>::iterator it;
    for (it = pos.begin(); x!=0; ++it,--x);

    pair<int, int> p = *it;

    for (auto i : v[p.first][p.second])
    {
        if(resolveu || tempo)
            return;
        set<int> c[9][9];
        copy_v(c, v);
        c[p.first][p.second] = {i};
        if(r_redution(c, p.first, p.second))
            if(solver(c))
                return;
    }

}

bool solved(set<int> c[9][9])
{
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            if(c[i][j].size() > 1)
                return false;

    return true;
}

bool has_solution(set<int> s[9][9])
{
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            if(s[i][j].size() == 0)
                return false;

    int W, val, l[9], c[9], q[3][3];

    W = 1^2^3^4^5^6^7^8^9;

    for (int i = 0; i < 9; ++i){
        l[i] = W;
        c[i] = W;
        q[i/3][i%3] = W;
    }

    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            val = *s[i][j].begin();
            l[i] ^= val;
            c[j] ^= val;
            q[i/3][j/3] ^= val;
        }
    }

    for (int i = 0; i < 9; ++i)
        if(l[i] || c[i] || q[i/3][i%3])
            return false;

    return true;
}

bool solver(set<int> c[9][9])
{
    if( ((clock()-inicio)/CLOCKS_PER_SEC) > 60 )
    {
        tempo = true;
        return true;
    }

    if(!solved(c))
        inference(c);
    else
    {
        if(!has_solution(c))
            return false;
        else
        {
            resolveu = true;
            for (int i = 0; i < 9; ++i)
                for (int j = 0; j < 9; ++j)
                    res[i][j] = *c[i][j].begin();
            return true;
        }
    }

    return false;
}

void init()
{
    srand(time(NULL));
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            sudoku[i][j] = {1,2,3,4,5,6,7,8,9};
}

int main()    
{
    int i,j,k,a,n;

    scanf("%d", &n);

    for (k = 0; k < n; ++k)
    {
        init();
        for ( i = 0; i < 9; ++i)
        {
            for (j = 0; j < 9; ++j)
            {
                scanf("%1d", &a);
                if(a != 0)
                {
                    sudoku[i][j] = {a};
                    r_redution(sudoku, i, j);
                }
            }
        }

        for (int u = 0; u < 5; ++u)
        {
            resolveu = tempo = false;
            inicio = clock();
            solver(sudoku);
            if(resolveu && !tempo)
                break;
        }

        printf("%Lf\n", (clock() - inicio)/(long double)(CLOCKS_PER_SEC/1000.0));

        for (i = 0; i < 9; ++i)
        {
            for (j = 0; j < 9; ++j)
                printf("%d", res[i][j]);
            printf("\n");
        }
    }

    return 0;
}

