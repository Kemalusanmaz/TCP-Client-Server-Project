#include "../include/clientObject.hpp"
#include "../../common/include/setup.hpp"
#include <fstream>
#include <string>

boost::asio::io_context ioContext;
Setup client(
    ioContext); // create instance of QemuCommSetup with ioContext argument

ClientObject::ClientObject() {

  std::string jsonPath = "C:\\Users\\T12022061821\\Desktop\\TCP Server-Client "
                         "Project\\connectionConf.json";
  std::ifstream jsonFile(jsonPath);
  nlohmann::json jsonData;
  jsonFile >> jsonData;
  m_qemuIp = jsonData["IP"];     // extract ip information from conf file
  m_qemuPort = jsonData["PORT"]; // extract port information from conf file
}

// Establish connection to QEMU
void ClientObject::initialize() { client.connect(); }

// Starts receiving messages from QEMU
void ClientObject::receiveMessage() {
  if (client.isConnected()) { // Check if the connection is established
                              // Check if data is available in the queue
    m_busControllerOnOffStatus = 1;
  } else {                          // No data received
    m_busControllerOnOffStatus = 0; // Set the bus controller status to OFF
  }
}

// Disconnect from QEMU manually
void ClientObject::terminate() { client.disconnect(); }