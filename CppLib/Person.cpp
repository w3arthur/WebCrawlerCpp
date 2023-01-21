#include "pch.h"
#include "Person.h"



string Person::print() const
{
	string str = "[ ID: ";
	str += "(" + to_string(id);
	str += ") | Name: (" + first_name + " " + last_name;
	str += ") | Email: (" + email;
	str += ") | Gender: (" + gender;
	str += ") | IP: (" + ip_address;
	str += ") ]";
	return str;
}


ostream& operator<<(ostream& os, const Person& p)
{
	os << p.print();
	return os;
}
