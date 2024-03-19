#pragma once

#include "Event.h"
#include "Cancellable.h"

class ChatEvent : public Event, public Cancellable {
	std::string message;
public:
	explicit ChatEvent(const std::string_view message) : Event() {
		this->message = message;
	}

	[[nodiscard]] const std::string& getMessage() const {
		return this->message;
	}

	void setMessage(const std::string_view msg) {
		this->message = msg;
	}
};