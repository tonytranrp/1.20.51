#pragma once

#include "../../System.h"

#include <Data/ManagedItem.h>

#include <nlohmann/json.hpp>
#include <winrt/windows.networking.sockets.h>
#include <winrt/windows.storage.streams.h>

#include <mutex>
#include <optional>
#include <string>

#undef ERROR
namespace Sockets = winrt::Windows::Networking::Sockets;
namespace Streams = winrt::Windows::Storage::Streams;
class ChatClient : public ::System<ManagedItem>
{
	//ChatClient version DO NOT TOUCH!!!
	static inline std::string CLIENT_VERSION = "1.0.0";
public:
	enum class OpCodes
	{
		SET_MOD,
		SET_NAME,
		MESSAGE,
		INCOMING_CHAT,
		ERROR,
		SET_CHANNEL,
		REQUEST_CONNECTED_COUNT,
		REQUEST_CONNECTED_NAMES,
		JOIN_CHANNEL,
		LEAVE_CHANNEL,
		RATELIMIT,

		/* Internal operations, DO NOT HANDLE THESE */
		WORK,
		COMPLETE,
		INCOMING_KEY,
		OUTGOING_KEY,
		IDENTIFIER,
		IDENTIFIER_VER_INFO
	};

	class ChatOperation
	{
	public:
		OpCodes code;
		std::string data;
		bool successful;

		static std::optional<ChatOperation> parse(nlohmann::json& message);
		ChatOperation(OpCodes code, std::string data, bool successful);

		[[nodiscard]] nlohmann::json serialize() const;
	};

	class EncryptedOperation
	{
		std::string encrypted;
	public:
		static std::optional<EncryptedOperation> parse(nlohmann::json& message);
		explicit EncryptedOperation(std::string payload);
		EncryptedOperation(const ChatOperation& toEncrypt, const std::string& hKey);

		[[nodiscard]] std::optional<ChatOperation> decrypt(const std::string& key) const;
		[[nodiscard]] nlohmann::json serialize() const;
	};

	using callback_t = std::function<void(const ChatOperation&)>;
	using dataevent_t = std::function<void(const std::string& data, bool successful)>;

	explicit ChatClient(std::string uri);
	~ChatClient() override;
	bool connect();
	void disconnect();
	[[nodiscard]] bool isAlive() const;

	/*
	 * Automatically determines if the operation needs to be encrypted or not before being sent.
	 * If it does, it will encrypt it automatically.
	 */
	bool sendOperationAuto(const ChatOperation& operation);
	//Sends an operation decrypted
	bool sendOperationDecrypted(const ChatOperation& operation);
	//Sends an operation encrypted
	bool sendOperationEncrypted(const EncryptedOperation& operation);

	void onServerError(callback_t callback);
	void onClientException(std::function<void(std::string)> callback);
	void onConnect(std::function<void()> callback);
	void onLoginComplete(std::function<void()> callback);
	void onDisconnect(std::function<void()> callback);

	template<OpCodes operation>
	void when(const dataevent_t& listener)
	{
		eventCallbacks[operation] = listener;
	}
private:
	//Sends any serializable operation
	template<typename operation_t> bool sendOperationBase(const operation_t& operation);

	/* Event callbacks */
	callback_t errorCallback;
	std::function<void(std::string)> exceptionCallback;
	std::function<void()> connectCallback;
	std::function<void()> loginCallback;
	std::function<void()> disconnectCallback;
	//OpCode specific callbacks
	std::unordered_map<OpCodes, dataevent_t> eventCallbacks;

	/* Internal state data */
	enum class ClientState
	{
		DISCONNECTED,
		CONNECTING,
		CONNECTED
	};
	Sockets::MessageWebSocket mws;
	Streams::DataWriter dw;
	std::mutex dwMut;
	std::string uri;
	ClientState state = ClientState::DISCONNECTED;

	/* Crash telemetry */
	void sendError(const std::string& error);

	/* Encryption data (Do not touch these, these are state values! They will change on their own as needed!) */
	bool shouldEncrypt = false;
	std::string serverKey;
	std::string clientKey;
};