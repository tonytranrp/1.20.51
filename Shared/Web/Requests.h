#pragma once

#ifndef USE_UWP
#include <windows.h>
#include <winhttp.h>
#else
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Web.Http.Headers.h>
using namespace winrt;
using namespace Windows;
#endif

#include <functional>
#include <map>
#include <string>

namespace Requests {
	std::string FormArgs(std::map<std::string, std::string> args);

	static int MakeRequest(
		std::string host,
		std::string route,
		std::map<std::string, std::string> args,
		std::string* response,
		std::function<bool(uint64_t current, uint64_t total)> progressLambda = [](uint64_t progress, uint64_t total) { return true; }
	)
	{
		std::string url_args = Requests::FormArgs(args);
		std::string fullRoute = route + url_args;

		std::wstring whost(host.begin(), host.end());
		std::wstring wfullRoute(fullRoute.begin(), fullRoute.end());
		std::wstring wfullUrl = L"https://" + whost + wfullRoute;

#if defined DEBUG_WEB and defined _DEBUG
		printf("Formed url args: %s", url_args.c_str());
		printf("Full route: %s", (host + route + url_args).c_str());
#endif
#ifndef USE_UWP //If not USE_UWP
		HINTERNET hSession = WinHttpOpen(L"Nuvola Web API/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
		if (hSession)
		{
			HINTERNET hConnect = WinHttpConnect(hSession, whost.c_str(), INTERNET_DEFAULT_PORT, 0);
			if (hConnect)
			{
				HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", wfullRoute.c_str(), NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
				if (hRequest)
				{
					if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0))
					{
						DWORD responseSize = 0;
						DWORD statusCode = 0;
						DWORD statusCodeSize = sizeof(DWORD);
						DWORD index = 0;
						bool success = WinHttpReceiveResponse(hRequest, NULL);
						success = success && WinHttpQueryDataAvailable(hRequest, &responseSize);
						success = success && WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &statusCode, &statusCodeSize, &index);
						if (success)
						{
							std::vector<BYTE> responseData;
							DWORD bytesRead = 0;
							BYTE buffer[4096];
							while (WinHttpReadData(hRequest, &buffer, sizeof(buffer), &bytesRead) && bytesRead > 0)
							{
								responseData.insert(responseData.end(), buffer, buffer + bytesRead);
								if (progressLambda(responseData.size(), responseSize) == false)
								{
#if defined DEBUG_WEB and defined _DEBUG
									printf("The request was cancelled");
#endif
									return -5;
								}
							}

							*response = std::string(responseData.begin(), responseData.end());
							return statusCode;
						}
						else
						{
#if defined DEBUG_WEB
							printf("WinHttpReceiveResponse call fail, GLE: %x", GetLastError());
#endif
							return -4;
						}
					}
					else
					{
#if defined DEBUG_WEB
						printf("WinHttpSendRequest call fail, GLE: %x", GetLastError());
#endif
						return -3;
					}
				}
				else
				{
#if defined DEBUG_WEB
					printf("WinHttpOpenRequest call fail, GLE: %x", GetLastError());
#endif
					return -2;
				}
			}
			else
			{
#if defined DEBUG_WEB
				printf("WinHttpConnect call fail, GLE: %x", GetLastError());
#endif
				return -6;
			}
		}
		else
		{
#if defined DEBUG_WEB
			printf("WinHttpOpen call fail, GLE: %x", GetLastError());
#endif
			return -1;
		}
#else //If USE_UWP
		// Create an HttpClient object.
		Windows::Web::Http::HttpClient httpClient;

		// Add a user-agent header to the GET request.
		auto headers{ httpClient.DefaultRequestHeaders() };

		Foundation::Uri requestUri{ wfullUrl.c_str() };

		// Send the GET request asynchronously, and retrieve the response as a string.
		Windows::Web::Http::HttpResponseMessage httpResponseMessage;
		std::wstring wresponse;

		try
		{
			// Send the GET request.
			httpResponseMessage = httpClient.GetAsync(requestUri).get();
			httpResponseMessage.EnsureSuccessStatusCode();
			wresponse = httpResponseMessage.Content().ReadAsStringAsync().get();
		}
		catch (winrt::hresult_error const& ex)
		{
			wresponse = ex.message();
		}

		for (wchar_t wc : wresponse) {
			response->push_back(wc);
		}

		return (int)httpResponseMessage.StatusCode();
#endif
	}
};