/**
 *  Plugin to control the sensor movement and data.
 */

#include <iostream>
#include <ignition/math/Pose3.hh>
#include <gazebo/gazebo.hh>
#include <gazebo/sensors/sensors.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/transport/transport.hh>

namespace gazebo {
    class SonarSensorPlugin : public SensorPlugin {
        public:
        // Called by Gazebo when the model is loaded into the simulation.
        virtual void Load(sensors::SensorPtr _sensor, sdf::ElementPtr _sdf)
        {
            this->sensor =
                std::dynamic_pointer_cast<sensors::SonarSensor>(_sensor);

            // Make sure the parent sensor is valid.
            if (!this->sensor) {
                gzerr << "SonarSensorPlugin requires a SonarSensor.\n";
                return;
            }

            // Initialize the communication module.
            // // this->node = transport::NodePtr(new transport::Node());
            // // node->Init();
            // // this->pub = node->Advertise<msgs::Vector2d>("sonar_ranges");
            // // this->pub->WaitForConnection();

            // Set callback to when sensor updates.
            this->update = this->sensor->ConnectUpdate(
                boost::bind(&SonarSensorPlugin::OnUpdate, this, _1));
        }

        // Called each time the neck joint updates.
        void OnUpdate(msgs::SonarStamped msg)
        {
            ignition::math::Pose3d pose = this->sensor->Pose();
            std::cout << pose.Rot().Yaw() << "\t\t" << msg.sonar().range() <<
                std::endl;
        }

      private:
        sensors::SonarSensorPtr sensor;
        event::ConnectionPtr update;
        transport::NodePtr node;
        transport::PublisherPtr pub;
    };

    GZ_REGISTER_SENSOR_PLUGIN(SonarSensorPlugin)
}