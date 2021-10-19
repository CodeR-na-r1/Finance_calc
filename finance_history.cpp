#include "finance_history.h"

finance_history::finance_history()
{
	this->name = "None";
	this->data.clear();
	this->comment.clear();
}

finance_history::finance_history(string name)
{
	this->name = name;
	this->data.clear();
	this->comment.clear();
}

finance_history::finance_history(const finance_history& fs)
{
	this->name = fs.name;
	
	this->data.clear();
	for (int i = 0; i < fs.data.size(); i++)
	{
		this->data.push_back(fs.data[i]);
	}

	this->comment.clear();
	for (int i = 0; i < fs.comment.size(); i++)
	{
		this->comment.push_back(fs.comment[i]);
	}
}

size_t finance_history::get_num_of_records()
{
	return this->data.size();
}

void finance_history::Add()
{
	cout << "Введите числовое значение:\n> ";
	long int value;
enter_value:
	cin >> value;
	if (cin.fail()) { cin.clear(); cin.ignore(999, '\n'); cout << "Неверный ввод! Введите заного: "; goto enter_value; }
	data.push_back(value);

	if (char(cin.peek()) == '\n')
		cin.ignore();

	cout << "Введите комментарий к значению:\n> ";
	string str;
enter_name:
	SetConsoleCP(1251);
	getline(cin, str);
	SetConsoleCP(866);

	if (str.size() >= 15)
	{
		cout << "Нельзя имя более 14 символов, введите заного: ";
		goto enter_name;
	}

	comment.push_back(str);

	return;
}

void finance_history::Erase()
{
enter_index:
	string input;
	int index;
	cout << "Введите номер удаляемой строки истории: (для отмены - q)" << endl;
	cin >> input;
	if (input == "q")
		return;
	else
		index = stoi(input);
	if (cin.fail() || index < 1 || index > data.size()) { cin.clear(); cin.ignore(999, '\n'); cout << "Неверный ввод! Введите заного: "; goto enter_index; }

	int count = 1;
	for (auto i = data.begin(); i < data.end(); i++)
	{
		if (count == index)
		{
			data.erase(i);
			break;
		}
		++count;
	}

	count = 1;
	for (auto i = comment.begin(); i < comment.end(); i++)
	{
		if (count == index)
		{
			comment.erase(i);
			break;
		}
		++count;
	}

	cout << "Удалено!" << endl;

	return;
}

void finance_history::See()
{
	cout << "---------------------------------------";
	cout << "\nНазвание истории: " << this->name << endl << endl;

	int total(0), profit(0), costs(0);

	cout << setw(5) << "<Строка>" << setw(25) << "Числовое значение" << "\t" << setw(14) << "Комментарий" << endl;
	for (long int i = 0; i < data.size(); i++)
	{
		cout << setw(5) << "<" << i + 1 << ">  " << setw(20) << data[i] << "\t" << setw(25) << comment[i] << endl;
		total += data[i];
		data[i] >= 0 ? profit += data[i] : costs += data[i];
	}

	cout << "Получено: " << profit << endl;
	cout << "Потрачено: " << costs << endl;
	cout << "Итого: " << total << endl;
	cout << "---------------------------------------";

	return;
}

void finance_history::save_in_file(ofstream& out)
{
	out.write((char*)&this->name, sizeof(string));

	int num = this->data.size();
	out.write((char*)&num, sizeof(int));
	for (int i = 0; i < this->data.size(); i++)
	{
		out.write((char*)&this->data[i], sizeof(int));
	}

	num = this->comment.size();
	out.write((char*)&num, sizeof(int));
	for (int i = 0; i < this->comment.size(); i++)
	{
		out.write((char*)&this->comment[i], sizeof(string));
	}

	return;
}

void finance_history::load_from_file(ifstream& in)
{
	in.read((char*)&this->name, sizeof(string));

	int num, temp_data;
	in.read((char*)&num, sizeof(int));
	for (int i = 0; i < num; i++)
	{
		in.read((char*)&temp_data, sizeof(int));
		this->data.push_back(temp_data);
	}

	in.read((char*)&num, sizeof(int));
	string str_in;
	for (int i = 0; i < num; i++)
	{
		in.read((char*)&str_in, sizeof(string));
		this->comment.push_back(str_in);
	}
	
	return;
}

finance_history::~finance_history()
{
	this->name.~basic_string();
	this->data.~vector();

	for (int i = 0; i < comment.size(); i++)
	{
		comment[i].~basic_string();
	}
	this->comment.~vector();
}