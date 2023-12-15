// #include "FileSystemObject.h"
#include "../include/FileSystemObject.h"
// #include "FileSystemObject.h"


FileSystemObjectType FileSystemObject::getType() const { return _type; }
std::string FileSystemObject::getName() const { return _name; }
std::string FileSystemObject::getLocation() const { return _location; }
std::string FileSystemObject::getPath() const { return getLocation() + getNameQuoted(); }
std::string FileSystemObject::getNameQuoted() const { return (_containsSpecialChars(_name) ? _wrapQuotes(_name) : _name); }

void FileSystemObject::setName(const std::string& name) { _name = name; }
void FileSystemObject::setName(std::string&& name) { std::swap(_name, name); }
void FileSystemObject::setLocation(const std::string& location) { _location = location; }
void FileSystemObject::setLocation(std::string&& location) {  std::swap(_location, location); }
void FileSystemObject::_setLastModified(const time_t& time) { _last_modified = time; }
void FileSystemObject::_setType(const FileSystemObjectType type) { _type = type; }

void FileSystemObject::_setQuoteSymbol(const std::string& str) {
    _quote_symbol = (str.find('"') == std::string::npos ? '\'' : '"');
}
bool FileSystemObject::_containsSpecialChars(const std::string& str) const {
    return std::regex_match(str, _special_chars);
}
std::string FileSystemObject::_wrapQuotes(const std::string& str) const {
    return _quote_symbol + str + _quote_symbol;
}

FileSystemObjectParams FileSystemObject::_getParams() const {
    FileSystemObjectParams p {
        _type, // type;
        _name, // name;
        _location, // location;
        _last_modified // last_modified;
    };
    return p;
}
void FileSystemObject::swap(FileSystemObject& other) {
    std::swap(_name, other._name);
    std::swap(_location, other._location);
    std::swap(_last_modified, other._last_modified);
    std::swap(_type, other._type);
    std::swap(_name, other._name);
}
// FileSystemObject& FileSystemObject::clone() const {
//     return *(new FileSystemObject(*this));
// }


FileSystemObject::FileSystemObject(const FileSystemObjectParams& param) {
    _name = param.name;
    _location = param.location;
    _last_modified = std::time(0);
    _type = param.type;
    _setQuoteSymbol(_name);
}
FileSystemObject::FileSystemObject(const FileSystemObject& other) {
    _name = other._name;
    _location = other._location;
    _last_modified = std::time(0);
    _type = other._type;
    _setQuoteSymbol(_name);
}
FileSystemObject& FileSystemObject::operator=(const FileSystemObject& other) {
    if (this == &other)
        return *this;
    
    _name = other._name;
    _location = other._location;
    _last_modified = std::time(0);
    _type = other._type;
    
    return *this;
}

FileSystemObject::FileSystemObject(FileSystemObjectParams&& param) {
    std::swap(_name, param.name);
    std::swap(_location, param.location);
    _last_modified = std::time(0);
    std::swap(_type, param.type);
    _setQuoteSymbol(_name);
}
FileSystemObject::FileSystemObject(FileSystemObject&& other) {
    std::swap(_name, other._name);
    std::swap(_location, other._location);
    _last_modified = std::time(0);
    std::swap(_type, other._type);
    _setQuoteSymbol(_name);
}
FileSystemObject& FileSystemObject::operator=(FileSystemObject&& other) {
    if (this == &other) // can this happen at all?
        return *this;
    
    std::swap(_name, other._name);
    std::swap(_location, other._location);
    _last_modified = std::time(0);
    std::swap(_type, other._type);
    _setQuoteSymbol(_name);
    
    return *this;
}

bool operator==(const FileSystemObject& lhs, const FileSystemObject& rhs) {
    return lhs._name == rhs._name && lhs._location == lhs._location;
}
bool operator!=(const FileSystemObject& lhs, const FileSystemObject& rhs) {
    return lhs._name != rhs._name || lhs._location != lhs._location;
}


FileSystemObjectParams::FileSystemObjectParams(
                const FileSystemObjectType& type,
                const std::string& name,
                const std::string& location,
                const std::time_t& last_modified) {
    this->name = name;
    this->location = location;
    this->type = type;
    this->last_modified = last_modified;
}
FileSystemObjectParams::FileSystemObjectParams(
        FileSystemObjectType&& type,
        std::string&& name,
        std::string&& location,
        std::time_t&& last_modified) {
    std::swap(this->name, name);
    std::swap(this->location, location);
    std::swap(this->type, type);
    std::swap(this->last_modified, last_modified);
}
