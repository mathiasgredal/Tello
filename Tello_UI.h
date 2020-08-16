#ifndef TELLO_UI_H
#define TELLO_UI_H

#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "Tello_Terminal.h"

namespace Tello {


class UI
{
public:
    UI(sf::RenderWindow& _window);
    ~UI();

    // Initializes main loop
    void Run();

private:
    // Drawing
    void Draw_SFML();
    void Draw_ImGui();

    // Handle Events for eg. controlling drone
    void HandleEvents(sf::Event event);

    sf::Clock deltaClock;
    sf::RenderWindow* window;

    Terminal_State terminal_state;
    ImTerm::terminal<Tello_Terminal>* terminal;;


};

}

#endif // TELLO_UI_H
