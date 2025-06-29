#include "../include/qemuClientObject.hpp"

int main() {
  QemuClientObject q;
  q.initialize();
  q.receiveMessage();
  return 0;
}