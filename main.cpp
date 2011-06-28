#include <SFML/Window.hpp>
//#include <iostream>

using namespace sf;
//using namespace std;

int main()
{
    Window app(VideoMode(800, 600, 32), "SFML Window");
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
        }

        //Get elapsed time since last loop
        float frameRate = 1.0f / = Clock.GetElapsedTime();
        clock.Reset();

        // Always display the window last, after all updates.
        app.Display();
    }

    return EXIT_SUCCESS;
}
