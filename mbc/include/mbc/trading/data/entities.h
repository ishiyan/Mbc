#pragma once

#include "../../time/dotNetTicks.h"

namespace mbc::trading {

    //! Enumerates \c ohlcv price types.
    enum class OhlcvPriceType
    {
        //! The closing price.
        Close = 0,
        //! The opening price.
        Open,
        //! The highest price.
        High,
        //! The lowest price.
        Low,
        //! The median price, calculated as (low + high)/2.
        Median,
        //! The typical price, calculated as (low + high + close)/3.
        Typical,
        //! The weighted price, calculated as (low + high + close + open)/4.
        Weighted
    };

    //! Enumerates \c ohlcv components.
    enum class OhlcvComponent
    {
        //! The closing price.
        ClosingPrice = 0,
        //! The opening price.
        OpeningPrice,
        //! The highest price.
        HighPrice,
        //! The lowest price.
        LowPrice,
        //! The median price, calculated as (low + high)/2.
        MedianPrice,
        //! The typical price, calculated as (low + high + close)/3.
        TypicalPrice,
        //! The weighted price, calculated as (low + high + open + close)/4.
        WeightedPrice,
        //! The volume.
        Volume
    };

    //! An abstract temporal entity.
    class TemporalEntity
    {
    public:
        //! The date and time represented in 100-nanosecond ticks.
        [[nodiscard]] long long ticks() const noexcept { return ticks_; }

        //! Sets the date and time to the number of 100-nanosecond \p ticks.
        void ticks(const long long ticks) noexcept { ticks_ = ticks; }

        //! Adds the number of 100-nanosecond \p ticks to the time.
        //! \param ticks The number of 100-nanosecond ticks to add. This argument is permitted to be negative.
        void addTicks(const long long ticks) noexcept { ticks_ += ticks; }

        //! Adds a fractional number of \p days to the time.
        //! The result is computed by rounding the fractional number of days given by value to the nearest millisecond, and adding that interval.
        //! \param days A fractional number of days to add. This argument is permitted to be negative.
        void addDays(const double days) noexcept { ticks_ = time::DotNetTicks::addDays(ticks_, days); }

        //! Adds a fractional number of \p hours to the time.
        //! The result is computed by rounding the fractional number of hours given by value to the nearest millisecond, and adding that interval.
        //! \param hours A fractional number of hours to add. This argument is permitted to be negative.
        void addHours(const double hours) noexcept { ticks_ = time::DotNetTicks::addHours(ticks_, hours); }

        //! Adds a fractional number of \p  minutes to the time.
        //! The result is computed by rounding the fractional number of minutes given by value to the nearest millisecond, and adding that interval.
        //! \param minutes A fractional number of minutes to add. This argument is permitted to be negative.
        void addMinutes(const double minutes) noexcept { ticks_ = time::DotNetTicks::addMinutes(ticks_, minutes); }

        //! Adds a fractional number of \p seconds to the time.
        //! The result is computed by rounding the fractional number of seconds given by value to the nearest millisecond, and adding that interval.
        //! \param seconds A fractional number of seconds to add. This argument is permitted to be negative.
        void addSeconds(const double seconds) noexcept { ticks_ = time::DotNetTicks::addSeconds(ticks_, seconds); }

        //! Adds a fractional number of \p milliseconds to the time.
        //! The result is computed by rounding the fractional number of milliseconds given by value to the nearest millisecond, and adding that interval.
        //! \param milliseconds A fractional number of milliseconds to add. This argument is permitted to be negative.
        void addMilliseconds(const double milliseconds) noexcept { ticks_ = time::DotNetTicks::addMilliseconds(ticks_, milliseconds); }

        //! Makes a deep copy of this instance.
        [[nodiscard]] virtual TemporalEntity* clone() const = 0;

        TemporalEntity(const TemporalEntity& other) = default;
        TemporalEntity(TemporalEntity&& other) = default;
        TemporalEntity& operator=(const TemporalEntity& other) = default;
        TemporalEntity& operator=(TemporalEntity&& other) = default;
        virtual ~TemporalEntity() = default;

    protected:
        explicit TemporalEntity(const long long ticks) : ticks_{ ticks } {}

        long long ticks_;
    };

