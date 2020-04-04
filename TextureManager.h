#include <SFML/Graphics.hpp>
#include <unordered_map>
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