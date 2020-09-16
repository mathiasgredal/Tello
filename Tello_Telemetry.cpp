#include "Tello_Telemetry.h"

void extract_segment_data(std::string segment, std::string key, float& target)
{
    if (segment.find(key) == 0) {
        target = static_cast<float>(atof(segment.substr(key.length()).c_str()));
    }
}

Tello::Telemetry::Telemetry()
{
}

Tello::Telemetry::Telemetry(std::string message)
{
    std::stringstream input(message);
    std::string data_segment;

    while (std::getline(input, data_segment, ';')) {
        extract_segment_data(data_segment, "pitch:", pitch);
        extract_segment_data(data_segment, "yaw:", yaw);
        extract_segment_data(data_segment, "roll:", roll);

        extract_segment_data(data_segment, "vgx:", velocity.x);
        extract_segment_data(data_segment, "vgy:", velocity.y);
        extract_segment_data(data_segment, "vgz:", velocity.z);

        extract_segment_data(data_segment, "agx:", acceleration.x);
        extract_segment_data(data_segment, "agy:", acceleration.y);
        extract_segment_data(data_segment, "agz:", acceleration.z);

        extract_segment_data(data_segment, "h:", height);

        extract_segment_data(data_segment, "time:", flight_time);
        extract_segment_data(data_segment, "tof:", flight_distance);

        extract_segment_data(data_segment, "templ:", lowest_temperature);
        extract_segment_data(data_segment, "temph:", highest_temperature);

        extract_segment_data(data_segment, "bat:", batterypercentage);
        extract_segment_data(data_segment, "baro:", barometer);
    }
}

bool Tello::Telemetry::CheckIfTelemetry(std::string message)
{
    return message.find("pitch:") != std::string::npos
        && message.find("roll:") != std::string::npos
        && message.find("yaw:") != std::string::npos;
}

void Tello::Telemetry::Print()
{
    std::cout << "telemetry printing is not implemented" << std::endl;
}
