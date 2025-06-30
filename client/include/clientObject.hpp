/**********************************TRICK HEADER***************************************
PURPOSE: Defines a class that manages communication between a MIL-STD-1553 simulation 
running on QEMU and the simulation software. This includes establishing a TCP 
connection, handling incoming and outgoing messages using the MessageHandler, 
and maintaining the communication state with the simulated Bus Controller.
*************************************************************************************/
#ifndef CLIENTOBJECT_HPP
#define CLIENTOBJECT_HPP

#include "../../common/include/messageHandler.hpp"

constexpr int BUFFER_LENGTH = 140; // Expected length of each message m_chunk

class ClientObject {
public:
ClientObject();
/**
   * Initializes the QEMU client communication setup.
   * Typically includes creating socket connections or necessary setup before communication starts.
   */
void initialize();
/**
   * Starts a background thread to receive and handle incoming messages from QEMU.
   * Accumulates data until a delimiter is found, then parses complete messages using the messageHandler.
   */
void receiveMessage();
/**
   * Cleans up resources and terminates the QEMU client communication.
   * Should be called during shutdown or reset operations.
   */
static void terminate();

private:
  MessageHandler messageHandler; // Responsible for parsing and handling MIL-STD-1553 messages
  int m_busControllerOnOffStatus{}; // Indicates whether the simulated Bus Controller is active (1) or not (0)
  std::string m_buffer; // Temporary buffer to accumulate incoming data until complete messages are detected
  std::string m_chunk; // Placeholder to store each 140-byte message chunk, if fixed-length parsing is used
  std::string m_receivedData; // Stores the latest data received from the TCP connection
  std::string m_result; // General-purpose string to store processed or intermediate results
  std::string m_qemuIp; // IP address that QEMU sw is deployed.
  int m_qemuPort; // Port number for QEMU connection
};

#endif // CLIENTOBJECT_HPP
