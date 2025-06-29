#ifndef SERVER_HPP
#define SERVER_HPP
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/post.hpp>
#include <queue>
#include <nlohmann/json.hpp>

class QemuServer {

public:
  explicit QemuServer(boost::asio::io_context &mIoContext);
  void createSocket(const std::string &ipAddress, int portNumber);

  void closeSocket();

  void send(const std::string &message);
  /**
   * Receive message form the Qemu
   */
  void receive();

  /**
   * Check the connection status
   * @return Connection status.
   */
  bool isConnected();

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
  boost::asio::ip::tcp::socket m_socket;
  boost::asio::ip::tcp::acceptor m_acceptor;

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

  boost::asio::thread_pool m_ioThread; // Thread pool used for asynchronous I/O
                                       // operations with Boost.Asio.
                                       // std::thread m_ioThread;
  std::queue<std::string>
      m_dataQueue; // Queue used to temporarily store received messages.
  boost::asio::streambuf m_receivedBuffer; // the data from server stores.
  std::condition_variable
      m_condition; // Condition variable used for synchronizing threads and
                   // checking if the queue is empty.
  boost::asio::io_context
      &m_ioContext; // the object that manages async operations. Get the server
                    // IP and port info
  std::string m_qemuIp; // IP address that QEMU sw is deployed.
  int m_qemuPort;       // Port number for QEMU connection
};

#endif // SERVER_HPP
