#pragma once

#include <string>

class SerialPort {
    private:
        std::string devicePath{};
        int baudRate{};
        int fileDescriptor{-1};
    public: 
        SerialPort(const std::string &devicePath, int baudRate);
        ~SerialPort();

        //Disable Copying
        SerialPort(const SerialPort&) = delete;
        SerialPort& operator=(const SerialPort&) = delete;

        bool open();
        bool close();
        bool configure();

        
    };