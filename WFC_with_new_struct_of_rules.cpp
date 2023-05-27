#include <sstream>
#include <iostream>
#include <fstream>

#include <random>
#include <algorithm>
#include <typeinfo>

#include <time.h>
#include <queue>
#include <set>
#include <tuple>
#include <string>
#include <vector>
#include <iterator>
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
    North = 0,
    East = 1,
    South = 2,
    West = 3
};

std::ostream &operator<<(std::ostream &os, Direction d)
{
    switch (d)
    {
    case 0:
        os << "North";
        break;
    case 1:
        os << "East";
        break;
    case 2:
        os << "South";
        break;
    case 3:
        os << "West";
        break;
    }
    return os;
}

//
class Rules : public std::vector<std::tuple<std::set<int>, std::set<int>, std::set<int>, std::set<int>>>
{
private:
    unsigned int diversity;
    unsigned int dimension;

public:
    //
    Rules(){};

    //
    Rules(unsigned int Diversity, unsigned int Dimension)
    {
        diversity = Diversity;
        dimension = Dimension;
    }

    //
    unsigned int getDiversity() { return diversity; }

    //
    void fillRules(std::istream &in)
    {
        std::string buff;

        std::cout << "Enter the number of different tiles: \n";

        std::getline(in, buff);
        std::istringstream div(buff);
        diversity = *(std::istream_iterator<int>(div));
        (*this).resize(diversity);

        for (unsigned int i = 0; i < diversity; i++)
        {
            std::cout << "Enter tile numbers that can be attached with " << i << "th tile:\n";
            for (unsigned int j = 0; j < 4; j++)
            {
                std::cout << "\twith " << (Direction)j << " side: \n";

                std::getline(in, buff);
                std::istringstream is(buff);

                if (j == 0)
                    std::copy(std::istream_iterator<int>(is), std::istream_iterator<int>(), inserter(std::get<North>((*this)[i]), std::get<North>((*this)[i]).begin()));
                else if (j == 1)
                    std::copy(std::istream_iterator<int>(is), std::istream_iterator<int>(), inserter(std::get<East>((*this)[i]), std::get<East>((*this)[i]).begin()));
                else if (j == 2)
                    std::copy(std::istream_iterator<int>(is), std::istream_iterator<int>(), inserter(std::get<South>((*this)[i]), std::get<South>((*this)[i]).begin()));
                else if (j == 3)
                    std::copy(std::istream_iterator<int>(is), std::istream_iterator<int>(), inserter(std::get<West>((*this)[i]), std::get<West>((*this)[i]).begin()));
            }
        }
    }

    //
    void printRules()
    {
        for (unsigned int i = 0; i < (*this).size(); i++)
        {
            std::cout << "Rules for " << i << "th tile: \n";

            std::cout << "\tfor North side:\t";
            Print(std::get<North>((*this)[i]));
            std::cout << "\tfor East side:\t";
            Print(std::get<East>((*this)[i]));
            std::cout << "\tfor South side:\t";
            Print(std::get<South>((*this)[i]));
            std::cout << "\tfor West side:\t";
            Print(std::get<West>((*this)[i]));
        }
    }
};

//
class Chunk
{
private:
    std::vector<Tile> wave; // Волна. Хранит номера возможных тайлов для данной позиции.
    // TODO: std::vector<int> size_of_space; // Размеры пространсва. Вектор на случай многомерных пространств.
    std::set<int> collapsed_positions; // Множество уже сколлапсированных тайлов.
    // std::vector<std::set<int>> rules;  // Вектор множеств возможных тайлов.
    Rules rules; // Вектор правил соединений тайлов.
    // Содержит пару Direction (направление) и вектор возмодных соединений (вектор номеров тайлов)
    // TODO: unsigned int dimetion; // Размерность пространства.
    int size;        // Размер грани chunk.
    int linear_size; // Размер линеаризованного пространства.
    int diversity;   // Модуль разнообразия тайлов.
    // TODO: unsigned int smooth_coef; // Количество применений к chunk сглаживающей функции.

    // Возвращает множество возможных соседних тайлов для данной позиции.
    std::set<int> PossibleNearbyTiles(int position, Direction dir)
    {
        std::set<int> possible_tiles;
        std::set<int> temp;

        for (std::set<int>::iterator it = wave[position].begin(); it != wave[position].end(); it++)
        {
            temp = possible_tiles;
            if (dir == North)
                std::set_union(std::get<North>(rules[*it]).begin(), std::get<North>(rules[*it]).end(), temp.begin(), temp.end(), inserter(possible_tiles, possible_tiles.begin()));
            else if (dir == East)
                std::set_union(std::get<East>(rules[*it]).begin(), std::get<East>(rules[*it]).end(), temp.begin(), temp.end(), inserter(possible_tiles, possible_tiles.begin()));
            else if (dir == South)
                std::set_union(std::get<South>(rules[*it]).begin(), std::get<South>(rules[*it]).end(), temp.begin(), temp.end(), inserter(possible_tiles, possible_tiles.begin()));
            else if (dir == West)
                std::set_union(std::get<West>(rules[*it]).begin(), std::get<West>(rules[*it]).end(), temp.begin(), temp.end(), inserter(possible_tiles, possible_tiles.begin()));
        }

        return possible_tiles;
    }

    // Проверяет на возможностть коллапсирования цели в данной позиции.
    /*
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
    */

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

