#pragma once


#include <stdint.h>
#include "../Macro.h"

namespace RakNet
{
	class RakPeerInterface
	{
	private:
		virtual void Private_1();
	public:
		virtual void Startup(uint32_t arg_1, class SocketDescriptor* arg_2, uint32_t arg_3, int arg_4); //2
		virtual void InitializeSecurity(char const* arg_1, char const* arg_2, bool arg_3); //3
		virtual void DisableSecurity(); //4
		virtual void AddToSecurityExceptionList(char const* arg_1); //5
		virtual void RemoveFromSecurityExceptionList(char const* arg_1); //6
		virtual void IsInSecurityExceptionList(char const* arg_1); //7
		virtual void SetMaximumIncomingConnections(uint32_t arg_1); //8
		virtual void GetMaximumIncomingConnections(); //9
		virtual void NumberOfConnections(); //10
		virtual void SetIncomingPassword(char const* arg_1, int arg_2); //11
		virtual void GetIncomingPassword(char* arg_1, int* arg_2); //12
		virtual void Connect(char const* arg_1, uint16_t arg_2, char const* arg_3, int arg_4, class PublicKey* arg_5, uint32_t arg_6, uint32_t arg_7, uint32_t arg_8, uint32_t arg_9); //13
		virtual void ConnectWithSocket(char const* arg_1, uint16_t arg_2, char const* arg_3, int arg_4, class RakNetSocket2* arg_5, class PublicKey* arg_6, uint32_t arg_7, uint32_t arg_8, uint32_t arg_9); //14
		virtual void Shutdown(unsigned int arg_1, uint8_t arg_2, class _PacketPriority arg_3); //15
		virtual void IsActive(); //16
		virtual void GetConnectionList(class SystemAddress* arg_1, uint16_t* arg_2); //17
		virtual void GetNextSendReceipt(); //18
		virtual void IncrementNextSendReceipt(); //19
		virtual void Send(char const* arg_1, int arg_2, _PacketPriority arg_3, class PacketReliability arg_4, char arg_5, class AddressOrGUID arg_6, bool arg_7, uint32_t arg_8); //20
		virtual void SendLoopback(char const* arg_1, int arg_2); //21
		virtual void Send(class BitStream const* arg_1, _PacketPriority arg_2, PacketReliability arg_3, char arg_4, AddressOrGUID arg_5, bool arg_6, uint32_t arg_7); //22
		virtual void SendList(char const** arg_1, int const* arg_2, int arg_3, _PacketPriority arg_4, PacketReliability arg_5, char arg_6, AddressOrGUID arg_7, bool arg_8, uint32_t arg_9); //23
		virtual void Receive(); //24
		virtual void DeallocatePacket(class Packet* arg_1); //25
		virtual void GetMaximumNumberOfPeers(); //26
		virtual void CloseConnection(AddressOrGUID arg_1, bool arg_2, uint8_t arg_3, _PacketPriority arg_4); //27
		virtual void GetConnectionState(AddressOrGUID arg_1); //28
		virtual void CancelConnectionAttempt(SystemAddress arg_1); //29
		virtual void GetIndexFromSystemAddress(SystemAddress arg_1); //30
		virtual void GetSystemAddressFromIndex(uint32_t arg_1); //31
		virtual void GetGUIDFromIndex(uint32_t arg_1); //32
		virtual void GetSystemList(void* arg_1, void* arg_2); //33
		virtual void AddToBanList(char const* arg_1, uint32_t arg_2); //34
		virtual void RemoveFromBanList(char const* arg_1); //35
		virtual void ClearBanList(); //36
		virtual bool IsBanned(char const* arg_1); //37
		virtual void SetLimitIPConnectionFrequency(bool arg_1); //38
		virtual void Ping(SystemAddress arg_1); //39
		virtual void Ping(char const* arg_1, uint16_t arg_2, bool arg_3, uint32_t arg_4); //40
		virtual void SendNatTraversalMessage(SystemAddress arg_1); //41
		virtual int GetAveragePing(class AddressOrGUID* arg_1); //42
		virtual int GetLastPing(AddressOrGUID* arg_1); //43
		virtual int GetLowestPing(AddressOrGUID* arg_1); //44
		virtual void SetOccasionalPing(bool arg_1); //45
		virtual void GetClockDifferential(AddressOrGUID arg_1); //46
		virtual void SetOfflinePingResponse(char const* arg_1, uint32_t arg_2); //47
		virtual void GetOfflinePingResponse(char** arg_1, uint32_t* arg_2); //48
		virtual void GetInternalID(SystemAddress arg_1, int arg_2); //49
		virtual void SetInternalID(SystemAddress arg_1, int arg_2); //50
		virtual void GetExternalID(SystemAddress arg_1); //51
		virtual class AddressOrGUID* GetMyGUID(); //52
		virtual void resetMyGUID(); //53
		virtual void GetMyBoundAddress(int arg_1); //54
		virtual void GetGuidFromSystemAddress(SystemAddress arg_1); //55
		virtual void GetSystemAddressFromGuid(class RakNetGUID arg_1); //56
		virtual void GetClientPublicKeyFromSystemAddress(SystemAddress arg_1, char* arg_2); //57
		virtual void SetTimeoutTime(uint32_t arg_1, SystemAddress arg_2); //58
		virtual void GetTimeoutTime(SystemAddress arg_1); //59
		virtual void GetMTUSize(SystemAddress arg_1); //60
		virtual void GetNumberOfAdapters(); //61
		virtual void GetLocalAdapter(uint32_t arg_1); //62
		virtual void GetNumberOfAddresses(); //63
		virtual void GetLocalIP(uint32_t arg_1); //64
		virtual void IsLocalIP(char const* arg_1); //65
		virtual void AllowConnectionResponseIPMigration(bool arg_1); //66
		virtual void AdvertiseSystem(char const* arg_1, uint16_t arg_2, char const* arg_3, int arg_4, uint32_t arg_5); //67
		virtual void SetSplitMessageProgressInterval(int arg_1); //68
		virtual void GetSplitMessageProgressInterval(); //69
		virtual void SetUnreliableTimeout(uint32_t arg_1); //70
		virtual void SendTTL(char const* arg_1, uint16_t arg_2, int arg_3, uint32_t arg_4); //71
		virtual void AttachPlugin(class PluginInterface2* arg_1); //72
		virtual void DetachPlugin(PluginInterface2* arg_1); //73
		virtual void PushBackPacket(class Packet* arg_1, bool arg_2); //74
		virtual void ChangeSystemAddress(class RakNetGUID arg_1, SystemAddress const& arg_2); //75
		virtual void AllocatePacket(uint32_t arg_1); //76
		virtual void GetSocket(SystemAddress arg_1); //77
		virtual void GetSockets(void* arg_1); //78
		virtual void ReleaseSockets(void* arg_1); //79
		virtual void WriteOutOfBandHeader(BitStream* arg_1); //80
		virtual void SetUserUpdateThread(void* arg_1, void* arg_2, void* arg_3); //81
		virtual void SetIncomingDatagramEventHandler(void* arg_1); //82
		virtual void ApplyNetworkSimulator(float arg_1, uint16_t arg_2, uint16_t arg_3); //83
		virtual void SetPerConnectionOutgoingBandwidthLimit(uint32_t arg_1); //84
		virtual void IsNetworkSimulatorActive(); //85
		virtual void GetStatistics(SystemAddress arg_1, class RakNetStatistics* arg_2); //86
		virtual void GetStatistics(uint32_t arg_1, RakNetStatistics* arg_2); //87
		virtual void GetStatisticsList(void* arg_1, void* arg_2, void* arg_3); //88
		virtual void GetReceiveBufferSize(); //89
		virtual void RunUpdateCycle(BitStream& arg_1); //90
		virtual void SendOutOfBand(char const* arg_1, uint16_t arg_2, char const* arg_3, uint32_t arg_4, uint32_t arg_5); //91
		virtual void DeallocRNS2RecvStruct(class RNS2RecvStruct* arg_1, char const* arg_2, uint32_t arg_3); //92
		virtual void AllocRNS2RecvStruct(char const* arg_1, uint32_t arg_2); //93
		virtual void OnRNS2Recv(RNS2RecvStruct* arg_1); //94
	};
}