/*************************************************************************
This includes establishing a TCP connection, handling
incoming and outgoing messages using the MessageHandler, and maintaining the
communication state with the simulated Bus Controller.
***************************************************************************/
#ifndef SERVEROBJECT_HPP
#define SERVEROBJECT_HPP

#include "../../common/include/messageHandler.hpp"

class ServerObject {
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
  void transmitMessage();
  /**
   * Cleans up resources and terminates the client communication.
   * Should be called during shutdown or reset operations.
   */
  static void terminate();

private:
  MessageHandler messageHandler; // Responsible for parsing and handling
                                 // MIL-STD-1553 messages
};

#endif // SERVEROBJECT_HPP
