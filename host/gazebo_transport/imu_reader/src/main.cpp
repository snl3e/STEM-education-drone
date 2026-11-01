#include <iostream>

#include <gz/transport/Node.hh>

#include <chrono>
#include <thread>

#include "imu_callback.h"


int main() {
    gz::transport::Node node;

    if (!node.Subscribe("/X3/imu", onImu)){
        std::cerr << "Failed to subscribe to /X3/imu\n";
        return 1;
    }

    while(true){
            std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
