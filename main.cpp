#include "game/World.hpp"
#include "environment/Island.hpp"

#include "scene/SteadyCamera.hpp"
#include "actors/Player.hpp"

#include <SFML/Graphics.hpp>
using namespace sf;

int main()
{
    RenderWindow app(VideoMode(800, 600, 32), "SFML Graphics");
    Clock clock;

    // Create the ground
    Image groundImage1;
    if(!groundImage1.LoadFromFile("media/textures/Asteroid1.png"))
    {
        // Error
        return EXIT_FAILURE;
    }
    Sprite groundSprite1;
    groundSprite1.SetImage(groundImage1);
    Island island1(200, 400, 235, 235, &groundSprite1);

    Image groundImage2;
    if(!groundImage2.LoadFromFile("media/textures/Asteroid2.png"))
    {
        // Error
        return EXIT_FAILURE;
    }
    Sprite groundSprite2;
    groundSprite2.SetImage(groundImage2);
    Island island2(600, 200, 235, 235, &groundSprite2);

    // Create the player
    Image playerImage;
    if(!playerImage.LoadFromFile("media/textures/CharacterBoy.png"))
        return EXIT_FAILURE;
    Sprite playerSprite;
    playerSprite.SetImage(playerImage);
    Player player(150, 200, 68, 85, 500, &playerSprite);

    // Create the world
    World world;
    world.setPlayer(&player);
    world.addLevelObject(&island1);
    world.addLevelObject(&island2);

    // Create the camera
    Camera *camera = new SteadyCamera();
    world.setCamera(camera);

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
        }

        world.update(&clock, &app);
        clock.Reset();

        // Clear the screen (fill it with black color)
        app.Clear(Color(200, 0, 0));

        world.draw(&app);

        // Always display the window last, after all updates.
        app.Display();
    }

    delete camera;
    return EXIT_SUCCESS;
}
