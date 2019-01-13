#include <vector>
#include <string>
#include <algorithm>		// Needed to perform the sorts
#include <functional>		// Needed for the binary_function class
#include <fstream>
#include <dirent.h>
#include <ctype.h>

#include "rom.h"

using namespace std;

static const string whitespace = " \r\n\t";

/**
 *  @brief Returns indices nessary to trim leading and trailing 
 *  		whitespace from a string
 *  @param str String to examine
 *  @param from Start index of trimmed string (within str)
 *  @param to End index of trimmed string (within str)
 */
void trim(string& str) {
    int from = 0;
    int to = 0;

    if (str.size() < 1)
	return;

    from = str.find_first_not_of(whitespace, 0);
    to = str.find_last_not_of(whitespace, str.size());
    if (from < 0) // no whitespace at all
	return;
    if (to < 0) // whitespace exists, but not at end
	to = str.size();
    str = str.substr(from, (to - from + 1));
}

/**
 *  @brief Changes a string to all uppercase
 *  @param str The string to modify
 */
void str_to_upper(string& str) {
    for (unsigned int i = 0; i < str.size(); ++i)
	str[i] = toupper(str[i]);
}

// Sorting functor for sorting on the ROM's full name
struct rom_fullname_comp:public binary_function<const Rom&, const Rom&, bool> {
    bool operator() (const Rom& x, const Rom& y) {
	return (strcmp(x.name.c_str(), y.name.c_str()) < 0 ? true : false);
}};

// sorting functor for sorting on the ROMs internal name
struct rom_internal_comp:public binary_function<const Rom&, const Rom&, bool> {
    bool operator() (const Rom& x, const Rom& y) {
	return (strcmp(x.rom_name.c_str(), y.rom_name.c_str()) < 0 ?
		true : false);
}};

/**
 * @brief Parses out a MAME generated gamelist (the gamelist needs to have the
 *        header lines removed
 * @param path Path to the game list file
 * @param pList Pointer to a RomList that will receive the sorted list of ROMs 
 *              (sorted by full name)
 */
int parse_master_gamelist(const string& path, RomList& pList) {
    string token;
    string name;
    string rom_name;
    string search_name;
    string line;
    Rom* pRom;
    ifstream ml_is;
    unsigned int last_idx = 0;
    unsigned int idx = 0;
    unsigned int field = 0;

    ml_is.open(path.c_str(), ios_base::in);
    if (ml_is.is_open()) {
	while (!ml_is.eof()) {
	    getline(ml_is, line);

	    idx = line.find_first_of('|', 0);
	    last_idx = 0;
	    field = 0;
	    do {
		token = line.substr(last_idx, (idx - last_idx));
		trim(token);

		if (field == 1)
		    name = token;
		else if (field == 6)
		    rom_name = token;
		field++;
	        last_idx = idx + 1;
	        idx = line.find_first_of('|', last_idx);
	    } while (idx < line.size());

	    search_name = name;
	    str_to_upper(search_name);
	    pRom = new Rom(name, rom_name, search_name);
	    pList.push_back(*pRom);
	    delete pRom;
	}
	ml_is.close();

	sort(pList.begin(), pList.end(), rom_fullname_comp());

	return 1;
    }

    return 0;
}

void select_roms(const RomList& pick_list, RomList& found_list,
		 const string& key) {
    RomList::const_iterator i;
    bool in_alpha = false;
    string ukey;

    // Get everything out of the found list
    found_list.clear();

    // Use upper case to ignore case during the compare
    // Rom.search_name is upper-case
    ukey = key;
    str_to_upper(ukey);

    // Pick all of the strings that match the key beginning from the front
    for (i = pick_list.begin(); i != pick_list.end(); i++) {
	if ((int)(i->search_name.find(ukey, 0)) == 0) {
	    // Mark that we've reached the beginning of the list
	    // of items that match the key
	    in_alpha = true;
	    // actually add the item
	    found_list.push_back(*i);
	} else {
	    // If we were in the right alphabetical region, and we just got a 
	    // false from strstr, then we're out of the alphabetical region
	    // break out and head home.
	    if (in_alpha)
		break;
	}
    }
}

#ifdef DEBUG
static const string path = "../test_list3.txt";
int main(int argc, char** argv) {
    RomList rl;
    RomList pl;
    int rc = 0;
    RomList::const_iterator it;
    string* s = NULL;

    s = new string(argv[1]);
    trim(*s);
    rc = parse_master_gamelist(path, rl);
    select_roms(rl, pl, *s);
    if (rc == 1) {
	for (it = pl.begin(); it < pl.end(); ++it) {
	    cout << "Picked Rom: '" << it->name << "' '" << it->rom_name << "'" << "'" << it->search_name << "'" << endl;
	}
    }
    delete s;
    return 0;
}

struct print_rom:public unary_function <Rom, void > {
    void operator() (const Rom& pRom) {
	printf("%s (%s)\n", pRom->name, pRom->rom_name);
}};

void print_rom_list(const RomList& pList) {
    for_each(pList->begin(), pList->end(), print_rom());
}
#endif
