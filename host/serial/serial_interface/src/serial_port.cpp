#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
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

bool SerialPort::configure(){
    struct termios tty{};
    if (fileDescriptor == -1) {
        std:: cerr << "Serial path do not exist.\n";
        return false;
    }

    //Get current terminal setting
    if(tcgetattr(fileDescriptor, &tty) == -1){
        perror("tcgetattr");
        return false;
    }

    //Set output & input baud rate
    if (baudRate == 115200){
        if (cfsetispeed(&tty, B115200) == -1 || cfsetospeed(&tty, B115200) == -1){
            perror("cfset baud rate");
            return false;
        }   
    }
    else {
        std::cerr << "Unsupported baud rate.\n";
        return false;
    }

    cfmakeraw(&tty);                // Disable terminal processing   
    tty.c_cflag |= CREAD | CLOCAL;  // Enable the receiver             
    tty.c_cflag &= ~PARENB;         // Disable parity
    tty.c_cflag &= ~CSTOPB;         // Select one stop bit
    tty.c_cflag &= ~CRTSCTS;        // Disable hardware flow control
    tty.c_cflag &= ~CSIZE;          // Clear character-size bits
    tty.c_cflag |= CS8;             // Select 8 data bits


    //Set terminal settings
    if(tcsetattr(fileDescriptor, TCSANOW, &tty) == -1){
        perror("tcsetattr");
        return false;
    }

    return true;
}
