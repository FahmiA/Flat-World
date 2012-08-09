#include "game/World.hpp"
#include "environment/Island.hpp"

#include "scene/SteadyCamera.hpp"
#include "scene/ManualCamera.hpp"

#include "actors/Player.hpp"
#include "actors/Sheep.hpp"

#include "gameIO/ContentManager.hpp"
#include "gameIO/LevelLoader.hpp"
#include "gameIO/LevelBuilderStd.hpp"

#include <SFML/Graphics.hpp>
using namespace sf;

#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

#define MAX_FPS 60

int main()
{
    // Initialize random seed
    srand(time(NULL));

    RenderWindow app(VideoMode(800, 600, 32), "SFML Graphics");
    app.setFramerateLimit(MAX_FPS);
    Clock clock;

    ContentManager content;

    // Create the world
    World *world = new World();

    // Load the level
    LevelBuilderStd levelBuilder(world, &content);
    LevelLoader levelLoader(&levelBuilder, &content);

    string xmlPath = "configuration\\levels\\level_1.xml";
    bool worldLoaded = levelLoader.loadWorld(xmlPath);

    if(!worldLoaded)
        return EXIT_FAILURE;

    // Create the camera
    View *view = new View();
    Camera *camera = new SteadyCamera(view);
    world->setCamera(camera);

    // Create the hud

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

            // Take screenshot
            if (event.key.code == Keyboard::F1)
            {
                Image screen = app.capture();
                screen.saveToFile("screenshot.jpg");
            }

            if (event.key.code == Keyboard::Num1)
            {
                // Switch to steady camera
                cout << "Switching to Steady Camera" << endl;
                delete camera;
                camera = new SteadyCamera(view);
                world->setCamera(camera);
            }else if(event.key.code == Keyboard::Num2){
                // Switch to debug manual camera
                cout << "Switching to Manual Camera" << endl;
                delete camera;
                camera = new ManualCamera(view);
                world->setCamera(camera);
            }
        }

        world->update(&clock, &app);
        clock.restart();

        // Clear the screen (fill it with black color)
        app.clear(Color(200, 0, 0));

        world->draw(&app);

        // Always display the window last, after all updates.
        app.display();
    }

    delete camera;
    delete view;
    delete world;

    return EXIT_SUCCESS;
}
