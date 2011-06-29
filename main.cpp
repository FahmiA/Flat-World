#include <SFML/Graphics.hpp>

#include "game/World.hpp"
#include "environment/Island.hpp"

using namespace sf;

int main()
{
    RenderWindow app(VideoMode(800, 600, 32), "SFML Graphics");
    Clock clock;

    Image groundBlockImage;
    if(!groundBlockImage.LoadFromFile("media/textures/DirtBlock.png"))
    {
        // Error
        return EXIT_FAILURE;
    }

    Sprite groundBlock;
    groundBlock.SetImage(groundBlockImage);
    Island island(400, 550, 770, 100, &groundBlock);

    World world;
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
