#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "Tello_UI.h"

#include <chrono>
#include <thread>

int main() {
    sf::RenderWindow window(sf::VideoMode(640*2, 480+240), "Tello Drone Controller");

    Tello::UI application = Tello::UI(window);

    application.Run();

    return 0;
}

