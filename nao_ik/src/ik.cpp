// Copyright 2021 Kenji Brameld
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "nao_ik/ik.hpp"
#include "nao_command_msgs/msg/joint_indexes.hpp"
#include "nao_ik/bhuman/ik_bhuman.hpp"
#include "nao_ik/indexes.hpp"

IK::IK()
: Node("IK")
{
  sub_sole_poses =
    create_subscription<biped_interfaces::msg::SolePoses>(
    "motion/sole_poses", 1,
    [this](biped_interfaces::msg::SolePoses::SharedPtr sole_poses) {
      nao_command_msgs::msg::JointPositions joints;
      RobotDimensions rd;
      rd.yHipOffset = 0.050;
      rd.zHipOffset = 0.085;
      rd.upperLegLength = 0.100;
      rd.lowerLegLength = 0.1029;
      rd.footHeight = 0.04519;
      ik_bhuman::calcLegJoints(sole_poses->l_sole, sole_poses->r_sole, joints, rd);
      pub_stiffnesses->publish(stiffnessMax);
      pub_joints->publish(joints);
    });

  stiffnessMax = nao_command_msgs::msg::JointStiffnesses()
  .set__indexes(indexes::indexes)
  .set__stiffnesses({1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
  stiffnessMax.stiffnesses[nao_command_msgs::msg::JointIndexes::LSHOULDERPITCH] = 0;
  stiffnessMax.stiffnesses[nao_command_msgs::msg::JointIndexes::RSHOULDERPITCH] = 0;
  stiffnessMax.stiffnesses[nao_command_msgs::msg::JointIndexes::LSHOULDERROLL] = 0;
  stiffnessMax.stiffnesses[nao_command_msgs::msg::JointIndexes::RSHOULDERROLL] = 0;
  pub_stiffnesses =
    this->create_publisher<nao_command_msgs::msg::JointStiffnesses>("effectors/joint_stiffnesses", 10);

  pub_joints =
    this->create_publisher<nao_command_msgs::msg::JointPositions>("effectors/joint_positions", 10);
}
