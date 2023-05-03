#ifndef TILE_H
#define TILE_H

namespace Coordinates
{
    // Вспомогательный класс координат 2D
    class Coord
    {
    protected:
        int x, y;

    public:
        Coord(int X = 0, int Y = 0);
        Coord(const Coord &XY);

        int getX();
        int getY();

        void setX(int X);
        void setY(int Y);

        void print();
    };

    // Линеаризация координат для двумерного пространства
    int Fi(Coord coords, int size);

    Coord ReversFi(int lin_position, int size);
}

namespace Tiles
{
    // Вспомогательнай класс, хранящий в себе порядковый номер тайла (кусочка карты) ~collapse_target~, который скаллапсировал в данных координатах ~xy~
    class Tile : public std::set<int>
    {
    private:
        Coordinates::Coord xy;

    public:
        Tile(int X = 0, int Y = 0, int diversity = 0);
        Tile(Coordinates::Coord XY, int diversity = 0);
        Tile(const Tile &t);

        bool operator<(const Tile &t);
        bool operator>(const Tile &t);

        Coordinates::Coord getCoords();
        std::set<int> getTarget();

        void setCoord(int X, int Y);
        void setTarget(int target);

        void print();
    };
}

#endif