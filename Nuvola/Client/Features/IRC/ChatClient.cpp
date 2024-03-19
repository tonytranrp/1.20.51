#include "ChatClient.h"

#include "../../Preferences/Preferences.h"
#include "base64.h"
#include "KairunVM.h"

#include <magic_enum.hpp>
#include <winrt/windows.foundation.h>
#include <winrt/windows.networking.sockets.h>
#include <winrt/windows.storage.streams.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <codecvt>
#include <random>
#include <regex>
#include <string>
#include <utility>

//The winrt api likes to define this
#ifdef ERROR
#undef ERROR
#endif

namespace Foundation = winrt::Windows::Foundation;
namespace Networking = winrt::Windows::Networking;

std::optional<ChatClient::ChatOperation> ChatClient::ChatOperation::parse(nlohmann::json& message)
{
	bool parsable = message.contains("op") && message.contains("d");
	if (!parsable) return {};

	nlohmann::json& operation = message["op"];
	if (!operation.is_number_integer()) return {};
	nlohmann::json& data = message["d"];
	if (!data.is_string()) return {};

	bool successful = true;
	if(message.contains("s"))
	{
		nlohmann::json& success = message["s"];
		if (!success.is_boolean()) return {};
		successful = success.get<bool>();
	}

	int operationId = operation.get<int>();
	std::string dataStr = data.get<std::string>();

	return ChatClient::ChatOperation(magic_enum::enum_value<OpCodes>(operationId), dataStr, successful);
}

ChatClient::ChatOperation::ChatOperation(const OpCodes code, std::string data, const bool successful) : code(code), data(std::move(data)), successful(successful) {}

nlohmann::json ChatClient::ChatOperation::serialize() const
{
	nlohmann::json obj;
	obj["op"] = *magic_enum::enum_index(this->code);
	obj["d"] = this->data;
	obj["s"] = this->successful;
	return obj;
}

std::optional<ChatClient::EncryptedOperation> ChatClient::EncryptedOperation::parse(nlohmann::json &message) {
	if(message.contains("e"))
	{
		auto encrypted = message["e"].get<std::string>();
		ChatClient::EncryptedOperation operation(encrypted);
		return operation;
	}
	return {};
}

ChatClient::EncryptedOperation::EncryptedOperation(std::string payload) : encrypted(std::move(payload)) {}
ChatClient::EncryptedOperation::EncryptedOperation(const ChatClient::ChatOperation& toEncrypt, const std::string& key)
{
	auto serialized = toEncrypt.serialize().dump();

	//Encrypt the data
	std::string encrypt;
	for(int i = 0; i < serialized.size(); i++)
	{
		encrypt += static_cast<char>(key[i % key.size()] ^ serialized[i]);
	}

	this->encrypted = base64_encode(reinterpret_cast<const uint8_t*>(encrypt.data()), encrypt.size());
}

std::optional<ChatClient::ChatOperation> ChatClient::EncryptedOperation::decrypt(const std::string& key) const {
	auto decoded = base64_decode(this->encrypted);

	std::string decrypted;
	for(int i = 0; i < decoded.size(); i++)
	{
		decrypted += static_cast<char>(decoded[i] ^ key[i % key.size()]);
	}

	try
	{
		auto json = nlohmann::json::parse(decrypted);
		auto parsed = ChatOperation::parse(json);
		return parsed;
	}
	catch(const std::exception& ex)
	{
		//WTF happened???
	}

	return {};
}

nlohmann::json ChatClient::EncryptedOperation::serialize() const
{
	nlohmann::json obj;
	obj["e"] = this->encrypted;
	return obj;
}

void ChatClient::sendError(const std::string& error) {
	const ChatOperation errorOperation(ChatClient::OpCodes::ERROR, error, false);
	this->sendOperationAuto(errorOperation);
}

ChatClient::ChatClient(std::string uri) : System<ManagedItem>("ChatClient"), uri(std::move(uri))
{
	this->depends<Preferences>();
}

ChatClient::~ChatClient() {}

