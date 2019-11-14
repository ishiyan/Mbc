#pragma once
#include <thread>

#include "../data/entities.h"
#include "indicator.h"

namespace mbc::trading {

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
        virtual Scalar* update(double sample, long long ticks) { return new Scalar(ticks, update(sample)); }

        //! Updates the value of the indicator.
        //! \param scalar A new sample, expressed as a Scalar, to update the indicator.
        //! \return The new value of the indicator expressed as a Scalar.
        virtual Scalar* update(Scalar scalar) { return new Scalar(scalar.ticks(), update(scalar.value())); }

        //! Updates the value of the indicator.
        //! \param ohlcv A new sample, expressed as an Ohlcv, to update the indicator.
        //! \return The new value of the indicator expressed as an Ohlcv.
        virtual Scalar* update(Ohlcv ohlcv) { return new Scalar(ohlcv.ticks(), update(ohlcv.component(ohlcvComponent_))); }

        LineIndicator(const LineIndicator& other) = default;
        LineIndicator(LineIndicator&& other) = default;
        LineIndicator& operator=(const LineIndicator& other) = default;
        LineIndicator& operator=(LineIndicator&& other) = default;
        ~LineIndicator() override {}

    protected:
        explicit LineIndicator(std::string name, std::string moniker, std::string description) :
            Indicator(name, moniker, description) {}

        explicit LineIndicator(std::string name, std::string moniker, std::string description, OhlcvComponent ohlcvComponent) :
            Indicator(name, moniker, description, ohlcvComponent) {}
    };
}
