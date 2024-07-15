﻿#include "Session.h"
#include "Packet.h"
#include "NetWorker.h"

FSession::FSession(TSharedPtr<net::Socket> Socket, ServerType Type)
	: Socket(Socket), Type(Type)
{
}

FSession::~FSession()
{
}

void FSession::OnConnected()
{
	Worker = MakeShared<FNetWorker>(this);
}

void FSession::OnDisconnected()	
{
}

void FSession::OnReceive(std::span<char> buffer, int32 length)
{
	if (length <= 2)
		return;
}

void FSession::Send(std::span<char> data)
{
	if (!Socket->send(data))
		OnDisconnected();
}

void FSession::Send(Packet* pkt)
{
	pkt->Write();
	Send(pkt->Data());
}

TSharedPtr<net::Socket> FSession::GetHandle()
{
	return this->Socket;
}

void FSession::Flush()
{
	while (!JobQue.IsEmpty())
	{
		TFunction<void()> Handler;
		if (JobQue.Dequeue(Handler))
		{
			if (Handler) Handler();
		}
	}
}

void FSession::Disconnect() const
{
	Socket->disconnect();
}

ServerType FSession::GetType() const
{
	return Type;
}

void FSession::PushJob(TFunction<void()> Functor)
{
	JobQue.Enqueue(Functor);
}
