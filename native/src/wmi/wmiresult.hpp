/**
  *
  * WMI
  * @author Thomas Sparber (2016)
  *
 **/

#ifndef WMIRESULT_HPP
#define WMIRESULT_HPP

#include <map>
#include <string>
#include <vector>

namespace Wmi {

    class WmiResult {

    public:
        WmiResult() :
                result() {}

        void set(std::size_t index, std::wstring name, const std::wstring &value);

        std::vector<std::map<std::wstring, std::wstring> >::iterator begin() {
            return result.begin();
        }

        std::vector<std::map<std::wstring, std::wstring> >::iterator end() {
            return result.end();
        }

        std::vector<std::map<std::wstring, std::wstring> >::const_iterator cbegin() const {
            return result.cbegin();
        }

        std::vector<std::map<std::wstring, std::wstring> >::const_iterator cend() const {
            return result.cend();
        }

        std::size_t size() const {
            return result.size();
        }

        bool extract(std::size_t index, const std::string &name, std::wstring &out) const;

        bool extract(std::size_t index, const std::string &name, std::string &out) const;

        bool extract(std::size_t index, const std::string &name, int &out) const;

        bool extract(std::size_t index, const std::string &name, bool &out) const;

        bool extract(std::size_t index, const std::string &name, uint64_t &out) const;

        bool extract(std::size_t index, const std::string &name, uint8_t &out) const;

        bool extract(std::size_t index, const std::string &name, uint32_t &out) const;

        bool extract(std::size_t index, const std::string &name, uint16_t &out) const;

        bool extract(std::size_t index, const std::string &name, std::vector<std::wstring> &out) const;

        bool extract(std::size_t index, const std::string &name, std::vector<std::string> &out) const;

        bool extract(std::size_t index, const std::string &name, std::vector<int> &out) const;

        bool extract(std::size_t index, const std::string &name, std::vector<bool> &out) const;

        bool extract(std::size_t index, const std::string &name, std::vector<uint64_t> &out) const;

        bool extract(std::size_t index, const std::string &name, std::vector<uint32_t> &out) const;

        bool extract(std::size_t index, const std::string &name, std::vector<uint16_t> &out) const;

        bool extract(std::size_t index, const std::string &name, std::vector<uint8_t> &out) const;

    private:
        std::vector<std::map<std::wstring, std::wstring> > result;

    }; //end class WmiResult

}; //end namespace Wmi

#endif //WMIRESULT_HPP