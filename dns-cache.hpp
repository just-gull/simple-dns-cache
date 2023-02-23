#ifndef DNS_CACHE_H
#define DNS_CACHE_H

#include <string>
#include <unordered_map>

class DNSCache
{
  std::unordered_map<std::string, std::string> cache{};
public:
  void update(const std::string& name, const std::string& ip);
  std::string resolve(const std::string& name) const;
};
#endif