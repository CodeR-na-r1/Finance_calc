#include <iostream>
#include <clocale>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <Windows.h>
#include <iomanip>
#include "finance_history.h"

using namespace std;

string path_main_file = "C:\\RR_logs\\path.r1";
string path_files;
vector<string> file_names;
vector<finance_history> f_historyes;
int now_fs = 0;
bool is_edit;

void set_now_fs()
{
	if (!now_fs)
	{
		cout << "Введите номер истории для дальнейшей обработки: ";
	enter_now_fs:
		cin >> now_fs;
		if (cin.fail() || now_fs < 1 || now_fs > f_historyes.size()) { cin.clear(); cin.ignore(999, '\n'); cout << "Неверный ввод! Введите заного: "; goto enter_now_fs; }
	}
	
	return;
}

void Tune()
{
	is_edit = false;

	ifstream in(path_main_file);
	string data;
	in.read((char*)&path_files, sizeof(string));

	while (in.read((char*)&data, sizeof(string)))
	{
		file_names.push_back(data);
	}
	in.close();
	cout << "Считано файлов: " << file_names.size() << endl;
	if (file_names.size() < 1)
		return;

	cout << "Имена файлов: " << endl;

	
	for (long long int i = 0; i < file_names.size(); i++)
	{
		string temp_path = path_files + "\\" + file_names[i] + ".r1";
		finance_history fs;
		cout << "1";
		ifstream file(temp_path);
		cout << "2";
		/*if (!file.is_open())
			continue;
			*/
		fs.load_from_file(file);
		cout << "3";
		f_historyes.push_back(fs);
		cout << "<" << i + 1 << ">  " << fs.name << endl;
	}

	file_names.clear();
	return;
}

void First_tune()
{
	cout << "Данные не обнаружены возможно программа запущена впервые" << endl;
	CreateDirectory(L"C:\\RR_logs", NULL);
	string in;
	cout << "Введи траекторию для хранения данных: (пример: С:\\\\folder) 0 - задаст по умолчанию" << endl;

	SetConsoleCP(1251);
	getline(cin, in);
	SetConsoleCP(866);

	if (in == "0")
		path_files = "C:\\RR_logs";
	else
		path_files = in;

	ofstream out(path_main_file);
	out.write((char*)&path_files, sizeof(string));
	out.close();

	is_edit = false;
	return;
}

void list()
{
	cout << "\n----------------------------------------\n";
	cout << "\nФайлов: " << f_historyes.size() << endl << endl;

	cout << setw(5) << "<Строка>" << setw(27) << "Имя файла" << "\t" << setw(12) << "Кол-во записей" << endl;
	for (long long int i = 0; i < f_historyes.size(); i++)
	{
		cout << setw(3) << "<" << i + 1 << ">  " << setw(25) << f_historyes[i].name << "\t" << setw(14) << f_historyes[i].get_num_of_records() << endl;
	}
	cout << "\n----------------------------------------\n";
	return;
}

void create()
{
	f_historyes.push_back(finance_history());
	cout << "Введите название финансовой истории: " << endl;
enter_name:
	SetConsoleCP(1251);
	getline(cin, f_historyes[f_historyes.size() - 1].name);
	SetConsoleCP(866);

	if (f_historyes[f_historyes.size() - 1].name.size() >= 15)
	{
		cout << "Нельзя имя более 14 символов, введите заного: ";
		goto enter_name;
	}

	cout << "Создано!" << endl;
	is_edit = true;
	return;
}

void Delete()
{
enter_index:
	string input;
	int index;
	cout << "Введите номер удаляемой истории: (для отмены - q)" << endl;
	cin >> input;
	if (input == "q")
		return;
	else
		index = stoi(input);
	if (cin.fail() || index < 1 || index > f_historyes.size()) { cin.clear(); cin.ignore(999, '\n'); cout << "Неверный ввод! Введите заного: "; goto enter_index; }

	file_names.push_back(f_historyes[index - 1].name);

	int count = 1;
	for (auto i = f_historyes.begin(); i < f_historyes.end(); i++)
	{
		if (count == index)
		{
			f_historyes.erase(i);
			break;
		}
		++count;
	}

	cout << "Удалено!" << endl;

	is_edit = true;
	now_fs = 0;
	return;
}

