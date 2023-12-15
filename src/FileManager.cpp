// #include "FileManager.h"
#include "../include/FileManager.h"
using path_t = std::string;
using location_t = std::string;
using name_t = std::string;
using argv_t = std::vector<std::string>;


/* * * * * * * * * * * * * * * * * * *
 *      FileManager Interface        *
 * * * * * * * * * * * * * * * * * * */

void FileManager::makeDirectory(const argv_t& argv) {
    bool verbose = false;
    argv_t directories;
    // get relevant flags
    for (auto arg = argv.begin() + 1; arg != argv.end(); arg++) {
        if ((*arg)[0] == '-') {
            if (*arg ==  "--help"
                    || *arg ==  "-h") {
                std::cout << "Usage: " << argv[0] << " [OPTION]... [DIRECTORY]...\n"
                            << "Create the DIRECTORY[ies] if they do not exist.\n\n"
                            << "OPTION[s]:\n"
                            << "\t--help, -h: display this message\n"
                            << "\t--verbose, -v: print a message for each created directory\n";
            } else if (*arg == "--verbose"
                    || *arg == "-v") {
                verbose = true;
            } else {
                std::cout << "Error: invalid option: " << *arg << '\n';
            }
        } else {
            directories.push_back(*arg);
        }
    }
    
    // make directories
    for (auto path : directories) {
        // get location and name
        std::pair<location_t, name_t> p = _pathSplit(path);
        
        // get directory from location
        Directory* dir = dynamic_cast<Directory*>(_getObject(p.first));
        if (dir == nullptr) {
            std::cerr << "Error: '" << p.first << "': No such file or directory" << std::endl;
            continue;
        }
        
        // insert new directory
        auto it_bool_pair = dir->insert(*(new Directory(p.second, *dir)));
        
        if (!it_bool_pair.second) {
            std::cerr << "Error: '" << path << "': Already exists" << std::endl;
            continue;
        }
        
        if (verbose) {
            std::cout << "Created new directory: " << path << std::endl;
        }
    }
}
void FileManager::makeFile(const argv_t& argv) {
    bool verbose = false;
    argv_t directories;
    // get relevant flags
    for (auto arg = argv.begin() + 1; arg != argv.end(); arg++) {
        if ((*arg)[0] == '-') {
            if (*arg ==  "--help"
                    || *arg ==  "-h") {
                std::cout << "Usage: " << argv[0] << " [OPTION]... [FILE]...\n"
                            << "Create the FILE[s] if they do not yet exist.\n\n"
                            << "OPTION[s]:\n"
                            << "\t--help, -h: display this message\n"
                            << "\t--verbose, -v: print a message for each created directory\n";
            } else if (*arg == "--verbose"
                    || *arg == "-v") {
                verbose = true;
            } else {
                std::cout << "Error: invalid option: " << *arg << '\n';
            }
        } else {
            directories.push_back(*arg);
        }
    }
    
    // make directories
    for (auto path : directories) {
        // get location and name
        std::pair<location_t, name_t> p = _pathSplit(path);
        
        try {
            // get directory from location
            Directory* dir = dynamic_cast<Directory*>(_getObject(p.first));
            if (dir == nullptr) {
                throw std::bad_cast();
                // std::cerr << "Error: '" << path << "': Already exists" << std::endl;
                // continue;
            }
            
            // insert new directory
            auto it_bool_pair = dir->insert(*(new File(p.second, *dir)));
            
            if (!it_bool_pair.second) {
                throw std::runtime_error("Oh No");
                // std::cerr << "Error: '" << path << "': Already exists" << std::endl;
                // continue;
            }
            
            if (verbose) {
                std::cout << "Created new directory: " << path << std::endl;
            }
        } catch (const std::runtime_error& e) {
            std::cerr << "Error: '" << path << "': Already exists" << std::endl;
        } catch (const std::bad_cast& e) {
            std::cerr << "Error: '" << p.first << "': No such file or directory" << std::endl;
        }
    }
}
void FileManager::remove(const argv_t& argv) {
    bool verbose = false;
    bool recursive = false;
    bool force = false;
    argv_t objects;
    // get relevant flags
    for (auto arg = argv.begin() + 1; arg != argv.end(); arg++) {
        if ((*arg)[0] == '-') {
            if (*arg ==  "--help"
                    || *arg ==  "-h") {
                std::cout << "Usage: " << arg[0] << " [OPTION]... [FILE]...\n"
                            << "       " << arg[0] << " [OPTION]... -r [DIRECTORY]...\n"
                            << "Remove FILE[s] and/or DIRECTORY[ies] if they exist.\n\n"
                            << "OPTION[s]:\n"
                            << "\t--help, -h: display this message\n"
                            << "\t--verbose, -v: print a message for each removed directory\n"
                            << "\t--recursive, -r, -R   remove directories and their contents recursively\n"
                            << "\t--force, -f: remove dir even if not empty\n";
            } else if (*arg == "--verbose"
                    || *arg == "-v") {
                verbose = true;
            } else if (*arg == "--recursive"
                    || *arg == "-R"
                    || *arg == "-r") {
                verbose = true;
            } else if (*arg == "--force"
                    || *arg == "-f") {
                force = true;
            } else {
                std::cout << "Error: invalid option: " << *arg << '\n';
            }
        } else {
            objects.push_back(*arg);
        }
    }
    
    for (auto path : objects) {
        // get location and name
        std::pair<location_t, name_t> p = _pathSplit(path);
        // get parent directory from path
        Directory* parent_dir = dynamic_cast<Directory*>(_getObject(p.first));
        FileSystemObject* obj = _getObject(p.first);
        if (parent_dir == nullptr || obj == nullptr) {
            std::cerr << "Error: '" << path << "': No such directory" << std::endl;
            continue;
        }
        
        if (*obj == *_root) {
            std::cerr << "Error: Unable to delete root directory" << std::endl;
            continue;
        }
        
        if (obj->getType() == FILE_SYSTEM_OBJECT_FOLDER) {
            if (!recursive) {
                std::cerr << "Error: cannot remove '" << path << "'; is a directory" << std::endl; 
                continue;
            }
            if (!dynamic_cast<Directory*>(obj)->empty() && !force) {
                std::cerr << "Error: directory '" << path << "' is not empty" << std::endl; 
                continue;
            }
        }
        
        if (verbose) {
            std::cout << "Removed object: " << path << std::endl;
        }
        
        // cleanup
        auto it = parent_dir->find(obj->getName());
        delete it->second;
        parent_dir->erase(it);
    }
}
void FileManager::move(const argv_t& argv) {
    // TODO: copy paste halp messages in makeDirectory() and removeDirectory() just like here
    bool verbose = false;
    bool force = false;
    bool update = false;
    argv_t sources;
    path_t dest = "";
    // get relevant flags
    for (auto arg = argv.begin() + 1; arg != argv.end(); arg++) {
        if ((*arg)[0] == '-') {
            if (*arg == "--force"
                    || *arg == "-f") {
                force = true;
            } else if (*arg == "--target-directory"
                    || *arg == "-t") {
                if (++arg != argv.end()) {
                    dest = *arg;
                }
            // } else if (*arg == "--update"
            //         || *arg == "-u") {
            //     update = true;
            } else if (*arg == "--verbose"
                    || *arg == "-v") {
                verbose = true;
            } else if (*arg ==  "--help"
                    || *arg ==  "-h") {
                std::cout << "Usage: mv [OPTION]... SOURCE... DIRECTORY" << '\n'
                    << "  or:  mv [OPTION]... -t DIRECTORY SOURCE..." << '\n'
                    << "Move SOURCE(s) to DIRECTORY." << '\n'
                    << "  -f, --force                  do not prompt before overwriting" << '\n'
                    << "  -t, --target-directory=DIRECTORY  move all SOURCE arguments into DIRECTORY" << '\n'
                    // << "  -u, --update                 move only when the SOURCE file is newer" << '\n'
                    // << "                                 than the destination file or when the" << '\n'
                    // << "                                 destination file is missing" << '\n'
                    << "  -v, --verbose                explain what is being done" << '\n'
                    << "  -h, --help        display this help and exit" << '\n';
            } else {
                std::cout << "Error: invalid option: " << *arg << '\n';
            }
        } else {
            sources.push_back(*arg);
        }
    }
    
    if (dest.empty()) {
        dest = sources.back();
        sources.pop_back();
    }
    
    // move all sources into dest
    for (auto path : sources) {
        std::string err_string = "";
        {
            // get location and name
            std::pair<location_t, name_t> p = _pathSplit(path);
            
            // get directory where source is located
            Directory* source_parent_dir = dynamic_cast<Directory*>(_getObject(p.first));
            if (source_parent_dir == nullptr) {
                err_string = path;
                goto err_not_exist;
            }
            // get source object
            auto it = source_parent_dir->find(p.second);
            if (it == source_parent_dir->end()) {
                err_string = path;
                goto err_not_exist;
            }
            // and dest directory
            Directory* dest_dir = dynamic_cast<Directory*>(_getObject(dest));
            if (dest_dir == nullptr) {
                err_string = dest;
                goto err_not_exist;
            }
            
            // try to insert into dest
            auto it_bool_pair = dest_dir->insert(*it->second);
            if (!it_bool_pair.second) {
                if (!force) {
                    err_string = path;
                    goto err_already_exist;
                }
                // force insertion
                dest_dir->erase(it_bool_pair.first);
                dest_dir->insert(*it->second);
            }
            
            if (verbose) {
                std::cout << "Moved object from '" << path << "' to '" << dest << "'" << std::endl;
            }
            
            // clean up
            FileSystemObject& obj_res = *it_bool_pair.first->second;
            updateLocation(obj_res, *dest_dir);
            source_parent_dir->erase(it);
            continue;
        }
        
    err_not_exist:
        std::cerr << "Error: '" << err_string << "': No such file or directory" << std::endl;
        continue;
        
    err_already_exist:
        std::cerr << "Error: '" << err_string << "': Already exists" << std::endl;
        continue;
    }
}
void FileManager::rename(const argv_t& argv) {
    // TODO: copy paste halp messages in makeDirectory() and removeDirectory() just like here
    bool verbose = false;
    bool force = false;
    bool update = false;
    argv_t sources;
    path_t dest = "";
    path_t source = "";
    // get relevant flags
    for (auto arg = argv.begin() + 1; arg != argv.end(); arg++) {
        if ((*arg)[0] == '-') {
            if (*arg == "--force"
                    || *arg == "-f") {
                force = true;
            } else if (*arg == "--target-directory"
                    || *arg == "-t") {
                if (++arg != argv.end()) {
                    dest = *arg;
                }
            } else if (*arg == "--no-target-directory"
                    || *arg == "-T") {
                if (++arg != argv.end()) {
                    sources.push_back(*arg);
                }
            // } else if (*arg == "--update"
            //         || *arg == "-u") {
            //     update = true;
            } else if (*arg == "--verbose"
                    || *arg == "-v") {
                verbose = true;
            } else if (*arg ==  "--help"
                    || *arg ==  "-h") {
                std::cout << "Usage: " << argv[0] << " [OPTION]...  SOURCE DEST" << '\n'
                    << "Rename SOURCE to DEST, or move SOURCE(s) to DIRECTORY." << '\n'
                    << "  -f, --force                  do not prompt before overwriting" << '\n'
                    << "  -v, --verbose                explain what is being done" << '\n'
                    << "  -h, --help        display this help and exit" << '\n';
            } else {
                std::cout << "Error: invalid option: " << *arg << '\n';
            }
        } else {
            sources.push_back(*arg);
        }
    }

    if (dest.empty()) {
        dest = sources.back();
        sources.pop_back();
    }
    if (sources.size() > 1) {
        std::cerr << "Error: To many arguments\n";
        return;
    } else if (sources.size() < 1) {
        std::cerr << "Error: Not enough arguments\n";
        return;
    }
    
    source = sources.back();
    
    // get location and name
    std::pair<location_t, name_t> pair_source = _pathSplit(source);
    std::pair<location_t, name_t> pair_dest = _pathSplit(dest);
    try {
        // get source location
        Directory* source_dir = dynamic_cast<Directory*>(_getObject(pair_source.first));
        if (source_dir == nullptr) { throw std::out_of_range(pair_source.first); }
        
        // get dest location
        Directory* dest_dir = dynamic_cast<Directory*>(_getObject(pair_dest.first));
        if (dest_dir == nullptr) { throw std::out_of_range(pair_dest.first); }
        
        // source and dest location must be the same
        if (*source_dir != *dest_dir) { throw std::logic_error(pair_dest.second); }
        
        // get source object iterator
        auto source_it = source_dir->find(pair_source.second);
        if (source_it == source_dir->end()) { throw std::out_of_range(source); }
        
        // check if dest already exist
        auto dest_it = dest_dir->find(pair_dest.second);
        if (dest_it != source_dir->end()) {
            if (!force) { throw std::domain_error(source); }
            // rewrite
            dest_dir->erase(dest_it);
        }
        
        // change object name
        FileSystemObject& obj = *source_it->second;
        obj.setName(pair_dest.second);
        source_dir->erase(source_it);
        dest_dir->insert(obj);
        
        // update location
        if (obj.getType() == FILE_SYSTEM_OBJECT_FOLDER) {
            updateLocation(obj, *dest_dir);
        }
        if (verbose) {
            std::cout << "Renamed object " << source << " to " << dest << std::endl;
        }
    } catch (const std::domain_error& e) {
        std::cerr << "Error: '" << e.what() << "': Already exists" << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: '" << e.what() << "': No such file or directory" << std::endl;
    } catch (const std::logic_error& e) {
        std::cerr << "Error: can not move object to " << e.what() << std::endl;
    }
}




