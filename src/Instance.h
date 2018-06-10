#pragma once

#include <mutex>
#include <map>
#include <memory>


class Instance
{
public:
    ~Instance();

    static Instance& get();
private:
    Instance();

    void lock(bool lock, int lock_id);
    static void callback(int mode, int n, const char *file, int line);

private:
    std::map< int, std::unique_ptr<std::mutex> > m_locks;
};

