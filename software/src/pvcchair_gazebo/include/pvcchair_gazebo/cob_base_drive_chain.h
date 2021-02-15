#ifndef COB_BASE_DRIVE_CHAIN_H
#define COB_BASE_DRIVE_CHAIN_H

#include <map>
#include <mutex>
#include <string>

#include <control_msgs/JointTrajectoryControllerState.h>
#include <ros/callback_queue.h>
#include <ros/ros.h>

#include <gazebo/common/Events.hh>
#include <gazebo/common/Plugin.hh>
#include <gazebo/physics/physics.hh>

namespace gazebo {

class CobBaseDriveChain : public ModelPlugin {
 public:
  CobBaseDriveChain();
  virtual ~CobBaseDriveChain() override;
  virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf) override;
  virtual void Reset() override;

 private:
  inline bool isSteeringJoint(const physics::JointPtr &joint) const {
    return joint->GetName().find("rotation") != std::string::npos;
  }

  inline double getMaxTorque(const physics::JointPtr &joint) const {
    return isSteeringJoint(joint) ? max_steering_torque_ : max_drive_torque_;
  }

  bool initState();
  void onUpdate(const common::UpdateInfo &info);
  double enforceLimit(const physics::JointPtr &joint, bool is_steering_joint,
                      double command, double period);
  void jointCommandCallback(
      const control_msgs::JointTrajectoryControllerStateConstPtr &msg);

  physics::WorldPtr world_;
  physics::ModelPtr model_;

  double max_steering_torque_;
  double max_drive_torque_;
  double max_steering_vel_;
  double max_drive_accel_;

  ros::NodeHandlePtr nh_;
  ros::CallbackQueue callback_queue_;
  ros::AsyncSpinner spinner_;

  ros::Publisher state_publisher_;
  control_msgs::JointTrajectoryControllerState state_msg_;
  std::map<std::string, double> last_commands_;

  ros::Subscriber joint_command_subscriber_;
  control_msgs::JointTrajectoryControllerState command_msg_;
  std::mutex command_mutex_;

  double update_period_;
  common::Time last_update_time_;
  event::ConnectionPtr update_connection_;
};

}  // namespace gazebo

#endif  // COB_BASE_DRIVE_CHAIN_H
