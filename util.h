#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <string>
#include <unordered_map>
#include <vector>
class TextureManager {
public:
    static std::unordered_map<std::string, sf::Texture> texture;
    static void loadTexture(std::string textureName)
    {
        std::string filePath = "images/" + textureName + ".png";
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