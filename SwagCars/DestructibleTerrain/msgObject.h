/**
 * Any kind of object capable of accepting a message.
 */

#ifndef MSGOBJECT_H
#define MSGOBJECT_H

#include "message.h"

class msgObject
{
private:

public:
	inline virtual void readMessage(string s, Message m) {};

};

#endif