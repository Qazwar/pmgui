#include "../PmguiConfig.h"
#include <iostream>
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "DemoManager.h"


int main()
{
    pmgui::DemoManager demo;
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    demo.initialize("Pmgui Demo", {mode.width, mode.height});

    demo.run();
    return 0;
}

//int main()
//{
//    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
//    // ... create window as usual ...
//    sf::Cursor cursor;
//
//    if (cursor.loadFromSystem(sf::Cursor::NotAllowed))
//        window.setMouseCursor(cursor);
//
//    while(window.isOpen())
//    {
//        // Process events
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            // Close window: exit
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//        // Clear screen
//        window.clear(sf::Color(0, 51, 102));
//        // Update the window
//        window.display();
//    }
//
//    return 0;
//}