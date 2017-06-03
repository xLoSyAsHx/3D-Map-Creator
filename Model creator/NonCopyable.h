#ifndef NON_COPYABLE_H
#define NON_COPYABLE_H

class NonCopyable
{
private:
	// copy and assignment not allowed
	NonCopyable(const NonCopyable&) = delete;
	void operator=(const NonCopyable&) = delete;
};

#endif NON_COPYABLE_H