#include <iostream>

#include "dns-cache.hpp"

int main() {
  DNSCache dns_cache{};
  dns_cache.update("host1.local", "1.1.1.1");

  std::cout << dns_cache.resolve("host1.local") << std::endl;
}