void FileManager::changeDirectory(const argv_t& argv) {
    path_t dest;
    // get relevant flags
    for (auto arg = argv.begin() + 1; arg != argv.end(); arg++) {
        if ((*arg)[0] == '-') {
            if (*arg ==  "--help"
                    || *arg ==  "-h") {
                std::cout << "Usage: " << argv[0] << " [OPTION]... [DIRECTORY]...\n"
                            << "Change the current working directory\n\n"
                            << "\t--help, -h: display this message\n";
            } else {
                std::cout << "Error: invalid option: " << *arg << '\n';
            }
        } else {
            if (!dest.empty()) {
                std::cout << "Error: Too many arguments" << '\n';
                return;
            }
            dest = *arg;
        }
    }
    
    
    // change _current_directory
    Directory* dir = dynamic_cast<Directory*>(_getObject(dest));
    if (dir == nullptr) {
        std::cerr << "Error: '" << dest << "': No such file or directory" << std::endl;
        return;
    }
    
    _current_folder = dir;
}
void FileManager::printWorkingDirectory(const argv_t& argv) {
    for (auto arg = argv.begin() + 1; arg != argv.end(); arg++) {
        if ((*arg)[0] == '-') {
            if (*arg ==  "--help"
                    || *arg ==  "-h") {
                std::cout << "Usage: " << argv[0] << " [OPTIONS]..." << '\n'
                            << "    Print the full filename of the current working directory." << '\n'
                            << "\t--help, -h: display this message, lol\n";
            } else {
                std::cout << "Error: invalid option: " << *arg << '\n';
            }
        } else {
        }
    }
    
    std::cout << _current_folder->getPath()
                    // << '/'
                    << '\n';
}
void FileManager::list(const argv_t& argv) {
    bool all = false;
    bool recursive = false;
    bool list = false;
    bool no_format = false;
    bool show_object_type[FILE_SYSTEM_OBJECT_TYPE_COUNT] = { false };
    argv_t directories;
    
    // get relevant flags
    for (auto arg = argv.begin() + 1; arg != argv.end(); arg++) {
        if ((*arg)[0] == '-') {
            if (*arg ==  "--help"
                    || *arg ==  "-h") {
                std::cout << "Usage: " << argv[0] << " [OPTION]... [FILE]..." << '\n'
                        << "List information about the FILEs (the current directory by default)." << '\n'
                        << "Sort entries alphabetically if none of -cftuvSUX nor --sort is specified." << '\n'
                        << "  -a, --all                  do not ignore entries starting with ." << '\n'
                        << "  -R, --recursive            list subdirectories recursively" << '\n'
                        << "  -l                         use a long listing format" << '\n'
                        << "  -d, --directory            show all objects of type directory" << '\n'
                        << "  -f, --file                 show all objects of type file" << '\n'
                        << "      --no-format            list directories withour pretty formating" << '\n';
            } else if (*arg == "--all"
                    || *arg == "-a") {
                all = true;
            } else if (*arg == "--recursive"
                    || *arg == "-R") {
                recursive = true;
            } else if (*arg == "-l") {
                list = true;
            } else if (*arg == "--directory"
                    || *arg == "-d") {
                show_object_type[FILE_SYSTEM_OBJECT_FOLDER] = true;
            } else if (*arg == "--file"
                    || *arg == "-f") {
                show_object_type[FILE_SYSTEM_OBJECT_FILE] = true;
            } else if (*arg == "--no--format") {
                no_format = true;
            } else {
                std::cout << "Error: invalid option: " << *arg << '\n';
            }
        } else {
            directories.push_back(*arg);
        }
    }
    
    // default directory
    if (directories.size() == 0) {
        directories.push_back(".");
    }
    
    
    // select object name condition
    auto if_name = [all]() -> std::function<bool(const std::string&)> {
        if (all) {
            return [](const std::string& name) -> bool { return true; };
        // all except paths starting with '.'
        } else {
            return [](const std::string& obj) -> bool { return obj[0] != '.'; };
        }
    }();
    // select object type condition
    auto if_type = [show_object_type]()
                            -> std::function<bool(const FileSystemObject&)> {
        bool any_of = false;
        for (size_t i = 0; i < FILE_SYSTEM_OBJECT_TYPE_COUNT; i++) {
            any_of = any_of || show_object_type[i];
        }
        
        if (any_of) {
            return [show_object_type](const FileSystemObject& obj) -> bool {
                        return show_object_type[obj.getType()];
                    };
        // none of => gotta display every type
        } else {
            return [show_object_type](const FileSystemObject& obj) -> bool {
                        return true;
                    };
        }
    }();
    
    // list objects in all objects
    std::function<void(Directory const&, path_t const&)> loop =
            [if_type, if_name, list, recursive, no_format, &loop]
            (const Directory& dir, const path_t& path)
            -> void {
        const std::string name = dir.getName();
        const path_t path_rec = path + name + (name != "" ? "/" : "" );
        if (recursive && !no_format) { std::cout << path_rec << '\n'; }
        
        for (auto obj = dir.cbegin(); obj != dir.cend(); obj++) {
            if (!if_type(*obj->second)) { continue; }
            if (!if_name(obj->first)) { continue; }
            
            std::cout << obj->first;
            if (list) { std::cout << '\n'; }
            else { std::cout << ' '; }
            
        }
        std::cout << '\n';
        
        if (!recursive) { return; }
        for (auto obj = dir.cbegin(); obj != dir.cend(); obj++) {
            if (obj->first == ".." || obj->first == "."
                    || obj->second->getType() != FILE_SYSTEM_OBJECT_FOLDER) {
                continue;
            }
            loop(dynamic_cast<const Directory&>(*obj->second), path_rec);
        }
    };
    
    path_t path_rec = "./";
    for (path_t path : directories) {
        Directory* dir = dynamic_cast<Directory*>(_getObject(path));
        if (dir == nullptr) {
            std::cerr << "Error: No such file or directory" << std::endl;
            continue;
        }
        loop(*dir, path_rec);
    }
    std::cout << '\n';
}
void FileManager::find(const argv_t& argv) {
    bool all = false;
    bool show_object_type[FILE_SYSTEM_OBJECT_TYPE_COUNT] = { false };
    argv_t args;
    
    // get relevant flags
    for (auto arg = argv.begin() + 1; arg != argv.end(); arg++) {
        if ((*arg)[0] == '-') {
            if (*arg ==  "--help"
                    || *arg ==  "-h") {
                std::cout << "Usage: " << argv[0] << " [OPTION]... [SEARCH_DIR] [PATTERN]" << '\n'
                        << "Search for PATTERN within SEARCH_DIR, and list matches" << '\n'
                        << "  -a, --all                  do not ignore entries starting with ." << '\n'
                        << "  -d, --directory            show all objects of type directory" << '\n'
                        << "  -f, --file                 show all objects of type file" << '\n';
            } else if (*arg == "--all"
                    || *arg == "-a") {
                all = true;
            } else if (*arg == "--directory"
                    || *arg == "-d") {
                show_object_type[FILE_SYSTEM_OBJECT_FOLDER] = true;
            } else if (*arg == "--file"
                    || *arg == "-f") {
                show_object_type[FILE_SYSTEM_OBJECT_FILE] = true;
            } else {
                std::cout << "Error: invalid option: " << *arg << '\n';
            }
        } else {
            args.push_back(*arg);
        }
    }
    
    if (args.size() > 2) {
        std::cerr << "Error: To many argumants" << '\n';
        return;
    } else if (args.size() < 2) {
        std::cerr << "Error: Not enough argumants" << '\n';
        return;
    }
    
    
    // define vars
    path_t search_path = args[0];
    std::string pattern = args[1];
    Directory* search_dir = dynamic_cast<Directory*>(_getObject(search_path));
    if (search_dir == nullptr) {
        std::cerr << "Error: '" << args[0] << "': No such file or directory" << std::endl;
        return;
    }
    
    // select object name condition
    auto if_name = [all]() -> std::function<bool(const std::string&)> {
        if (all) {
            return [](const std::string& name) -> bool { return true; };
        // all except paths starting with '.'
        } else {
            return [](const std::string& obj) -> bool { return obj[0] != '.'; };
        }
    }();
    // select object type condition
    auto if_type = [show_object_type]()
                            -> std::function<bool(const FileSystemObject&)> {
        bool any_of = false;
        for (size_t i = 0; i < FILE_SYSTEM_OBJECT_TYPE_COUNT; i++) {
            any_of = any_of || show_object_type[i];
        }
        
        if (any_of) {
            return [show_object_type](const FileSystemObject& obj) -> bool {
                        return show_object_type[obj.getType()];
                    };
        // none of => gotta display every type
        } else {
            return [show_object_type](const FileSystemObject& obj) -> bool {
                        return true;
                    };
        }
    }();
    // search recursively
    std::function<void(Directory const&, path_t const&)> loop =
            [if_type, if_name, pattern, &loop]
            (const Directory& dir, const path_t& path)
            -> void {
        const std::string name = dir.getName();
        const path_t path_rec = path + name + (name != "" ? "/" : "" );
        
        for (auto obj = dir.cbegin(); obj != dir.cend(); obj++) {
            if (!if_type(*obj->second)) { continue; }
            if (!if_name(obj->first)) { continue; }
            
            if (_pathMatchWildcard(path_rec + obj->first, pattern)) {
                std::cout << path_rec << obj->first << '\n';
            }
            
        }
        
        for (auto obj = dir.cbegin(); obj != dir.cend(); obj++) {
            if (obj->first == ".." || obj->first == "."
                    || obj->second->getType() != FILE_SYSTEM_OBJECT_FOLDER) {
                continue;
            }
            loop(dynamic_cast<const Directory&>(*obj->second), path_rec);
        }
    };
    
    loop(*search_dir, search_path);
    std::cout << '\n';
}
void FileManager::count(const argv_t& argv) {
    bool all = false;
    bool show_object_type[FILE_SYSTEM_OBJECT_TYPE_COUNT] = { false };
    argv_t args;
    
    // get relevant flags
    for (auto arg = argv.begin() + 1; arg != argv.end(); arg++) {
        if ((*arg)[0] == '-') {
            if (*arg ==  "--help"
                    || *arg ==  "-h") {
                std::cout << "Usage: " << argv[0] << " [OPTION]... [SEARCH_DIR] [PATTERN]" << '\n'
                        << "Search for PATTERN within SEARCH_DIR, and list matches" << '\n'
                        << "  -a, --all                  do not ignore entries starting with ." << '\n'
                        << "  -d, --directory            show all objects of type directory" << '\n'
                        << "  -f, --file                 show all objects of type file" << '\n';
            } else if (*arg == "--all"
                    || *arg == "-a") {
                all = true;
            } else if (*arg == "--directory"
                    || *arg == "-d") {
                show_object_type[FILE_SYSTEM_OBJECT_FOLDER] = true;
            } else if (*arg == "--file"
                    || *arg == "-f") {
                show_object_type[FILE_SYSTEM_OBJECT_FILE] = true;
            } else {
                std::cout << "Error: invalid option: " << *arg << '\n';
            }
        } else {
            args.push_back(*arg);
        }
    }
    
    if (args.size() > 2) {
        std::cerr << "Error: To many argumants" << '\n';
        return;
    } else if (args.size() < 2) {
        std::cerr << "Error: Not enough argumants" << '\n';
        return;
    }
    
    
    // define vars
    path_t search_path = args[0];
    std::string pattern = args[1];
    Directory* search_dir = dynamic_cast<Directory*>(_getObject(search_path));
    if (search_dir == nullptr) {
        std::cerr << "Error: '" << args[0] << "': No such file or directory" << std::endl;
        return;
    }
    size_t count = 0;
    
    // select object name condition
    auto if_name = [all]() -> std::function<bool(const std::string&)> {
        if (all) {
            return [](const std::string& name) -> bool { return true; };
        // all except paths starting with '.'
        } else {
            return [](const std::string& obj) -> bool { return obj[0] != '.'; };
        }
    }();
    // select object type condition
    auto if_type = [show_object_type]()
                            -> std::function<bool(const FileSystemObject&)> {
        bool any_of = false;
        for (size_t i = 0; i < FILE_SYSTEM_OBJECT_TYPE_COUNT; i++) {
            any_of = any_of || show_object_type[i];
        }
        
        if (any_of) {
            return [show_object_type](const FileSystemObject& obj) -> bool {
                        return show_object_type[obj.getType()];
                    };
        // none of => gotta display every type
        } else {
            return [show_object_type](const FileSystemObject& obj) -> bool {
                        return true;
                    };
        }
    }();
    
    // print objects in all objects
    std::function<void(Directory const&, path_t const&)> loop =
            [&count, if_type, if_name, pattern, &loop]
            (const Directory& dir, const path_t& path)
            -> void {
        const std::string name = dir.getName();
        const path_t path_rec = path + name + (name != "" ? "/" : "" );
        
        for (auto obj = dir.cbegin(); obj != dir.cend(); obj++) {
            if (!if_type(*obj->second)) { continue; }
            if (!if_name(obj->first)) { continue; }
            
            if (_pathMatchWildcard(path_rec + obj->first, pattern)) {
                count++;
            }
        }
        
        for (auto obj = dir.cbegin(); obj != dir.cend(); obj++) {
            if (obj->first == ".." || obj->first == "."
                    || obj->second->getType() != FILE_SYSTEM_OBJECT_FOLDER) {
                continue;
            }
            loop(dynamic_cast<const Directory&>(*obj->second), path_rec);
        }
    };
    
    loop(*search_dir, search_path);
    std::cout << count << '\n';
}

