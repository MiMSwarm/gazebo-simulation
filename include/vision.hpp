/**
 *  Plugin to control the sensor movement and data.
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <list>
#include <string>
#include <vector>

#include <gazebo/gazebo.hh>
#include <gazebo/common/common.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/sensors/sensors.hh>


namespace gazebo {
    class SonarPlugin : public ModelPlugin {

      public:

        SonarPlugin() : ranges(
          2620, std::numeric_limits<double>::infinity()) {}

        // Called by Gazebo when the model is loaded into the simulation.
        virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);

        // Called each time the neck joint updates.
        void OnJointUpdate();

        // Write the ranges and positions to a file.
        void WritePositionsRanges();

        // Returns a copy of the currently recorded ranges.
        std::vector<double> GetSonarRanges() const;

      private:

        // For joint control.
        double velocity;
        double upper_limit;
        double lower_limit;

        common::PID pid;
        physics::JointPtr joint;
        physics::ModelPtr model;
        physics::JointControllerPtr jctrl;
        event::ConnectionPtr connect;

        // For sensor monitoring.
        std::ofstream ranges_f;
        std::vector<double> ranges;
        sensors::RaySensorPtr sensor;

    };

    GZ_REGISTER_MODEL_PLUGIN(SonarPlugin)
}
