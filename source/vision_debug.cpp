/**
 *
 * Vision Plugin, vision.cpp
 * @brief Defines the methods of the Vision plugin.
 * @author Abhijit J Theophilus
 *
 */

#include "vision_debug.hpp"

using namespace gazebo;

// Called by Gazebo when the model is loaded into the simulation.
void VisionPlugin::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
{

    // Initialize the model and joint.
    this->model = _model;
    this->joint = _model->GetJoint("vision::neck_top_joint");
    this->jctrl = _model->GetJointController();

    // Initialize a few parameters from the SDF.
    this->velocity = _sdf->Get<double>("velocity", 0).first;
    this->upper_limit = this->joint->UpperLimit();
    this->lower_limit = this->joint->LowerLimit();

    std::string joint_name = this->joint->GetScopedName();

    double p = 0;                                   // Proportional
    double i = 0;                                   // Integral
    double d = 0;                                   // Derivative
    sdf::ElementPtr pid_sdf = _sdf->GetElementImpl("pid");
    if (pid_sdf != nullptr)
    {
        p = pid_sdf->Get<double>("p", 0).first;
        i = pid_sdf->Get<double>("i", 0).first;
        d = pid_sdf->Get<double>("d", 0).first;
    }

    // Setup neck-turning velocity control.
    this->pid = common::PID(p, i, d);
    this->joint->SetVelocity(0, this->velocity);

    this->jctrl->SetVelocityPID(joint_name, this->pid);
    this->jctrl->SetVelocityTarget(joint_name, this->velocity);

    this->connect = this->joint->ConnectJointUpdate(
        boost::bind(&VisionPlugin::OnJointUpdate, this));

    // Set up sensor monitoring via SensorManager.
    sensors::SensorManager *mgr = sensors::SensorManager::Instance();
    this->sensor = std::dynamic_pointer_cast<sensors::RaySensor>(
        mgr->GetSensor("sonar_sensor"));

    if (this->sensor == nullptr)
        gzerr << "Unable to locate ultrasonic sensor.";
}


// Called each time the neck joint updates.
void VisionPlugin::OnJointUpdate()
{
    double current_pos = this->joint->Position(0);
    std::string joint_name = this->joint->GetScopedName();

    // If the position crosses the limit, flip velocity.
    if (current_pos > this->upper_limit) {
        this->jctrl->SetVelocityTarget(joint_name, -this->velocity);
        this->WritePositionsRanges();
    } else if (current_pos < this->lower_limit) {
        this->jctrl->SetVelocityTarget(joint_name, this->velocity);
        this->WritePositionsRanges();
    } else {
        std::vector<double> ranges(32);
        this->sensor->Ranges(ranges);
        auto min_it = std::min_element(std::begin(ranges), std::end(ranges));

        // This is to only store 3 digits of precision in pos.
        uint curr_i = round(current_pos * 1000) + 1309;
        this->ranges[curr_i] = *min_it;
    }
}


// Write the ranges and positions to a file.
inline void VisionPlugin::WritePositionsRanges() {
    uint sz = this->ranges.size();
    if (sz < 10) return;

    this->ranges_f.open(
        "res/data/" + this->model->GetName() + "_ranges.csv");

    for (uint pos = 0; pos != sz; ++pos)
        this->ranges_f << (static_cast<int>(pos) - 1309) / 1000.0 << ", " <<
            this->ranges[pos] << std::endl;

    this->ranges_f.close();
}


// Get a copy of the positions vector.
inline std::vector<double> VisionPlugin::GetSonarRanges() const
{
    std::vector<double> v;
    v.reserve(this->ranges.size());
    std::copy(std::begin(this->ranges), std::end(this->ranges),
              std::back_inserter(v));
    return v;
}
