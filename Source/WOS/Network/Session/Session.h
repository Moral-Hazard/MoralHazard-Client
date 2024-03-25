﻿#pragma once
#include "net/Socket.hpp"

#include "Templates/SharedPointer.h"

class FSession : public TSharedFromThis<FSession>
{
	friend class FIoThread;
public:
	FSession(net::Socket* socket);
	virtual ~FSession() = default;
public:
	virtual void OnConnected() {}
	virtual void OnDisconnected() {}
	virtual void OnReceive(std::span<char> buffer, int32 length);
public:
	void Send(std::span<char> data);
	void Send(class Packet* pkt);
public:
	net::Socket* GetHandle() const;
	void Flush();
protected:
	void PushJob(TFunction<void()> Functor);
	
	template<class TRet, class ...TArgs>
	FORCEINLINE void PushJob(TFunction<TRet(TArgs...)> Functor, TArgs... Args)
	{
		JobQue.Enqueue([=] { Functor(Args...); });
	}
private:
	net::Socket* Socket;
	char m_buffer[4096];
	TQueue<TFunction<void()>> JobQue;
};