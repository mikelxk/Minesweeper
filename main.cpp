#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using sf::Sprite;
using sf::Texture;
using sf::Vector2f;
int main()
{
    //load window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Minesweeper");
    //load texture
    Texture debugText, digitText, faceHappyText, faceLoseText, faceWinText, flagText, mineText, num1Text, num2Text, num3Text, num4Text, num5Text, num6Text, num7Text, num8Text, test1Text, test2Text, test3Text, tileHideText, tileRevealText;
    debugText.loadFromFile("images/debug.png");
    digitText.loadFromFile("images/digits.png");
    faceHappyText.loadFromFile("images/face_happy.png");
    faceLoseText.loadFromFile("images/face_happy.png");
    faceWinText.loadFromFile("images/face_win.png");
    flagText.loadFromFile("images/flag.png");
    mineText.loadFromFile("images/mine.png");
    num1Text.loadFromFile("images/number_1.png");
    num2Text.loadFromFile("images/number_2.png");
    num3Text.loadFromFile("images/number_3.png");
    num4Text.loadFromFile("images/number_4.png");
    num5Text.loadFromFile("images/number_5.png");
    num6Text.loadFromFile("images/number_6.png");
    num7Text.loadFromFile("images/number_7.png");
    num8Text.loadFromFile("images/number_8.png");
    test1Text.loadFromFile("images/test_1.png");
    test2Text.loadFromFile("images/test_2.png");
    test3Text.loadFromFile("images/test_3.png");
    tileHideText.loadFromFile("images/tile_hidden.png");
    tileRevealText.loadFromFile("images/tile_revealed.png");
    //create sprite
    auto debug = Sprite(debugText);
    auto digit = Sprite(digitText);
    auto faceHappy = Sprite(faceHappyText);
    auto faceLose = Sprite(faceLoseText);
    auto faceWin = Sprite(faceWinText);
    auto flag = Sprite(flagText);
    auto mine = Sprite(mineText);
    auto num1 = Sprite(num1Text);
    auto num2 = Sprite(num2Text);
    auto num3 = Sprite(num3Text);
    auto num4 = Sprite(num4Text);
    auto num5 = Sprite(num5Text);
    auto num6 = Sprite(num6Text);
    auto num7 = Sprite(num7Text);
    auto num8 = Sprite(num8Text);
    auto test1 = Sprite(test1Text);
    auto test2 = Sprite(test2Text);
    auto test3 = Sprite(test3Text);
    auto tileHide = Sprite(tileHideText);
    auto tileReveal = Sprite(tileRevealText);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        window.draw(faceHappy);
        window.display();
    }
    return 0;
}