void FileManager::help(const argv_t& argv) {
    for (auto p : _callback) {
        std::cout << p.first << " ";
    }
        std::cout << std::endl;
}







void FileManager::setDirectory() { _current_folder = _root; }
void FileManager::setDirectory(const Directory& dir) { _current_folder = &const_cast<Directory&>(dir); }
Directory& FileManager::getRootDirectory() const { return *_root; }
Directory& FileManager::getCurrentDirectory() const { return *_current_folder; }
void FileManager::updateLocation(FileSystemObject& obj, Directory& parent_dir) {
    // get new location
    const std::string location = parent_dir.getPath() + "/";
    // change the locaion
    obj.setLocation(location);
    // change all sub locations if obj is a directory
    if (obj.getType() == FILE_SYSTEM_OBJECT_FOLDER) {
        Directory& dir = dynamic_cast<Directory&>(obj);
        const std::string path = obj.getPath() + "/";
        for (auto p : dir) {
            if (p.first == ".." || p.first == ".") { continue; }
            
            p.second->setLocation(path);
            
            // recursive call
            if (p.second->getType() == FILE_SYSTEM_OBJECT_FOLDER) {
                updateLocation(*p.second, dir);
            }
        }
    }
}

void FileManager::exec(const std::string& cmd) {
    // std::cout << cmd << '\n';
    argv_t argv = _cmdTokenize(cmd);
    if (argv.size() == 0) {
        return;
    }
    
    // find callback by name
    auto it = _callback.find(argv[0]);
    if (it == _callback.end()) {
        std::cerr << "Error: " << argv[0] << ": Command not found" << '\n';
        return;
    }
    
    // callback
    (*this.*it->second)(argv); // will never do this again



    // std::cout << '\n' << ">>> ";
}
void FileManager::exec(std::string&& cmd) {
    const std::string& s = cmd;
    exec(s);
}


