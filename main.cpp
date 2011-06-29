#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
    RenderWindow app(VideoMode(800, 600, 32), "SFML Graphics");
    Clock clock;

    Image image;
    if(!image.LoadFromFile("media/textures/Key.png"))
    {
        // Error
        return EXIT_FAILURE;
    }

    Sprite sprite;
    sprite.SetImage(image);
    sprite.SetX(200);
    sprite.SetY(200);
    // Set the center for proper rotation
    sprite.SetCenter(image.GetWidth()/2, image.GetHeight()/2);

    View view(FloatRect(100, 100, 300, 300));
    view.SetCenter(300, 300);
    app.SetView(view);

    while(app.IsOpened())
    {

        // Get the elapsed time since the last frame.
        float elapsedTime = clock.GetElapsedTime();
        clock.Reset();

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

            // Move the sprite
            if (app.GetInput().IsKeyDown(Key::Left))    sprite.Move(-1000 * elapsedTime, 0);
            if (app.GetInput().IsKeyDown(Key::Right))   sprite.Move( 1000 * elapsedTime, 0);
            if (app.GetInput().IsKeyDown(Key::Up))      sprite.Move(0, -1000 * elapsedTime);
            if (app.GetInput().IsKeyDown(Key::Down))    sprite.Move(0,  1000 * elapsedTime);

            // Rotate the sprite
            if (app.GetInput().IsKeyDown(Key::Add))  sprite.Rotate(-500 * elapsedTime);
            if (app.GetInput().IsKeyDown(Key::Subtract))sprite.Rotate(500 * elapsedTime);


            // Handle window resize
            //if(event.Type == Event::Resized)
            //    glViewport(0, 0, event.Size.Width, event.Size.Height);
        }

        // Clear the screen (fill it with black color)
        app.Clear(Color(200, 0, 0));

        // Draw the sprite
        app.Draw(sprite);

        // Always display the window last, after all updates.
        app.Display();
    }

    return EXIT_SUCCESS;
}
