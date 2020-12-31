#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
class TextureManager {
public:
    static std::unordered_map<std::string, sf::Texture> texture;
    static void loadTexture(const std::string &textureName)
    {
        std::string filePath = "../../images/" + textureName + ".png";
        texture[textureName].loadFromFile(filePath);
    }
};
std::unordered_map<std::string, sf::Texture> TextureManager::texture;
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