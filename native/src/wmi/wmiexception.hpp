/**
  *
  * WMI
  * @author Thomas Sparber (2016)
  *
 **/

#ifndef WMIEXCEPTION_HPP
#define WMIEXCEPTION_HPP

#include <sstream>
#include <string>
#include <utility>

namespace Wmi {

    struct WmiException {

        WmiException(std::string str_errorMessage, long l_errorCode) :
                errorMessage(std::move(str_errorMessage)),
                errorCode(l_errorCode) {}

        std::string hexErrorCode() const {
            std::stringstream ss;
            ss << "0x" << std::hex << errorCode;
            return ss.str();
        }

        std::string errorMessage;

        long errorCode;

    }; //end struct WmiException

} //end namespace Wmi

#endif //WMIEXCEPTION_HPP