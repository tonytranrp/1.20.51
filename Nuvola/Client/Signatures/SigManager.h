#pragma once

#include <Data/Manager.h>

#include "../System.h"

#include "SigProvider.h"
#include "SigTable.h"

#include <future>

class SigManager : public ::System<Manager<SigProvider>> {
public:
    SigManager();
    virtual void install(const std::shared_ptr<SystemInterface>& ownerPtr) override;
    template<typename provider_t>
    void provide()
    {
        initializingProviders.emplace_back(std::async([this]() {
            this->addItem<provider_t>();
        }));
    }

	void* findAddressOf(SigTable signature);
	template<typename sig_t>
	sig_t findAddressAs(SigTable signature) {
		void* addr = findAddressOf(signature);
		return *reinterpret_cast<sig_t*>(&addr);
	}
private:
    void awaitProviders();
    std::vector<std::future<void>> initializingProviders{};
};