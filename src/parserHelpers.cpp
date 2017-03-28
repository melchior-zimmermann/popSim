
#include "parserHelpers.hpp"

using std::cout;
using std::endl;
using std::ifstream;
using std::invalid_argument;
using std::out_of_range;
using std::stod;
using std::stoul;
using std::string;
using std::vector;

vector<string> split(const string& line, char delimiter) {
	vector<string> pieces;
	size_t i = 0, j = 0;
	auto it = line.begin();
	advance(it, j);

	i = j;

	while (j < line.length()) {
		if (line[j] == delimiter && i < j) {
			pieces.push_back(line.substr(i, j-i));
			i = j;
			do {
				i += 1;
			}while (line[i] == delimiter && i < line.length());
			advance(it, j);
		}

		++j;
	}		

	if (i < j) {
		pieces.push_back(line.substr(i, j-i));
	}

	return pieces;
}

string prune(const string& line, char delimiter) {
	string pruned;
	size_t i = 0, j = 0;
	auto it = line.begin();
	advance(it, j);

	i = j;

	while (j < line.length()) {
		if (line[j] == delimiter && i < j) {
			pruned += line.substr(i, j-i);
			i = j;
			do {
				i += 1;
			}while (line[i] == delimiter && i < line.length());
			advance(it, j);
		}
		++j;
	}	
	if (i < j) {
		pruned += line.substr(i, j-i);
	}	

	return pruned;
}

bool findName(ifstream& file, const string name, string& line) {
	do {
		getline(file, line);
	} while (file.good() && line.compare(0, name.size(), name));

	if (!file.good()) {
		return false;
	}

	return true;
}

bool readValue(const string& line, double& d) {
	vector<string> name_value = split(line, ',');
	if (name_value.size() >= 2) {
		try {
			d = stod(name_value[1]);
		}
		catch(const invalid_argument& ia) {
			cout<<"Invalid argument during parse of "<<line<<": "<<ia.what()<<endl;
			return false;
		}
		catch(const out_of_range& oor) {
			cout<<"Out-of-range argument during parse of "<<line<<": "<<oor.what()<<endl;
			return false;
		}

		return true;
	}

	return false;
}

bool readValue(const string& line, vector<double>& d) {
	vector<string> name_value = split(line, ',');
	if (name_value.size() >= 3) {
		for (size_t i = 1; i < name_value.size(); i++) {
			try {
				d.push_back(stod(name_value[i]));
			}
			catch(const invalid_argument& ia) {
				cout<<"Invalid argument during parse of "<<line<<": "<<ia.what()<<endl;
				return false;
			}
			catch(const out_of_range& oor) {
				cout<<"Out-of-range argument during parse of "<<line<<": "<<oor.what()<<endl;
				return false;
			}
		}

		return true;
	}

	return false;
}

bool readValue(const string& line, int& u) {
	vector<string> name_value = split(line, ',');
	if (name_value.size() >= 2) {
		try {
			u = stoi(name_value[1]);
		}
		catch(const invalid_argument& ia) {
			cout<<"Invalid argument during parse of "<<line<<": "<<ia.what()<<endl;
			return false;
		}
		catch(const out_of_range& oor) {
			cout<<"Out-of-range argument during parse of "<<line<<": "<<oor.what()<<endl;
			return false;
		}

		return true;
	}

	return false;
}

bool readValue(const string& line, vector<int>& u) {
	vector<string> name_value = split(line, ',');
	if (name_value.size() >= 2) {
		for (size_t i = 1; i < name_value.size(); i++) {
			try {
				u.push_back(stoi(name_value[i]));
			}
			catch(const invalid_argument& ia) {
				cout<<"Invalid argument during parse of "<<line<<": "<<ia.what()<<endl;
				return false;
			}
			catch(const out_of_range& oor) {
				cout<<"Out-of-range argument during parse of "<<line<<": "<<oor.what()<<endl;
				return false;
			}
		}

		return true;
	}

	return false;
}

bool readValue(const string& line, char& c) {
	vector<string> name_value = split(line, ',');
	if (name_value.size() >= 2) {
		try {
			c = name_value[1].c_str()[0];
		}
		catch(const invalid_argument& ia) {
			cout<<"Invalid argument during parse of "<<line<<": "<<ia.what()<<endl;
			return false;
		}
		catch(const out_of_range& oor) {
			cout<<"Out-of-range argument during parse of "<<line<<": "<<oor.what()<<endl;
			return false;
		}

		return true;
	}

	return false;
}

bool readValue(const string& line, bool& b) {
	char c;
	vector<string> name_value = split(line, ',');
	if (name_value.size() >= 2) {
		try {
			c = name_value[1].c_str()[0];
			if (c == 'y') {
				b = true;
			}
			else {
				b = false;
			}
		}
		catch(const invalid_argument& ia) {
			cout<<"Invalid argument during parse of "<<line<<": "<<ia.what()<<endl;
			return false;
		}
		catch(const out_of_range& oor) {
			cout<<"Out-of-range argument during parse of "<<line<<": "<<oor.what()<<endl;
			return false;
		}

		return true;
	}

	return false;
}