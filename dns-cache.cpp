#include "dns-cache.hpp"

#include <stdexcept>

void DNSCache::update(const std::string& name, const std::string& ip) {
  const std::lock_guard<std::shared_mutex> lock(cache_mutex);

  // checking if a record with given name is already exists  
  if (auto map_record = cache_map.find(name); map_record != cache_map.end()) {
    // removing it before inserting so it will be inserted at the beginning of the cache
    cache_data.erase(map_record->second);
  }

  // checking if the cache is full
  if (cache_data.size() == maximum_size) {
    // removing the last element from cache
    auto last_record = cache_data.back();
    cache_data.pop_back();
    cache_map.erase(last_record.first);
  }
  
  // adding a new record to the cache
  cache_data.push_front(std::make_pair(name, ip));
  cache_map[name] = cache_data.begin();
}

std::string DNSCache::resolve(const std::string& name) const {
  const std::shared_lock<std::shared_mutex> lock(cache_mutex);
  
  auto record = cache_map.at(name);
  return record->second;
}

DNSCache::DNSCache(size_t size) : maximum_size{ size } {}

DNSCache& DNSCache::getInstance(size_t size) {
  static DNSCache instance{ size };
  if (size == instance.maximum_size) {
    return instance;
  } else {
    throw std::runtime_error{ "Requested DNS Cache with size " + std::to_string(size) + " but existing instance has size " + std::to_string(instance.maximum_size) };
  }
}