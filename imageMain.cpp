#include <SFML/Graphics.hpp>
using namespace sf;

#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

#include "util/AnimatedSprite.hpp"

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
    AnimatedSprite sprite(image);

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
        }

        clock.restart();

        // Clear the screen (fill it with black color)
        app.clear(Color(200, 0, 0));

        //sprite.update(&clock);
        sprite.draw(&app);

        // Always display the window last, after all updates.
        app.display();
    }

    return EXIT_SUCCESS;
}
