#ifndef SYNCHCONSOLE_H
#define SYNCHCONSOLE_H

#include "copyright.h"
#include "console.h"
#include "utility.h"
#include "synch.h"

// The following class defines a hardware console device.
// Input and output to the device is simulated by reading
// and writing to UNIX files ("readFile" and "writeFile").
//
// Since the device is asynchronous, the interrupt handler "readAvail"
// is called when a character has arrived, ready to be read in.
// The interrupt handler "writeDone" is called when an output character
// has been "put", so that the next character can be written.

class SynchConsole {
	public:
		SynchConsole(char * in, char * out);
		~SynchConsole();
		void SynchPutChar(char c);
		char SynchGetChar();
		void SynchPutString(char * s);
		void SynchGetString(char * s, int n);

	private:
		Console * console;
		Lock * lock;
};

#endif // SYNCHCONSOLE_H