/* * * * * * * * * * * * * * * * * * *
 *         Private Methods           *
 * * * * * * * * * * * * * * * * * * */


argv_t FileManager::_cmdTokenize(const std::string& cmd) {
    /// split path into tokens
    const size_t size = cmd.length();
    char* ccmd = new char[size + 1];
    strcpy(ccmd, cmd.c_str());
    const char* delim = " ";
    argv_t token_container;

    // tokenize
    char* token = strtok(ccmd, delim);
    while (token) {
        // push the copied token to argv_t container
        token_container.push_back(name_t(token));
        // get next token
        token = strtok(nullptr, delim);
    }
    
    return token_container;
}
/**
 * Tokenazes a location_t string into individual name_t strings, puts them into argv_t
 * @param location to tokenize
 * @return argv_t for tokenized location
 */
argv_t FileManager::_pathTokenize(const path_t& path) {
    /// split path into tokens
    const size_t size = path.length();
    char* cpath = new char[size + 1];
    strcpy(cpath, path.c_str());
    const char* delim = "/";
    argv_t token_container;

    // tokenize
    char* token = strtok(cpath, delim);
    while (token) {
        // push the copied token to argv_t container
        token_container.push_back(name_t(token));
        // get next token
        token = strtok(nullptr, delim);
    }
    
    return token_container;
}
bool FileManager::_pathMatchWildcard(const path_t& path, const std::string& pattern) {
    /// @note source: https://www.geeksforgeeks.org/wildcard-pattern-matching/
    // check if pattern can be subdivided
    size_t pos = path.find("|");
    if (pos != std::string::npos) {
        const std::string pattern1 = pattern.substr(0, pos);
        const std::string pattern2 = pattern.substr(pos + 1, path.length() - pos - 1);
        std::cout << pattern1 << "|" << pattern2;
        return _pathMatchWildcard(path, pattern1) || _pathMatchWildcard(path, pattern2);
    }
    
    size_t sz_path = path.length();
    size_t sz_pattern = pattern.length();
    size_t idx_path = 0, idx_pattern = 0, start_index = -1, match = 0;

    while (idx_path < sz_path) {
        if (idx_pattern >= sz_pattern) {
            return true;
        }
        if (idx_pattern < sz_pattern && (pattern[idx_pattern] == '?'
                    || pattern[idx_pattern] == path[idx_path])) {
            idx_path++;
            idx_pattern++;
        // mark the current position in the pattern and the text as a proper match.
        } else if (idx_pattern < sz_pattern && pattern[idx_pattern] == '*') {
            start_index = idx_pattern;
            match = idx_path;
            idx_pattern++;
        // No match, but a previous wildcard was found.
        // Backtrack to the last '*' character position and try for a different match.
        } else if (start_index != (size_t) -1) {
            idx_pattern = start_index + 1;
            match++;
            idx_path = match;
        // Hope the next char will have something
        } else {
            idx_path++;
            idx_pattern = 0;
        }
    }

    // Consume any remaining '*' characters in the given pattern.
    while (idx_pattern < sz_pattern && pattern[idx_pattern] == '*') {
        idx_pattern++;
    }

    // iterated whole pattern ?
    return idx_pattern == sz_pattern;
}

