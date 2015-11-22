#ifndef TILED_EXCEPTIONS_H_
#define TILED_EXCEPTIONS_H_

#include <string>

namespace tiled {

class BaseException {
  public:
    BaseException(const char * fmt, ...); 
    virtual ~BaseException() throw() {}

    inline virtual const char * what() const throw() {
        return reason_.c_str();
    }

  private:
    std::string reason_;
};

template<class T, typename ...Args>
void AssertCondition(bool condition, Args... args) {
    if (!condition) {
        throw T(std::forward<Args>(args)...);
    }
}

} // namespace tiled

#endif // TILED_EXCEPTIONS_H_