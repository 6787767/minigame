#include <SFML/Graphics.hpp>
#include <vector>

// 植物类：以绿色矩形表示
class Plant {
public:
    sf::RectangleShape shape;
    Plant(sf::Vector2f pos) {
        shape.setSize(sf::Vector2f(50, 50));
        shape.setFillColor(sf::Color::Green);
        shape.setPosition(pos);
    }
};

// 僵尸类：以红色矩形表示，并从右向左移动
class Zombie {
public:
    sf::RectangleShape shape;
    float speed;
    Zombie(sf::Vector2f pos, float s = 50.f) : speed(s) {
        shape.setSize(sf::Vector2f(50, 50));
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(pos);
    }
    void update(float dt) {
        shape.move(-speed * dt, 0);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "简易植物大战僵尸");
    window.setFramerateLimit(60);

    std::vector<Plant> plants;
    std::vector<Zombie> zombies;

    // 初始化一株植物和一个僵尸
    plants.push_back(Plant(sf::Vector2f(100, 300)));
    zombies.push_back(Zombie(sf::Vector2f(750, 300), 50.f));

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
                
            // 鼠标左键点击位置添加植物
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f pos(static_cast<float>(event.mouseButton.x),
                                 static_cast<float>(event.mouseButton.y));
                plants.push_back(Plant(pos));
            }
        }

        float dt = clock.restart().asSeconds();

        // 更新所有僵尸的位置
        for (auto &zombie : zombies) {
            zombie.update(dt);
        }

        // 碰撞检测：如果僵尸与植物碰撞，则移除该僵尸
        for (auto it = zombies.begin(); it != zombies.end(); ) {
            bool collided = false;
            for (auto &plant : plants) {
                if (it->shape.getGlobalBounds().intersects(plant.shape.getGlobalBounds())) {
                    collided = true;
                    break;
                }
            }
            if (collided) {
                it = zombies.erase(it);
            } else {
                ++it;
            }
        }
        
        // 每过 5 秒添加一只新的僵尸
        static float spawnTimer = 0.f;
        spawnTimer += dt;
        if (spawnTimer >= 5.f) {
            spawnTimer = 0.f;
            // 僵尸随机生成在窗口较右侧的随机纵坐标处
            float yPos = static_cast<float>(rand() % (window.getSize().y - 50));
            zombies.push_back(Zombie(sf::Vector2f(window.getSize().x - 50, yPos), 50.f));
        }

        window.clear(sf::Color::Black);
        for (auto &plant : plants)
            window.draw(plant.shape);
        for (auto &zombie : zombies)
            window.draw(zombie.shape);
        window.display();
    }

    return 0;
}
