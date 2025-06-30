#include "../include/Setup.hpp"
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

Setup::Setup(boost::asio::io_context &mIoContext)
    : m_ioContext(mIoContext), m_ioThread(1), m_resolver(mIoContext),
      m_socket(mIoContext), m_acceptor(mIoContext) {
  std::string jsonPath = "C:\\Users\\T12022061821\\Desktop\\TCP Server-Client "
                         "Project\\connectionConf.json";
  std::ifstream jsonFile(jsonPath);
  nlohmann::json jsonData;
  jsonFile >> jsonData;
  m_ip = jsonData["IP"];     // extract ip information from conf file
  m_port = jsonData["PORT"]; // extract port information from conf file
}

void Setup::createSocket() {
  boost::asio::ip::address address = boost::asio::ip::make_address(m_ip);
  boost::asio::ip::tcp::endpoint endpoint(address, m_port);
  m_acceptor.open(endpoint.protocol());
  m_acceptor.bind(endpoint);
  m_acceptor.listen();

  std::cout << "Server is listening on port " << m_port << "..." << std::endl;

  m_acceptor.accept(m_socket);
  std::cout << "Client connected!" << std::endl;

  post(m_ioThread, [this]() { m_ioContext.run(); });
  ;

  boost::asio::ip::tcp::no_delay option(true); // Nigle algorithm is closed.
  m_socket.set_option(option);
}

/**
 * The function provides that client can connect server.
 */
void Setup::connect() {
  try {
    auto endpoints = m_resolver.resolve(
        m_ip,
        std::to_string(
            m_port)); // resolve endpoints according to serrver ip and port.
    boost::asio::async_connect(m_socket, endpoints,
                               [this](boost::system::error_code errorCode,
                                      const boost::asio::ip::tcp::endpoint &) {
                                 onConnect(errorCode);
                               }); // async connection is started. With lambda
                                   // funciton callback function returns.

    post(m_ioThread, [this]() {
      m_ioContext.run();
    }); // Ensure the async event loop is running
  } catch (...) {
    std::cerr << "Connection error" << std::endl;
  }
}

/**
 * The function checks the connection status.
 */
bool Setup::isConnected() { return m_socket.is_open(); }

/**
 * This callback function is invoked when connection is established.
 */
void Setup::onConnect(const boost::system::error_code &errorCode) {
  if (!errorCode) { // If there is no error while async connection is tried to
                    // established.
    std::cout << "Connection is established with: "
              << m_socket.remote_endpoint().address().to_string()
              << " successfully" << std::endl;
    receive(); // Receiving data is started after connection is established
               // successfully.

  } else {
    std::cerr
        << "There is an error while connection is established. Error Message: "
        << errorCode.message() << std::endl;
    disconnect(); // connection is closed.
  }
}

/**
 * This function provides that client can send a message packet.
 */
void Setup::send(const std::string &message) {
  boost::asio::async_write(
      m_socket, boost::asio::buffer(message),
      [this](boost::system::error_code errorCode, std::size_t length) {
        onSend(errorCode, length);
      });
}

/**
 * This callback function is invoked when sending a message process is
 * completed.
 */
void Setup::onSend(const boost::system::error_code &errorCode,
                   const std::size_t &length) {
  if (!errorCode) { // If there is no error while a message is tried to sent to
                    // the Server.
    std::cout << "Sent message: " << length << " bytes." << std::endl;
  } else {
    std::cerr << "Write failed: " << errorCode.message() << std::endl;
  }
}

/**
 * This function provides that client can receive a message packet
 */
void Setup::receive() {
  boost::asio::async_read_until(
      m_socket, m_receivedBuffer, // a message packet stores in this variable.
      "\n",                       // Read message until null char.
      [this](boost::system::error_code errorCode, std::size_t /*length*/) {
        onReceive(errorCode);
      });
}

/**
 * This callback function is invoked when receiving a message process is
 * completed.
 */
void Setup::onReceive(const boost::system::error_code &errorCode) {
  if (!errorCode) { // If there is no error while a message is received from
                    // Server.

    std::istream bufferStream(&m_receivedBuffer);
    std::string receivedMessage((std::istreambuf_iterator<char>(bufferStream)),
                                std::istreambuf_iterator<char>());
    std::cout << "Received Message: " << receivedMessage << std::endl;

    receive(); // This function is invoked to get a message continuously
  } else {
    std::cerr << "Read failed: " << errorCode.message() << std::endl;
  }
}

/**
 * This function is called when a message has been successfully received and
 * parsed. It pushes the parsed data into the queue and notifies any waiting
 * threads.
 */
void Setup::pushData(const std::string &data) {
  m_dataQueue.push(data);
  m_condition.notify_one();
}

/**
 * This function is used to retrieve the next available message from the queue.
 * If the queue is empty, it returns false. Otherwise, it pops the front message
 * and returns true.
 */
bool Setup::getNextReceivedData(std::string &outData) {
  if (m_dataQueue.empty()) {
    return false;
  }

  outData = m_dataQueue.front();
  m_dataQueue.pop();
  return true;
}

/**
 * This function provides to close connection between client and server.
 */
void Setup::disconnect() {
  if (isConnected()) { // If connection is still open
    m_socket.close();
    std::cout << "Connection is closed successfully." << std::endl;
  }
}