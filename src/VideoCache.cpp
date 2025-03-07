#include "VideoCache.h"

bool VideoCache::checkFile(string url, string hash) {
    if (urlMap.find(url) == urlMap.end()) {
        urlMap[url] = std::make_unique<Poco::LRUCache<std::string, int>>(cacheSize);
        urlMap[url]->add(hash, urlMap[url]->size() + 1);
        return false;
    } else if (urlMap[url]->has(hash)) {
        return true;
    } else {
        urlMap[url]->add(hash, urlMap[url]->size() + 1);
        return false;

    }
}