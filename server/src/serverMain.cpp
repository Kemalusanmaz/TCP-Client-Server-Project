#include "../include/serverObject.hpp"

int main() {
  ServerObject serverObj;
  serverObj.initialize();
  serverObj.transmitMessage();
  return 0;
}