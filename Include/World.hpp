/*
** EPITECH PROJECT, 2019
** Demineur
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
    Int8 checkEnd() const;
    bool setReveal(const Vector2u &pos);
    bool setFlag(const Vector2u &pos);
    bool setAutoReveal(const Vector2u &pos);
    Vector2u convertMousePos(const Vector2i &mousePos) const;

private:
    void draw(RenderTarget &target, RenderStates states) const;
    void generate();
    void setNbMineAround(const Vector2u &pos);
    void revealMine();
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
