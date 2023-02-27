#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>

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
  DNSCache::default_maximum_size = 50;
  DNSCache& dns_cache = DNSCache::getInstance();
  
  std::vector<std::thread> filler_threads, reader_threads, writer_threads;
  std::cout << "Loading DNS Cache" << std::endl;
  for (int i = 0; i < 25; ++i) {
      filler_threads.emplace_back([&dns_cache, i]() {
        dns_cache.update("host" + std::to_string(i) + ".local", "1.1.1." + std::to_string(i));
      });
  }
  for (auto& t : filler_threads) { t.join(); }
  std::cout << "DNS Cache is loaded" << std::endl;
  std::cout << "Testing parallel reads and writes" << std::endl;
  for (int i = 0; i < 25; ++i) {
      reader_threads.emplace_back([&dns_cache, i]() {
        volatile auto host_address = dns_cache.resolve("host" + std::to_string(i) + ".local");
      });
  }
  for (int i = 25; i < 50; ++i) {
      writer_threads.emplace_back([&dns_cache, i]() {
        dns_cache.update("host" + std::to_string(i) + ".local", "1.1.1." + std::to_string(i));
      });
  }
  for (auto& t : reader_threads) { t.join(); }
  for (auto& t : writer_threads) { t.join(); }
  std::cout << "Parallel reads and writes are tested" << std::endl;
  
  // checking cache size and dns record update
  dns_cache.update("host1.local", "1.1.1.1");
  for (int i{ 50 }; i < 75; i++) {
    dns_cache.update("host" + std::to_string(i) + ".local", "1.1.1." + std::to_string(i));
  }
  // host35.local should be in the cache
  print_host_address(dns_cache, "host35.local");
  // host5.local should not be in the cache
  print_host_address(dns_cache, "host5.local");
  // host1.local should still be in the cache because it was updated and moved to the beginning of the cache
  print_host_address(dns_cache, "host1.local");

}