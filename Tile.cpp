#include "Tile.h"
#include <iostream>
#include <set>

using namespace Coordinates;
using namespace Tiles;

Coord::Coord(int X, int Y)
{
    x = X;
    y = Y;
}

Coord::Coord(const Coord &XY)
{
    x = XY.x;
    y = XY.y;
}

int Coord::getX() { return x; }
int Coord::getY() { return y; }

void Coord::setX(int X) { x = X; }
void Coord::setY(int Y) { y = Y; }

void Coord::print()
{
    std::cout << "Coords: x = " << x << ", y = " << y << "\n";
}

// Линеаризация координат для двумерного пространства
int Fi(Coord coords, int size)
{
    return coords.getX() + coords.getY() * size;
}

Coord ReversFi(int lin_position, int size)
{
    Coord xy = Coord(lin_position % size, lin_position / size);
    return xy;
}

Tile::Tile(int X, int Y, int diversity)
{
    xy = Coord(X, Y);
    for (int i = 0; i < diversity; i++)
    {
        insert(i);
    }
}

Tile::Tile(Coord XY, int diversity)
{
    xy = XY;
    for (int i = 0; i < diversity; i++)
    {
        insert(i);
    }
}

Tile::Tile(const Tile &t)
{
    xy = t.xy;
    for (std::set<int>::iterator it = t.begin(); it != t.end(); it++)
    {
        insert(*it);
    }
}

bool Tile::operator<(const Tile &t)
{
    if (size() > 1 && t.size() > 1)
        return size() < t.size();
    else
        return false;
}

bool Tile::operator>(const Tile &t)
{
    if (size() > 1 && t.size() > 1)
        return size() > t.size();
    else
        return false;
}

Coord Tile::getCoords() { return xy; }

std::set<int> Tile::getTarget() { return *this; }

void Tile::setCoord(int X, int Y)
{
    xy = Coord(X, Y);
}

void Tile::setTarget(int target)
{
    clear();
    insert(target);
}

void Tile::print()
{
    xy.Coord::print();
    std::cout << "Tile target: " << std::endl;
    for (std::set<int>::iterator it = begin(); it != end(); it++)
    {
        std::cout << *it << " ";
    }
}
