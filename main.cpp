#include <iostream>

#include "Tello_UI.h"

//#include <chrono>
//#include <thread>
//using namespace std::chrono_literals;

//#include "Tello_Video.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(640*2, 480+240), "Tello Drone Controller");

    Tello::UI application = Tello::UI(window);

    application.Run();

    //Tello::Video video = Tello::Video("udp://192.168.1.242:23000");

    //while(true){
    //    std::this_thread::sleep_for(10ms);
    //}

    return 0;
}

