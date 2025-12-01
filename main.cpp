#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <optional> 

using namespace std;

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;
const int NUM_BALLS = 100; 

const float SPEED_UP = 1.1f;    
const float SLOW_DOWN = 0.9f;   
const float MAX_SPEED = 15.0f;  
const float MIN_SPEED = 1.0f;   

struct Ball {
    float x, y;
    float vx, vy;
    float radius;
    sf::Color color;

    Ball(float startX, float startY, float r) 
        : x(startX), y(startY), radius(r) {
        vx = (rand() % 5 + 2) * (rand() % 2 == 0 ? 1 : -1);
        vy = (rand() % 5 + 2) * (rand() % 2 == 0 ? 1 : -1);
        color = sf::Color(rand() % 255, rand() % 255, rand() % 255);
    }

    void update() {
        x += vx;
        y += vy;

        if (x - radius < 0) { 
            x = radius; 
            vx *= -SLOW_DOWN; 
            if (abs(vx) < MIN_SPEED) vx = (vx > 0 ? MIN_SPEED : -MIN_SPEED);
        }

        if (x + radius > (float)WINDOW_WIDTH) { 
            x = (float)WINDOW_WIDTH - radius; 
            vx *= -SLOW_DOWN; 
            if (abs(vx) < MIN_SPEED) vx = (vx > 0 ? MIN_SPEED : -MIN_SPEED);
        }

        if (y - radius < 0) { 
            y = radius; 
            vy *= -SPEED_UP; 
            if (abs(vy) > MAX_SPEED) vy = (vy > 0 ? MAX_SPEED : -MAX_SPEED);
        }

        if (y + radius > (float)WINDOW_HEIGHT) { 
            y = (float)WINDOW_HEIGHT - radius; 
            vy *= -SPEED_UP; 
            if (abs(vy) > MAX_SPEED) vy = (vy > 0 ? MAX_SPEED : -MAX_SPEED);
        }
    }

    void draw(sf::RenderWindow& window) {
        sf::CircleShape circle(radius);
        circle.setPosition({x - radius, y - radius}); 
        circle.setFillColor(color);
        window.draw(circle);
    }

    sf::FloatRect getBounds() {
        return sf::FloatRect({x - radius, y - radius}, {radius * 2, radius * 2});
    }
};

struct Rectangle {
    float x, y, w, h;
    
    bool contains(const Ball* ball) {
        return (ball->x >= x && ball->x <= x + w &&
                ball->y >= y && ball->y <= y + h);
    }

    bool intersects(const sf::FloatRect& range) {
        return !(range.position.x > x + w ||
                 range.position.x + range.size.x < x ||
                 range.position.y > y + h ||
                 range.position.y + range.size.y < y);
    }
};

class Quadtree {
    Rectangle boundary;
    int capacity;
    vector<Ball*> balls;
    bool divided;
    
    unique_ptr<Quadtree> northwest;
    unique_ptr<Quadtree> northeast;
    unique_ptr<Quadtree> southwest;
    unique_ptr<Quadtree> southeast;

public:
    Quadtree(Rectangle boundary, int capacity) 
        : boundary(boundary), capacity(capacity), divided(false) {}

    void subdivide() {
        float x = boundary.x;
        float y = boundary.y;
        float w = boundary.w;
        float h = boundary.h;

        northwest = make_unique<Quadtree>(Rectangle{x, y, w/2, h/2}, capacity);
        northeast = make_unique<Quadtree>(Rectangle{x + w/2, y, w/2, h/2}, capacity);
        southwest = make_unique<Quadtree>(Rectangle{x, y + h/2, w/2, h/2}, capacity);
        southeast = make_unique<Quadtree>(Rectangle{x + w/2, y + h/2, w/2, h/2}, capacity);
        
        divided = true;
    }

    bool insert(Ball* ball) {
        if (!boundary.contains(ball)) return false;

        if (balls.size() < capacity) {
            balls.push_back(ball);
            return true;
        }

        if (!divided) subdivide();

        if (northwest->insert(ball)) return true;
        if (northeast->insert(ball)) return true;
        if (southwest->insert(ball)) return true;
        if (southeast->insert(ball)) return true;

        return false;
    }

    void query(sf::FloatRect range, vector<Ball*>& found) {
        if (!boundary.intersects(range)) return;

        for (Ball* b : balls) {
            sf::FloatRect bRect = b->getBounds();
            if (range.position.x < bRect.position.x + bRect.size.x &&
                range.position.x + range.size.x > bRect.position.x &&
                range.position.y < bRect.position.y + bRect.size.y &&
                range.position.y + range.size.y > bRect.position.y) {
                found.push_back(b);
            }
        }

        if (divided) {
            northwest->query(range, found);
            northeast->query(range, found);
            southwest->query(range, found);
            southeast->query(range, found);
        }
    }
};

void resolveCollision(Ball* b1, Ball* b2) {
    float dx = b2->x - b1->x;
    float dy = b2->y - b1->y;
    float distanceSquared = dx*dx + dy*dy;
    float radiusSum = b1->radius + b2->radius;

    if (distanceSquared < radiusSum * radiusSum) {
        float distance = sqrt(distanceSquared);
        float overlap = radiusSum - distance;
        float nx = dx / distance;
        float ny = dy / distance;
        
        b1->x -= nx * overlap * 0.5f;
        b1->y -= ny * overlap * 0.5f;
        b2->x += nx * overlap * 0.5f;
        b2->y += ny * overlap * 0.5f;

        float dvx = b2->vx - b1->vx;
        float dvy = b2->vy - b1->vy;
        float dotProduct = dvx * nx + dvy * ny;

        if (dotProduct > 0) return; 

        float impulse = dotProduct;
        b1->vx += impulse * nx;
        b1->vy += impulse * ny;
        b2->vx -= impulse * nx;
        b2->vy -= impulse * ny;
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    bool useQuadtree = true; 

    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Mode: QUADTREE (Tekan SPASI untuk Ganti)");
    window.setFramerateLimit(60);

    vector<Ball> balls;

    for (int i = 0; i < NUM_BALLS; i++) {
        float r = (rand() % 5) + 5; 
        float startX = rand() % (WINDOW_WIDTH - 20) + 10;
        float startY = rand() % (WINDOW_HEIGHT - 20) + 10;
        balls.emplace_back(startX, startY, r);
    }

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            else if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                if (keyEvent->code == sf::Keyboard::Key::Space) {

                    useQuadtree = !useQuadtree;

                    if (useQuadtree) {
                        window.setTitle("Mode: QUADTREE (Cepat) - Tekan SPASI untuk Ganti");
                    } else {
                        window.setTitle("Mode: BRUTE FORCE (Lambat) - Tekan SPASI untuk Ganti");
                    }
                }
            }
        }

        for (auto& ball : balls) ball.update();

        if (!useQuadtree) {
            for (size_t i = 0; i < balls.size(); i++) {
                for (size_t j = i + 1; j < balls.size(); j++) {
                    resolveCollision(&balls[i], &balls[j]);
                }
            }
        } else {
            Rectangle boundary = {0, 0, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT};
            Quadtree tree(boundary, 4); 

            for (auto& ball : balls) tree.insert(&ball);

            for (auto& ball : balls) {
                vector<Ball*> candidates;
                tree.query(ball.getBounds(), candidates);
                for (auto* other : candidates) {
                    if (&ball != other) resolveCollision(&ball, other);
                }
            }
        }

        window.clear(sf::Color::Black);
        for (auto& ball : balls) ball.draw(window);
        window.display();
    }

    return 0;
}
