#include <SFML/Graphics.hpp>

#include "game/World.hpp"
#include "environment/Island.hpp"

using namespace sf;

int main()
{
    RenderWindow app(VideoMode(800, 600, 32), "SFML Graphics");
    Clock clock;

    // Create the ground
    Image groundImage;
    if(!groundImage.LoadFromFile("media/textures/Ground.png"))
    {
        // Error
        return EXIT_FAILURE;
    }
    Sprite groundSprite;
    groundSprite.SetImage(groundImage);
    Island island(400, 550, 800, 122, &groundSprite);

    // Create the player
    Image playerImage;
    if(!playerImage.LoadFromFile("media/textures/CharacterBoy.png"))
        return EXIT_FAILURE;
    Sprite playerSprite;
    playerSprite.SetImage(playerImage);
    Player player(100, 470, 68, 85, 500, &playerSprite);

    // Create the world
    World world;
    world.setPlayer(&player);
    world.addLevelObject(&island);

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

    return EXIT_SUCCESS;
}
