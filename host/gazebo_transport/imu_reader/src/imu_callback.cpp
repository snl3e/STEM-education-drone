#include <iostream>
#include <gz/msgs/imu.pb.h>
#include "imu_callback.h"

void onImu(const gz::msgs::IMU &msg){
    const auto &angularVelocity = msg.angular_velocity();
    const auto &linearAcceleration = msg.linear_acceleration();

    std::cout << "Angular velocity: "
              << angularVelocity.x() << ", "
              << angularVelocity.y() << ", "
              << angularVelocity.z() << '\n';

    std::cout << "Linear Acceleration: "
              << linearAcceleration.x() << ", "
              << linearAcceleration.y() << ", "
              << linearAcceleration.z() << '\n';

    std::cout << "------------------------------\n";
}