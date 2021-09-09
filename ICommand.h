#pragma once

class ICommand
{
public:
	virtual void execute() = 0;
	virtual void unexecute() = 0;

	virtual ~ICommand() { }
};
