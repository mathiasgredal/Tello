#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "Tello_UI.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(640*2, 480+240), "ImGui + SFML = <3");

    Tello::UI application = Tello::UI(window);

    application.Run();
    return 0;
}

