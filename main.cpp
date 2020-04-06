#include "TextureManager.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>
using namespace std;
using sf::Sprite;
using sf::Texture;
using sf::Vector2f;
using sf::Vector2i;
enum Digits { zero,
              one,
              two,
              three,
              four,
              five,
              six,
              seven,
              eight,
              nine,
              negative
};
int main()
{
    //load window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Minesweeper");
    //load texture
    array<string, 20> textureName{"debug", "digits", "face_happy", "face_lose", "face_win", "flag", "mine", "number_1", "number_2", "number_3", "number_4", "number_5", "number_6", "number_7", "number_8", "test_1", "test_2", "test_3", "tile_hidden", "tile_revealed"};
    //number of flag
    int flagNum{};
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
    //lambda to draw mine
    auto drawMine = [&]() {
        for (unsigned y = 0; y < 16; ++y) {
            for (unsigned x = 0; x < 25; ++x) {
                if (board[y][x]) {
                    auto mine = Sprite(TextureManager::texture["mine"]);
                    mine.setPosition(x * 32, y * 32);
                    window.draw(mine);
                }
            }
    } };
    //lambda to draw 1 number with given y axis
    auto drawNum = [&](Digits digit, int xAis, int yAxis = 512) {
        auto nums = Sprite(TextureManager::texture["digits"]);
        nums.setTextureRect(sf::IntRect(digit * 21, 0, 21, 32));
        nums.setPosition(xAis, yAxis);
        window.draw(nums);
    };
    //lambda to draw number in minecounter(depends on drawNum)
    auto drawCounter = [&](int mineCnt) {
        int xAxis{};
        stack<int> sd;
        if (mineCnt < 0) {
            mineCnt = -mineCnt;
            drawNum(negative, xAxis);
            xAxis += 21;
        }
        //make sure there's 3 digit available
        for (unsigned i = 0; i < 3; ++i) {
            int digit = mineCnt % 10;
            mineCnt /= 10;
            if (digit) {
                sd.push(digit);
            }
            else {
                sd.push(0);
            }
        }
        while (!sd.empty()) {
            int digit = sd.top();
            sd.pop();
            drawNum((Digits)digit, xAxis);
            xAxis += 21;
        }
    };
    while (window.isOpen()) {
        sf::Event event;
        Vector2i mousePos = sf::Mouse::getPosition(window);
        bool debugMode{};
        //lambda to get mine count
        auto getMineCount = [board]() {
            int cnt{};
            for (auto row : board) {
                for (auto i : row) {
                    if (i)
                        ++cnt;
                }
            }
            return cnt;
        };
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        //draw background
        auto tileRevealed = Sprite(TextureManager::texture["tile_revealed"]);
        for (unsigned y = 0; y < 512; y += 32) {
            for (unsigned x = 0; x < window.getSize().x; x += 32) {
                tileRevealed.setPosition(x, y);
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
        //draw mine count
        drawCounter(getMineCount() - flagNum);
        //mouse operation on bottons
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            auto checkBotton = [&](Sprite sp, int num, bool reset = false, bool debugButton = false) {
                auto spBound = sp.getGlobalBounds();
                if (spBound.contains(Vector2f(mousePos))) {
                    if (!reset && !debugButton) {
                        loadBrd(num);
                    }
                    else if (reset && !debugButton) {
                        randMap();
                    }
                    else if (debugButton) {
                        debugMode = true;
                    }
                }
            };
            checkBotton(test1, 1);
            checkBotton(test2, 2);
            checkBotton(test3, 3);
            checkBotton(faceHappy, 1, true);
            checkBotton(debug, 1, false, true);
        }
        //draw mine
        drawMine();
        //debug mode
        if (debugMode) {
            drawMine();
        }
        window.display();
    }
    TextureManager::texture.clear();
    return 0;
}
