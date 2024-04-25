#include <SFML/Graphics.hpp>
#include "complexPlane.h"
#include <iostream>

int main()
{
    VideoMode desktopMode = VideoMode::getDesktopMode();
    unsigned int screenWidth = desktopMode.width /2;
    unsigned int screenHeight =desktopMode.height / 2;

    //Construct the RenderWindwo
    RenderWindow window(VideoMode(screenWidth, screenHeight), "Complex Plane Visualization");

    //construct the ComplexPlane
    ComplexPlane complexPlane(screenWidth, screenHeight);

    // construct text objects
    Font font;
    font.loadFromFile("KOMIKAP_.ttf");// need to get text file //
    Text text("", font, 16);
    text.setFillColor(Color::Green);

    //Loop
    while (window.isOpen())
    {
        Event event;
        while(window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            else if (event.type == Event::MouseButtonPressed)
            {
                if(event.mouseButton.button == Mouse::Right)
                {
                    complexPlane.zoomOut();
                    complexPlane.setCenter(Vector2i(event.mouseButton.x, event.mouseButton.y));
                }
                else if (event.mouseButton.button == sf::Mouse::Left)
                {
                    complexPlane.zoomIn();
                    complexPlane.setCenter(Vector2i(event.mouseButton.x, event.mouseButton.y));
                }
            }
            else if (event.type == Event::MouseMoved)
            {
                complexPlane.setMouseLocation(Vector2i(event.mouseMove.x, event.mouseMove.y));
            }
        }
        //check if escape key was pressed
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        //Update scene
        complexPlane.updateRender();
        complexPlane.loadText(text);

        //draw scene
        window.clear();
        window.draw(complexPlane);
        window.draw(text);
        window.display();
    }
    return 0;
}