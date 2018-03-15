#pragma once

#include <string>
#include <exception>

class ssl_exc : public std::exception
{
public:
    explicit ssl_exc(const std::string& text );
    virtual const char* what() const noexcept override;

private:
    std::string m_error;
};

