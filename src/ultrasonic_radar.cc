/**
 *  Plugin to control the sensor movement and data.
 */

#include <string>

#include <ignition/math/Pose3.hh>
#include <gazebo/gazebo.hh>
#include <gazebo/common/common.hh>
#include <gazebo/physics/physics.hh>

namespace gazebo {
    class UltrasonicRadarPlugin : ModelPlugin {
      public:

        /// \brief Constructor
        UltrasonicRadarPlugin() { this->joint_name = "rd_joint"; }

        /// \brief Called by Gazebo when the model is loaded into the
        /// simulation.
        /// \param[in] _model Pointer to model this plugin is attached to.
        /// \param[in] _sdf Pointer to model's sdf element.
        virtual void load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
        {
            // Initialize the model and joint.
            this->model = _model;
            this->radar = _model->GetLink("radar");

            double velocity = _sdf->HasElement("velocity") ?
                _sdf->Get<double>("velocity") : 0;

            // Add a P-Controller to the joint.
            this->pid = common::PID(0.1, 0, 0);
            this->model->GetJointController()->SetVelocityPID(
                this->joint_name, this->pid);
            this->model->GetJointController()->SetVelocityTarget(
                this->joint_name, velocity);

            // Set a callback to change velocity at end of arc.
            _model->GetJoint(this->joint_name)->ConnectJointUpdate(
                boost::bind(&UltrasonicRadarPlugin::ChangeDirection, this));
        }

        void ChangeDirection() {
            ignition::math::Pose3d pose = this->radar->RelativePose();
            std::cout << pose;
        }

      private:
        common::PID pid;
        std::string joint_name;
        physics::LinkPtr radar;
        physics::ModelPtr model;
    };
}