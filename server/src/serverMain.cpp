#include "../include/serverSetup.hpp"
#include "../../common/include/setup.hpp"
#include <iostream>
#include <thread>

boost::asio::io_context ioContext;
Setup server(ioContext);
int main() {

  // std::cout << "Starting server on " << QEMU_IP << ":" << QEMU_PORT
  //           << std::endl;
  server.createSocket();

  if (server.isConnected()) {

    while (true) {
      std::string msg = "Test\n";
      server.send(msg);
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    };
  }

  return 0;
}
