#include "../include/serverObject.hpp"
#include "../../common/include/setup.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

boost::asio::io_context ioContext;
Setup server(ioContext); // create instance of setup with ioContext argument

// Establish connection to Client
void ServerObject::initialize() { server.createSocket(); }

// Starts receiving messages from Client
void ServerObject::transmitMessage() {
  std::string message;

  if (server.isConnected()) {
    std::cout << "Starting server on " << server.getIp() << ":"
              << server.getPort() << std::endl;

    while (true) {
      std::cout << "Message: ";
      std::cin >> message;
      message += "\n";
      server.send(message);
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    };
  }
}

// Disconnect from Client manually
void ServerObject::terminate() { server.disconnect(); }