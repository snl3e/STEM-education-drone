#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <cerrno>

#include <string>
#include <iostream>

#include <chrono>
#include <poll.h>
#include "serial_port.hpp"


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
    tty.c_cc[VMIN] = 1;             // 1 bytes required for read()
    tty.c_cc[VTIME] = 0;            // No timeout
    //Set terminal settings
    if(tcsetattr(fileDescriptor, TCSANOW, &tty) == -1){
        perror("tcsetattr");
        return false;
    }

    return true;
}

bool SerialPort::write(const std::string &message){
    if(fileDescriptor == -1) {
        std::cerr << "Serial port do not exist.\n";
        return false;
    }

    size_t totalWritten{0};
    
    while (totalWritten < message.size())
    {
        const char* remainingMessage = message.c_str() + totalWritten;
        size_t remaining = message.size() - totalWritten;

        ssize_t bytesWritten = ::write(fileDescriptor, remainingMessage, remaining);

        if (bytesWritten == -1) {
            std::cerr << "Failed to send message to " << devicePath << '\n';
            return false;
        }

        if (bytesWritten == 0) {
            std::cerr << "No bytes were written to " << devicePath << '\n';
            return false;
        }

        totalWritten += bytesWritten;
    }
    
    return true;
}

bool SerialPort::read(std::string &message, int timeoutMs){
    if (fileDescriptor == -1) {
        std::cerr << "Serial port do not exist.\n";
        return false;
    }

    if (timeoutMs <= 0) {
        std::cerr << "Invalid timeout.\n";
        return false;
    }

    constexpr size_t MAX_MESSAGE_LENGTH = 127;
    const auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeoutMs);

    message.clear();
    
    while (true) { 
        //Timeout if the execution time > timeoutMs
        auto now = std::chrono::steady_clock::now();
        if (now >= deadline){
            std::cerr << "Read timeout.\n";
            return false;
        }
        int remainingMs = std::chrono::duration_cast<std::chrono::milliseconds>(deadline - now).count();

        struct pollfd pfd{};
        pfd.fd = fileDescriptor;
        pfd.events = POLLIN;

        int pollResult = poll(&pfd, 1, remainingMs);
        if (pollResult == -1) {
            if (errno == EINTR) continue;
            perror("poll");
            return false;
        }
        if (pollResult == 0){
            std::cerr << "Read timeout.\n";
            return false;
        }
        
        if (pfd.revents & POLLNVAL) {
            std::cerr << "Invalid file descriptor.\n";
            return false;
        }

        if (pfd.revents & POLLERR) {
            std::cerr << "Serial port error.\n";
            return false;
        }

        if (pfd.revents & POLLHUP) {
            std::cerr << "Serial port disconnected.\n";
            return false;
        }

        if (!(pfd.revents & POLLIN)) {
            std::cerr << "Unexpected poll event.\n";
            return false;
        }

        //Reading loop
        char receivedByte;
        ssize_t readResult = ::read(fileDescriptor, &receivedByte, 1);

        if (readResult == -1 && errno == EINTR) {
            continue;
        }

        if (readResult == -1) {
            std::cerr << "Failed to read message from " << devicePath << '\n';
            return false;
        }

        if (readResult == 0) {
            std::cerr << "No data received from " << devicePath << '\n';
            return false;
        }

        if (receivedByte == '\n') break;

        if (message.size() == MAX_MESSAGE_LENGTH) {
            std::cerr << "Message exceed max length (127 bytes).\n";
            return false;
        }
        message += receivedByte;
    }

    return true;
}