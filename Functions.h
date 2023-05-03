#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <exception>
#include <set>
#include <vector>
#include <typeinfo>

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
#endif
