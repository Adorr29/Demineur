/*
** EPITECH PROJECT, 2019
** for_norme
** File description:
** World.hpp
*/

#ifndef WORLD_HPP
#define WORLD_HPP

#include <SFML/Graphics.hpp>

using namespace sf;

class World : public Drawable, public Transformable
{
    struct Tab
    {
        Uint8 nbMineAround;
        bool reveal;
        bool flag;
        bool mine;
    };

public:
    World();
    World(const Uint32 &_size);
    World(const Vector2u &_size);
    World(const Vector2u &_size, const size_t &_nbMine);
    ~World();
    const Vector2u &getSize() const;
    const size_t &getNbMine() const;
    bool setReveal(const Vector2i &pos);
    bool setFlag(const Vector2i &pos);

private:
    void draw(RenderTarget &target, RenderStates states) const;
    void generate();
    void setNbMineAround(const Vector2u &pos);
    void spreadReveal(const Vector2u &pos);

public:
    IntRect affRect;

private:
    Vector2u size;
    Tab **tab;
    size_t nbMine;
    bool first;
};

#endif
