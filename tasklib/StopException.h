#ifndef EXCEPTION
#define EXCEPTION

#include <exception>
#include <string>

class StopException : public std::exception {
public: 
    StopException(const std::string& message = "") noexcept 
    : m_message_(message)
    { }        
    virtual ~StopException() = default;
    
    const char* what() const noexcept {
    	return m_message_.c_str();
    }

private:
    std::string m_message_;
};

#endif
