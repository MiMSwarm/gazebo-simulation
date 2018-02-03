#ifndef _VELODYNE_PLUGIN_HH_
#define _VELODYNE_PLUGIN_HH_

#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>

namespace gazebo {
    /// \brief A plugin to control a Velodyne sensor.
    class VelodynePlugin : public ModelPlugin
    {
    public:
        
        /// \brief Constructor
        VelodynePlugin() {}

        /// \brief The load function is called by Gazebo when the plugin is
        /// inserted into simulation
        /// \param[in] _model A pointer to the model that this plugin is
        /// attached to.
        /// \param[in] _sdf A pointer to the plugin's SDF element.
        virtual void Load(physics::ModelPtr _model,
            sdf::ElementPtr _sdf)
        {
            // Safety check
            if (_model->GetJointCount() == 0) {
                std::cerr << "Invalid joint count, Velodyne plugin not "
                    "loaded.\n";
                return;
            }

            // Default to zero velocity
            double velocity = 0;

            // Check that the velocity element exists, then read the value
            if (_sdf->HasElement("velocity"))
                velocity = _sdf->Get<double>("velocity");

            // Store the model parameter for convenience.
            this->model = _model;

            // Get the first joint. We are making an assumption about the model
            // having one joint that is the rotational joint.
            this->joint = _model->GetJoints()[0];

            // Setup a P-controller, with a gain of 1.
            this->pid = common::PID(0.1, 0, 0);

            // Apply the P-controller to the joint.
            this->model->GetJointController()->SetVelocityPID(
                this->joint->GetScopedName(), this->pid);
            
            // Set the joint's target velocity. This target velocity is just
            // for demonstration purposes.
            this->model->GetJointController()->SetVelocityTarget(
                this->joint->GetScopedName(), velocity);

            // Create the node
            this->node = transport::NodePtr(new transport::Node());            
            this->node->Init(this->model->GetWorld()->Name());

            // Create a topic name
            std::string topicName = "vel_cmd";

            // Subscribe to the topic and register a callback.
            this->sub = this->node->Subscribe(topicName,
                &VelodynePlugin::OnMsg, this);
        }

        /// \brief Set the velocity of the Velodyne
        /// \param[in] _vel New target velocity
        void SetVelocity(const double &_vel)
        {
            // Set the joint's target velocity.
            this->model->GetJointController()->SetVelocityTarget(
                this->joint->GetScopedName(), _vel);
        }

      private:

        /// \brief Pointer to the model.
        physics::ModelPtr model;

        /// \brief POinter to the joint.
        physics::JointPtr joint;

        /// \brief A PID controller for the joint.
        common::PID pid;

        /// \brief A node used for transport.
        transport::NodePtr node;

        /// \brief A subscriber to a named topic.
        transport::SubscriberPtr sub;

        /// \brief Handle incoming messages
        /// \param[in] _msg Repurpose a vector3 message. This function will
        /// only use the x component.
        void OnMsg(ConstVector3dPtr &_msg)
        {
            this->SetVelocity(_msg->x());
        }
    };
    GZ_REGISTER_MODEL_PLUGIN(VelodynePlugin)
}


#endif