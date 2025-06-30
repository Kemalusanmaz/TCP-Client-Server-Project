#ifndef __MESSAGEHANDLER_HPP__
#define __MESSAGEHANDLER_HPP__

#include <boost/asio/streambuf.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

class MessageHandler {
public:
  explicit MessageHandler();
  
  /**
   * Parses the received raw message string and extracts metadata and data words.
   * @param receivedData Raw message string received via TCP or other communication protocol.
   */
  void parseData(const std::string &receivedData);

  /**
   * Converts a hexadecimal string into an array of 16-bit unsigned integers.
   * @param dataWord Hexadecimal string to be converted.
   * @return Pointer to an array of 16-bit unsigned integers.
   */
  static uint16_t *convertUint16(const std::string &dataWord);
  
  /**
   * Converts an array of 16-bit unsigned integers to a formatted message string.
   * If the number of words is less than the maximum (32), the remaining words are filled with zeros.
   * @param dataWord Pointer to an array of 16-bit unsigned integers.
   * @return Formatted message string with metadata and data words.
   */
  std::string convertString(uint16_t *dataWord);
  
  /**
   * Maps the parsed hexadecimal data words into a vector of double values.
   * @return Vector of double values parsed from data words.
   */
  std::vector<double> mapReceivedData();
  
  /**
   * Returns whether the message is Transmit (1) or Receive (0).
   * @return 1 if transmit, 0 if receive.
   */
  int getIsTransmitReceive() const { return m_isTransmitReceive; }
  
  /**
   * Returns the raw data word string extracted from the message.
   * @return Data word string.
   */
  std::string getDataWord() const { return m_dataWord; }

  /**
   * Returns the Remote Terminal (RT) address.
   * @return RT address as integer.
   */
  int getRt() const { return m_rt; }

  /**
   * Returns the Subaddress (SA) value.
   * @return SA value as integer.
   */
  int getSa() const { return m_sa; }

  /**
   * Returns the Word Count (WC).
   * @return Word Count as integer.
   */
  int getWc() const { return m_wc; }

private:

  /**
   * Returns the current date and time as a string.
   * @return Formatted date and time string.
  */
  static std::string getTimeDate();
  
  std::vector<double> m_parsedValues{}; //Vector of parsed values converted to doubles.
  nlohmann::json m_jsonData; //JSON object used for internal structured data representation.
  std::string m_dataWord; //Hexadecimal string representing the data portion of the message.
  int m_isTransmitReceive; //Transmit/Receive flag: 1 for transmit, 0 for receive.
  int m_rt; //Remote Terminal (RT) address.
  int m_sa; //Subaddress (SA) of the terminal.
  int m_wc; //Word Count (WC), the number of 16-bit data words.
};

#endif // MESSAGEHANDLER_HPP
