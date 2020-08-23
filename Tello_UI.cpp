#include "Tello_UI.h"

Tello::UI::UI(sf::RenderWindow& _window)
{
    window = &_window;

    window->setFramerateLimit(60);
    ImGui::SFML::Init(*window);

// handle retina screen, we propably shouldn't assume only apple screens have highdpi
#ifdef __APPLE__
    ImGui::GetIO().FontGlobalScale = 2;
#endif

    video_save_location = std::filesystem::current_path();

    // Perhaps make some kind if connect dialog where ip and port can be selected, and when we click connect then call this function
    //udp_server.SDK_StartServer();

    // This feels unsafe, perhaps there is a better way?
    terminal_state.udp_server = nullptr;
    terminal = new ImTerm::terminal<Tello_Terminal>(terminal_state);

    //Start_VideoServer("udp://192.168.10.1:11111");
}

Tello::UI::~UI()
{
    delete terminal;
    delete video_server;
    delete udp_server;
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

        if (terminal_state.ShouldQuit)
            window->close();

        ImGui::SFML::Update(*window, deltaClock.restart());
        Draw_ImGui();

        window->clear();
        if (video_connected && video_server != nullptr)
            video_server->applyBufferToTexture(video_texture);
        Draw_SFML();

        ImGui::SFML::Render(*window);
        window->display();
    }
}

void Tello::UI::Start_Video()
{
    if (video_server != nullptr)
        delete video_server;

    video_server = new Tello::Video(video_url);

    video_connected = true;

    auto video_size = video_server->getVideo_size();
    if (!video_texture.create(video_size.x, video_size.y))
        throw std::runtime_error("ERROR: Unable to initialize a texture");

    video_sprite.setTexture(video_texture);
}

void Tello::UI::Draw_SFML()
{
    window->draw(video_sprite);
}

void Tello::UI::Draw_ImGui()
{
    auto win_size = window->getSize();

    ImVec2 conf_size = { win_size.x * 0.333333f, (float)win_size.y };
    ImVec2 conf_position = { win_size.x - conf_size.x, 0.0f };

    ImGui::SetNextWindowSize(conf_size);
    ImGui::SetNextWindowPos(conf_position);
    ImGui::Begin("Configuration");

    ImGui::Text("VIDEO SERVER:");
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("URL: ");
    ImGui::SameLine();
    ImGui::InputText("##url", video_url.data(), video_url.size());
    ImGui::SameLine();

    if (ImGui::Button(video_connected ? "Disconnect" : "Connect")) {
        try {
            if (!video_connected && video_server == nullptr) {
                Start_Video();
            } else if (video_connected && video_server != nullptr) {
                delete video_server;
                video_server = nullptr;
                video_connected = false;
            }
        } catch (std::exception error) {
            std::cout << "ERROR(initializing video server): " << error.what() << std::endl;
        }
    }

    ImGui::Text("Status: ");
    ImGui::SameLine();

    if (video_server != nullptr)
        ImGui::TextColored({ 0, 255, 0, 255 }, "Connected");
    else
        ImGui::TextColored({ 255, 0, 0, 255 }, "Disconnected");

    ImGui::Button("Capture Image");
    ImGui::SameLine();
    ImGui::Button("Record Video");

    ImGui::Spacing();

    ImGui::Text("Save:");

    ImGui::SameLine();

    ImGui::InputText("##save", video_save_location.data(), video_save_location.size(), ImGuiInputTextFlags_ReadOnly);
    ImGui::SameLine();

    if (ImGui::Button("Set Folder")) {
        auto result = tinyfd_selectFolderDialog("Select folder for saving drone footage", "./");
        if (result != nullptr)
            video_save_location = result;
    }

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text("UDP SERVER:");
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Send: ");
    ImGui::SameLine();
    ImGui::PushItemWidth(225);
    if (ImGui::InputText("##udp_address", udp_ip_address.data(), udp_ip_address.size()) && udp_connected && udp_server != nullptr)
        udp_server->SDK_Address = udp_ip_address;
    ImGui::PopItemWidth();
    ImGui::SameLine();
    ImGui::Text(":");
    ImGui::SameLine();
    ImGui::PushItemWidth(200);
    if (ImGui::InputInt("##send", (int*)&udp_send_port) && udp_connected && udp_server != nullptr)
        udp_server->SDK_SendPort = static_cast<ushort>(udp_send_port);
    ImGui::PopItemWidth();
    ImGui::Spacing();

    ImGui::Text("Listen: ");
    ImGui::SameLine();
    ImGui::PushItemWidth(200);

    ImGui::InputInt("##listen", (int*)&udp_listen_port, 1, 100, udp_connected ? ImGuiInputTextFlags_ReadOnly : 0);
    ImGui::PopItemWidth();
    ImGui::SameLine();

    if (ImGui::Button(udp_connected ? "Disconnected##udp" : "Connect##udp")) {
        try {
            if (!udp_connected && udp_server == nullptr) {
                udp_server = new Tello::UDP(udp_ip_address, udp_send_port, udp_listen_port);
                udp_connected = true;
                terminal_state.udp_server = udp_server;
            } else if (udp_connected && udp_server != nullptr) {
                delete udp_server;
                udp_server = nullptr;
                udp_connected = false;
                terminal_state.udp_server = nullptr;
            }
        } catch (std::exception error) {
            std::cout << "ERROR(initializing udp server): " << error.what() << std::endl;
        }
    }

    if (ImGui::Button("Send test") && udp_connected && udp_server != nullptr) {
        udp_server->SDK_SendRequest("yeeet", 1200, [](UDP_Response res) {
            std::cout << "response" << std::endl;
        });
    }

    ImGui::End();

    ImVec2 term_size = { win_size.x * 0.666667f, win_size.y * 0.5f };
    ImVec2 term_position = { 0, win_size.y - term_size.y };

    ImGui::SetNextWindowSize(term_size);
    ImGui::SetNextWindowPos(term_position);
    terminal->set_flags(ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    terminal->set_level_list_text("Generel", "FFMPEG", "UDP", "stdout", "", "", "");
    terminal->show();
}

void Tello::UI::HandleEvents(sf::Event event)
{
}
