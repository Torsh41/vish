// TODO: change most strings to const
#ifndef INCLUDE_FILE_SYSTEM_OBJECT_H_
#define INCLUDE_FILE_SYSTEM_OBJECT_H_

#include <regex>
#include <string>
#include <ctime>
#include <utility> // std::swap

static const char* FileSystemObjectTypeName[] = {
    "Folder",
    "File"
};
enum FileSystemObjectType {
    FILE_SYSTEM_OBJECT_FOLDER,
    FILE_SYSTEM_OBJECT_FILE,
    FILE_SYSTEM_OBJECT_TYPE_COUNT
};

struct FileSystemObjectParams {
protected:
    // FileSystemObjectParams();
public:
    FileSystemObjectType type;
    std::string name;
    std::string location;
    std::time_t last_modified;
    
    FileSystemObjectParams(const FileSystemObjectType& type, const std::string& name,
                const std::string& location, const std::time_t& last_modified = time(0));
    FileSystemObjectParams(const FileSystemObjectParams& other) = default;
    FileSystemObjectParams& operator=(const FileSystemObjectParams& other) = default;
    
    FileSystemObjectParams(FileSystemObjectType&& type, std::string&& name,
                std::string&& location, std::time_t&& last_modified = time(0));
    FileSystemObjectParams(FileSystemObjectParams&& other) = default;
    FileSystemObjectParams& operator=(FileSystemObjectParams&& other) = default;
    
    ~FileSystemObjectParams() = default;
};

class FileSystemObject {
protected:
    FileSystemObjectType _type;
    std::string _name;
    std::string _location;
    std::time_t _last_modified;
    
    const std::regex _special_chars = std::regex("[/\\\"' ]");
    /* const */ char _quote_symbol;
    /**
     * Calculate if string contains any special characters
     * @param string
     * @return contains special chars ?
     */
    bool _containsSpecialChars(const std::string& str) const;
    
    /**
     * Wrap stirng in quotes
     * @return wrapped string
     */
    std::string _wrapQuotes(const std::string& str) const;
    
    /**
     * Setter; check string for illigal characters
     * @param object name
     */
    void _setQuoteSymbol(const std::string& str);
    /**
     * Setter; set object last modified date
     * @param date
     */
    void _setLastModified(const time_t& time);
    /**
     * Setter; set object type
     * @param type
     */
    void _setType(const FileSystemObjectType type);
    /**
     * Generate params for this object
     * @return params struct
     */
    FileSystemObjectParams _getParams() const;
    /**
     * Swap this and other object values
     * @param other object
     */
    void swap(FileSystemObject& other);
public:
    /**
     * Clone this object
     * @param copy of this object
     */
    virtual FileSystemObject& clone() const = 0; /* returns a copy of the object; Why is it here?? */
    
    /**
     * Getter; get object type
     * @return object type
     */
    FileSystemObjectType getType() const;
    /**
     * Getter; get name
     * @return name
     */
    std::string getName() const;
    /**
     * Getter; get location
     * @return location
     */
    std::string getLocation() const;
    /**
     * Getter; get path
     * @return path
     */
    std::string getPath() const; // location + name
    /**
     * Getter; get name wrapped in quotes
     * @return name
     */
    std::string getNameQuoted() const;
    
    /**
     * Setter; set object name
     * @param new name
     */
    void setName(const std::string& name);
    void setName(std::string&& name);
    /**
     * Setter; set object location
     * @param new location
     */
    void setLocation(const std::string& location);
    void setLocation(std::string&& location);
    
    /**
     * Class constructor
     * @param struct containing nessesary params
     */
    FileSystemObject(const FileSystemObjectParams& param);
    FileSystemObject(FileSystemObjectParams&& param);
    
    /**
     * Class constructor
     * @param other object to take values feom
     */
    FileSystemObject(const FileSystemObject& other); /* copy constructor */
    FileSystemObject(FileSystemObject&& other); /* move constructor */
    
    /**
     * Assignment operator overload
     * @param other directory
     */
    FileSystemObject& operator=(const FileSystemObject& other); /* copy assignment */
    FileSystemObject& operator=(FileSystemObject&& other); /* move assignment */
    
    /**
     * Compares if lhs elemet is equal to rhs element
     * Specificly checks _name and _location
     * @return are equal?
     */
    friend bool operator==(const FileSystemObject& lhs, const FileSystemObject& rhs);
    /**
     * Compares if lhs elemet is not equal to rhs element
     * Specificly checks _name and _location
     * @return are not equal?
     */
    friend bool operator!=(const FileSystemObject& lhs, const FileSystemObject& rhs);
    
    /**
     * Class destructor;
     */
    virtual ~FileSystemObject() = default;
};


#endif /* INCLUDE_FILE_SYSTEM_OBJECT_H_ */
