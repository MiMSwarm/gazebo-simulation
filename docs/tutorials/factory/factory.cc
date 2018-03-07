#include <ignition/math/Pose3.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <gazebo/gazebo.hh>

namespace gazebo {
    class Factory: public WorldPlugin
    {
        public:
        void Load(physics::WorldPtr _parent, sdf::ElementPtr)
        {
            // Option 1: Insert model from file via function call.
            _parent->InsertModelFile("model://box");

            // Option 2: Insert model from string via function call.
            sdf::SDF sphereSDF;
            sphereSDF.SetFromString(
                "<sdf version='1.4'>\
                  <model name='sphere'>\
                    <pose>1 0 0 0 0 0</pose>\
                    <link name='link'>\
                      <pose>0 0 .5 0 0 0</pose>\
                      <collision name='collision'>\
                        <geometry>\
                          <sphere><radius>0.5</radius></sphere>\
                        </geometry>\
                      </collision>\
                      <visual name='visual'>\
                        <geometry>\
                          <sphere><radius>0.5</radius></sphere>\
                        </geometry>\
                      </visual>\
                    </link>\
                  </model>\
                </sdf>");
            // Demonstrate using custom model name.
            sdf::ElementPtr model = sphereSDF.Root()->GetElement("model");
            model->GetAttribute("name")->SetFromString("unique_sphere");
            _parent->InsertModelSDF(sphereSDF);

            // Option 3: Insert from file via message passing.
            {
                // Trasport node.
                transport::NodePtr node(new transport::Node());
                
                // Initialize node with world name.
                node->Init(_parent->Name());

                // Create a publisher on the factory topic.
                transport::PublisherPtr factoryPub =
                    node->Advertise<msgs::Factory>("~/factory");
                
                // Create the message.
                msgs::Factory msg;

                // Model file to load.
                msg.set_sdf_filename("model://cylinder");

                // Pose to initialize the model to.
                msgs::Set(msg.mutable_pose(),
                          ignition::math::Pose3d(
                              ignition::math::Vector3d(1, -2, 0),
                              ignition::math::Quaterniond(0, 0, 0)));
                
                // Send the message.
                factoryPub->Publish(msg);
            }
        }
    };

    // Register this plugin with the simulator.
    GZ_REGISTER_WORLD_PLUGIN(Factory);
}