#include "Tello_UI.h"

Tello::UI::UI(sf::RenderWindow& _window)
{
    window = &_window;

    window->setFramerateLimit(20);
    window->setKeyRepeatEnabled(true);
    ImGui::SFML::Init(*window);

// handle retina screen, we propably shouldn't assume only apple screens have highdpi
#ifdef __APPLE__
    ImGui::GetIO().FontGlobalScale = 2;
#endif

    video_save_location = std::filesystem::current_path().string();

    // We should propably use std::optional from c++ 17, since the handle lifetime in a better way with exceptions
    terminal_state.udp_server = nullptr;
    terminal = new ImTerm::terminal<Tello_Terminal>(terminal_state);
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

        GetRCInput();

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

    sf::Vector2f video_size = video_server->getVideo_size();
    std::cout << "Sprite size: " << video_size.x << ", " << video_size.y << std::endl;

    if (!video_texture.create(video_size.x, video_size.y))
        throw std::runtime_error("ERROR: Unable to initialize a texture");

    video_sprite.setTexture(video_texture);
}

void Tello::UI::Draw_SFML()
{
    // We should only draw the video if we know its size
    if (video_connected && video_server != nullptr) {
        // We need to calculate the scale for the sprite, such that it fits half the screen height and 2/3 the screen width
        auto win_size = window->getSize();
        auto sprite_size = video_sprite.getTexture()->getSize();
        sf::Vector2f sprite_scaling = { (float)sprite_size.x / win_size.x, (float)sprite_size.y / win_size.y };

        video_sprite.setScale((win_size.x * 0.666667) / video_texture.getSize().x, (win_size.y * 0.5) / video_texture.getSize().y);

        window->draw(video_sprite);
    }
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
        udp_server->SDK_SendPort = static_cast<uint8_t>(udp_send_port);
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
                udp_server->WriteToTerminal = [this](std::string message) { terminal->add_text("RECIEVED: " + message); };
                udp_server->RecievedTelemetry = [this](std::string message) { ParseTelemetryData(message); };
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

    if (ImGui::TreeNode("UDP Request Queue: ")) {
        ImGui::Columns(1, NULL, false);
        if (udp_connected && udp_server != nullptr) {
            auto queue = udp_server->getSDK_requestQueue();
            size_t i = 0;

            while (queue.size() > 0) {
                std::ostringstream infomessage;
                infomessage << i << ". {message: \"" << queue.front().sentMessage << "\", timeout: " << queue.front().timeout << "}";
                ImGui::Text(infomessage.str().c_str());
                queue.pop();
                i++;
                ImGui::NextColumn();
            }
        }
        ImGui::Columns(1);
        ImGui::TreePop();
    }

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text("Telemetry:");
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Columns(3, nullptr, false);

    ImGui::Text("pitch:%.2f", telemetry.pitch);
    ImGui::NextColumn();
    ImGui::Text("roll:%.2f", telemetry.roll);
    ImGui::NextColumn();
    ImGui::Text("yaw:%.2f", telemetry.yaw);
    ImGui::NextColumn();
    ImGui::Columns(1);

    ImGui::Text("velocity: (%.3f, %.3f, %.3f)", telemetry.velocity.x, telemetry.velocity.y, telemetry.velocity.z);
    ImGui::NextColumn();

    ImGui::Text("acceleration: (%.3f, %.3f, %.3f)", telemetry.acceleration.x, telemetry.acceleration.y, telemetry.acceleration.z);

    //    ImGui::Text("height:%.2f", telemetry.height);
    ImGui::NextColumn();

    ImGui::Columns(3);

    ImGui::Text("height:%.2f", telemetry.height);
    ImGui::NextColumn();
    ImGui::Text("time:%.2f", telemetry.flight_time);
    ImGui::NextColumn();
    ImGui::Text("distance:%.2f", telemetry.flight_distance);
    ImGui::NextColumn();

    ImGui::Columns(1);

    ImGui::Text("RC Input:%.2f, %.2f, %.2f, %.2f", (float)rc_control_input.x, (float)rc_control_input.y, (float)rc_control_input.z, (float)rc_control_input.yaw);

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

// Called every frame
void Tello::UI::GetRCInput()
{
    rc_control_input = { 0, 0, 0, 0 };

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        rc_control_input.y = 100;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        rc_control_input.y = -100;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        rc_control_input.x = 100;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        rc_control_input.x = -100;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        rc_control_input.z = 100;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        rc_control_input.z = -100;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        rc_control_input.yaw = -100;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        rc_control_input.yaw = 100;

    if (sf::Joystick::isConnected(0)) {
        // joystick number 0 is connected
        rc_control_input.x = -sf::Joystick::getAxisPosition(0, sf::Joystick::X);
        rc_control_input.y = -sf::Joystick::getAxisPosition(0, sf::Joystick::Y);

        if (sf::Joystick::isButtonPressed(0, 0))
            rc_control_input.z = 100;
        if (sf::Joystick::isButtonPressed(0, 1))
            rc_control_input.z = -100;

        if (sf::Joystick::isButtonPressed(0, 2))
            rc_control_input.yaw = 100;
        if (sf::Joystick::isButtonPressed(0, 3))
            rc_control_input.yaw = -100;
    }

    if (udp_connected && udp_server != nullptr) {

        std::stringstream rc_command;
        rc_command << "rc " << rc_control_input.x << " " << rc_control_input.y << " " << rc_control_input.z << " " << rc_control_input.yaw;

        udp_server->SDK_Transmit(rc_command.str());
    }
}

void extract_segment_data(std::string segment, std::string key, float& target)
{
    if (segment.find(key) == 0) {
        target = static_cast<float>(atof(segment.substr(key.length()).c_str()));
        //std::cout << segment << ": " << target << std::endl;
    }
}

// Called everytime we have recieved telemetry
void Tello::UI::ParseTelemetryData(std::string message)
{
    std::stringstream input(message);
    std::string data_segment;

    while (std::getline(input, data_segment, ';')) {
        extract_segment_data(data_segment, "pitch:", telemetry.pitch);
        extract_segment_data(data_segment, "yaw:", telemetry.yaw);
        extract_segment_data(data_segment, "roll:", telemetry.roll);

        extract_segment_data(data_segment, "vgx:", telemetry.velocity.x);
        extract_segment_data(data_segment, "vgy:", telemetry.velocity.y);
        extract_segment_data(data_segment, "vgz:", telemetry.velocity.z);

        extract_segment_data(data_segment, "agx:", telemetry.acceleration.x);
        extract_segment_data(data_segment, "agy:", telemetry.acceleration.y);
        extract_segment_data(data_segment, "agz:", telemetry.acceleration.z);

        extract_segment_data(data_segment, "h:", telemetry.height);

        extract_segment_data(data_segment, "time:", telemetry.flight_time);
        extract_segment_data(data_segment, "tof:", telemetry.flight_distance);

        extract_segment_data(data_segment, "templ:", telemetry.lowest_temperature);
        extract_segment_data(data_segment, "temph:", telemetry.highest_temperature);

        extract_segment_data(data_segment, "bat:", telemetry.batterypercentage);
        extract_segment_data(data_segment, "baro:", telemetry.barometer);
    }
}
