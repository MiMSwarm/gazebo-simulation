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
        
        // Write the ranges to a stream and write stream to a file.
        if (this->pos_ranges.size() > 10) {
            this->ranges_f.open(
                "data/" + this->model->GetName() + "_ranges.txt");
            for (double i : this->pos_ranges)
                this->ranges_f << i << std::endl;
            this->ranges_f.close();
            this->pos_ranges.clear();
        }
    } else if (current_pos < this->lower_limit) {
        this->jctrl->SetVelocityTarget(joint_name, this->velocity);

        // Write the ranges to a stream and write stream to a file.
        if (this->pos_ranges.size() > 10) {
            this->ranges_f.open(
                "data/" + this->model->GetName() + "_ranges.txt");
            for (double i : this->pos_ranges)
                this->ranges_f << i << std::endl;
            this->ranges_f.close();
            this->pos_ranges.clear();
        }
    } else {
        std::vector<double> ranges(32);
        this->sensor->Ranges(ranges);

        double min_range = ranges[0];
        for (double d : ranges)
            if (d < min_range)
                min_range = d;

        // This is to only store 3 digits of precision in pos.
        this->pos_ranges.push_back(min_range);
        this->pos_ranges.push_back(current_pos);
    }
}
