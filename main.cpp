#include "TextureManager.h"
#include <SFML/Graphics.hpp>
#include <array>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
using sf::Sprite;
using sf::Texture;
using sf::Vector2f;
int main()
{
    //load window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Minesweeper");
    //load texture
    array<string, 20> textureName{"debug", "digits", "face_happy", "face_lose", "face_win", "flag", "mine", "number_1", "number_2", "number_3", "number_4", "number_5", "number_6", "number_7", "number_8", "test_1", "test_2", "test_3", "tile_hidden", "tile_revealed"};
    for (auto s : textureName) {
        TextureManager::loadTexture(s);
    }
    //board for
    array<array<bool, 25>, 16> board{};
    auto loadBrd = [&board](int brdNum) { ifstream brd("boards/testboard" + to_string(brdNum) + ".brd"); 
    for(auto &j:board){
        for(auto &i:j){
            char tmp;
            brd >> tmp;
            tmp == '0' ? i = false:i = true;
        }
    } };
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        //draw background
        for (unsigned j = 0; j < 512; j += 32) {
            for (unsigned i = 0; i < window.getSize().x; i += 32) {
                auto tmp = Sprite(TextureManager::texture["tile_revealed"]);
                tmp.setPosition(i, j);
                window.draw(tmp);
            }
        }
        //draw mine
        for (unsigned j = 0; j < 16; ++j) {
            for (unsigned i = 0; i < 25; ++i) {
                if (board[j][i]) {
                    auto mine = Sprite(TextureManager::texture["mine"]);
                    mine.setPosition(i * 32, j * 32);
                    window.draw(mine);
                }
            }
        }
        window.display();
    }
    TextureManager::texture.clear();
    return 0;
}
