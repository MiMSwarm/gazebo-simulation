/**
 *  Plugin to control the robot movement.
 */

#include <string>
#include <vector>

#include <ignition/math/Vector2.hh>

#include <gazebo/gazebo.hh>
#include <gazebo/common/common.hh>
#include <gazebo/physics/physics.hh>


namespace gazebo {
    class MiniMapperPlugin : public ModelPlugin {
  
      public:

        // Called by Gazebo when the model is loaded into the simulation.
        virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);

        // Write the ranges and positions to a file.
        ignition::math::Vector2d GetPotential();


    };

    GZ_REGISTER_MODEL_PLUGIN(MiniMapperPlugin)
}