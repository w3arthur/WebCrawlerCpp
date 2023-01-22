#pragma once
using json = nlohmann::json;	//to improve!
using std::string;
using std::to_string;
using std::ostream;
using std::unordered_map;
using std::function;
using std::pair;

class Person
{
public:
	int id;
	string first_name;
	string last_name;
	string email;
	string gender;
	string ip_address;


private:
	unordered_map<string, function<void(const json&)>> setValueMap
	{
		pair("id",[&](const json& value)-> void { id = static_cast<int>(value); })
		, pair("first_name",[&](const json& value)-> void { first_name = value; })
		, pair("last_name",[&](const json& value)-> void { last_name = value; })
		, pair("email",[&](const json& value)-> void { email = value; })
		, pair("gender",[&](const json& value)-> void { gender = value; })
		, pair("ip_address",[&](const json& value)-> void { ip_address = value; })
	};
public:
	void setValue(const string& key, const json& value) { setValueMap[key](value); };
private:
	string print() const;
public:
	friend ostream& operator<<(ostream& os, const Person& p);
};

