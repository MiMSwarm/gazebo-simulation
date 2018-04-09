/**
 *
 * Speaker Plugin, speaker.hpp
 * @brief Declares the plugin to control the Wireless Transmitter module.
 * @author Abhijit J Theophilus
 *
 */

#pragma once

#include <gazebo/gazebo.hh>
#include <gazebo/common/common.hh>
#include <gazebo/sensors/sensors.hh>


/** @brief The Gazebo Namespace */
namespace gazebo {

    /** @brief Controls the transmitter of the MiM. */
    class SpeakerPlugin : public SensorPlugin {

    public:

        /** @brief Constructor */
        SpeakerPlugin() {}

        /**
         *  @brief Called by Gazebo when the sensor is loaded into the
         *  simulation.
         *  @param[in] _sensor A pointer to the sensor attached to this plugin.
         *  @param[in] _sdf A pointer to the plugin's SDF element.
         */
        virtual void Load(sensors::SensorPtr _sensor, sdf::ElementPtr _sdf);

        /** @brief Called each time the sensor updates. */
        void OnSensorUpdate();

    private:

        // For sensor control.
        sensors::WirelessTransmitterPtr sensor;
        event::ConnectionPtr connect;

    };

    GZ_REGISTER_SENSOR_PLUGIN(SpeakerPlugin)

}
