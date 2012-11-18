#include <SFML/Graphics.hpp>
using namespace sf;

#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
using namespace std;

#include "util/TSprite.hpp"
#include "util/CoordinateUtil.hpp"

#define MAX_FPS 60

int main1()
{
    // Initialize random seed
    srand(time(NULL));

    RenderWindow app(VideoMode(800, 600, 32), "SFML Graphics");
    app.setFramerateLimit(MAX_FPS);
    Clock clock;

    // Load the image
    Image image;
    image.loadFromFile("media/textures/star.png");

    // Create the sprite
    TSprite sprite(image);
    sprite.setOrigin(0.5, 0.5);
    sprite.setPosition(400.0f, 300.0f);

    // Create the target
    Image targetImage;
    targetImage.loadFromFile("media/textures/yCube.png");
    TSprite target(targetImage);
    target.setOrigin(0.5, 0.5);

    CoordinateUtil coordUtil;

    while(app.isOpen())
    {
        // Process events
        Event event;
        while(app.pollEvent(event))
        {
            // Window closed
            if(event.type == Event::Closed)
                app.close();

            // Escape key pressed
            if((event.type == Event::KeyPressed) && (event.key.code == Keyboard::Escape))
                app.close();

            if((event.type == Event::KeyPressed) && (event.key.code == Keyboard::L))
                sprite.lookLeft();

            // Move the target to the mouse
            Vector2i mousePos = Mouse::getPosition(app);
            target.setPosition(mousePos.x, mousePos.y);

            // Set the angle of the source to "land" on the target
            float angleRad = coordUtil.getAngle(sprite.getPosition(), 0, target.getPosition());
            sprite.setRotation(angleRad - M_PI_2);

        }

        clock.restart();

        // Clear the screen (fill it with black color)
        app.clear(Color(200, 0, 0));

        //sprite.update(&clock);
        sprite.draw(&app);
        target.draw(&app);

        // Always display the window last, after all updates.
        app.display();
    }

    return EXIT_SUCCESS;
}
