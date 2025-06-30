#ifndef __SETUP_HPP__
#define __SETUP_HPP__

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <nlohmann/json.hpp>
#include <queue>
#include <string>

constexpr int BUFFER_SIZE = 1024; // Buffer size is defined.

class Setup {

public:
  explicit Setup(boost::asio::io_context &mIoContext);

  void createSocket();

  /**
   * Establish async connection with server
   * @param serverIP Qemu IP Address.
   * @param serverPort Qemu Port Number.
   */
  void connect();
  /**
   * Check the connection status
   * @return Connection status.
   */
  bool isConnected();
  /**
   * Send the message to the Qemu
   * @param message a message that is sent to the Qemu.
   */
  void send(const std::string &message);
  /**
   * Receive message form the Qemu
   */
  void receive();
  /**
   * Close the Tcp connection with Qemu
   */
  void disconnect();

  /**
   * boost::asio::streambuf is noc copyable object. Therefore, the reference of
   * object is returned.
   * @param errorCode error code if there is an problem during connection.
   * @return m_receivedBuffer stores received data taking from Qemu..
   */
  bool getNextReceivedData(std::string &outData);

  /**
   * Retrieves the next received message from the queue.
   * This function checks if the queue is empty and returns the first available
   * message. If the queue is empty, it returns false. If a message is
   * available, it returns true.
   * @param outData A reference to a string where the retrieved message will be
   * stored.
   * @return True if there is a message in the queue; false if the queue is
   * empty.
   */
  std::queue<std::string> &getDataQueue() { return m_dataQueue; }

private:
  /**
   * Callback function that the connection is established successfully.
   * @param errorCode error code if there is an problem during connection.
   */
  void onConnect(const boost::system::error_code &errorCode);
  /**
   * Callback function that a message is sent successfully.
   */
  static void onSend(const boost::system::error_code &errorCode,
                     const std::size_t &length);
  /**
   * Callback function that a message is received successfully.
   * @param errorCode error code if there is an problem during connection.
   * @param length length of a message that sent.
   */
  void onReceive(const boost::system::error_code &errorCode);

  /**
   * This callback function is called when a message has been successfully
   * received. It pushes the received data into the queue to be processed later.
   * @param data The received message to be pushed into the queue.
   */
  void pushData(const std::string &data);

  boost::asio::io_context
      &m_ioContext; // the object that manages async operations. Get the server
                    // IP and port info
  boost::asio::ip::tcp::socket m_socket; // TCP Socket that connects with server
  boost::asio::ip::tcp::resolver
      m_resolver; // resolver resolves the server IP address.
  boost::asio::streambuf m_receivedBuffer; // the data from server stores.
  boost::asio::thread_pool m_ioThread; // Thread pool used for asynchronous I/O
                                       // operations with Boost.Asio.
  std::queue<std::string>
      m_dataQueue; // Queue used to temporarily store received messages.
  std::condition_variable
      m_condition; // Condition variable used for synchronizing threads and
                   // checking if the queue is empty.
  boost::asio::ip::tcp::acceptor m_acceptor;
  std::string m_ip; // IP address that QEMU sw is deployed.
  int m_port;       // Port number for QEMU connection
};

#endif // __SETUP_HPP__