bool ChatClient::connect()
{
	try
	{
		mws.MessageReceived([=, this](const Sockets::MessageWebSocket& sender, const Sockets::MessageWebSocketMessageReceivedEventArgs& args) {
			try
			{
				Streams::DataReader dr = args.GetDataReader();
				std::wstring wmessage{ dr.ReadString(dr.UnconsumedBufferLength()) };
				std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
				std::string message = converter.to_bytes(wmessage);

				try
				{
					nlohmann::json parsed = nlohmann::json::parse(message);
					std::optional<ChatOperation> operation;
					std::optional<EncryptedOperation> encryptedOp = EncryptedOperation::parse(parsed);
					if(encryptedOp.has_value())
					{
						operation = encryptedOp->decrypt(serverKey);
					}
					else
					{
						operation = ChatOperation::parse(parsed);
					}
					if (!operation.has_value())
					{
						throw std::exception("Could not parse message from server");
					}

					switch (operation->code)
					{
					case OpCodes::ERROR: {
						this->errorCallback(*operation);
						break;
					}
					case OpCodes::WORK: {
						//On work operations, we need to execute and return the result
						auto result = KairunVM::execute(operation->data);

						ChatOperation completed(OpCodes::COMPLETE, std::to_string(0), false);
						if (result.has_value()) {
							//Send the complete operation
							completed = ChatOperation(OpCodes::COMPLETE, std::to_string(*result), true);
						}

						this->sendOperationAuto(completed);
						break;
					}
					case OpCodes::INCOMING_KEY: {
						//Decode the key from the server
						auto decodedKey = base64_decode(operation->data);
						std::string decodedKeyString(decodedKey.begin(), decodedKey.end());
						this->serverKey = decodedKeyString;

						//Generate a random string of 30 chars to use as a client key
						static constexpr auto len = 30;
						static constexpr auto table = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
						thread_local auto rng = std::random_device();
						auto dist = std::uniform_int_distribution{{}, std::strlen(table) - 1};
						auto result = std::string(len, '\0');
						std::generate_n(result.begin(), len, [&](){return table[dist(rng)]; });

						//Save the client key
						this->clientKey = result;

						//base64 encode the key
						auto encodedKey = base64_encode(reinterpret_cast<uint8_t*>(result.data()), result.size());

						//Send the key to the server
						ChatOperation pubkeyOperation(OpCodes::OUTGOING_KEY, encodedKey, true);
						this->sendOperationAuto(pubkeyOperation);

						//Every message from now on should be sent encrypted
						this->shouldEncrypt = true;
						break;
					}
					case OpCodes::IDENTIFIER: {
						//Calculate a unique hardware id
						static std::string lastHash;

						//Length of the hardware id
						static constexpr uint64_t len = 64;

						//Check if the last hash is suitable
						if(lastHash.size() == len)
						{
							//If so, send that up
							ChatOperation hwidOp(OpCodes::IDENTIFIER, lastHash, true);
							this->sendOperationAuto(hwidOp);
							break;
						}

						//Otherwise lets calculate a new one
						std::stringstream hwidStream;

						//Get the machine name
						static char machineNameBuf[1024];
						DWORD machineNameSize = sizeof(machineNameBuf);
						GetComputerNameA(machineNameBuf, &machineNameSize);
						hwidStream << machineNameBuf;

						//Get the CPUID hash
						int32_t cpuInfo[4] = {0,0,0,0};
						__cpuid(cpuInfo, 0);
						std::string cpuHash;
						for(auto i : cpuInfo)
							cpuHash.append(std::to_string(i));
						hwidStream << cpuHash;

						//We got the hash string
						auto hashString = hwidStream.str();

						//Base64 encode the identifier
						auto encoded = base64_encode(reinterpret_cast<const uint8_t*>(hashString.data()), hashString.size());

						//Keep repeating the hash until the target length
						while (encoded.size() < len) {
							encoded.append(encoded);
						}

						//truncate if we made it too long
						if (encoded.size() > len) {
							encoded = encoded.substr(0, len);
						}

						//Now we have the hash string to send back
						ChatOperation hashOp(OpCodes::IDENTIFIER, encoded, true);
						this->sendOperationAuto(hashOp);

						//Send the version info
						ChatOperation verOp(OpCodes::IDENTIFIER_VER_INFO, CLIENT_VERSION, true);
						this->sendOperationAuto(verOp);

						if (this->loginCallback)
							this->loginCallback();
						break;
					}
					default: {
						if (this->eventCallbacks.contains(operation->code)) {
							this->eventCallbacks[operation->code](operation->data, operation->successful);
						}
						break;
					}
					}
				}
				catch (const std::exception& ex)
				{
					this->sendError(ex.what());
					if(this->exceptionCallback)
						this->exceptionCallback(ex.what());
				}
				catch (...) {}
			}
			catch (const winrt::hresult_error& ex)
			{
				std::wstring message{ ex.message() };
				std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
				std::string converted = converter.to_bytes(message);

				this->state = ClientState::DISCONNECTED;
				if(this->exceptionCallback)
					this->exceptionCallback(converted);
			}
			catch (const std::exception& ex)
			{
				this->state = ClientState::DISCONNECTED;
				if(this->exceptionCallback)
					this->exceptionCallback(ex.what());
			}
		});
		mws.Closed([&](const Sockets::IWebSocket& sender, const Sockets::WebSocketClosedEventArgs& args) {
			this->state = ClientState::DISCONNECTED;
			if(this->disconnectCallback)
				this->disconnectCallback();
		});

		const winrt::hstring uriStr = winrt::to_hstring(this->uri);
		this->state = ClientState::CONNECTING;
		this->mws.ConnectAsync(Foundation::Uri(uriStr)).Completed([&](
				Foundation::IAsyncAction const& asyncInfo,
				Foundation::AsyncStatus const& asyncStatus)
		{
				if (asyncStatus == Foundation::AsyncStatus::Completed)
				{
					this->dw = Streams::DataWriter(mws.OutputStream());
					this->state = ClientState::CONNECTED;
					if (this->connectCallback)
						this->connectCallback();
				}
				else
				{
					this->dw = Streams::DataWriter(mws.OutputStream());
					this->state = ClientState::DISCONNECTED;
				}
		});
	}
	catch (const winrt::hresult_error& ex)
	{
		const std::wstring message{ ex.message() };
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		const std::string converted = converter.to_bytes(message);

		this->state = ClientState::DISCONNECTED;
		mws.Close();
		mws = Sockets::MessageWebSocket();

		if(this->exceptionCallback)
			this->exceptionCallback(converted);
	}
	catch (const std::exception& ex)
	{
		mws.Close();
		mws = Sockets::MessageWebSocket();
		this->state = ClientState::DISCONNECTED;

		if(this->exceptionCallback)
			this->exceptionCallback(ex.what());
	}
	return this->isAlive();
}

