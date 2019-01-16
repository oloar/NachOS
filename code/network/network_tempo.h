#include "copyright.h"

#ifndef NETWORK_TEMPO_H
#define NETWORK_TEMPO_H

#include "post.h"

#define TEMPO 50000

#define MAXREEMISSIONS 7

class Paquet_Tempo
{
	  public:
	PacketHeader pktHdr;
	MailHeader 	mailHdr;
	const char *data;
	int nb_trasmision;
	bool send ;
	int id ;
};

class Arg {
	public:
	Paquet_Tempo * paq;
	int net;
};


class Network_Tempo{
	  public:
	Network_Tempo(NetworkAddress addr, double reliability, int nBoxes);

	~Network_Tempo();

	void Send(PacketHeader pktHdr, MailHeader mailHdr, const char *data);

	void Receive(int box, PacketHeader *pktHdr, MailHeader *mailHdr,
		     char *data);
	void reSend(Arg *args);
	void WaitPaq();

	  private:
	PostOffice  *postOffice;       // Physical network connection
	NetworkAddress netAddr;// Network address of this machine

	int nb_Mail;

};
#endif