/**
 * TODO: a better name
 * Splits string by the last '/' symbol
 * @param path - string containing a path to a directory or a file;
 * @return std::pair containing a path_t before last '/' and a name_t after the '/'
 */
std::pair<location_t, name_t> FileManager::_pathSplit(const path_t& path) const {
    argv_t tokens = _pathTokenize(path);
    if (tokens.size() <= 1) {
        if (path[0] == '/') {
            return std::make_pair("/", path);
        }
        return std::make_pair(".", path);
    }
    std::ostringstream imploded;
    std::copy(tokens.begin(), tokens.end() - 1,
            std::ostream_iterator<std::string>(imploded, "/"));
    std::pair<location_t, name_t> p = std::make_pair(
            imploded.str(),
            *(tokens.end() - 1));
    // std::cout << "TEST: _pathSplit()'" << path << "' -->> '" << p.first << "' + '" << p.second << "';" << "\n";
    return p;
}
FileSystemObject* FileManager::_getObject(const path_t& path) const {
    // special cases
    if (path == ".") {
        return _current_folder;
    } else if (path == "..") {
        // return (*_current_folder)[".."];
    }
    argv_t path_tokens = _pathTokenize(path);
    
    /// Navigate directories
    FileSystemObject* obj = path[0] == '/' ? _root : _current_folder;
    for (auto name : path_tokens) {
        // make sure obj is a directory
        if (obj->getType() != FILE_SYSTEM_OBJECT_FOLDER) { goto defer; }
        // get next object inside obj directory
        obj = dynamic_cast<Directory&>(*obj)[name];
        // make shure obj is valid
        if (obj == nullptr) { goto defer; }
    }
    
    return obj;
    
defer:  // Error handling 
    // std::cerr << "Error: No such file or directory" << std::endl;
    return nullptr;
}


