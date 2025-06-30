#include "../include/clientObject.hpp"
#include "../../common/include/setup.hpp"

boost::asio::io_context ioContext;
Setup client(ioContext); // create instance of setup with ioContext argument

// Establish connection to server
void ClientObject::initialize() { client.connect(); }

// Starts receiving messages from server
void ClientObject::receiveMessage() {
  if (client.isConnected()) { // Check if the connection is established
                              // Check if data is available in the queue
    m_serverOnOffStatus = 1;
  } else {                   // No data received
    m_serverOnOffStatus = 0; // Set the bus controller status to OFF
  }
}

// Disconnect from server manually
void ClientObject::terminate() { client.disconnect(); }