// #include "Directory.h"
#include "../include/Directory.h"
using container_type = std::map<std::string,FileSystemObject*>;
using container_value_type = std::pair<std::string,FileSystemObject*>;


/*
    
int Directory::appendObject(FileSystemObjectParams& param) {
    // check whether _objects contains a FileSystemObject with _name == param.name
    auto it = std::find_if(_objects.begin(), _objects.end(),
            [param](std::pair<std::string, FileSystemObject*>&& p) -> bool {
                return (p.first == param.name);
            }
    );
    
    if (it != _objects.end())
        return 1;

    // append new file system object
    _objects.emplace(std::pair<std::string, FileSystemObject*>(param.name, _allocateFileSystemObject(param)));
    return 0;
}
int Directory::copyAppendObject(FileSystemObject*& other) {
    if (this == other)
        return 15;
    
    std::string other_name = other->getName();
    // check whether _objects contains a FileSystemObject with _name == param.name
    auto it = std::find_if(_objects.begin(), _objects.end(),
            [other_name](std::pair<std::string, FileSystemObject*>&& p) -> bool {
                return (p.first == other_name);
            }
    );
    
    if (it != _objects.end())
        return 1;
    
    struct FileSystemObjectParams param;
    param.name = other_name;
    param.type = other->getType();

    // append new file system object
    _objects.emplace(std::pair<std::string, FileSystemObject*>(param.name, _allocateFileSystemObject(param)));
    return 0;
}
int Directory::removeObject(FileSystemObjectParams& param) {
    // check whether _objects contains a FileSystemObject with _name == param.name
    auto it = std::find_if(_objects.begin(), _objects.end(),
            [param](std::pair<std::string, FileSystemObject*>&& p) -> bool {
                return (p.first == param.name);
            }
    );
    if (it == _objects.end()) {
        return 1;
    }
    // check whether objects contains anything inside
    constexpr int this_and_prev_folders_count = 2;
    if (it->second->getType() == FILE_SYSTEM_OBJECT_FOLDER) {
        if (dynamic_cast<Directory*>(it->second)->countObjects() > this_and_prev_folders_count) {
            return 2;
        }
    }
    
    _deleteFileSystemObject(it->second);
    _objects.erase(it);
    return 0;
}
void Directory::clearObject() {
    for (auto it = _objects.begin(); it != _objects.end(); ++it) {
        if (it->first == "." || it->first == "..")
            continue;
        // _deleteFileSystemObject(it->second);
    }
    _objects.clear();
}
int Directory::getObject(FileSystemObjectParams& param, Directory*& folder) {
    // check whether _objects contains a FileSystemObject with _name == param.name
    auto it = std::find_if(_objects.begin(), _objects.end(),
            [param](std::pair<std::string, FileSystemObject*>&& p) -> bool {
                return (p.first == param.name);
            }
    );
    
    if (it == _objects.end())
        return 1;
    if (it->second->getType() != param.type)
        return 2;
    
    folder = dynamic_cast<Directory*>(it->second);
    return 0;
}

*/

/* * * * * * * * * * * * * * * * * * *
 *            Capacity               *
 * * * * * * * * * * * * * * * * * * */

bool Directory::empty() const {
    constexpr size_t special_object_count = 2;
    if (_objects.size() == special_object_count)
        return true;
    return false;
}
size_t Directory::count() const {
    return _objects.size();
}
size_t Directory::count(FileSystemObjectType type) const {
    return std::count_if(_objects.begin(), _objects.end(),
                [type](container_value_type obj) -> bool {
                    return obj.second->getType() == type;
                });
}

/* * * * * * * * * * * * * * * * * * *
 *           Modifiers               *
 * * * * * * * * * * * * * * * * * * */

std::pair<container_type::iterator,bool> Directory::insert(const FileSystemObject& object) {
    return _objects.emplace(object.getName(), &object.clone());
}
std::pair<container_type::iterator,bool> Directory::insert(FileSystemObject&& object) {
    std::cout << "Hey";
    return _objects.emplace(object.getName(), &object);
    std::cout << "Hey";
}
container_type::iterator Directory::erase(container_type::iterator pos) { return _objects.erase(pos); }
container_type::iterator Directory::erase(container_type::const_iterator pos) { return _objects.erase(pos); }
container_type::iterator Directory::erase(container_type::iterator first, container_type::iterator last) {
    return _objects.erase(first, last);
}
container_type::iterator Directory::erase(container_type::const_iterator first, container_type::const_iterator last) {
    return _objects.erase(first, last);
}
void Directory::clear() {
    container_type::iterator it = _objects.begin();
    for ( /**/ ; it != _objects.end(); ++it) {
        if (it->second->getType() == FILE_SYSTEM_OBJECT_FOLDER)
            if (it->first != "." && it->first != "..")
                dynamic_cast<Directory*>(it->second)->clear();
        delete it->second;
    }
    
    _objects.clear();
}
void Directory::swap(Directory& other) {
    _objects.swap(other._objects);
    FileSystemObject::swap(other);
}

FileSystemObject& Directory::clone() const {
    FileSystemObject* p_copy = new Directory(*this);
    return *p_copy;
}


