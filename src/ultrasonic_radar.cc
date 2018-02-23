/**
 *  Plugin to control the sensor movement and data.
 */

#include <string>

#include <ignition/math/Pose3.hh>
#include <gazebo/gazebo.hh>
#include <gazebo/common/common.hh>
#include <gazebo/physics/physics.hh>

namespace gazebo {
    class UltrasonicRadarPlugin : public ModelPlugin {
      public:

        /// \brief Constructor
        UltrasonicRadarPlugin() {}

        /// \brief Called by Gazebo when the model is loaded into the
        /// simulation.
        /// \param[in] _model Pointer to model this plugin is attached to.
        /// \param[in] _sdf Pointer to model's sdf element.
        virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
        {
            // Initialize the model and joint.
            this->model = _model;
            this->radar = _model->GetLink("ultrasonic_radar::radar");
            this->rd_joint = _model->GetJoint("ultrasonic_radar::rd_joint");

            this->velocity = _sdf->HasElement("velocity") ?
                _sdf->Get<double>("velocity") : 1;

            // Add a P-Controller to the joint.
            this->pid = common::PID(0.5, 0.4, 0);
            this->model->GetJointController()->SetVelocityPID(
                this->rd_joint->GetScopedName(), this->pid);
            this->model->GetJointController()->SetVelocityTarget(
                this->rd_joint->GetScopedName(), this->velocity);

            // Set a callback to change velocity at end of arc.
            this->connect = event::Events::ConnectWorldUpdateBegin(
                boost::bind(&UltrasonicRadarPlugin::OnUpdate, this, _1));
        }

        void OnUpdate(const common::UpdateInfo & /*_info*/) {
            ignition::math::Pose3d pose = this->radar->RelativePose();
            if ((pose.Rot().Yaw() - this->rd_joint->LowerLimit() < 1e-3)||
                    (pose.Rot().Yaw() - this->rd_joint->UpperLimit() < 1e-3)) {
                this->model->GetJointController()->SetVelocityTarget(
                    this->rd_joint->GetScopedName(), -this->velocity);
                this->velocity = -this->velocity;
            }
        }

      private:
        double velocity;

        common::PID pid;
        physics::LinkPtr radar;
        physics::JointPtr rd_joint;
        physics::ModelPtr model;
        event::ConnectionPtr connect;
    };

    GZ_REGISTER_MODEL_PLUGIN(UltrasonicRadarPlugin)
}