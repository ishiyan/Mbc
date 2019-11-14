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
        explicit Indicator(string name, string moniker, string description) :
            name_{ name }, moniker_{ moniker }, description_{ description }, ohlcvComponent_{ OhlcvComponent.ClosingPrice } {}

        explicit Indicator(string name, string moniker, string description, OhlcvComponent ohlcvComponent) :
            name_{ name }, moniker_{ moniker }, description_{ description }, ohlcvComponent_{ ohlcvComponent } {}

        string name_;
        string moniker_;
        string description_;
        bool primed_;
        std::mutex updateLock_;
        OhlcvComponent ohlcvComponent_;
    };

    //! An abstract line indicator.
    class LineIndicator : public Indicator
    {
    public:
        //! Updates the value of the indicator.
        //! \param sample A new sample to update the value.
        //! \return The new value of the indicator.
        virtual double update(double sample) noexcept = 0;

        //! Updates the value of the indicator.
        //! \param sample A new sample to update the indicator.
        //! \param ticks The date and time represented in 100-nanosecond ticks.
        //! \return The new value of the indicator expressed as a Scalar.
        virtual Scalar update(double sample, long long ticks) { return new Scalar(ticks, update(sample)); }

        //! Updates the value of the indicator.
        //! \param scalar A new sample, expressed as a Scalar, to update the indicator.
        //! \return The new value of the indicator expressed as a Scalar.
        virtual Scalar update(Scalar scalar) { return new Scalar(scalar.ticks(), update(scalar.value())); }

        //! Updates the value of the indicator.
        //! \param ohlcv A new sample, expressed as an Ohlcv, to update the indicator.
        //! \return The new value of the indicator expressed as an Ohlcv.
        virtual Scalar update(Ohlcv ohlcv) { return new Scalar(ohlcv.ticks(), update(ohlcv.component(ohlcvComponent_))); }

        //! Constructs a new instance of the class.
        //! \param ticks The date and time represented in 100-nanosecond ticks.
        //! \param value The value.
        explicit Scalar(long long ticks, double value) : TemporalEntity(ticks), value_{ value } {}

        LineIndicator(const LineIndicator& other) = default;
        LineIndicator(LineIndicator&& other) = default;
        LineIndicator& operator=(const LineIndicator& other) = default;
        LineIndicator& operator=(LineIndicator&& other) = default;
        ~LineIndicator() override {}

    protected:
        explicit LineIndicator(string name, string moniker, string description) :
            Indicator(name, moniker, description) {}

        explicit LineIndicator(string name, string moniker, string description, OhlcvComponent ohlcvComponent) :
            Indicator(name, moniker, description, ohlcvComponent) {}
    };
}
