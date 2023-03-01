#ifndef DNS_CACHE_H
#define DNS_CACHE_H

#include <string>
#include <unordered_map>
#include <list>
#include <mutex>

class DNSCache
{
  // type aliases
  using dns_record = std::pair<std::string, std::string>; 

  // private members
  size_t maximum_size;
  std::unordered_map<std::string, std::list<dns_record>::iterator> cache_map{};
  std::list<dns_record> cache_data{};
  mutable std::mutex cache_mutex;
public:
  void update(const std::string& name, const std::string& ip);
  std::string resolve(const std::string& name) const;
  DNSCache(size_t size);
  // static members
  static DNSCache& getInstance(size_t size);
};
#endif