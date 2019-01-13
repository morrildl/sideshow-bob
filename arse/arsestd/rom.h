#ifndef ROM_INCLUDE
#define ROM_INCLUDE

#include <vector>
#include <string>
#include <algorithm>		// Needed to perform the sorts
#include <functional>		// Needed for the binary_function class
using std::vector;
using std::string;
using namespace std;		// redundant w/ above, but whatever...

static const string empty_str = "";

class Rom {
    public:
	string name;
	string rom_name;
	string search_name;

	Rom(const string& name_, const string& rom_name_, const string& search_name_);
	~Rom();

    protected:
	Rom();
};

typedef vector<Rom> RomList;

#endif
