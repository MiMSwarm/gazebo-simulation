/**
 *
 * Speaker Plugin, speaker.cpp
 * @brief Defines the plugin to control the Wireless Transmitter module.
 * @author Abhijit J Theophilus
 *
 */

#include "speaker.hpp"

using namespace gazebo;

// Called when the sensor is loaded.
void SpeakerPlugin::Load(sensors::SensorPtr _sensor, sdf::ElementPtr _sdf)
{
    // Initialize the sensor.
    this->sensor =
        std::dynamic_pointer_cast<sensors::WirelessTransmitter>(_sensor);

    if (!this->sensor) {
        gzerr << "Wireless Transmitter missing.\n";
        return;
    }

    // Connect to a function on update.
    this->connect = this->sensor->ConnectUpdated(
        boost::bind(&SpeakerPlugin::OnSensorUpdate, this));
}


// Called each time the sensor updates.
void SpeakerPlugin::OnSensorUpdate()
{
    double power = this->sensor->Power();
    std::string name = this->sensor->Name();
    std::cout << "Transmitted Power (" << name << "): " << power << std::endl;
}
