#include "Tello_UI.h"

Tello::UI::UI(sf::RenderWindow& _window)
{
    window = &_window;

    window->setFramerateLimit(60);
    ImGui::SFML::Init(*window);

    // Perhaps make some kind if connect dialog where ip and port can be selected, and when we click connect then call this function
    udp_server.SDK_StartServer();


    // This feels unsafe, perhaps there is a better way?
    terminal_state.udp_server = &udp_server;

    terminal = new ImTerm::terminal<Tello_Terminal>(terminal_state);
}

Tello::UI::~UI()
{
    delete terminal;
    udp_server.SDK_StopServer();
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


    auto win_size = window->getSize();
    ImVec2 term_size = { (float)win_size.y, win_size.y*0.5f};
    ImVec2 term_position = {0, win_size.y-term_size.y};

    ImGui::SetNextWindowSize(term_size);
    ImGui::SetNextWindowPos(term_position);
    terminal->set_flags(ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    terminal->set_level_list_text("Generel", "FFMPEG", "UDP", "stdout", "", "", "");
    terminal->show();
}

void Tello::UI::HandleEvents(sf::Event event)
{

}
