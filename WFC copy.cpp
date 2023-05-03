#include <vector>
#include <iostream>
#include <random>
#include <time.h>
#include <fstream>
#include <algorithm>
#include <queue>
#include <set>
#include <typeinfo>
// #include "Tile.h"
// #include "Functions.h"

// Функция вывода stl 2D вектора
template <class T>
void PrintVector2D(std::vector<std::vector<T>> &v)
{
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v[i].size(); j++)
        {
            if (v[i][j] >= 0)
                std::cout << " " << v[i][j] << " ";
            else
                std::cout << v[i][j] << " ";
        }
        std::cout << "\n";
    }
}

// Функци вывода stl вектора
template <class T>
void PrintVector(std::vector<T> &v)
{
    for (int j = 0; j < v.size(); j++)
    {
        std::cout << v[j] << " ";
    }
    std::cout << std::endl;
}

//
template <class Container>
void Print(const Container &c)
{
    for (auto e : c)
    {
        std::cout << e << " ";
    }
    std::cout << std::endl;
}

template <class Container>
void Print2D(const Container &c)
{
    for (auto sc : c)
    {
        for (auto e : sc)
        {
            std::cout << e << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

template <class Container, class T>
bool In(const Container &c, T value)
{
    if (typeid(*c.begin()) != typeid(value))
    {
        return 0;
    }
    return (c.find(value) != c.end());
}

template <class Container>
auto select_random(const Container &c)
{
    auto n = rand() % c.size();
    auto it = std::begin(c);
    std::advance(it, n);
    return it;
}

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

// Вспомогательнай класс, хранящий в себе порядковый номер тайла (кусочка карты) ~collapse_target~, который скаллапсировал в данных координатах ~xy~
class Tile : public std::set<int>
{
private:
    Coord xy;

public:
    Tile(int X = 0, int Y = 0, int diversity = 0);
    Tile(Coord XY, int diversity = 0);
    Tile(const Tile &t);

    bool operator<(const Tile &t);
    bool operator>(const Tile &t);

    Coord getCoords();
    std::set<int> getTarget();

    void setCoord(int X, int Y);
    void setTarget(int target);

    void print();
};

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

//
enum Direction
{
    North = 1,
    East = 2,
    South = 3,
    West = 4
};

//
class Chunk
{
private:
    std::vector<Tile> wave; // Волна. Хранит номера возможных тайлов для данной позиции.
    // TODO: std::vector<int> size_of_space; // Размеры пространсва. Вектор на случай многомерных пространств.
    std::set<int> collapsed_positions; // Множество уже сколлапсированных тайлов.
    // std::vector<std::set<int>> rules;  // Вектор множеств возможных тайлов.
    std::vector<std::pair<Direction, std::vector<int>>> rules;// Вектор правил соединений тайлов. 
    // Содержит пару Direction (направление) и вектор возмодных соединений (вектор номеров тайлов)
    // TODO: unsigned int dimetion; // Размерность пространства.
    int size;        // Размер грани chunk.
    int linear_size; // Размер линеаризованного пространства.
    int diversity;   // Модуль разнообразия тайлов.
    // TODO: unsigned int smooth_coef; // Количество применений к chunk сглаживающей функции.

    // Возвращает множество возможных соседних тайлов для данной позиции.
    std::set<int> PossibleNearbyTiles(int position)
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

    // Проверяет на возможностть коллапсирования цели в данной позиции.
    bool Possible(int collapse_target, int position)
    {
        if (In(rules[position + 1], collapse_target) &&
            In(rules[position - 1], collapse_target) &&
            In(rules[position + size], collapse_target) &&
            In(rules[position - size], collapse_target))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    // Мера неопределенности данной позиции. TODO: сделать исключение.
    int Entropy(int position)
    {
        /*
        if (wave[position].size() == 0)
            throw std::invalid_argument("Tile is impossible!");
        else
        */
        return wave[position].size() - 1;
    }

    // Проверка состояний волны в данный позиции. 1 - реализована, 0 - нет.
    bool IsRealized(int position) { return wave[position].size() == 1; }

    // Проверка вхождения дпанной позиции в границы карты
    bool InBounds(int position) { return ((position >= 0) && (position <= linear_size)); }

    // Инициализация волны
    void InitWave()
    {
        for (int i = 0; i < linear_size; i++)
        {
            wave.push_back(Tile(ReversFi(i, size), diversity));
        }
    }

    // Метод коолапса. Выбирает из возмодных тайлов для данной позиции. TODO: доделать вброс исключений.
    void Collapse(int position)
    {
        int target = *(select_random(wave[position]));

        wave[position].setTarget(target);
    }

    // Наблюдает (находит) тайл с минимальной не нулевой энтропией.
    // TODO: сделать поиск минимума из вершин которые были изменнены в методе Distribution. Будет ли это эфиктивно?
    int Observation()
    {
        int position, target, min_entropy;

        min_entropy = diversity + 1;
        position = -1;
        for (int i = 0; i < wave.size(); i++)
        {
            int entropy = Entropy(i);
            /*
            try
            {
                entropy = Entropy(i);
            }
            catch(std::invalid_argument e)
            {
                std::cout << e.what() << std::endl;
                break;
            }
            */

            if (entropy != 0 && entropy < min_entropy)
            {
                position = i;
                min_entropy = entropy;
            }
        }

        if (position == -1)
        {
            throw std::invalid_argument("Wave realised.");
        }

        try
        {
            Collapse(position);
        }
        catch (std::invalid_argument e)
        {
            std::cout << e.what() << "\n";
        }

        return position;
    }

    // Распространяет информацию о коллапсе по волне
    void Distribution(int position)
    {
        std::set<int> mark;
        std::queue<int> vertices;

        vertices.push(position);
        mark.insert(position);

        while (!vertices.empty())
        {
            int temp_position = vertices.front();
            vertices.pop();

            if (IsRealized(temp_position))
            {
                collapsed_positions.insert(temp_position);
            }

            if (wave[temp_position].size() == diversity)
            {
                continue;
            }

            // Add near tiles to queue
            if (!(mark.find(temp_position) == mark.end()))
            {
                if ((temp_position % size > 0) && (mark.find(temp_position - 1) == mark.end())) //
                {
                    vertices.push(temp_position - 1);
                    mark.insert(temp_position - 1);
                }
                if ((temp_position > size - 1) && (mark.find(temp_position - size) == mark.end()))
                {
                    vertices.push(temp_position - size);
                    mark.insert(temp_position - size);
                }
                if ((temp_position % size < size - 1) && (mark.find(temp_position + 1) == mark.end()))
                {
                    vertices.push(temp_position + 1);
                    mark.insert(temp_position + 1);
                }
                if ((temp_position < linear_size - 1 - size) && (mark.find(temp_position + size) == mark.end()))
                {
                    vertices.push(temp_position + size);
                    mark.insert(temp_position + size);
                }
            }

            std::set<int> possible_tiles = PossibleNearbyTiles(temp_position);

            if (possible_tiles.size() == diversity)
            {
                continue;
            }

            for (int nearby_position = -size + temp_position; nearby_position <= size + temp_position;)
            {
                if ((nearby_position >= 0) && (nearby_position <= linear_size))
                {
                    for (int j = 0; j < diversity; j++)
                    {
                        if (!In(possible_tiles, j) && (collapsed_positions.find(nearby_position) == collapsed_positions.end()))
                        {
                            try
                            {
                                wave[nearby_position].erase(j);
                            }
                            catch (std::invalid_argument e)
                            {
                                std::cout << e.what() << "\n";
                            }
                        }
                    }
                }
                if (nearby_position == -1 + temp_position)
                    nearby_position += 2;
                else
                    nearby_position += size - 1;
            }
        }
    }

    //
    void WFC()
    {
        srand(time(NULL));
        int position = -1;
        int c = 0;
        while (collapsed_positions.size() != linear_size)
        {
            try
            {
                position = Observation();
                c++;
            }
            catch (std::invalid_argument e)
            {
                std::cout << e.what() << "\n";
                break;
            }

            try
            {
                Distribution(position);
            }
            catch (std::invalid_argument e)
            {
                std::cout << e.what() << "\n";
                return;
            }
        }
        std::cout << "Iteration: " << c << std::endl;
    }

public:
    Chunk(std::vector<std::set<int>> Rules, int Diversity, int Size = 2)
    {
        rules = Rules;
        diversity = Diversity;
        size = Size;
        linear_size = size * size;
        InitWave();
    }

    void GenerateChunk()
    {
        WFC();
    }

    // TODO: надо придумать как генерировать присоединенные chunks.
    /*
    void GenerateJoinedChunk(Chunk &c, Direction dir = North)
    {

        // North : position < size
        // East : position % size == size - 1
        // South : position >= linear_size - size
        // West : position % size == 0


        if (dir == North)
        {
            for (int position = 0; position < size; position++)
            {
                wave[position] = c.wave[position];
            }
        }
        else if (dir == East)
        {
        }
    }
    */

    void PrintChunk()
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                int position = Fi(Coord(j, i), size);
                if (wave[position].size() == 1)
                {
                    int sum = 0;
                    for (auto e : wave[position])
                    {
                        sum += e;
                    }
                    std::cout << " " << sum << " ";
                }
                else
                {
                    std::cout << "-1 ";
                }
            }
            std::cout << "\n";
        }
    }

    void SaveChunk()
    {
        std::ofstream fout("map.txt");
        fout << size << " " << size << "\n";
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                int position = Fi(Coord(j, i), size);

                if (wave[position].size() == 1)
                {
                    int sum = 0;
                    for (auto e : wave[position])
                    {
                        sum += e;
                    }
                    fout << " " << sum << " ";
                }
                else
                {
                    fout << "-1 ";
                }
            }
        }
    }
};

class Map
{
private:
    std::vector<std::vector<Tile>> map;

public:
    Map() {}
    Map(const Map &m) { map = m.map; }
};

int main()
{

    std::vector<std::set<int>> rules;

    int size, diversity = 6;
    std::cin >> size;

    rules.resize(diversity);
    srand(time(NULL));

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

    Chunk c(rules, diversity, size);

    c.GenerateChunk();
    // c.PrintChunk();
    c.SaveChunk();
    system("python show.py");
}
