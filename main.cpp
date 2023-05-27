#include <vector>
#include <iostream>
#include <random>
#include <time.h>
#include <fstream>
#include <tuple>
#include <set>

enum Direction
{
    North = 0,
    East = 1,
    South = 2,
    West = 3
};

int main()
{
    std::tuple<std::set<Direction>, std::set<Direction>> test;
    std::get<North>(test).insert(North);
    std::get<North>(test).insert(East);
    std::get<North>(test).insert(South);
    std::get<North>(test).insert(North);

    for(auto e : std::get<0>(test))
    {
        std::cout << e << " ";
    }
    std::cout << std::endl;

}
