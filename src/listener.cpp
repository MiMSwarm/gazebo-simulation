/**
 *
 * Listener Plugin, listener.cpp
 * @brief Defines the plugin to control the Wireless Receiver module.
 * @author Abhijit J Theophilus
 *
 */

#include "listener.hpp"

using namespace gazebo;

// Called when the sensor is loaded.
void ListenerPlugin::Load(sensors::SensorPtr _sensor, sdf::ElementPtr _sdf)
{
    // Initialize the sensor.
    this->sensor =
        std::dynamic_pointer_cast<sensors::WirelessReceiver>(_sensor);

    if (!this->sensor) {
        gzerr << "Wireless Receiver missing.\n";
        return;
    }

    // Connect to a function on update.
    this->connect = this->sensor->ConnectUpdated(
        boost::bind(&ListenerPlugin::OnSensorUpdate, this));
}


// Called each time the sensor updates.
void ListenerPlugin::OnSensorUpdate()
{
    double power = this->sensor->Power();
    std::string name = this->sensor->Name();
    std::cout << "Received Power (" << name << "): " << power << std::endl;
}
