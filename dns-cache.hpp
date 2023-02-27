#ifndef DNS_CACHE_H
#define DNS_CACHE_H

#include <string>
#include <unordered_map>
#include <list>
#include <mutex>

typedef typename std::pair<std::string, std::string> dns_record;

class DNSCache
{
  size_t maximum_size;
  std::unordered_map<std::string, std::list<dns_record>::iterator> cache_map{};
  std::list<dns_record> cache_data{};
  mutable std::mutex cache_mutex;
public:
  void update(const std::string& name, const std::string& ip);
  std::string resolve(const std::string& name) const;
  DNSCache(size_t size);
};
#endif