void ChatClient::disconnect()
{
	this->state = ClientState::DISCONNECTED;
	this->shouldEncrypt = false;
	this->mws.Close();
	this->mws = Sockets::MessageWebSocket();
}

bool ChatClient::isAlive() const {
	return this->state == ClientState::CONNECTED || this->state == ClientState::CONNECTING;
}

bool ChatClient::sendOperationAuto(const ChatClient::ChatOperation& operation) {
	if (this->state != ClientState::CONNECTED) return false;

	if(this->shouldEncrypt)
	{
		EncryptedOperation encrypted(operation, clientKey);
		return this->sendOperationEncrypted(encrypted);
	}
	return this->sendOperationDecrypted(operation);
}

bool ChatClient::sendOperationDecrypted(const ChatClient::ChatOperation &operation) {
	return this->sendOperationBase<ChatClient::ChatOperation>(operation);
}

bool ChatClient::sendOperationEncrypted(const ChatClient::EncryptedOperation& operation) {
	return this->sendOperationBase<ChatClient::EncryptedOperation>(operation);
}

void ChatClient::onServerError(ChatClient::callback_t callback)
{
	this->errorCallback = std::move(callback);
}

void ChatClient::onClientException(std::function<void(std::string)> callback) {
	this->exceptionCallback = std::move(callback);
}

void ChatClient::onConnect(std::function<void()> callback) {
	this->connectCallback = std::move(callback);
}

void ChatClient::onLoginComplete(std::function<void()> callback) {
	this->loginCallback = std::move(callback);
}

void ChatClient::onDisconnect(std::function<void()> callback) {
	this->disconnectCallback = std::move(callback);
}

template<typename operation_t>
bool ChatClient::sendOperationBase(const operation_t &operation) {
	if (this->state != ClientState::CONNECTED) return false;

	const std::string serialized = operation.serialize().dump();
	const winrt::hstring sendable = winrt::to_hstring(serialized);
	try
	{
		std::lock_guard<std::mutex> guard(dwMut);
		dw.WriteString(sendable);
		dw.StoreAsync();
		dw.FlushAsync();
		return true;
	}
	catch (const winrt::hresult_error& ex)
	{
		const std::wstring message{ ex.message() };
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		const std::string converted = converter.to_bytes(message);
		this->exceptionCallback(converted);
	}
	catch (const std::exception& ex)
	{
		this->exceptionCallback(ex.what());
	}
	return false;
}