/* * * * * * * * * * * * * * * * * * *
 *             Lookup                *
 * * * * * * * * * * * * * * * * * * */

container_type::iterator Directory::find(const std::string& name) { return _objects.find(name); }
container_type::iterator Directory::find(std::string&& name) { return _objects.find(name); }
container_type::const_iterator Directory::find(const std::string& name) const { return _objects.find(name); }
container_type::const_iterator Directory::find(std::string&& name) const { return _objects.find(name); }

std::ostream& Directory::display(std::ostream& os, const std::string sep) const {
    for (auto obj : _objects)
        os << obj.first << sep;
    return os;
}
std::ostream& Directory::display(std::ostream& os,
                            const std::string sep,
                            const FileSystemObjectType type) const {
    for (auto obj : _objects)
        if (obj.second->getType() == type)
            os << obj.first << sep;
    return os;
}
FileSystemObject* Directory::operator[](const std::string& name) const {
    container_type::const_iterator it = find(name);
    if (it == cend())
        return nullptr;
    return it->second;
}
FileSystemObject* Directory::operator[](std::string&& name) const {
    container_type::const_iterator it = find(name);
    if (it == cend())
        return nullptr;
    return it->second;
}

/* * * * * * * * * * * * * * * * * * *
 *            Iterators              *
 * * * * * * * * * * * * * * * * * * */


container_type::iterator Directory::begin() { return _objects.begin(); }
container_type::reverse_iterator Directory::rbegin() { return _objects.rbegin(); }
// container_type::reverse_iterator Directory::rbegin() const { return _objects.rbegin(); }
container_type::const_iterator Directory::cbegin() { return _objects.cbegin(); }
container_type::const_iterator Directory::cbegin() const { return _objects.cbegin(); }
container_type::const_reverse_iterator Directory::crbegin() { return _objects.crbegin(); }
// container_type::const_reverse_iterator Directory::crbegin() const { return _objects.crbegin(); }

container_type::iterator Directory::end() { return _objects.end(); }
container_type::reverse_iterator Directory::rend() { return _objects.rend(); }
// container_type::reverse_iterator Directory::rend() const { return _objects.rend(); }
container_type::const_iterator Directory::cend() { return _objects.cend(); }
container_type::const_iterator Directory::cend() const { return _objects.cend(); }
container_type::const_reverse_iterator Directory::crend() { return _objects.crend(); }
// container_type::const_reverse_iterator Directory::crend() const { return _objects.crend(); }


/* * * * * * * * * * * * * * * * * * *
 *         Member functions          *
 * * * * * * * * * * * * * * * * * * */

// Directory::Directory(const std::string& name):
//     FileSystemObject(FileSystemObjectParams(FILE_SYSTEM_OBJECT_FOLDER, name, "")) {
//     _objects.emplace(std::string("."), this);
//     _objects.emplace(std::string(".."), this);
// }
Directory::Directory(const std::string& name, const Directory& parent_folder):
    FileSystemObject(FileSystemObjectParams(FILE_SYSTEM_OBJECT_FOLDER, name, parent_folder.getPath())) {
    _objects.emplace(std::string("."), this);
    _objects.emplace(std::string(".."), &dynamic_cast<FileSystemObject&>(
                                            const_cast<Directory&>(parent_folder)));
}
Directory::Directory(const Directory& other): FileSystemObject(other._getParams()) {
    for (container_value_type obj : other._objects) {
        if (obj.first != "." && obj.first != "..") {
            // deep copy
            _objects.emplace(obj.first, &obj.second->clone());
        } else {
            // shallow copy
            _objects.emplace(obj.first, obj.second);
        }
    }
}
Directory& Directory::operator=(const Directory& other) {
    if (*this == other)
        return *this;

    Directory temp(other);
    _objects.swap(temp._objects);
    return *this;
}


// Directory::Directory(std::string&& name):
//     FileSystemObject(FileSystemObjectParams(FILE_SYSTEM_OBJECT_FOLDER, name, "")) {
//     _objects.emplace(std::string("."), this);
//     _objects.emplace(std::string(".."), this);
// }
Directory::Directory(std::string&& name, Directory&& parent_folder):
    FileSystemObject(FileSystemObjectParams(FILE_SYSTEM_OBJECT_FOLDER, name, parent_folder.getPath())) {
    _objects.emplace(std::string("."), this);
    _objects.emplace(std::string(".."), &dynamic_cast<FileSystemObject&>(
                                            const_cast<Directory&>(parent_folder)));
}
Directory::Directory(Directory&& other): FileSystemObject(other._getParams()) {
    _objects.swap(other._objects);
}

Directory& Directory::operator=(Directory&& other) {
    if (*this == other)
        return *this;

    _objects.swap(other._objects);
    return *this;
}

Directory::Directory():
    FileSystemObject(FileSystemObjectParams(FILE_SYSTEM_OBJECT_FOLDER, "", "/")) {
    _objects.emplace(std::string("."), this);
    _objects.emplace(std::string(".."), this);
}
Directory::~Directory() {
    for (auto obj : _objects)
        if (obj.first != "." && obj.first != "..")
            delete obj.second;

    _objects.clear();
}


