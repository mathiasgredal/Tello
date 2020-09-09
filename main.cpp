#include "Tello_UI.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(640 * 3.5, 480 * 3), "Tello Drone Controller");

    Tello::UI application = Tello::UI(window);

    application.Run();

    return 0;
}
