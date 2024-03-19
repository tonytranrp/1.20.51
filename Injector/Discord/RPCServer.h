#include <iostream>

enum class RPCServerPackets : uintptr_t
{
	HELLO = 0x1,
	PING = 0x2
};

// RPCServer will run on a seperate thread
class RPCServer
{
private:
	int PORT = 56715;
public:
	void Start();
};