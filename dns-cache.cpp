#include "dns-cache.hpp"

void DNSCache::update(const std::string& name, const std::string& ip) {
  const std::lock_guard<std::mutex> lock(cache_mutex);

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
  const std::lock_guard<std::mutex> lock(cache_mutex);
  
  auto record = cache_map.at(name);
  return record->second;
}

DNSCache::DNSCache(size_t size) : maximum_size{ size } {}

size_t DNSCache::default_maximum_size = 1'000'000;

DNSCache& DNSCache::getInstance() {
  static DNSCache instance{ DNSCache::default_maximum_size };
  return instance;
}