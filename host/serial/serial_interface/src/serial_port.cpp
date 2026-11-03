#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include "../include/serial_port.hpp"


SerialPort::SerialPort(const std::string &devicePath, int baudRate)
    :devicePath(devicePath), baudRate(baudRate) {
    }

SerialPort::~SerialPort(){
    if (fileDescriptor != -1 ) ::close(fileDescriptor);
}

bool SerialPort::open(){
    if (fileDescriptor != -1) return true;
    fileDescriptor = ::open(devicePath.c_str(), O_RDWR);
    if (fileDescriptor == -1) {
        perror("Failed to open serial port");
        return false;
    }

    return true;
}

bool SerialPort::close(){
    if (fileDescriptor == -1) return true;
    if (::close(fileDescriptor) == -1) return false;
    
    fileDescriptor = -1;
    return true;
}