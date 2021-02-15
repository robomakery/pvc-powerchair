#include <pvcchair_gazebo/cob_base_drive_chain.h>

namespace gazebo {

CobBaseDriveChain::CobBaseDriveChain() : spinner_(1, &callback_queue_) {}

CobBaseDriveChain::~CobBaseDriveChain() {
  callback_queue_.clear();
  callback_queue_.disable();
  nh_->shutdown();
}

void CobBaseDriveChain::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf) {
  world_ = _model->GetWorld();
  model_ = _model;

  // Get max steering torque
  if (not _sdf->HasElement("steeringTorque")) {
    ROS_ERROR_STREAM("CobBaseDriveChain: Failed to get steeringTorque");
    return;
  }
  max_steering_torque_ = _sdf->Get<double>("steeringTorque");

  // Get max drive torque
  if (not _sdf->HasElement("driveTorque")) {
    ROS_ERROR_STREAM("CobBaseDriveChain: Failed to get driveTorque");
    return;
  }
  max_drive_torque_ = _sdf->Get<double>("driveTorque");

  // Get max steering velocity
  if (not _sdf->HasElement("steeringVel")) {
    ROS_ERROR_STREAM("CobBaseDriveChain: Failed to get steeringVel");
    return;
  }
  max_steering_vel_ = _sdf->Get<double>("steeringVel");

  // Get max drive acceleration
  if (not _sdf->HasElement("driveAccel")) {
    ROS_ERROR_STREAM("CobBaseDriveChain: Failed to get driveAccel");
    return;
  }
  max_drive_accel_ = _sdf->Get<double>("driveAccel");

  if (not initState()) return;

  if (not ros::isInitialized()) {
    ROS_FATAL_STREAM(
        "CobBaseDriveChain: Failed to load since ROS is not initialized");
    return;
  }

  nh_ = boost::make_shared<ros::NodeHandle>();
  nh_->setCallbackQueue(&callback_queue_);
  spinner_.start();

  // Initialize state publisher
  if (not _sdf->HasElement("stateTopic")) {
    ROS_ERROR_STREAM("CobBaseDriveChain: Failed to get stateTopic");
    return;
  }
  std::string state_topic_name = _sdf->Get<std::string>("stateTopic");
  state_publisher_ =
      nh_->advertise<control_msgs::JointTrajectoryControllerState>(
          state_topic_name, 10);

  // Initialize joint command subscriber
  if (not _sdf->HasElement("jointCommandTopic")) {
    ROS_ERROR_STREAM("CobBaseDriveChain: Failed to get jointCommandTopic");
    return;
  }
  std::string command_topic_name = _sdf->Get<std::string>("jointCommandTopic");
  joint_command_subscriber_ = nh_->subscribe(
      command_topic_name, 10, &CobBaseDriveChain::jointCommandCallback, this);

  double update_rate = _sdf->Get<double>("updateRate", 0).first;
  update_period_ = update_rate != 0.0 ? 1 / update_rate : 0.0;

  update_connection_ = event::Events::ConnectWorldUpdateBegin(
      std::bind(&CobBaseDriveChain::onUpdate, this, std::placeholders::_1));
}

void CobBaseDriveChain::Reset() {
  last_update_time_ = world_->SimTime();
  for (const auto& joint_name : state_msg_.joint_names) {
    auto joint = model_->GetJoint(joint_name);
    joint->SetParam("fmax", 0, getMaxTorque(joint));
  }
}

bool CobBaseDriveChain::initState() {
  // Initialize state
  state_msg_.joint_names = {
      "fl_caster_r_wheel_joint", "fl_caster_rotation_joint",
      "bl_caster_r_wheel_joint", "bl_caster_rotation_joint",
      "br_caster_r_wheel_joint", "br_caster_rotation_joint",
      "fr_caster_r_wheel_joint", "fr_caster_rotation_joint"};
  state_msg_.actual.positions.resize(state_msg_.joint_names.size());
  state_msg_.actual.velocities.resize(state_msg_.joint_names.size());
  command_msg_.joint_names = state_msg_.joint_names;
  command_msg_.desired.positions.resize(command_msg_.joint_names.size());
  command_msg_.desired.velocities.resize(command_msg_.joint_names.size());

  // Initialize joint parameters
  for (const auto& joint_name : state_msg_.joint_names) {
    physics::JointPtr joint = model_->GetJoint(joint_name);
    if (not joint) {
      ROS_ERROR_STREAM("CobBaseDriveChain: Failed to get joint " << joint_name);
      return false;
    }

    if (isSteeringJoint(joint)) {
      joint->SetParam("fmax", 0, max_steering_torque_);
      last_commands_[joint_name] = joint->Position(0);
    } else {
      joint->SetParam("fmax", 0, max_drive_torque_);
      last_commands_[joint_name] = joint->GetVelocity(0);
    }
  }

  return true;
}

void CobBaseDriveChain::onUpdate(const common::UpdateInfo& info) {
  double period = (info.simTime - last_update_time_).Double();

  // Throttle update rate
  if (period < update_period_) return;
  last_update_time_ = info.simTime;

  std::lock_guard<std::mutex> lock(command_mutex_);

  for (size_t i = 0; i < command_msg_.joint_names.size(); ++i) {
    // Get joint
    const std::string& joint_name = command_msg_.joint_names[i];
    physics::JointPtr joint = model_->GetJoint(joint_name);
    bool is_steering_joint = isSteeringJoint(joint);

    // Update actual state
    state_msg_.actual.positions[i] = joint->Position(0);
    state_msg_.actual.velocities[i] = joint->GetVelocity(0);

    // Apply command
    double output_command;
    double last_command = last_commands_[joint_name];
    if (is_steering_joint) {
      double input_command = command_msg_.desired.positions[i];
      output_command =
          fabs(joint->Position(0) - input_command) < 0.001
              ? input_command
              : last_command + enforceLimit(joint, is_steering_joint,
                                            input_command, period);
      joint->SetPosition(0, output_command);
    } else {
      double input_command = command_msg_.desired.velocities[i];
      output_command =
          fabs(joint->GetVelocity(0) - input_command) < 0.001
              ? input_command
              : last_command + enforceLimit(joint, is_steering_joint,
                                            input_command, period);
      joint->SetParam("vel", 0, output_command);
    }
    last_commands_[joint_name] = output_command;
  }

  state_msg_.header.stamp = ros::Time(info.simTime.Double());
  state_publisher_.publish(state_msg_);
}

double CobBaseDriveChain::enforceLimit(const physics::JointPtr& joint,
                                       bool is_steering_joint, double command,
                                       double period) {
  double state = is_steering_joint ? joint->Position(0) : joint->GetVelocity(0);
  double error = command - state;
  double dmax = is_steering_joint ? max_steering_vel_ : max_drive_accel_;
  double delta = fmin(fabs(error), dmax * period);
  return copysign(delta, error);
}

void CobBaseDriveChain::jointCommandCallback(
    const control_msgs::JointTrajectoryControllerStateConstPtr& msg) {
  std::lock_guard<std::mutex> lock(command_mutex_);
  command_msg_ = *msg;
}

GZ_REGISTER_MODEL_PLUGIN(CobBaseDriveChain)

}  // namespace gazebo
