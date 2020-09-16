#include "Tello_UI.h"

int main()
{
#ifdef __APPLE__
    sf::RenderWindow window(sf::VideoMode(640 * 3.5, 480 * 3), "Tello Drone Controller");
#else
    sf::RenderWindow window(sf::VideoMode(640*2, 480*2), "Tello Drone Controller");
#endif

    Tello::UI application = Tello::UI(window);

    application.Run();

    return 0;
}
