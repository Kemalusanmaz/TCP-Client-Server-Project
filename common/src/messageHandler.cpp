#include "../include/messageHandler.hpp"
#include <climits>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <vector>

MessageHandler::MessageHandler() : m_isTransmitReceive(), m_rt(), m_sa(), m_wc() {}

void MessageHandler::parseData(const std::string &receivedData) {

  std::vector<std::string> parsedData;
  parsedData.clear();
  std::stringstream ss(receivedData);
  std::string token;

  // Split receivedData by semicolon delimiter
  while (std::getline(ss, token, ';')) {
    parsedData.push_back(token);
  }
  // Parse and assign values
  m_isTransmitReceive = std::stoi(parsedData.at(0));
  m_rt = std::stoi(parsedData.at(1));
  m_sa = std::stoi(parsedData.at(2));
  m_wc = std::stoi(parsedData.at(3));
  m_dataWord = parsedData.at(4);
}


uint16_t *MessageHandler::convertUint16(const std::string &dataWord) {
  // Calculate number of 16-bit values
  size_t length = dataWord.length() / 4;
  auto *dataWordValues = new uint16_t[length];
  // Convert each 4-character hex chunk to uint16_t
  for (size_t i = 0; i < length; ++i) {
    std::string hexChunk = dataWord.substr(i * 4, 4);
    dataWordValues[i] = static_cast<uint16_t>(std::stoul(hexChunk, nullptr, 16));
  }

  return dataWordValues;
}

std::string MessageHandler::convertString(uint16_t *dataWord) {
  std::string result;
  char buffer[5]; // 4 hex characters + null terminator
  // Convert uint16_t values back to uppercase hex string
  for (size_t i = 0; i < m_wc; ++i) {
    std::snprintf(buffer, sizeof(buffer), "%04X", dataWord[i]);
    result += buffer;
  }
  // Pad the rest of the string to a total of 32 words (64 bytes)
  for (size_t i = m_wc; i < 32; ++i) {
    result += "0000";
  }
  // Add header fields
  result = std::to_string(m_isTransmitReceive) + ';' + std::to_string(m_rt) + ';' 
          + std::to_string(m_sa) + ';' + std::to_string(m_wc) +  ';' + result + '\0'; 

  return result;
}

std::vector<double> MessageHandler::mapReceivedData() {
  m_parsedValues.clear();
  // Loop through JSON mappings and match RT and SA
  for (auto &[key, value] : m_jsonData.items()) {
    if (m_rt == value["rt"] && m_sa == value["sa"]) {
      const auto &dataFormat = value["dataFormat"];
      std::cout << key << std::endl;

      size_t offset = 0;
      std::vector<double> parsedValues;
      // Loop through data format fields
      for (auto &[dataKey, dataType] : dataFormat.items()) {
        std::string format = dataType.get<std::string>();
        std::string hexData;
        // Parse 16-bit unsigned int
        if (format == "E16") {
          if (offset + 4 > m_dataWord.size()) {
            break;
          }
          hexData = m_dataWord.substr(offset, 4);
          uint16_t intValue = std::stoul(hexData, nullptr, 16);
          m_parsedValues.push_back(static_cast<double>(intValue));
          offset += 4;
          // Parse 64-bit double
        } else if (format == "F64") {
          if (offset + 16 > m_dataWord.size()) {
            break;
          }
          hexData = m_dataWord.substr(offset, 16);
          uint64_t intValue = std::stoull(hexData, nullptr, 16);
          double floatValue;
          std::memcpy(&floatValue, &intValue, sizeof(double)); // use memcpy due to IEEE 754. It interprats bits of
                                                               // memory. But static cast save uint64 value as double.
          m_parsedValues.push_back(floatValue);
          offset += 16;
          // Parse 32-bit unsigned int
        } else if (format == "C32") {
          if (offset + 8 > m_dataWord.size()) {
            break;
          }
          hexData = m_dataWord.substr(offset, 8);
          uint32_t intValue = std::stoul(hexData, nullptr, 16);
          m_parsedValues.push_back(static_cast<double>(intValue));
          offset += 8;
        } else {
          // Unknown data type format
          std::cerr << "Unknown format: " << format << std::endl;
        }
      }
    }
  }
  // Debug: print parsed values
  std::cout << "data: ";
  for (auto const &a : m_parsedValues) {
    std::cout << a << " ";
  }
  std::cout << std::endl;

  return m_parsedValues;
}

// std::string MessageHandler::getTimeDate() {
//   // Get current system time
//   auto t = std::time(nullptr);
//   std::tm timeInfo{};
//   localtime_s(&t, &timeInfo);
//   std::stringstream ss;
//   // Format as YYYY-MM-DD HH:MM:SS
//   ss << std::put_time(&timeInfo, "%Y-%m-%d %X");
//   return ss.str();
// }
