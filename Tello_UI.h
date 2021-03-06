#ifndef TELLO_UI_H
#define TELLO_UI_H

#include <chrono>
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "imgui-SFML.h"
#include "imgui.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <tinyfiledialogs.h>

#include "Tello_Telemetry.h"
#include "Tello_Terminal.h"
#include "Tello_UDP.h"
#include "Tello_Video.h"

namespace Tello {

struct RCData {
    float x;
    float y;
    float z;
    float yaw;
};

class UI {
public:
    UI(sf::RenderWindow& _window);
    ~UI();

    // Initializes main loop
    void Run();
    void Stop();

    void Start_Video();

private:
    // Drawing
    void Draw_SFML();
    void Draw_ImGui();

    // Handle Events for eg. controlling drone
    void HandleEvents(sf::Event event);
    void GetRCInput();
    RCData rc_control_input = { 0, 0, 0, 0 };

    sf::Clock deltaClock;
    sf::RenderWindow* window;

    // Terminal
    Terminal_State terminal_state;
    ImTerm::terminal<Tello_Terminal>* terminal;

    // UDP Coms
    Tello::UDP* udp_server = nullptr;
    std::string udp_ip_address = std::string("192.168.10.1", 1024);
    int udp_send_port = 8889;
    int udp_listen_port = 8890;
    bool udp_connected = false;
    void HandleUnqueuedUDPData(std::string message);
    Tello::Telemetry telemetry;

    // Video
    Tello::Video* video_server = nullptr;
    sf::Sprite video_sprite;
    sf::Texture video_texture;
    std::string video_url = std::string("udp://192.168.10.1:11111", 1024);
    std::string video_save_location = std::string("", 1024);
    bool video_connected = false;
};

}

#endif // TELLO_UI_H
