﻿#pragma once
#include "CoreMinimal.h"
#include "Session.h"

class FMMOSession final : public FSession
{
public:
	explicit FMMOSession(TSharedPtr<net::Socket> Socket)
		: FSession(Socket)
	{
	}

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnReceive(std::span<char> buffer, int32 length) override;
};