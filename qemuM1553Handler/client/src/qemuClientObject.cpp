#include "../include/qemuClientObject.hpp"
#include "qemuM1553Setup.hpp"
#include <fstream>
#include <string>

boost::asio::io_context ioContext;
QemuCommSetup client(
    ioContext); // create instance of QemuCommSetup with ioContext argument

QemuClientObject::QemuClientObject() {

  std::string jsonPath = "C:\\Users\\T12022061821\\Desktop\\TCP Server-Client "
                         "Project\\qemuM1553Handler\\connectionConf.json";
  std::ifstream jsonFile(jsonPath);
  nlohmann::json jsonData;
  jsonFile >> jsonData;
  m_qemuIp = jsonData["IP"];     // extract ip information from conf file
  m_qemuPort = jsonData["PORT"]; // extract port information from conf file
}

// Establish connection to QEMU
void QemuClientObject::initialize() { client.connect(m_qemuIp, m_qemuPort); }

// Starts receiving messages from QEMU
void QemuClientObject::receiveMessage() {
  if (client.isConnected()) { // Check if the connection is established
                              // Check if data is available in the queue
    m_busControllerOnOffStatus = 1;
  } else {                          // No data received
    m_busControllerOnOffStatus = 0; // Set the bus controller status to OFF
  }
}

// Disconnect from QEMU manually
void QemuClientObject::terminate() { client.disconnect(); }