#ifndef INCLUDE_FOLDER_H_
#define INCLUDE_FOLDER_H_

#include <iostream>
#include <utility>
#include <string>
#include <map>
#include "FileSystemObject.h"


class Directory : public FileSystemObject {
    using container_type = std::map<std::string,FileSystemObject*>;
    using container_value_type = std::pair<std::string,FileSystemObject*>;
    
    container_type _objects;
    
public:
    /* Capasity */
    
    /**
     * Check if directory contains any objects (other than itself and it's parent)
     * @return directory is empty ?
     */
    bool empty() const;
    /**
     * Count number of objects (including itself and it's parent)
     * @return number of objects
     */
    size_t count() const;
    /**
     * Count number of objects of type (including itself and it's parent)
     * @param type of objects
     * @return number of objects of specified type
     */
    size_t count(FileSystemObjectType type) const;
    
    /* Modifiers */
    
    
    /**
     * Insert a new object into directory
     * @param object to insert
     * @return pair, containing iterator to the object
     *      and true, if insertion was successfull
     */
    std::pair<container_type::iterator,bool> insert(const FileSystemObject& object); /* insert copy */
    std::pair<container_type::iterator,bool> insert(FileSystemObject&& object); /* insert assign */
    /**
     * Erase existing object
     * @param iterator to the object
     * @return iterator following last removed object
     */
    container_type::iterator erase(container_type::iterator pos);
    container_type::iterator erase(container_type::const_iterator pos);
    /**
     * Erase existing object in range [first, last)
     * @param iterator to the start of the range
     * @param iterator to the end of the range
     * @return iterator following last removed object
     */
    container_type::iterator erase(container_type::iterator first, container_type::iterator last);
    container_type::iterator erase(container_type::const_iterator first, container_type::const_iterator last);
    /**
     * Erase all objects
     */
    void clear();
    /**
     * Swap this and other directory values
     * @param other directory
     */
    void swap(Directory& other);
    
    /**
     * Clone this directory
     * @param copy of this directory
     */
    FileSystemObject& clone() const override; /* returns a copy of the object; Why is it here?? */
    
    
    /* Lookup */
    
    /**
     * Find next object by key
     * @param the key
     * @return iterator to the object,
     *      or to the end() iterator
     */
    container_type::iterator find(const std::string& name);
    container_type::iterator find(std::string&& name);
    container_type::const_iterator find(const std::string& name) const;
    container_type::const_iterator find(std::string&& name) const;
    /**
     * Check if Directory contains object with key
     * @param key
     * @return contains ?
     */
    bool contains(const std::string& name);
    
    /* Iterators */
    
    
    /**
     * Returns iterator to the first object of the Directory
     * @return iterator
     */
    container_type::iterator begin();
    container_type::const_iterator cbegin();
    container_type::const_iterator cbegin() const;
    /**
     * Returns iterator to the first object of reversed Directory
     * @return iterator
     */
    container_type::reverse_iterator rbegin();
    container_type::reverse_iterator rbegin() const;
    container_type::const_reverse_iterator crbegin();
    container_type::const_reverse_iterator crbegin() const;
    
    /**
     * Returns iterator to the last object of the Directory
     * @return iterator
     */
    container_type::iterator end();
    container_type::const_iterator cend();
    container_type::const_iterator cend() const;
    /**
     * Returns iterator to the last object of reversed Directory
     * @return iterator
     */
    container_type::reverse_iterator rend();
    container_type::reverse_iterator rend() const;
    container_type::const_reverse_iterator crend();
    container_type::const_reverse_iterator crend() const;
    
    
    /* Operator overloads */
    
    
    /**
     * DEPRECATED; Display objects inside directory
     * @param out stream
     * @param separator
     * @param type of objects to display
     * @return modified out stream
     */
    std::ostream& display(std::ostream& os, const std::string sep) const;
    std::ostream& display(std::ostream& os, const std::string sep, FileSystemObjectType type) const;
    /**
     * Return a pointer to the object mapped to a key
     * @param key
     * @return pointer to the object OR nullptr if no such object exists
     */
    FileSystemObject* operator[](const std::string& name) const; /* Get Directory objcet by name */
    FileSystemObject* operator[](std::string&& name) const; /* @return nullptr if no such object */
    
    
    /* Member functions */
    
    
    /**
     * Class constructor
     * @param name of the object
     * @param parent directory
     */
    Directory(const std::string& name, const Directory& parent_folder); /* every other directory copy constructor */
    Directory(std::string&& name, Directory&& parent_folder); /* every other directory move constructor */
    
    /**
     * Class constructor
     * @param other directory where to take values from
     */
    Directory(const Directory& other);
    Directory(Directory&& other);
    
    /**
     * Assignment operator overload
     * @param other directory
     */
    Directory& operator=(const Directory& other);
    Directory& operator=(Directory&& other);
    
    /**
     * Class constructor for the root directory
     */
    Directory(); /* root directory copy constructor */
    /**
     * Class destructor
     */
    ~Directory() override;
};

#endif /* INCLUDE_FOLDER_H_ */
