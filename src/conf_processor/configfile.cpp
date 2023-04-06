#include <fstream>

#include "configfile.h"

using namespace std;

void str2lower(string &str)
{
	for (unsigned int i = 0; i < str.size(); i++)
	{
		str[i] = tolower(str[i]);
	}
}
std::string trim(std::string const &source, char const *delims = " \t\r\n")
{
	std::string result(source);
	std::string::size_type index = result.find_last_not_of(delims);
	if (index != std::string::npos)
		result.erase(++index);

	index = result.find_first_not_of(delims);
	if (index != std::string::npos)
		result.erase(0, index);
	else
		result.erase();
	return result;
}

ConfigFile::ConfigFile() {}
ConfigFile::ConfigFile(string filename)
{
	if (!load(filename))
		cerr << "Configfile '" << filename << "' not found!" << endl;
}
bool ConfigFile::load(string filename)
{
	this->filename = filename;
	fstream f;
	f.open(filename.c_str(), fstream::in);
	if (!f.is_open())
	{
		return false;
	}
	string line;
	int lnr = -1;
	while (getline(f, line))
	{
		lnr++;
		// skip comments and empty lines
		if (!line.length())
			continue;
		if (line[0] == '#')
			continue;

		int posTrenner = line.find('=');
		if (posTrenner == -1)
			posTrenner = line.find(' ');
		if (posTrenner == -1)
		{
			cerr << "WARNING: Statement '" << line << "' in file " << filename << ":" << lnr << " is invalid and therefor will be ignored" << endl;
			continue;
		}
		string key = trim(line.substr(0, posTrenner));
		string value = trim(line.substr(posTrenner + 1));

		// case insensitive
		str2lower(key);

		if (datamap[key] != "")
		{
			cerr << "WARNING: Statement '" << line << "' in file " << filename << ":" << lnr << " redefines a value!" << endl;
		}
		datamap[key] = value;
	}
	f.close();
	return true;
}

void ConfigFile::print(void)
{
	for (map<string, string>::iterator iter = datamap.begin(); iter != datamap.end(); iter++)
	{
		cout << "key: '" << iter->first << "' \t value: '" << iter->second << "'" << endl;
	}
}
ConfigFile::~ConfigFile()
{
}