void rename()
{
	set_now_fs();

	string str;
	cout << "Введите новое имя: " << endl;

	SetConsoleCP(1251);
	getline(cin, str);
	SetConsoleCP(866);

	f_historyes[now_fs - 1].name = str;

	is_edit = true;
	return;
}

void save()
{
	if (!is_edit)
	{
		cout << "Последние изменения уже были сохранены!" << endl;
		return;
	}

	ofstream out(path_main_file);

	out.write((char*)&path_files, sizeof(string));

	for (int i = 0; i < f_historyes.size(); i++)
	{
		out.write((char*)&f_historyes[i].name, sizeof(string));
	}

	out.close();

	for (int i = 0; i < f_historyes.size(); i++)
	{
		out.open(path_files + "\\" + f_historyes[i].name + ".r1");
		f_historyes[i].save_in_file(out);
		out.close();
	}

	for (int i = 0; i < file_names.size(); i++)
	{
		string str = path_files + "\\" + file_names[i] + ".r1";
		remove(str.c_str());
	}
	file_names.clear();

	cout << "Сохранено!" << endl;
	is_edit = false;
	return;
}

void exit()
{
	if (is_edit)
	{
		string choose;
		cout << "Сохранить изменения перед выходом? (y/n)" << endl;
		cin >> choose;
		if (choose=="y")
			save();
	}
	return;
}

void help()
{

}

void clear()
{
	system("cls");
	return;
}

void cclear()
{
	while (f_historyes.size())
	{
		file_names.push_back(f_historyes[f_historyes.size() - 1].name);
		f_historyes.pop_back();
	}

	cout << "Очищено!" << endl;
	is_edit = true;
	return;
}

void reset()
{
	while (f_historyes.size())
	{
		file_names.push_back(f_historyes[f_historyes.size() - 1].name);
		f_historyes.pop_back();
	}
	is_edit = true;
	save();

	remove(path_main_file.c_str());

	RemoveDirectory(L"C:\\RR_logs");

	cout << "Удалить папку с хранением данных программы? (y/n)" << endl;
	string choose;

	cin >> choose;

	if (choose == "y")
	{
		RemoveDirectory(  wstring(path_files.begin(), path_files.end()).c_str()  );
	}

	cout << "Сброшено!" << endl;
	return;
}

int change(string input)
{
	if (input == "list" || input == "l") { list(); return 0; }
	if (input == "create" || input == "c") { create(); return 0; }
	if (input == "q") { now_fs = 0; return 0; }
	if (input == "rename") { rename(); return 0; }
	if (input == "help" || input == "h") { help(); return 0; }
	if (input == "clear" || input == "cl") { clear(); return 0; }
	if (input == "cclear" || input == "cc") { cclear(); return 0; }
	if (input == "reset" || input == "r") { reset(); return 2; }
	if (input == "save" || input == "s") { save(); return 0; }
	if (input == "exit" || input == "e") { exit(); return 2; }

	is_edit = true;

	if (f_historyes.size() < 1)
	{
		cout << "Историй нет" << endl;
		return 0;
	}

	if (input == "delete" || input == "d") { Delete(); return 0; }
	if (input == "add") { set_now_fs(); f_historyes[now_fs - 1].Add(); return 0; }
	if (input == "erase") { set_now_fs(); f_historyes[now_fs - 1].Erase(); return 0; }
	if (input == "see") { set_now_fs(); f_historyes[now_fs - 1].See(); return 0; }

	return -1;
}

int main()
{
	setlocale(LC_ALL, "ru");

	ifstream path_file(path_main_file);
	if (path_file.is_open())
		Tune();
	else
		First_tune();

	string command;
	int returned_res;

	cout << "\nЖду команду: \n";

	while (true)
	{
		cout << "\n> ";

		if (char(cin.peek()) == '\n')
			cin.ignore();

		SetConsoleCP(1251);
		getline(cin, command);
		SetConsoleCP(866);

		returned_res = change(command);

		if (returned_res == 2)
			break;

		if (returned_res == -1)
		{
			cout << "Неверная команда" << endl;
			continue;
		}

		if (now_fs)
		{
			cout << "\n\t\tnow_history [" << now_fs << "] " << "for exit, enter - q" << endl;
		}
	}
	
	system("pause");
	return 0;
}