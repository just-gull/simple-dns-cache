#include <iostream>
#include <stdexcept>

#include "dns-cache.hpp"

static void print_host_address(const DNSCache& dns_cache, const std::string& host_name) {
  try {
    auto host_address = dns_cache.resolve(host_name);
    std::cout << host_name << " - " << host_address << std::endl;
  } catch (const std::exception&) {
    std::cout << host_name << " is not found in the cache." << std::endl;
  }
}

int main() {
  DNSCache dns_cache{ 5 };
  
  // checking simple update and resolve
  dns_cache.update("host1.local", "1.1.1.1");
  print_host_address(dns_cache, "host1.local");

  // checking cache size
  for (int i{ 2 }; i < 15; i++) {
    dns_cache.update("host" + std::to_string(i) + ".local", "1.1.1." + std::to_string(i));
  }
  // host12.local should be in the cache
  print_host_address(dns_cache, "host12.local");
  // host5.local should not be in the cache
  print_host_address(dns_cache, "host5.local");

  // updating existing record so it moves to the beginning of the cache
  dns_cache.update("host12.local", "1.1.1.12");
  for (int i{ 15 }; i < 19; i++) {
    dns_cache.update("host" + std::to_string(i) + ".local", "1.1.1." + std::to_string(i));
  }
  // host12 still should be in the cache but host14 should not be
  print_host_address(dns_cache, "host12.local");
  print_host_address(dns_cache, "host14.local");
  
}