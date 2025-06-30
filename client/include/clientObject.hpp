/*************************************************************************
This includes establishing a TCP connection, handling
incoming and outgoing messages using the MessageHandler, and maintaining the
communication state with the simulated Bus Controller.
**************************************************************************/
#ifndef CLIENTOBJECT_HPP
#define CLIENTOBJECT_HPP

#include "../../common/include/messageHandler.hpp"

constexpr int BUFFER_LENGTH = 140; // Expected length of each message m_chunk

class ClientObject {
public:
  /**
   * Initializes the client communication setup.
   * Typically includes creating socket connections or necessary setup before
   * communication starts.
   */
  void initialize();
  /**
   * Starts a background thread to receive and handle incoming messages from
   * client. Accumulates data until a delimiter is found, then parses complete
   * messages using the messageHandler.
   */
  void receiveMessage();
  /**
   * Cleans up resources and terminates the client communication.
   * Should be called during shutdown or reset operations.
   */
  static void terminate();

private:
  MessageHandler messageHandler; // Responsible for parsing and handling
                                 // messages
  int m_serverOnOffStatus{};     // Indicates whether the simulated Bus
                                 // Controller is active (1) or not (0)
};

#endif // CLIENTOBJECT_HPP
