#include <iostream>

#include <gz/transport/Node.hh>
#include <gz/msgs/actuators.pb.h>

#include <chrono>
#include <thread>

int main() {
    gz::transport::Node node;
    gz::msgs::Actuators message;

    for (int i = 0; i < 4; i++) {
        message.add_velocity(0);
    }
   
    auto publisher = node.Advertise<gz::msgs::Actuators>("/X3/gazebo/command/motor_speed");
    if(!publisher.Valid()) {
        std::cerr << "The publisher is not valid\n";
        return 1;
    }
    
    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(1));
         bool success = publisher.Publish(message);
        std::cout << "Publish result: " << success << '\n';
    }
    return 0;
}