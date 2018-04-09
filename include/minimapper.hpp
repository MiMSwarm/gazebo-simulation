/**
 *
 * MiniMapper Plugin, minimapper.hpp
 * @brief Declares the plugin to control the MiniMapper Bot.
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

    /** @brief Controls the MiniMapper. */
    class MiniMapperPlugin : public ModelPlugin {

    public:

        /** @brief Constructor */
        MiniMapper() {}

        /**
         *  @brief Called by Gazebo when the model is loaded into the
         *  simulation.
         *  @param[in] _model A pointer to the model attached to this plugin.
         *  @param[in] _sdf A pointer to the plugin's SDF element.
         */
        virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);

        /**
         * @brief Prints the intensity read by the receiver.
         *
         */

    };

    GZ_REGISTER_MODEL_PLUGIN(MiniMapperPlugin)

}
