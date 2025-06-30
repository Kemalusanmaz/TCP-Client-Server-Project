#include "../include/clientObject.hpp"

int main() {
  ClientObject clientObj;
  clientObj.initialize();
  clientObj.receiveMessage();
  return 0;
}