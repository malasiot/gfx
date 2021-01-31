#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <regex>
#include <map>

// A class of key/value pairs of strings. 

class Dictionary
{
	public:


    typedef std::map<std::string, std::string> ContainerType ;

    Dictionary(const Dictionary &other): container_(other.container_) {}
    Dictionary(Dictionary &&other): container_(other.container_) {}

    Dictionary &operator = ( const Dictionary &other ) {
        container_ = other.container_ ;
        return *this ;
    }

	Dictionary() ;
	~Dictionary() {}

    Dictionary(const ContainerType &c): container_(c) {}
    Dictionary(ContainerType &&c): container_(c) {}

    static Dictionary decode(const char *data, size_t sz, const char del = '\0') ;
    std::string encode(const char del = '\0') const ;

    // if key exists run the given callback with corresponding value
    void visit(const std::string &key, std::function<void(const std::string &val)> cb) const {
       auto it = container_.find(key) ;
       if ( it != end() ) cb(it->second) ;
    }


	// add a key/val pair 
    void add(const std::string &key, const std::string &val) ;
	// remove entry with given key if exists
    void remove(const std::string &key) ;
    void removeSome(const std::regex &rx) ;

	// remove all items
	void clear() ;
		
	// get a the value of the given key if exists. Otherwise return defaultValue

    std::string get(const std::string &key, const std::string &defaultVal = std::string()) const ;
    std::string operator[] ( const std::string & key ) const ;
    std::string &operator[] ( const std::string & key ) ;
	
	// check the existance of a key

    bool contains(const std::string &key) const;

	// get a list of the keys in the dictionary

    std::vector<std::string> keys() const ;
    std::vector<std::string> keys(const std::regex &rx) const ;

	// get values 

    std::vector<std::string> values() const ;
    std::vector<std::string> values(const std::regex &key) const ;

	// number of entries

	int count() const ;
    int count(const std::regex &kx) const ;
    int count(const std::string &) const ;

    uint64_t capacity() const ;

	bool empty() const ;

    void dump() const ;

    std::string serialize(const char *sep) ;

public:
 // stl style iterators

    typedef typename ContainerType::iterator iterator;
    typedef typename ContainerType::const_iterator const_iterator;

    iterator begin() { return container_.begin(); }
    const_iterator begin() const { return container_.begin(); }
    const_iterator cbegin() const { return container_.cbegin(); }
    iterator end() { return container_.end(); }
    const_iterator end() const { return container_.end(); }
    const_iterator cend() const {return container_.cend(); }
	
	private:

	friend class DictionaryIterator ;

    ContainerType container_ ;

} ;

class DictionaryIterator
{
	public:

    DictionaryIterator(const Dictionary &dic): dict_(dic), it_(dic.container_.begin()) {}
    DictionaryIterator(const DictionaryIterator &other): dict_(other.dict_), it_(other.it_) {}

    bool operator == (const DictionaryIterator &other) const { return it_ == other.it_ ; }
    bool operator != (const DictionaryIterator &other) const { return it_ != other.it_ ; }

    operator int () const { return it_ != dict_.container_.end() ; }
	
    DictionaryIterator & operator++() { ++it_ ; return *this ; }
    DictionaryIterator operator++(int) { DictionaryIterator tmp(*this) ; ++it_; return tmp ; }

    std::string key() const { return (*it_).first ; }
    std::string value() const { return (*it_).second ; }

	private:

    const Dictionary &dict_ ;
    Dictionary::ContainerType::const_iterator it_ ;
} ;



#endif
