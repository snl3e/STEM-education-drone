#include <iostream>
#include "../include/serial_port.hpp" 

int main(){
    SerialPort esp32("/dev/ttyACM0", 115200);

    if (esp32.open()) std::cout << "ESP32 opened successfully.\n";
    if (esp32.configure()) std::cout << "ESP32 configured successfully.\n";
    if (esp32.close()) std::cout << "ESP32 closed successfully.\n";
    return 0;
}