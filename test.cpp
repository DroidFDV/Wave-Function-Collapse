#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <time.h>

using namespace std;

template <class Container>
auto select_random(const Container &c)
{
    auto n = rand() % c.size();
    auto it = std::begin(c);
    std::advance(it, n);
    return it;
}

class Coord
{
protected:
    int x, y;

public:
    Coord(int X = 0, int Y = 0)
    {
        x = X;
        y = Y;
    }

    Coord(const Coord &XY)
    {
        x = XY.x;
        y = XY.y;
    }

    int getX() { return x; }
    int getY() { return y; }

    void setX(int X) { x = X; }
    void setY(int Y) { y = Y; }

    void print()
    {
        std::cout << "Coords: x = " << x << ", y = " << y << "\n";
    }
};

class Tile : public set<int>
{
private:
    Coord xy;
    // std::set<int> collapse_target;

public:
    Tile(int X = 0, int Y = 0, int diversity = 0)
    {
        xy = Coord(X, Y);
        for (int i = 0; i < diversity; i++)
        {
            this->insert(i);
        }
    }

    Tile(Coord XY, int diversity = 0)
    {
        xy = XY;
        for (int i = 0; i < diversity; i++)
        {
            this->insert(i);
        }
    }

    Tile(const Tile &t)
    {
        xy = t.xy;
    }

    Coord getCoords() { return xy; }

    // std::set<int> getTarget() { return collapse_target; }

    void setCoord(int X, int Y)
    {
        xy = Coord(X, Y);
    }

    // void setTarget(std::set<int> target) { collapse_target = target; }

    void print()
    {
        xy.print();
        std::cout << "Tile target: " << std::endl;
        for (std::set<int>::iterator it = begin(); it != end(); it++)
        {
            std::cout << *it << " ";
        }
    }
};

std::set<int> PossibleNearbyTiles(int position, vector<Tile> &wave, vector<set<int>> rules)
{
    std::set<int> possible_tiles;
    std::set<int> temp;

    for (std::set<int>::iterator it = wave[position].begin(); it != wave[position].end(); it++)
    {
        temp = possible_tiles;
        std::set_union(rules[*it].begin(), rules[*it].end(), temp.begin(), temp.end(), inserter(possible_tiles, possible_tiles.begin()));
    }

    return possible_tiles;
}

int main()
{
    srand(time(NULL));

    std::vector<std::set<int>> rules;

    int size, diversity = 6;

    rules.resize(diversity);

    rules[0].insert(0);
    rules[1].insert(1);
    rules[2].insert(2);
    rules[3].insert(3);
    rules[4].insert(4);
    rules[5].insert(5);

    rules[0].insert(1);
    rules[1].insert(0);

    rules[2].insert(1);
    rules[1].insert(2);

    rules[2].insert(3);
    rules[3].insert(2);

    rules[4].insert(3);
    rules[3].insert(4);

    rules[4].insert(5);
    rules[5].insert(4);

    vector<Tile> wave = {Tile(Coord(0, 3), 3)};
    set<int> a = {1, 3, 5, 6};
    set<int> b = {1, 3, 5, 6, 7, 8, 10};
    set<int> c = {1, 3, 5, 6, 2, 4, 7, 9};

    cout << *select_random(c) << endl;

    for (auto e : PossibleNearbyTiles(1, wave, rules))
        cout << e << " ";
    cout << endl;

    Tile t = Tile(Coord(0, 3), 3);

    // cout << t.size();

    set<int> possible_tiles;
    set<int> temp;

    // int target = *(select_random(wave));
    //  cout << target;
}