/*
** EPITECH PROJECT, 2019
** for_norme
** File description:
** World.cpp
*/

#include <vector>
#include <cmath>
#include <thread> // ?
#include <chrono> // ?
#include "World.hpp"

using namespace std;

World::World()
    : World(10)
{
}

World::World(const Uint32 &_size)
    : World(Vector2u(_size, _size))
{
}

World::World(const Vector2u &_size)
    : World(Vector2u(_size.x, _size.y), sqrt(_size.x * _size.y))
{
}

World::World(const Vector2u &_size, const size_t &_nbMine)
    : affRect(0, 0, 0, 0), size(_size), nbMine(_nbMine), first(true)
{
    tab = new Tab* [size.x];
    for (Uint32 i = 0; i < size.x; i++) {
        tab[i] = new Tab [size.y];
        for (Uint32 j = 0; j < size.y; j++) {
            tab[i][j].nbMineAround = 0;
            tab[i][j].reveal = false;
            tab[i][j].flag = false;
            tab[i][j].mine = false;
        }
    }
}

World::~World()
{
}

const Vector2u &World::getSize() const
{
    return size;
}

const size_t &World::getNbMine() const
{
    return nbMine;
}

bool World::setReveal(const Vector2i &pos)
{
    Uint32 x;
    Uint32 y;

    if (pos.x < affRect.left || pos.y < affRect.top)
        return false;
    x = pos.x / (affRect.width / size.x) - affRect.left;
    y = pos.y / (affRect.height / size.y) - affRect.top;
    if (x >= size.x || y >= size.y)
        return false;
    if (tab[x][y].reveal || tab[x][y].flag)
        return false;
    tab[x][y].reveal = true;
    if (first) {
        generate();
        first = false;
    }
    spreadReveal(Vector2u(x, y));
    return true;
}

bool World::setFlag(const Vector2i &pos)
{
    Uint32 x;
    Uint32 y;

    if (pos.x < affRect.left || pos.y < affRect.top)
        return false;
    x = pos.x / (affRect.width / size.x) - affRect.left;
    y = pos.y / (affRect.height / size.y) - affRect.top;
    if (x >= size.x || y >= size.y)
        return false;
    if (tab[x][y].reveal)
        return false;
    tab[x][y].flag = !tab[x][y].flag;
    return true;
}

void World::draw(RenderTarget &target, RenderStates states) const
{
    VertexArray vertexArray;
    Texture texture;
    Vector2u affCaseSize;
    Uint32 caseSize;

    if (!texture.loadFromFile("Resources/Texture/cases.png"))
        return;
    vertexArray.setPrimitiveType(Quads);
    affCaseSize.x = affRect.width / size.x;
    affCaseSize.y = affRect.height / size.y;
    caseSize = texture.getSize().y;
    for (Uint32 i = 0; i < size.x; i++)
        for (Uint32 j = 0; j < size.y; j++) {
            Vertex vertex[4];
            size_t nb;

            if (tab[i][j].flag)
                nb = 10;
            else if (!tab[i][j].reveal)
                nb = 9;
            else if (tab[i][j].mine)
                nb = 11;
            else
                nb = tab[i][j].nbMineAround;
            vertex[0].position = Vector2f(affCaseSize.x * i + affRect.left, affCaseSize.y * j + affRect.top);
            vertex[1].position = Vector2f(affCaseSize.x * (i + 1) + affRect.left, affCaseSize.y * j + affRect.top);
            vertex[2].position = Vector2f(affCaseSize.x * (i + 1) + affRect.left, affCaseSize.y * (j + 1) + affRect.top);
            vertex[3].position = Vector2f(affCaseSize.x * i + affRect.left, affCaseSize.y * (j + 1) + affRect.top);

            vertex[0].texCoords = Vector2f(nb * caseSize, 0);
            vertex[1].texCoords = Vector2f((nb + 1) * caseSize, 0);
            vertex[2].texCoords = Vector2f((nb + 1) * caseSize, caseSize);
            vertex[3].texCoords = Vector2f(nb * caseSize, caseSize);
            for (size_t n = 0; n < 4; n++)
                vertexArray.append(vertex[n]);
        }
    states.transform *= getTransform();
    states.texture = &texture;
    target.draw(vertexArray, states);
}

void World::generate()
{
    vector<Vector2u> noRevealCase;

    for (Uint32 i = 0; i < size.x; i++)
        for (Uint32 j = 0; j < size.y; j++)
            if (!tab[i][j].reveal)
                noRevealCase.push_back(Vector2u(i, j));
    for (size_t i = 0; i < nbMine && noRevealCase.size(); i++) {
        size_t npa = rand() % noRevealCase.size();

        tab[noRevealCase[npa].x][noRevealCase[npa].y].mine = true;
        noRevealCase.erase(noRevealCase.begin() + npa);
    }
    for (Uint32 i = 0; i < size.x; i++)
        for (Uint32 j = 0; j < size.y; j++)
            if (tab[i][j].mine)
                setNbMineAround(Vector2u(i, j));
}

void World::setNbMineAround(const Vector2u &pos)
{
    Int8 mini = pos.x == 0 ? 0 : -1;
    Int8 minj = pos.y == 0 ? 0 : -1;
    Int8 maxi = pos.x == size.x - 1 ? 0 : 1;
    Int8 maxj = pos.y == size.y - 1 ? 0 : 1;

    for (Int8 i = mini; i <= maxi; i++)
        for (Int8 j = minj; j <= maxj; j++)
            tab[pos.x + i][pos.y + j].nbMineAround++;
}

void World::spreadReveal(const Vector2u &pos)
{
    tab[pos.x][pos.y].reveal = true;
    if (!tab[pos.x][pos.y].nbMineAround) {
        Int8 mini = pos.x == 0 ? 0 : -1;
        Int8 minj = pos.y == 0 ? 0 : -1;
        Int8 maxi = pos.x == size.x - 1 ? 0 : 1;
        Int8 maxj = pos.y == size.y - 1 ? 0 : 1;

        for (Int8 i = mini; i <= maxi; i++)
            for (Int8 j = minj; j <= maxj; j++)
                if (!tab[pos.x + i][pos.y + j].reveal)
                    spreadReveal(Vector2u(pos.x + i, pos.y + j));
    }
}
