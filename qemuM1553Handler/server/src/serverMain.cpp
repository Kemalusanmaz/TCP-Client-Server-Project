#include "../include/server.hpp"
#include <iostream>
#include <thread>

constexpr int QEMU_PORT = 9015;
constexpr std::string_view QEMU_IP = "127.0.0.1";

boost::asio::io_context ioContext;
QemuServer server(ioContext);
int main() {

  std::cout << "Starting server on " << QEMU_IP << ":" << QEMU_PORT
            << std::endl;
  server.createSocket(std::string(QEMU_IP), QEMU_PORT);

  if (server.isConnected()) {

    while (true) {
      std::string msg = "Test\n";
      server.send(msg);
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    };
  }

  return 0;
}
