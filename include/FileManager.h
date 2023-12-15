#ifndef INCLUDE_FILE_SYSTEM_MANAGER_H_
#define INCLUDE_FILE_SYSTEM_MANAGER_H_


#include <algorithm>
#include <exception>
#include <iostream>
#include <sstream>
#include <utility>
#include <functional>
#include <iterator>
#include <string>
#include <vector>
#include <array>
#include <cstring>
#include <cstdlib>

#include "FileSystemObject.h"
#include "Directory.h"
#include "File.h"



class FileManager {
    using path_t = std::string;     // location_t + name_t
    using location_t = std::string;  // e.g. "/some/specific/path/"
    using name_t = std::string;     // e.g. "__filename__dirname__"; NO SPACES OR SLASHES
                                // Also regex-like matching symbols "*?|" ARE NOT ALLOWED
    using argv_t = std::vector<std::string>;
    
    typedef void(FileManager::*p_method)(const argv_t&);
    const std::map<std::string, p_method> _callback = {
        { "mkdir", &FileManager::makeDirectory },
        { "touch", &FileManager::makeFile },
        { "rm", &FileManager::remove },
        { "rename", &FileManager::rename },
        { "mv", &FileManager::move },
        { "cd", &FileManager::changeDirectory },
        { "pwd", &FileManager::printWorkingDirectory },
        { "ls", &FileManager::list },
        { "find", &FileManager::find },
        { "count", &FileManager::count },
        { "", &FileManager::help },
        { "help", &FileManager::help },
    };
private:
    
    Directory* _root;
    Directory* _current_folder;
    
    /**
     * Split comand line string into tokens
     * @param comand line string
     * @return vector of tokens
     */
    static argv_t _cmdTokenize(const std::string& cmd);
    /**
     * Split path into name_t
     * @param path
     * @return vector of name_t
     */
    static argv_t _pathTokenize(const path_t& path);
    /**
     * Wildcard path matching
     * @param path
     * @param pattern
     * @return path matches pattern ?
     */
    static bool _pathMatchWildcard(const path_t& path, const std::string& pattern);
    /**
     * Get object from the path
     * @param path
     * @return pointer to the object
     */
    FileSystemObject* _getObject(const path_t& path) const;
    /**
     * Split path into a location_t and a name_t
     * @param path
     * @return pair, containing location_t and a name_t
     */
    std::pair<location_t, name_t> _pathSplit(const path_t& path) const;
public:
    
    /**
     * Create new directory
     * @note to see al the options look at the source code
     * @param argv_t
     */
    void makeDirectory(const argv_t& argv);
    /**
     * Create new file
     * @note to see al the options look at the source code
     * @param argv_t
     */
    void makeFile(const argv_t& argv);
    
    /**
     * Remove object
     * @note to see al the options look at the source code
     * @param argv_t
     */
    void remove(const argv_t& argv);
    /**
     * Rename source to dest
     * @note to see al the options look at the source code
     * @param argv_t
     */
    void rename(const argv_t& argv);
    /**
     * Move source to dest
     * @note to see al the options look at the source code
     * @param argv_t
     */
    void move(const argv_t& argv);
    
    
    /**
     * Change current working directory
     * @note to see al the options look at the source code
     * @param argv_t
     */
    void changeDirectory(const argv_t& argv);
    /**
     * Print current working directory
     * @note to see al the options look at the source code
     * @param argv_t
     */
    void printWorkingDirectory(const argv_t& argv);
    /**
     * List all objects for a directory
     * @note to see al the options look at the source code
     * @param argv_t
     */
    void list(const argv_t& argv);
    /**
     * Find and print objects adhearing to a condition
     * @note to see al the options look at the source code
     * @param argv_t
     */
    void find(const argv_t& argv); /* use regex */
    /**
     * Count number of objects adhearing to a condition
     * @note to see al the options look at the source code
     * @param argv_t
     */
    void count(const argv_t& argv);
    
    /**
     * Display FileManager available methods
     * @note to see al the options look at the source code
     * @param argv_t
     */
    void help(const argv_t& argv);
    
    
    
    /**
     * Execude comand line string
     * @param comand line string
     */
    void exec(const std::string& cmd);
    void exec(std::string&& cmd);
    
    
    /**
     * Setter; Set current working directory to root
     */
    void setDirectory(); /* set root */
    /**
     * Setter; Set current working directory to dir
     * @param new current working directory
     */
    void setDirectory(const Directory& dir);
    /**
     * Getter;
     * @return root directory
     */
    Directory& getRootDirectory() const;
    /**
     * Getter;
     * @return current directory
     */
    Directory& getCurrentDirectory() const;
    /**
     * Recursively update location of the dir
     * @param dir to update location
     * @param parent dir to get the location from
     */
    static void updateLocation(FileSystemObject& dir, Directory& parent_dir);
    
    
    /**
     * Class constructor
     */
    FileManager();
    /**
     * Class destructor
     */
    ~FileManager();
    
    /**
     * Somewhat of a unit test
     */
    void test_Directory();
};


#endif /* INCLUDE_FILE_SYSTEM_MANAGER_H_ */
