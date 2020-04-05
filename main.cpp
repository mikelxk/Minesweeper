#include "TextureManager.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
using sf::Sprite;
using sf::Texture;
using sf::Vector2f;
using sf::Vector2i;
int main()
{
    //load window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Minesweeper");
    //load texture
    array<string, 20> textureName{"debug", "digits", "face_happy", "face_lose", "face_win", "flag", "mine", "number_1", "number_2", "number_3", "number_4", "number_5", "number_6", "number_7", "number_8", "test_1", "test_2", "test_3", "tile_hidden", "tile_revealed"};
    for (auto s : textureName) {
        TextureManager::loadTexture(s);
    }
    //load board from file
    array<array<bool, 25>, 16> board{};
    auto loadBrd = [&board](int brdNum) { ifstream brd("boards/testboard" + to_string(brdNum) + ".brd"); 
    for(auto &j:board){
        for(auto &i:j){
            char tmp;
            brd >> tmp;
            tmp == '0' ? i = false:i = true;
        }
    } };
    //randomize initialze board
    auto randMap = [&board](int mineNum = 40) {
        array<bool, 400> origin{};
        for (int i = 0; i < mineNum; ++i) {
            origin[i] = true;
        }
        random_shuffle(origin.begin(), origin.end());
        for (int k = 0; k < 400; ++k) {
            int j = k / 25;
            int i = k % 25;
            board[j][i] = origin[k];
        }
    };
    randMap();
    while (window.isOpen()) {
        sf::Event event;
        Vector2i mousePos = sf::Mouse::getPosition(window);
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        //draw background
        auto tileRevealed = Sprite(TextureManager::texture["tile_revealed"]);
        for (unsigned j = 0; j < 512; j += 32) {
            for (unsigned i = 0; i < window.getSize().x; i += 32) {
                tileRevealed.setPosition(i, j);
                window.draw(tileRevealed);
            }
        }
        //draw non-variant buttons
        auto faceHappy = Sprite(TextureManager::texture["face_happy"]);
        auto test1 = Sprite(TextureManager::texture["test_1"]);
        auto test2 = Sprite(TextureManager::texture["test_2"]);
        auto test3 = Sprite(TextureManager::texture["test_3"]);
        auto debug = Sprite(TextureManager::texture["debug"]);
        int botPosX = 560; //position of test 1 botton
        faceHappy.setPosition(400 - 32, 512);
        debug.setPosition(botPosX - 64, 512);
        test1.setPosition(botPosX, 512);
        test2.setPosition(botPosX + 64, 512);
        test3.setPosition(botPosX + 64 + 64, 512);
        window.draw(faceHappy);
        window.draw(debug);
        window.draw(test1);
        window.draw(test2);
        window.draw(test3);
        //mouse operation on bottons
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            auto checkBot = [&](Sprite sp, int num, bool test = true) {
                auto spBound = sp.getGlobalBounds();
                if (spBound.contains(Vector2f(mousePos))) {
                    if (test) {
                        loadBrd(num);
                    }
                    else {
                        randMap();
                    }
                }
            };
            checkBot(test1, 1);
            checkBot(test2, 2);
            checkBot(test3, 3);
            checkBot(faceHappy, 1, false);
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
