/**
 *  Plugin to control the sensor movement and data.
 */

#include <cmath>
#include <fstream>
#include <string>

#include <ignition/math/Pose3.hh>
#include <gazebo/gazebo.hh>
#include <gazebo/common/common.hh>
#include <gazebo/physics/physics.hh>

namespace gazebo {
    class SonarPlugin : public ModelPlugin {
      public:

        /// \brief Constructor
        SonarPlugin() {
            this->vfile.open("data/velocities.txt");
        }

        /// \brief Called by Gazebo when the model is loaded into the
        /// simulation.
        /// \param[in] _model Pointer to model this plugin is attached to.
        /// \param[in] _sdf Pointer to model's sdf element.
        virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
        {
            // Initialize the model and joint.
            this->model = _model;
            this->joint = _model->GetJoint("sonar::neck_joint");
            this->jctrl = _model->GetJointController();

            // Initialize a few parameters from the SDF.
            this->velocity = _sdf->Get<double>("velocity", 0).first;
            this->upper_limit = this->joint->UpperLimit();
            this->lower_limit = this->joint->LowerLimit();

            std::string joint_name = this->joint->GetScopedName();

            double p = 0;
            double i = 0;
            double d = 0;
            sdf::ElementPtr pid_sdf = _sdf->GetElementImpl("pid");
            if (pid_sdf != nullptr) {
                p = pid_sdf->Get<double>("p", 0).first;
                i = pid_sdf->Get<double>("i", 0).first;
                d = pid_sdf->Get<double>("d", 0).first;
            }

            // Set the PID controller.
            this->pid = common::PID(p, i, d);
            this->jctrl->SetVelocityPID(joint_name, this->pid);
            this->jctrl->SetVelocityTarget(joint_name, this->velocity);

            // Set a callback to change velocity at end of arc.
            this->connect = this->joint->ConnectJointUpdate(
                boost::bind(&SonarPlugin::OnUpdate, this));
        }

        void OnUpdate() {
            vfile << this->joint->GetVelocity(0) << std::endl;

            double current_pos = this->joint->Position(0);
            std::string joint_name = this->joint->GetScopedName();

            // If the position crosses the limit, flip velocity.
            if (current_pos > this->upper_limit)
                this->jctrl->SetVelocityTarget(joint_name, -this->velocity);
            else if (current_pos < this->lower_limit)
                this->jctrl->SetVelocityTarget(joint_name, this->velocity);
        }

      private:
        double velocity;
        double upper_limit;
        double lower_limit;

        std::ofstream vfile;

        common::PID pid;
        physics::JointPtr joint;
        physics::ModelPtr model;
        physics::JointControllerPtr jctrl;
        event::ConnectionPtr connect;
    };

    GZ_REGISTER_MODEL_PLUGIN(SonarPlugin)
}