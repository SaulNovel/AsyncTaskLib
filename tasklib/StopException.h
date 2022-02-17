#ifndef EXCEPTION
#define EXCEPTION

#include <exception>
#include <string>

class StopException : public std::exception {
public: 
    StopException(const std::string& message = "") noexcept 
    : message_(message)
    { }        
    virtual ~StopException() = default;
    
    const char* what() const noexcept {
    	return message_.c_str();
    }

private:
    std::string message_;
};

#endif
