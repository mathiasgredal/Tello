#ifndef TELLO_TELEMETRY_H
#define TELLO_TELEMETRY_H

#include <iostream>
#include <sstream>
#include <string>

#include <SFML/System/Vector3.hpp>

namespace Tello {
class Telemetry {
public:
    Telemetry();
    Telemetry(std::string message);
    static bool CheckIfTelemetry(std::string message);
    void Print();

    float pitch = 0;
    float roll = 0;
    float yaw = 0;
    sf::Vector3f velocity = { 0, 0, 0 };
    sf::Vector3f acceleration = { 0, 0, 0 };
    float height = 0;
    float flight_time = 0;
    float flight_distance = 0;

    float lowest_temperature = 0;
    float highest_temperature = 0;
    float batterypercentage = 0;
    float barometer = 0;
};

}

#endif // TELLO_TELEMETRY_H
