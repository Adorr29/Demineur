/*
** EPITECH PROJECT, 2019
** Demineur
** File description:
** Main.cpp
*/

#include <iostream>
#include <regex>
#include <SFML/Graphics.hpp>
#include "World.hpp"

using namespace std;
using namespace sf;

#define IS_NUMBER(str) regex_match(str, regex(R"(^[1-9]\d*$)"))

void game(RenderWindow &window, World &world)
{
    while (window.isOpen()) {
        for (Event event; window.pollEvent(event);) {
            if (event.type == Event::Closed)
                window.close();
            else if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left)
                    world.setReveal(Vector2i(event.mouseButton.x, event.mouseButton.y));
                else if (event.mouseButton.button == Mouse::Right)
                    world.setFlag(Vector2i(event.mouseButton.x, event.mouseButton.y));
            }
        }
        window.clear();
        window.draw(world);
        window.display();
    }
}

void menu(const Vector2u &size, const size_t &nbMine)
{
    World world(size, nbMine);
    RenderWindow window(VideoMode(size.x * 50, size.y * 50), "Demineur");

    world.affRect = IntRect(0, 0, window.getSize().x, window.getSize().y);
    window.setFramerateLimit(30);
    game(window, world);
}

static void help(const string &exe)
{
    cout << "Usage: " << exe << " [SIZEX] [SIZEY] [NBMINE]" << endl << endl;
    cout << "SIZEX:\twidth of tab" << endl;
    cout << "SIZEY:\theight of tab" << endl;
    cout << "NBMINE:\thnumber of mine on tab" << endl;
}

int main(int argc, char **argv)
{
    if (argc == 1)
        menu(Vector2u(10, 10), 10);
    else if (argc == 2 && IS_NUMBER(argv[1]))
        menu(Vector2u(atoi(argv[1]), atoi(argv[1])), atoi(argv[1]));
    else if (argc == 3 && IS_NUMBER(argv[1]) && IS_NUMBER(argv[2]))
        menu(Vector2u(atoi(argv[1]), atoi(argv[2])), atoi(argv[1]) * atoi(argv[2]) / 10);
    else if (argc == 4 && IS_NUMBER(argv[1]) && IS_NUMBER(argv[2]) && IS_NUMBER(argv[3]) && atoi(argv[3]) <= atoi(argv[1]) * atoi(argv[2]))
        menu(Vector2u(atoi(argv[1]), atoi(argv[2])), atoi(argv[3]));
    else if (argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")))
        help(string(argv[0]));
    else {
        cout << "Usage: " << argv[0] << " [SIZEX] [SIZEY] [NBMINE]" << endl;
        cout << "Try '" << argv[0] << " --help' for more information." << endl;
    }
    return 0;
}
