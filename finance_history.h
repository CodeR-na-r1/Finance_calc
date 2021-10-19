#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <windows.h>

using namespace std;

class finance_history
{
public:

	finance_history();

	finance_history(string name);

	finance_history(const finance_history& fs);

	size_t get_num_of_records();

	void Add();

	void Erase();

	void See();

	string name;

	void save_in_file(ofstream& out);

	void load_from_file(ifstream& in);

	~finance_history();

private:

	vector<int> data;
	vector<string> comment;

};