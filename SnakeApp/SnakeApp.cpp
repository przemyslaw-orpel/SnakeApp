/////////////////////////////////
//Author: S7443 Przemyslaw Orpel 

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>


// Window properties
const int WINDOW_WIDTH = 400;
const int WINDOW_HEIGHT = 300;
const int BLOCK_SIZE = 20;
const int WIDTH = WINDOW_WIDTH / BLOCK_SIZE;
const int HEIGHT = WINDOW_HEIGHT / BLOCK_SIZE;

// Keyboard 
enum Direction { Up, Down, Left, Right };

// Snake segment positions
struct SnakeSegment {
	int x, y;
	SnakeSegment(int x, int y) : x(x), y(y) {}
};

class Snake {
private:
	std::vector<SnakeSegment> segments;
	Direction dir;
public:
	Snake(int x, int y) {
		// Initialize snake position
		segments.push_back(SnakeSegment(x, y));
		// Initialize start direction
		dir = Direction::Right;
	}

	void move() {
		// Assigns segment previous position
		for (int i = segments.size() - 1; i > 0; --i) {
			segments[i].x = segments[i - 1].x;
			segments[i].y = segments[i - 1].y;
		}
		// Snake head new position
		switch (dir) {
		case Direction::Up:
			--segments[0].y;
			break;
		case Direction::Down:
			++segments[0].y;
			break;
		case Direction::Left:
			--segments[0].x;
			break;
		case Direction::Right:
			++segments[0].x;
			break;
		}
	}

	void grow() {
		// Add segment with reference to the last snake segment
		segments.push_back(SnakeSegment(segments.back().x, segments.back().y));
	}

	bool checkCollision() {
		// Body collision
		for (int i = 1; i < segments.size(); ++i) {
			if (segments[i].x == segments[0].x && segments[i].y == segments[0].y) {
				return true;
			}
		}
		// Window collision 
		if (segments[0].x < 0 || segments[0].x >= WIDTH || segments[0].y < 0 || segments[0].y >= HEIGHT) {
			return true;
		}
		return false;
	}

	void setDirection(Direction d) {
		// Check possible moves
		if ((dir == Direction::Up && d != Direction::Down) ||
			(dir == Direction::Down && d != Direction::Up) ||
			(dir == Direction::Left && d != Direction::Right) ||
			(dir == Direction::Right && d != Direction::Left)) {
			dir = d;
		}
	}

	std::vector<SnakeSegment> getSegments() {
		return segments;
	}

};


class Game {
private:
	const int FPS = 10;
	sf::RenderWindow window;
	Snake snake;
	sf::RectangleShape food;
	int foodX, foodY;

public:
	// Constructor with initialize snake and window object
	Game() : snake(WIDTH / 2, HEIGHT / 2), window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Snake Game") {
		// Speed snake - FPS Limit
		window.setFramerateLimit(FPS);
		// Spawn food
		spawnFood();
	}

	void run() {
		while (window.isOpen()) {
			// Check user changed move
			processEvents();
			// Update snake positon and check rules 
			update();
			// Render window  
			render();
		}
	}

private:
	void processEvents() {
		sf::Event event;
		// Pop the event on top of the event queue, if any, and return it.
		while (window.pollEvent(event)) {
			// Windows close button
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			// Chceck user pressed key
			else if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
				case sf::Keyboard::Up:
					snake.setDirection(Direction::Up);
					break;
				case sf::Keyboard::Down:
					snake.setDirection(Direction::Down);
					break;
				case sf::Keyboard::Left:
					snake.setDirection(Direction::Left);
					break;
				case sf::Keyboard::Right:
					snake.setDirection(Direction::Right);
					break;
				}
			}
		}
	}

	void update() {
		// Update snake position
		snake.move();
		// Check collision
		if (snake.checkCollision()) {
			snake = Snake(WIDTH / 2, HEIGHT / 2);
			//window.close();
		}
		// Check snake eaten food
		if (snake.getSegments()[0].x == foodX && snake.getSegments()[0].y == foodY) {
			snake.grow();
			spawnFood();
		}
	}

	void render() {
		window.clear();

		// Render Snake
		for (auto segment : snake.getSegments()) {
			sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE - 1, BLOCK_SIZE - 1));
			block.setFillColor(sf::Color::Green);
			block.setPosition(segment.x * BLOCK_SIZE, segment.y * BLOCK_SIZE);
			window.draw(block);
		}

		// Render Food
		food.setSize(sf::Vector2f(BLOCK_SIZE - 1, BLOCK_SIZE - 1));
		food.setFillColor(sf::Color::Red);
		food.setPosition(foodX * BLOCK_SIZE, foodY * BLOCK_SIZE);
		window.draw(food);

		window.display();
	}

	void spawnFood() {
		foodX = rand() % WIDTH;
		foodY = rand() % HEIGHT;
	}
};

int main() {
	Game game;
	game.run();
	return 0;
}
