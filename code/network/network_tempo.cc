#include "network_tempo.h"
#include "copyright.h"
#include "system.h"

#include <strings.h> /* for bzero */

static void PrintHeader(PacketHeader pktHdr, MailHeader mailHdr) {
	printf("From (%d, %d) to (%d, %d) bytes %d\n", pktHdr.from,
	       mailHdr.from, pktHdr.to, mailHdr.to, mailHdr.length);
}


Network_Tempo::Network_Tempo(NetworkAddress addr, double reliability, int nBoxes){
	netAddr = addr;
	postOffice = new PostOffice(addr , reliability , nBoxes);
	nb_Mail = 0 ;

}

Network_Tempo::~Network_Tempo(){
	delete postOffice;

}
static void PaquetHandler(int arg){
	printf("...... %lld\n",stats->totalTicks );
	Arg * args = (Arg *)arg;
	Network_Tempo * net = (Network_Tempo* )args->net;
	if (args->paq->send == false && args->paq->nb_trasmision < MAXREEMISSIONS )
	{
		args->paq->nb_trasmision += 1;
		net->reSend(args);

	}


}
static void PaquetWaiter(int arg){
	Network_Tempo *net  = (Network_Tempo *)arg;
	net->WaitPaq();
}
void Network_Tempo::Send(PacketHeader pktHdr, MailHeader mailHdr, const char *data){
	char *buffer = new char[MaxPacketSize];
	int try_num = 1 ;
	Paquet_Tempo  paquet ;
	paquet.pktHdr=pktHdr;
	paquet.mailHdr=mailHdr;
	paquet.nb_trasmision = try_num;
	paquet.send = false;
	paquet.data = data ;
	nb_Mail = (nb_Mail +1) % TEMPO;
	paquet.id =nb_Mail;
	Arg  args ;
	args.paq = &paquet;
	args.net= (int)this;
	bcopy(&nb_Mail, buffer, sizeof(nb_Mail));
	bcopy(data, buffer + sizeof(nb_Mail), mailHdr.length);
	mailHdr.length += sizeof(nb_Mail);
	//printf("%d....\n",pktHdr.to);
	postOffice->Send(pktHdr, mailHdr, buffer);
	interrupt->Schedule(PaquetHandler, (int)&args, TEMPO,
			    NetworkRecvInt);
	Thread *t = new Thread("paquet waiter");
	t->Fork(PaquetWaiter, (int)this);
}

void Network_Tempo::reSend(Arg *args){
	const char *data = args->paq->data;
	PrintHeader( args->paq->pktHdr,  args->paq->mailHdr);
	args->paq->mailHdr.to = 1;
	//printf("%d....\n",(unsigned int )args->paq->pktHdr.length);
	postOffice->Send(args->paq->pktHdr, args->paq->mailHdr, data);
	interrupt->Schedule(PaquetHandler, (int)args, TEMPO,
			    NetworkRecvInt);
}
void Network_Tempo::Receive(int box, PacketHeader *pktHdr, MailHeader *mailHdr,
		     char *data){
	char buffer[MaxMailSize];
	postOffice->Receive( box,  pktHdr,  mailHdr,buffer);
	PacketHeader ackpktHdr;
	MailHeader ackmailHdr;
	ackpktHdr.to = pktHdr->from;
	ackpktHdr.from = pktHdr->to;
	ackmailHdr.to =7;
	ackmailHdr.from = mailHdr->to;
	ackmailHdr.length=sizeof(int *);
	int id ;
	bcopy(buffer,&id, sizeof(int));
	bcopy(buffer+sizeof(int), data ,mailHdr->length -4);
	const char *ack = (char* )&id;
	printf("%d.................\n",ackmailHdr.length);

	//PrintHeader(ackpktHdr,  ackmailHdr);
	postOffice->Send(ackpktHdr,ackmailHdr,ack);


}

void Network_Tempo::WaitPaq(){
	PacketHeader pktHdr;
	MailHeader mailHdr;
	char data;
	postOffice->Receive(7,   &pktHdr,  &mailHdr, &data);
	int id ;
	bcopy(&id,&data, sizeof(int));
	printf("%d\n",id );


}