    inline bool operator==(const TemporalEntity& lhs, const TemporalEntity& rhs) { return lhs.ticks() == rhs.ticks(); }
    inline bool operator!=(const TemporalEntity& lhs, const TemporalEntity& rhs) { return lhs.ticks() != rhs.ticks(); }
    inline bool operator< (const TemporalEntity& lhs, const TemporalEntity& rhs) { return lhs.ticks() < rhs.ticks(); }
    inline bool operator> (const TemporalEntity& lhs, const TemporalEntity& rhs) { return lhs.ticks() > rhs.ticks(); }
    inline bool operator<=(const TemporalEntity& lhs, const TemporalEntity& rhs) { return lhs.ticks() <= rhs.ticks(); }
    inline bool operator>=(const TemporalEntity& lhs, const TemporalEntity& rhs) { return lhs.ticks() >= rhs.ticks(); }

    //! A scalar value entity.
    class Scalar final : public TemporalEntity
    {
    public:
        //! The value.
        [[nodiscard]] double value() const noexcept { return value_; }

        //! Sets the value.
        void value(const double value) noexcept { value_ = value; }

        //! Makes a deep copy of this instance.
        [[nodiscard]] TemporalEntity* clone() const override { return new Scalar(ticks_, value_); }

        //! Constructs a new instance of the class.
        //! \param ticks The date and time represented in 100-nanosecond ticks.
        //! \param value The value.
        explicit Scalar(const long long ticks, const double value) : TemporalEntity(ticks), value_{ value } {}

        Scalar(const Scalar& other) = default;
        Scalar(Scalar&& other) = default;
        Scalar& operator=(const Scalar& other) = default;
        Scalar& operator=(Scalar&& other) = default;
        ~Scalar() override = default;

    private:
        double value_;
    };

    //! A trade (price and volume) entity.
    class Trade final : public TemporalEntity
    {
    public:
        //! The price.
        [[nodiscard]] double price() const noexcept { return price_; }

        //! Sets the price.
        void price(const double price) noexcept { price_ = price; }

        //! The volume (quantity).
        [[nodiscard]] double volume() const noexcept { return volume_; }

        //! Sets the volume (quantity).
        void volume(const double volume) noexcept { volume_ = volume; }

        //! Makes a deep copy of this instance.
        [[nodiscard]] TemporalEntity* clone() const override { return new Trade(ticks_, price_, volume_); }

        //! Constructs a new instance of the class.
        //! \param ticks The date and time represented in 100-nanosecond ticks.
        //! \param price The price.
        //! \param volume The volume.
        explicit Trade(const long long ticks, const double price, const double volume) : TemporalEntity(ticks), price_{ price }, volume_{ volume } {}

        Trade(const Trade& other) = default;
        Trade(Trade&& other) = default;
        Trade& operator=(const Trade& other) = default;
        Trade& operator=(Trade&& other) = default;
        ~Trade() override = default;

    private:
        double price_, volume_;
    };

    //! A price quote (bid/ask price and size pair).
    class Quote final : public TemporalEntity
    {
    public:
        //! The bid price.
        [[nodiscard]] double bidPrice() const noexcept { return bidPrice_; }

        //! Sets the bid price.
        void bidPrice(const double price) noexcept { bidPrice_ = price; }

        //! The bid size.
        [[nodiscard]] double bidSize() const noexcept { return bidSize_; }

        //! Sets the bid size.
        void bidSize(const double size) noexcept { bidSize_ = size; }

        //! The ask price.
        [[nodiscard]] double askPrice() const noexcept { return askPrice_; }

        //! Sets the ask price.
        void askPrice(const double price) noexcept { askPrice_ = price; }

        //! The ask size.
        [[nodiscard]] double askSize() const noexcept { return askSize_; }

        //! Sets the ask size.
        void askSize(const double size) noexcept { askSize_ = size; }

        //! Makes a deep copy of this instance.
        [[nodiscard]] TemporalEntity* clone() const override { return new Quote(ticks_, bidPrice_, bidSize_, askPrice_, askSize_); }

        //! Constructs a new instance of the class.
        //! \param ticks The date and time represented in 100-nanosecond ticks.
        //! \param bidPrice The bid price.
        //! \param bidSize The bid size.
        //! \param askPrice The ask price.
        //! \param askSize The ask size.
        explicit Quote(const long long ticks, const double bidPrice, const double bidSize, const double askPrice, const double askSize) :
            TemporalEntity(ticks), bidPrice_{ bidPrice }, bidSize_{ bidSize }, askPrice_{ askPrice }, askSize_{ askSize } {}

