#pragma once

#include <mutex>
#include <map>
#include <memory>


class Instance
{
public:
    Instance(const Instance&) = delete;
    Instance& operator = (const Instance&) = delete;

    ~Instance();

    static Instance& get();
private:
    Instance();
};

