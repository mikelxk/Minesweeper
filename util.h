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
struct Tile {
    std::vector<Tile *> adj;
    void link(Tile *rhs)
    {
        //if the linking tile does not exist
        if (find(adj.begin(), adj.end(), rhs) == adj.end()) {
            adj.push_back(rhs);
            rhs->adj.push_back(this);
        }
    }
};
class TileBoard {
public:
    std::array<std::array<Tile, 25>, 16> tileBoard{};
    TileBoard()
    {
        //horizontal link
        auto checkLink = [&](int yAxis, int xAxis, int oriY, int oriX) {
            if(xAxis>=0&&xAxis<25&&yAxis>=0&&yAxis<16) {
                tileBoard[yAxis][xAxis].link(&(tileBoard[oriY][oriX]));
            } };
        for (unsigned y = 0; y < 16; ++y) {
            for (unsigned x = 0; x < 24; ++x) {
                checkLink(y - 1, x - 1, y, x);
                checkLink(y - 1, x, y, x);
                checkLink(y - 1, x + 1, y, x);
                checkLink(y, x - 1, y, x);
                checkLink(y, x + 1, y, x);
                checkLink(y + 1, x - 1, y, x);
                checkLink(y + 1, x, y, x);
                checkLink(y + 1, x + 1, y, x);
            }
        }
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