/* * * * * * * * * * * * * * * * * * *
 *         Member Functions          *
 * * * * * * * * * * * * * * * * * * */

FileManager::FileManager():
    _root(new Directory()),
    _current_folder(_root) {
// #define TEST_TEST_TEST

#ifdef TEST_TEST_TEST
    test_Directory();
#endif
}
FileManager::~FileManager() {
    delete _root;
}

#ifdef TEST_TEST_TEST
#define TEST_TEST_TEST


#include <cassert>
void FileManager::test_Directory() {
    std::cout << "Starting tests\n";
    std::cout << "TEST: Directory::empty \n";
    assert(_root->empty() == true);
    
    std::cout << "TEST: insert lvalue directory\n";
    Directory* dir1 = new Directory(std::string("aaa"), *_root);
    std::cout << "    :lvalue directory created\n";
    _root->insert(*dir1);
    delete dir1;
    std::cout << "TEST: insert rvalue directory\n";
    _root->insert(*(new Directory(std::string("bbb"), *_root)));
    std::cout << "TEST: insert with existing name\n";
    auto res = _root->insert(*(new Directory(std::string("bbb"), *_root)));
    assert(res.second == false);
    std::cout << "TEST: insert lvalue file\n";
    File* file1 = new File(std::string("ggg"), *_root);
    _root->insert(*file1);
    std::cout << "TEST: insert rvalue file\n";
    _root->insert(*(new File(std::string("fff"), *_root)));
    
    std::cout << "TEST: Directory::empty\n";
    assert(_root->empty() == false);
    std::cout << "TEST: Directory::objectCount\n";
    assert(_root->count() == 6);
    std::cout << "TEST: Directory::objectCount, with type speified\n";
    assert(_root->count(FILE_SYSTEM_OBJECT_FOLDER) == 4);
    assert(_root->count(FILE_SYSTEM_OBJECT_FILE) == 2);
    
    std::cout << "TEST: Directory::display\n";
    _root->display(std::cout, " ");
    
    std::cout << "TEST: Directory::objectCount, with type speified\n";
    std::cout << "TEST: Directory::objectCount, with type speified\n";
    std::cout << "TEST: Directory::objectCount, with type speified\n";
    std::cout << "TEST: Directory::objectCount, with type speified\n";
    
    std::cout << "Completing tests\n";
}


#endif


