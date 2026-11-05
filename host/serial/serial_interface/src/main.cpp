#include <iostream>
#include <string>
#include "../include/serial_port.hpp" 


int main(){
    SerialPort esp32("/dev/ttyACM0", 115200);

    if (esp32.open()) std::cout << "ESP32 opened successfully.\n";
    if (esp32.configure()) std::cout << "ESP32 configured successfully.\n";
    std::string message = "Hello\n";
    if (esp32.write(message)) std::cout << "Message sended.\n";
    if (esp32.close()) std::cout << "ESP32 closed successfully.\n";
    return 0;
}