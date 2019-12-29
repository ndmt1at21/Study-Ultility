#pragma once
#include <iostream>
#include <string>

class ProgramFrame
{
protected:
	virtual void startMessage(std::ostream&);
	virtual void input(std::istream& in) = 0;
	virtual bool validData() = 0;
	virtual void errorMessage(std::ostream&);
	virtual void output(std::ostream&, std::string function = 0) = 0;
	virtual void processing(std::istream& in, std::string function);
	virtual bool askToContinue(std::istream&, std::ostream&);

public:
	void run(std::istream&, std::ostream&, std::string function = "");
};

