#pragma once
#include "stdafx.h"

#ifdef DR_HOST_MANAGER_STATIC
#define DR_HOST_MANAGER_CLASS
#define DR_HOST_MANAGER_FUNC
#else
#ifdef DR_HOST_MANAGER_EXPORT
#define DR_HOST_MANAGER_CLASS __declspec(dllexport)
#define DR_HOST_MANAGER_FUNC __declspec(dllexport)
#else
#define DR_HOST_MANAGER_CLASS __declspec(dllimport)
#define DR_HOST_MANAGER_FUNC __declspec(dllimport)
#endif
#endif

enum DR_HOST_MANAGER_CLASS DR_HostType
{
	DR_HostTypeUnknown = 0,
	DR_HostTypeX64 = 1,
	DR_HostTypeX86 = 2
};

enum DR_HOST_MANAGER_CLASS DR_SessionStatus
{
	DR_SessionStatusClosed = 0,
	DR_SessionStatusRunning = 1,
	DR_SessionStatusOnPause = 2
};

class DR_HOST_MANAGER_CLASS DR_ISession
{
public:
	virtual void Run() = 0;
	virtual void Close() = 0;
	virtual DR_SessionStatus GetSessionStatus() const = 0;
	virtual DWORD GetFuncRVAAddress() const = 0;
	virtual const std::wstring& GetImagePath() const = 0;
	virtual bool IsRunning() const = 0;
	virtual DR_HostType GetHostType() const = 0;
	virtual const std::wstring& GetHostName() const = 0;
	virtual bool IsLastOperationSuccess() const = 0;
	virtual ~DR_ISession() = default;
};
typedef std::unique_ptr<DR_ISession> DR_UnPtrSession;

class DR_HOST_MANAGER_CLASS DR_IHostManager
{
public:
	virtual const std::list<DR_UnPtrSession>& GetSessions() const = 0;
	virtual void CreateSession(const std::wstring& PEPath, DWORD RVAAddress) const = 0; 
	virtual bool IsLastOeprationSuccess() const = 0;
	virtual ~DR_IHostManager() = default;
};