        Quote(const Quote& other) = default;
        Quote(Quote&& other) = default;
        Quote& operator=(const Quote& other) = default;
        Quote& operator=(Quote&& other) = default;
        ~Quote() override = default;

    private:
        double bidPrice_, bidSize_, askPrice_, askSize_;
    };

    //! An [open, high, low, close, volume] bar.
    class Ohlcv final : public TemporalEntity
    {
    public:
        //! The opening price.
        [[nodiscard]] double open() const noexcept { return open_; }

        //! Sets the opening price.
        void open(const double price) noexcept { open_ = price; }

        //! The highest price.
        [[nodiscard]] double high() const noexcept { return high_; }

        //! Sets the highest price.
        void high(const double price) noexcept { high_ = price; }

        //! The lowest price.
        [[nodiscard]] double low() const noexcept { return low_; }

        //! Sets the lowest price.
        void low(const double price) noexcept { low_ = price; }

        //! The closing price.
        [[nodiscard]] double close() const noexcept { return close_; }

        //! Sets the closing price.
        void close(const double price) { close_ = price; }

        //! The volume.
        [[nodiscard]] double volume() const noexcept { return volume_; }

        //! Sets the volume.
        void volume(const double volume) noexcept { volume_ = volume; }

        //! The median price, calculated as (low + high)/2.
        [[nodiscard]] double median() const noexcept { return (low_ + high_) / 2; }

        //! The typical price, calculated as (low + high + close)/3.
        [[nodiscard]] double typical() const noexcept { return (low_ + high_ + close_) / 3; }

        //! The weighted close price, calculated as (low + high + 2*close)/4.
        [[nodiscard]] double weighted() const noexcept { return (low_ + high_ + close_ + close_) / 4; }

        //! The value of the specified price type.
        //! \param priceType The price type.
        [[nodiscard]] double price(const OhlcvPriceType priceType) const noexcept
        {
            switch (priceType)
            {
                case OhlcvPriceType::Open:     return open_;
                case OhlcvPriceType::High:     return high_;
                case OhlcvPriceType::Low:      return low_;
                case OhlcvPriceType::Close:    return close_;
                case OhlcvPriceType::Median:   return median();
                case OhlcvPriceType::Typical:  return typical();
                case OhlcvPriceType::Weighted: return weighted();
                default:                       return close_;
            }
        }

        //! The value of the specified component.
        //! \param component The component.
        [[nodiscard]] double component(const OhlcvComponent component) const noexcept
        {
            switch (component)
            {
                case OhlcvComponent::OpeningPrice:  return open_;
                case OhlcvComponent::HighPrice:     return high_;
                case OhlcvComponent::LowPrice:      return low_;
                case OhlcvComponent::ClosingPrice:  return close_;
                case OhlcvComponent::MedianPrice:   return median();
                case OhlcvComponent::TypicalPrice:  return typical();
                case OhlcvComponent::WeightedPrice: return weighted();
                case OhlcvComponent::Volume:        return volume_;
                default:                            return close_;
            }
        }

        //! Indicates a rising bar; the opening price is less than the closing price.
        [[nodiscard]] bool isRising() const noexcept { return open_ < close_; }

        //! Indicates a falling bar; the closing price is less than the opening price.
        [[nodiscard]] bool isFalling() const noexcept { return open_ > close_; }

        //! Makes a deep copy of the instance.
        [[nodiscard]] Ohlcv* cloneAggregation() const
        {
            return new Ohlcv(ticks_, open_, high_, low_, close_, volume_);
        }

        //! Constructs a new instance from the specified \p scalar.
        //! \param scalar The scalar to construct an instance from.
        static Ohlcv* cloneAggregation(const Scalar& scalar)
        {
            const double price = scalar.value();
            return new Ohlcv(scalar.ticks(), price, price, price, price);
        }

        //! Constructs a new instance from the specified \p trade.
        //! \param trade The trade to construct an instance from.
        static Ohlcv* cloneAggregation(const Trade& trade)
        {
            const double price = trade.price();
            return new Ohlcv(trade.ticks(), price, price, price, price, trade.volume());
        }

