/**
 *
 * Vision Plugin, vision.hpp
 * @brief Declares the plugin to control the Vision module.
 * @author Abhijit J Theophilus
 *
 */

#pragma once

#include <algorithm>
#include <fstream>
#include <limits>
#include <string>
#include <vector>

#include <gazebo/gazebo.hh>
#include <gazebo/common/common.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/sensors/sensors.hh>


/** @brief The Gazebo Namespace */
namespace gazebo {

    /** @brief Controls the SONAR module of the MiM. */
    class VisionPlugin : public ModelPlugin {

    public:

        /** @brief Constructor */
        VisionPlugin() : ranges(
          2620, std::numeric_limits<double>::infinity()) {}

        /**
         *  @brief Called by Gazebo when the model is loaded into the
         *  simulation.
         *  @param[in] _model A pointer to the model attached to this plugin.
         *  @param[in] _sdf A pointer to the plugin's SDF element.
         */
        virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);

        /** @brief Called each time the neck joint updates. */
        void OnJointUpdate();

        /** @brief Returns a copy of the currently recorded ranges. */
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

    GZ_REGISTER_MODEL_PLUGIN(VisionPlugin)

}
