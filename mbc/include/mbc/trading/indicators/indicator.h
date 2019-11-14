#pragma once
#include <thread>

#include "../data/entities.h"

namespace mbc::trading {

    //! An abstract indicator.
    class Indicator
    {
    public:
        //! Identifies the indicator.
        const char* name() noexcept { return name_.c_str(); }

        //! Identifies an instance of the indicator.
        const char* moniker() noexcept { return moniker_.c_str(); }

        //! Describes the indicator.
        const char* description() noexcept { return description_.c_str(); }

        //! Is the indicator primed.
        bool isPrimed() noexcept { std::lock_guard<std::mutex> guard(updateLock_); return primed_; }

        //! Resets the indicator.
        virtual void reset() = 0;

        //! The Ohlcv component.
        OhlcvComponent ohlcvComponent() const noexcept { return ohlcvComponent_; }

        //! Sets the Ohlcv component.
        //! \param ohlcvComponent A new compopent value to set.
        void ohlcvComponent(OhlcvComponent ohlcvComponent) noexcept { ohlcvComponent_ = ohlcvComponent; }

        Indicator(const Indicator& other) = default;
        Indicator(Indicator&& other) = default;
        Indicator& operator=(const Indicator& other) = default;
        Indicator& operator=(Indicator&& other) = default;
        virtual ~Indicator(){}

    protected:
        explicit Indicator(std::string name, std::string moniker, std::string description) :
            name_{ name }, moniker_{ moniker }, description_{ description }, ohlcvComponent_{ OhlcvComponent::ClosingPrice } {}

        explicit Indicator(std::string name, std::string moniker, std::string description, OhlcvComponent ohlcvComponent) :
            name_{ name }, moniker_{ moniker }, description_{ description }, ohlcvComponent_{ ohlcvComponent } {}

        std::string name_;
        std::string moniker_;
        std::string description_;
        bool primed_;
        std::mutex updateLock_;
        OhlcvComponent ohlcvComponent_;
    };
}
