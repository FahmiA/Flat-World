#include "game/World.hpp"
#include "environment/Island.hpp"

#include "scene/SteadyCamera.hpp"
#include "scene/ManualCamera.hpp"

#include "actors/Player.hpp"
#include "actors/Sheep.hpp"

#include "gameIO/ContentManager.hpp"
#include "gameIO/LevelLoader.hpp"
#include "gameIO/LevelBuilderXML.hpp"

#include <SFML/Graphics.hpp>
using namespace sf;

#include <iostream>
using namespace std;

#define MAX_FPS 60

int main()
{
    RenderWindow app(VideoMode(800, 600, 32), "SFML Graphics");
    app.SetFramerateLimit(MAX_FPS);
    Clock clock;

    ContentManager content;
    // Load the level
    World *world = new World();

    LevelBuilderXML levelBuilder(world, &content);
    LevelLoader levelLoader(&levelBuilder, &content);

    string xmlPath = "configuration\\levels\\level_1.xml";
    bool worldLoaded = levelLoader.loadWorld(xmlPath);

    if(!worldLoaded)
        return EXIT_FAILURE;

    // Create the camera
    View *view = new View();
    Camera *camera = new SteadyCamera(view);
    world->setCamera(camera);

    while(app.IsOpened())
    {
        // Process events
        Event event;
        while(app.GetEvent(event))
        {
            // Window closed
            if(event.Type == Event::Closed)
                app.Close();

            // Escape key pressed
            if((event.Type == Event::KeyPressed) && (event.Key.Code == Key::Escape))
                app.Close();

            // Take screenshot
            if (event.Key.Code == sf::Key::F1)
            {
                Image Screen = app.Capture();
                Screen.SaveToFile("screenshot.jpg");
            }

            if (event.Key.Code == sf::Key::Num1)
            {
                // Switch to steady camera
                cout << "Switching to Steady Camera" << endl;
                delete camera;
                camera = new SteadyCamera(view);
                world->setCamera(camera);
            }else if(event.Key.Code == sf::Key::Num2){
                // Switch to debug manual camera
                cout << "Switching to Manual Camera" << endl;
                delete camera;
                camera = new ManualCamera(view);
                world->setCamera(camera);
            }
        }

        world->update(&clock, &app);
        clock.Reset();

        // Clear the screen (fill it with black color)
        app.Clear(Color(200, 0, 0));

        world->draw(&app);

        // Always display the window last, after all updates.
        app.Display();
    }

    delete camera;
    delete view;
    delete world;
    return EXIT_SUCCESS;
}
