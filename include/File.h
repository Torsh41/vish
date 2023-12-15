#ifndef INCLUDE_FILE_H_
#define INCLUDE_FILE_H_

// #include <>
// #include "FileSystemObject.h"
#include "../include/FileSystemObject.h"
#include "../include/Directory.h"

class Directory;

class File : public FileSystemObject {
public:
    
    /**
     * Clone this file
     * @param copy of this file
     */
    FileSystemObject& clone() const override;
    
    /* Member functions */
    
    
    /**
     * Class constructor
     * @param name of the object
     * @param parent directory
     */
    File(const std::string& name, const Directory& parent_folder); /* copy constructor */
    File(std::string&& name, Directory&& parent_folder); /* move constructor */
    
    /**
     * Class constructor
     * @param other file where to take values from
     */
    File(const File& other) = default;
    File(File&& other) = default;
    
    /**
     * Assignment operator overload
     * @param other file
     */
    File& operator=(const File& other) = default;
    File& operator=(File&& other) = default;
    
    ~File() override = default;
};


#endif /* INCLUDE_FILE_H_ */
