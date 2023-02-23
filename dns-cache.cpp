#include "dns-cache.hpp"

void DNSCache::update(const std::string& name, const std::string& ip) {
  cache[name] = ip;
}

std::string DNSCache::resolve(const std::string& name) const {
  return cache.at(name);
}