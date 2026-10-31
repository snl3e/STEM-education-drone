#include <iostream>

#include <gz/transport/Node.hh>
#include <gz/msgs/imu.pb.h>

#include <chrono>
#include <thread>

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

int main() {
    gz::transport::Node node;

    if (!node.Subscribe("/X3/imu", onImu)){
        std::cerr << "Failed to connect to /X3/imu\n.";
        return 1;
    }

    while(true){
            std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