        //! Constructs a new instance from the specified \p quote.
        //! \param quote The quote to construct an instance from.
        //! \param isBid Whether to use bid or ask price and size.
        static Ohlcv* cloneAggregation(const Quote& quote, const bool isBid)
        {
            double price, size;
            if (isBid)
            {
                price = quote.bidPrice();
                size = quote.bidSize();
            }
            else
            {
                price = quote.askPrice();
                size = quote.askSize();
            }
            return new Ohlcv(quote.ticks(), price, price, price, price, size);
        }

        //! Aggregates this ohlcv bar with a next one in a continuous sequence of ohlcv bars.
        //! The resulting date and time is the date and time of the last aggregated bar.
        //! \param other An ohlcv bar to aggregate with.
        void aggregate(const Ohlcv& other) noexcept
        {
            ticks_ = other.ticks_; // Always the closing time.
            close_ = other.close_;
            volume_ += other.volume_;
            if (high_ < other.high_)
                high_ = other.high_;
            if (low_ > other.low_)
                low_ = other.low_;
        }

        //! Aggregates this ohlcv bar with a next scalar in a continuous sequence of scalars.
        //! The resulting date and time is the date and time of the last aggregated scalar.
        //! \param scalar A scalar to aggregate ohlcv bar with.
        void aggregate(const Scalar& scalar) noexcept
        {
            ticks_ = scalar.ticks(); // Always the closing time.
            const double price = scalar.value();
            close_ = price;
            if (high_ < price)
                high_ = price;
            if (low_ > price)
                low_ = price;
        }

        //! Aggregates this ohlcv bar with a next trade in a continuous sequence of trades.
        //! The resulting date and time is the date and time of the last aggregated trade.
        //! \param trade A trade to aggregate ohlcv bar with.
        void aggregate(const Trade& trade) noexcept
        {
            ticks_ = trade.ticks(); // Always the closing time.
            const double price = trade.price();
            close_ = price;
            volume_ += trade.volume();
            if (high_ < price)
                high_ = price;
            if (low_ > price)
                low_ = price;
        }

        //! Aggregates this ohlcv bar with a next quote in a continuous sequence of quotes.
        //! The resulting date and time is the date and time of the last aggregated quote.
        //! \param quote A quote to aggregate ohlcv bar with.
        //! \param isBid Whether to use bid or ask price and size.
        void aggregate(const Quote& quote, const bool isBid) noexcept
        {
            ticks_ = quote.ticks(); // Always the closing time.
            double price, size;
            if (isBid)
            {
                price = quote.bidPrice();
                size = quote.bidSize();
            }
            else
            {
                price = quote.askPrice();
                size = quote.askSize();
            }
            close_ = price;
            volume_ += size;
            if (high_ < price)
                high_ = price;
            if (low_ > price)
                low_ = price;
        }

        //! Makes a deep copy of this instance.
        [[nodiscard]] TemporalEntity* clone() const override { return new Ohlcv(ticks_, open_, high_, low_, close_, volume_); }

        //! Constructs a new instance of the class.
        //! \param ticks The date and time represented in 100-nanosecond ticks.
        //! \param open The opening price.
        //! \param high The highest price.
        //! \param low The lowest price.
        //! \param close The closing price.
        //! \param volume The volume.
        explicit Ohlcv(const long long ticks, const double open, const double high, const double low, const double close, const double volume) :
            TemporalEntity(ticks), open_{ open }, high_{ high }, low_{ low }, close_{ close }, volume_{ volume } {}

        //! Constructs a new instance of the class.
        //! \param ticks The date and time represented in 100-nanosecond ticks.
        //! \param open The opening price.
        //! \param high The highest price.
        //! \param low The lowest price.
        //! \param close The closing price.
        explicit Ohlcv(const long long ticks, const double open, const double high, const double low, const double close) :
            TemporalEntity(ticks), open_{ open }, high_{ high }, low_{ low }, close_{ close }, volume_{ 0 } {}

        Ohlcv(const Ohlcv& other) = default;
        Ohlcv(Ohlcv&& other) = default;
        Ohlcv& operator=(const Ohlcv& other) = default;
        Ohlcv& operator=(Ohlcv&& other) = default;
        ~Ohlcv() override = default;

    private:
        double open_, high_, low_, close_, volume_;
    };
}
