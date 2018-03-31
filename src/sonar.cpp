/**
 *  Plugin to control the sensor movement and data.
 */

#include "sonar.hpp"

using namespace gazebo;

// Called by Gazebo when the model is loaded into the simulation.
void SonarPlugin::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
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
        boost::bind(&SonarPlugin::OnJointUpdate, this));

    // Set up sensor monitoring via SensorManager.
    sensors::SensorManager *mgr = sensors::SensorManager::Instance();
    this->sensor = std::dynamic_pointer_cast<sensors::RaySensor>(
        mgr->GetSensor("sonar_sensor"));

    if (this->sensor == nullptr)
        gzerr << "Unable to locate ultrasonic sensor.";
}


// Called each time the neck joint updates.
void SonarPlugin::OnJointUpdate()
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

        double min_range = ranges[0];
        for (double d : ranges)
            if (d < min_range)
                min_range = d;

        // This is to only store 3 digits of precision in pos.
        this->ranges.push_back(min_range);
        this->positions.push_back(current_pos);
    }
}


// Write the ranges and positions to a file.
inline void SonarPlugin::WritePositionsRanges() {
    unsigned int sz = this->positions.size();
    
    if (sz > 10)
    {
        this->ranges_f.open(
            "res/data/" + this->model->GetName() + "_ranges.csv");

        auto pos_it = this->positions.begin();
        auto rng_it = this->ranges.begin();
        for (; pos_it != this->positions.end(); pos_it++, rng_it++)
            this->ranges_f << *pos_it << "," << *rng_it << std::endl;
        
        this->ranges_f.close();
        this->ranges.clear();
        this->positions.clear();
    }

}


// Get a copy of the positions vector.
inline std::vector<double> SonarPlugin::GetSonarPositions() const
{
    std::vector<double> v;
    v.reserve(this->positions.size());
    std::copy(std::begin(this->positions), std::end(this->positions),
              std::back_inserter(v));
    return v;
}


// Get a copy of the ranges vector.
inline std::vector<double> SonarPlugin::GetSonarRanges() const
{
    std::vector<double> v;
    v.reserve(this->positions.size());
    std::copy(std::begin(this->ranges), std::end(this->ranges),
              std::back_inserter(v));
    return v;
}
