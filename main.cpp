#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
    RenderWindow app(VideoMode(800, 600, 32), "SFML Graphics");
    Clock clock;

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

            // Handle window resize
            //if(event.Type == Event::Resized)
            //    glViewport(0, 0, event.Size.Width, event.Size.Height);
        }

        // Clear the screen (fill it with black color)
        app.Clear(Color(200, 0, 0));

        // Always display the window last, after all updates.
        app.Display();
    }

    return EXIT_SUCCESS;
}
