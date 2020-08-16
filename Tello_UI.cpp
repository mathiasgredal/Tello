#include "Tello_UI.h"

Tello::UI::UI(sf::RenderWindow& _window)
{
    window = &_window;

    window->setFramerateLimit(60);
    ImGui::SFML::Init(*window);

    terminal = new ImTerm::terminal<Tello_Terminal>(terminal_state);
}

Tello::UI::~UI()
{
    delete terminal;
    ImGui::SFML::Shutdown();
}

void Tello::UI::Run()
{
    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            HandleEvents(event);

            if (event.type == sf::Event::Closed) {
                window->close();
            }
        }

        if(terminal_state.ShouldQuit)
            window->close();

        ImGui::SFML::Update(*window, deltaClock.restart());
        Draw_ImGui();

        window->clear();
        Draw_SFML();

        ImGui::SFML::Render(*window);
        window->display();
    }
}

void Tello::UI::Draw_SFML()
{

}

void Tello::UI::Draw_ImGui()
{
    ImGui::Begin("Hello, world!");
    if(ImGui::Button("Look at this pretty button")){
        terminal->add_text("yeet");
    }
    ImGui::End();

    terminal->show();
}

void Tello::UI::HandleEvents(sf::Event event)
{

}
