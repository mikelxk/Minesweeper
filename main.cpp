#include "util.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <array>
#include <fstream>
#include <functional>
#include <numeric>
#include <random>
#include <stack>
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
    //texture name
    array<string, 20> textureName{"debug", "digits", "face_happy", "face_lose", "face_win", "flag", "mine", "number_1", "number_2", "number_3", "number_4", "number_5", "number_6", "number_7", "number_8", "test_1", "test_2", "test_3", "tile_hidden", "tile_revealed"};
    //bool on switches
    bool debugMode{}, defeated{}, win{};
    //load texture
    for (auto &&s : textureName) {
        TextureManager::loadTexture(s);
    }
    Texture mine = TextureManager::texture["mine"];
    Texture flag = TextureManager::texture["flag"];
    Texture tileHidden = TextureManager::texture["tile_hidden"];
    Texture tileRevealed = TextureManager::texture["tile_revealed"];
    Sprite faceLose = Sprite(TextureManager::texture["face_lose"]);
    //load board from file
    array<array<bool, 25>, 16> board{}, flags{}, fullBoard{}, boardRevealed{};
    // array<array<int, 25>, 16> adjMine{};
    vector<vector<int>> adjMine(16, vector<int>(25));
    for (auto &&row : fullBoard) {
        row.fill(true);
    }
    //lambda to draw given texture with given array
    auto drawGeneric = [&window](Texture &text, auto &toDraw) {
        for (unsigned y = 0; y < toDraw.size(); ++y) {
            for (unsigned x = 0; x < toDraw[y].size(); ++x) {
                if (toDraw[y][x]) {
                    auto spirit = Sprite(text);
                    spirit.setPosition((float)x * 32, (float)y * 32);
                    window.draw(spirit);
                }
            }
        }
    };
    //clear a 2-d bool board
    auto clearBoard = [](auto &board) {
        for_each(board.begin(), board.end(), [](auto &line) { fill(line.begin(), line.end(), false); });
    };
    auto reverseDraw = [&](Texture &text, auto toReverse) {
        for_each(toReverse.begin(), toReverse.end(), [](auto &row) {
            transform(row.begin(), row.end(), row.begin(), logical_not<bool>());
        });
        drawGeneric(text, toReverse);
    };
    //lambda to draw numbers on board
    auto drawMineCnt = [&](int num, int xAxis, int yAxis) {
        auto numSpirit = Sprite(TextureManager::texture["number_" + to_string(num)]);
        numSpirit.setPosition((float)xAxis, (float)yAxis);
        window.draw(numSpirit);
    };
    //lambda to add adjacent counter
    auto addAdj = [&]() {
        auto checkAdd = [&](int yAxis, int xAxis) {
            if (xAxis >= 0 && xAxis < 25 && yAxis >= 0 && yAxis < 16) {
                ++adjMine[yAxis][xAxis];
            }
        };
        for (unsigned y = 0; y < adjMine.size(); ++y) {
            for (unsigned x = 0; x < adjMine[y].size(); ++x) {
                if (board[y][x]) {
                    //make it 9 an unreachable value
                    adjMine[y][x] = 9;
                    checkAdd(y - 1, x - 1);
                    checkAdd(y - 1, x);
                    checkAdd(y - 1, x + 1);
                    checkAdd(y, x - 1);
                    checkAdd(y, x + 1);
                    checkAdd(y + 1, x - 1);
                    checkAdd(y + 1, x);
                    checkAdd(y + 1, x + 1);
                }
            }
        } };
    auto drawAdj = [&]() {
        for (unsigned y = 0; y < adjMine.size(); ++y) {
            for (unsigned x = 0; x < adjMine[y].size(); ++x) {
                if (adjMine[y][x] < 9 && adjMine[y][x] != 0) {
                    drawMineCnt(adjMine[y][x], x * 32, y * 32);
                }
            }
        }
    };
    auto redoBoard = [&]() {
        clearBoard(flags);
        clearBoard(adjMine);
        clearBoard(boardRevealed);
        debugMode = false;
        defeated = false;
        win = false;
        addAdj();
    };
    //lambda to draw 1 number with given y axis
    auto drawNum = [&](Digits digit, int xAis, int yAxis = 512) {
        auto nums = Sprite(TextureManager::texture["digits"]);
        nums.setTextureRect(sf::IntRect(digit * 21, 0, 21, 32));
        nums.setPosition((float)xAis, (float)yAxis);
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
            sd.push(digit);
        }
        while (!sd.empty()) {
            int digit = sd.top();
            sd.pop();
            drawNum((Digits)digit, xAxis);
            xAxis += 21;
        }
    };
    auto loadBrd = [&](int brdNum) {
        ifstream brd("../../boards/testboard" + to_string(brdNum) + ".brd");
        for (auto &&row : board) {
            for (auto &&i : row) {
                char tmp;
                brd >> tmp;
                i = tmp != '0';
            }
        }
        redoBoard();
    };
    //randomize initialze board
    auto randMap = [&](int mineNum = 50) {
        vector<bool> origin(400);
        fill_n(origin.begin(), mineNum, true);
        random_device rd;
        mt19937 g(rd());
        shuffle(origin.begin(), origin.end(), g);
        for (unsigned k = 0; k < origin.size(); ++k) {
            int j = k / 25;
            int i = k % 25;
            board[j][i] = origin[k];
        }
        redoBoard();
    };
    randMap();
    while (window.isOpen()) {
        sf::Event event;
        Vector2i mousePos = sf::Mouse::getPosition(window);
        //lambda to get mine count
        auto getCount = [](auto &cntBoard) {
            return accumulate(cntBoard.begin(), cntBoard.end(), 0, [](int val, auto &row) {
                return val + count(row.begin(), row.end(), true);
            });
        };
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed: {
                window.close();
                break;
            }
            case sf::Event::MouseButtonPressed: {
                int rowNum = mousePos.x / 32;
                int colNum = mousePos.y / 32;
                if (event.mouseButton.button == sf::Mouse::Right && !defeated) {
                    if (rowNum < 25 && rowNum >= 0 && colNum >= 0 && colNum < 16 && !boardRevealed[colNum][rowNum]) {
                        flags[colNum][rowNum] = !flags[colNum][rowNum];
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Left) {
                    //mouse is on those bottons
                    if (mousePos.y >= 512 && mousePos.y <= 512 + 64) {
                        if (mousePos.x >= 400 - 32 && mousePos.x < 432) {
                            randMap();
                            win = false;
                            defeated = false;
                        }
                        else if (mousePos.x >= 560 - 64 && mousePos.x < 560) {
                            debugMode = !debugMode;
                        }
                        else if (mousePos.x >= 560 && mousePos.x < 752) {
                            int testNum = (mousePos.x - 560) / 64 + 1;
                            loadBrd(testNum);
                        }
                    }
                    //mouse is on the board
                    else if (defeated) {
                        break;
                    }
                    else if (mousePos.y < 512 && !flags[colNum][rowNum]) {
                        //the tile revealed is mine
                        if (board[colNum][rowNum]) {
                            defeated = true;
                            debugMode = true;
                            for (unsigned y = 0; y < board.size(); ++y) {
                                for (unsigned x = 0; x < board[y].size(); ++x) {
                                    if (board[y][x])
                                        boardRevealed[y][x] = true;
                                }
                            }
                        }
                        else {
                            function<void(int, int)> revealBoard = [&](int x, int y) {
                                //the tile revealed has number
                                if (adjMine[y][x] <= 8 && adjMine[y][x] > 0) {
                                    boardRevealed[y][x] = true;
                                    return;
                                }
                                else if (adjMine[y][x] > 8 || flags[y][x]) {
                                    return;
                                }
                                else {
                                    boardRevealed[y][x] = true;
                                    auto check = [&](int x, int y) {
                                        if (x >= 0 && x < 25 && y >= 0 && y < 16 && !boardRevealed[y][x] && !flags[y][x]) {
                                            boardRevealed[y][x] = true;
                                            revealBoard(x, y);
                                        }
                                    };
                                    check(x - 1, y - 1);
                                    check(x - 1, y);
                                    check(x - 1, y + 1);
                                    check(x, y - 1);
                                    check(x, y + 1);
                                    check(x + 1, y - 1);
                                    check(x + 1, y);
                                    check(x + 1, y + 1);
                                }
                            };
                            revealBoard(rowNum, colNum);
                        }
                    }
                }
                break;
            }
            default:
                break;
            }
        }
        window.clear(sf::Color::White);
        //draw background
        drawGeneric(tileRevealed, fullBoard);
        //draw non-variant buttons
        auto faceHappy = Sprite(TextureManager::texture["face_happy"]);
        auto test1 = Sprite(TextureManager::texture["test_1"]);
        auto test2 = Sprite(TextureManager::texture["test_2"]);
        auto test3 = Sprite(TextureManager::texture["test_3"]);
        auto debug = Sprite(TextureManager::texture["debug"]);
        auto faceWin = Sprite(TextureManager::texture["face_win"]);
        int botPosX = 560; //position of test 1 botton
        faceHappy.setPosition(400 - 32, 512);
        faceLose.setPosition(400 - 32, 512);
        faceWin.setPosition(400 - 32, 512);
        debug.setPosition(botPosX - 64, 512);
        test1.setPosition(botPosX, 512);
        test2.setPosition(botPosX + 64, 512);
        test3.setPosition(botPosX + 64 + 64, 512);
        unsigned numTileReveal = getCount(boardRevealed);
        if (numTileReveal == 400 - getCount(board) && board == flags) {
            win = true;
        }
        if (defeated) {
            window.draw(faceLose);
        }
        else if (win) {
            window.draw(faceWin);
        }
        else {
            window.draw(faceHappy);
        }
        window.draw(debug);
        window.draw(test1);
        window.draw(test2);
        window.draw(test3);
        //draw mine count
        drawCounter(getCount(board) - getCount(flags));
        //draw mine
        drawGeneric(mine, board);
        //draw numbers
        drawAdj();
        //draw tile
        reverseDraw(tileHidden, boardRevealed);
        //draw flag
        drawGeneric(tileHidden, flags);
        drawGeneric(flag, flags);
        //debug mode
        if (debugMode) {
            drawGeneric(mine, board);
        }
        window.display();
    }
    TextureManager::texture.clear();
    return 0;
}