            for (int direction = 0; direction < 4; direction++)
            {
                int nearby_position;
                if (direction == 0)
                    nearby_position = temp_position - size;
                else if (direction == 1)
                    nearby_position = temp_position + 1;
                else if (direction == 2)
                    nearby_position = temp_position + size;
                else if (direction == 3)
                    nearby_position = temp_position - 1;

                std::set<int> possible_tiles = PossibleNearbyTiles(temp_position, (Direction)direction);

                if (possible_tiles.size() == diversity)
                {
                    continue;
                }

                if ((nearby_position >= 0) && (nearby_position <= linear_size))
                {
                    for (int j = 0; j < diversity; j++)
                    {
                        if ((possible_tiles.find(j) == possible_tiles.end()) && (collapsed_positions.find(nearby_position) == collapsed_positions.end()))
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
            }
        }
    }

    //
    void WFC()
    {
        srand(time(NULL));
        int position = rand() % linear_size;
        int c = 0;
        while (collapsed_positions.size() != linear_size)
        {
            if (c != 0)
            {
                try
                {
                    position = Observation();
                }
                catch (std::invalid_argument e)
                {
                    std::cout << e.what() << "\n";
                    break;
                }
            }

            try
            {
                Distribution(position);
                c++;
            }
            catch (std::invalid_argument e)
            {
                std::cout << e.what() << "\n";
                return;
            }
        }
        std::cout << "Iterations: " << c << std::endl;
    }

public:
    Chunk(Rules &Rules, int Size = 2)
    {
        rules = Rules;
        diversity = Rules.getDiversity();
        size = Size;
        linear_size = size * size;
        InitWave();
    }

    Chunk(const Chunk &c)
    {
        rules = c.rules;
        diversity = c.diversity;
        size = c.size;
        linear_size = c.linear_size;
        InitWave();
    }

    void GenerateChunk()
    {
        WFC();
    }

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
    std::vector<std::vector<Chunk>> matrix_of_chunks;

public:
    Map(Rules &rules, unsigned int size_of_map, unsigned int dimension_of_map = 2)
    {
        matrix_of_chunks.resize(size_of_map);
        for (int i = 0; i < size_of_map; i++)
        {
            Chunk c(rules, 10);
            matrix_of_chunks[i].push_back(c);
        }
    }
};

int main()
{

    // North = 0 : position < size
    // East = 1: position % size == size - 1
    // South = 2: position >= linear_size - size
    // West = 3: position % size == 0
    int size = 15;
    // std::cin >> size;

    // std::vector<std::tuple<std::set<int>, std::set<int>, std::set<int>, std::set<int>>> rules;
    Rules rules;
    rules.fillRules(std::cin);
    rules.printRules();

    Chunk c(rules, 10);
    
    try
    {
        c.GenerateChunk();
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
    // c.PrintChunk();
    c.SaveChunk();
    system("python show.py");

    /*
    rules.resize(diversity);

    std::get<North>(rules[0]).insert(0);
    std::get<East>(rules[0]).insert(0);
    std::get<South>(rules[0]).insert(0);
    std::get<West>(rules[0]).insert(0);

    std::get<North>(rules[1]).insert(1);
    std::get<East>(rules[1]).insert(1);
    std::get<South>(rules[1]).insert(1);
    std::get<West>(rules[1]).insert(1);

    std::get<North>(rules[2]).insert(2);
    std::get<East>(rules[2]).insert(2);
    std::get<South>(rules[2]).insert(2);
    std::get<West>(rules[2]).insert(2);

    std::get<North>(rules[3]).insert(3);
    std::get<East>(rules[3]).insert(3);
    std::get<South>(rules[3]).insert(3);
    std::get<West>(rules[3]).insert(3);

    std::get<North>(rules[4]).insert(4);
    std::get<East>(rules[4]).insert(4);
    std::get<South>(rules[4]).insert(4);
    std::get<West>(rules[4]).insert(4);

    std::get<North>(rules[5]).insert(5);
    std::get<East>(rules[5]).insert(5);
    std::get<South>(rules[5]).insert(5);
    std::get<West>(rules[5]).insert(5);

    std::get<South>(rules[1]).insert(0);
    std::get<North>(rules[0]).insert(1);

    std::get<North>(rules[1]).insert(2);
    std::get<East>(rules[1]).insert(2);
    std::get<West>(rules[1]).insert(2);
    std::get<South>(rules[2]).insert(1);
    std::get<East>(rules[2]).insert(1);
    std::get<West>(rules[2]).insert(1);

    std::get<North>(rules[2]).insert(3);
    std::get<East>(rules[2]).insert(3);
    std::get<South>(rules[2]).insert(3);
    std::get<West>(rules[2]).insert(3);
    std::get<North>(rules[3]).insert(2);
    std::get<East>(rules[3]).insert(2);
    std::get<South>(rules[3]).insert(2);
    std::get<West>(rules[3]).insert(2);

    std::get<North>(rules[3]).insert(4);
    std::get<East>(rules[3]).insert(4);
    std::get<West>(rules[3]).insert(4);
    std::get<East>(rules[4]).insert(3);
    std::get<South>(rules[4]).insert(3);
    std::get<West>(rules[4]).insert(3);

    std::get<North>(rules[4]).insert(5);
    std::get<South>(rules[5]).insert(4);
    */
}
