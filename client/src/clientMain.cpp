#include "../include/clientObject.hpp"

int main() {
  ClientObject q;
  q.initialize();
  q.receiveMessage();
  return 0;
}