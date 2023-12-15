// #include "File.h"
#include "../include/File.h"



FileSystemObject& File::clone() const {
    FileSystemObject* p_copy = new File(*this);
    return *p_copy;
}


/* * * * * * * * * * * * * * * * * * *
 *         Member functions          *
 * * * * * * * * * * * * * * * * * * */

File::File(const std::string& name, const Directory& parent_folder):
    FileSystemObject(FileSystemObjectParams(FILE_SYSTEM_OBJECT_FILE, name, parent_folder.getName())) {
}


File::File(std::string&& name, Directory&& parent_folder):
    FileSystemObject(FileSystemObjectParams(FILE_SYSTEM_OBJECT_FILE, name, parent_folder.getName())) {
}

