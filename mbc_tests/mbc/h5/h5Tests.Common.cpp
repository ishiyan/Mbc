#include "mbc/testharness.h"
#include "mbc/h5.h"

namespace h5 { namespace m5 = mbc::h5;

using File = m5::File;
using Instrument = m5::Instrument;
using DataTimeFrame = m5::DataTimeFrame;
using OhlcvKind = m5::OhlcvKind;
using OhlcvData = m5::OhlcvData;
using QuoteData = m5::QuoteData;
using ScalarData = m5::ScalarData;
using TradeData = m5::TradeData;
using TradePriceOnlyData = m5::TradePriceOnlyData;
using DuplicateTimeTicks = m5::DuplicateTimeTicks;

void createEnumerationDatasets(const char* fileName)
{
    remove(fileName);
    const std::shared_ptr<File> file = File::openReadWrite(fileName, true);

    std::shared_ptr<Instrument> instrument = file->openInstrument("a", true);
    const std::shared_ptr<OhlcvData> dsA1 = instrument->openOhlcvData(OhlcvKind::Default, DataTimeFrame::Aperiodic, true);
    const std::shared_ptr<OhlcvData> dsA2 = instrument->openOhlcvAdjustedData(OhlcvKind::Default, DataTimeFrame::Day1, true);
    dsA1->close();
    dsA2->close();
    instrument->close();

    instrument = file->openInstrument("b/c", true);
    const std::shared_ptr<QuoteData> dsBc1 = instrument->openQuoteData(true);
    const std::shared_ptr<TradeData> dsBc2 = instrument->openTradeData(true);
    dsBc1->close();
    dsBc2->close();
    instrument->close();

    instrument = file->openInstrument("d/e/f", true);
    const std::shared_ptr<ScalarData> dsDef1 = instrument->openScalarData(DataTimeFrame::Minute3, true);
    const std::shared_ptr<TradePriceOnlyData> dsDef2 = instrument->openTradePriceOnlyData(true);
    dsDef1->close();
    dsDef2->close();
    instrument->close();

    file->close();
}

void createEnumerationDatasetsInReverseOrder(const char* fileName)
{
    remove(fileName);
    const std::shared_ptr<File> file = File::openReadWrite(fileName, true);

    std::shared_ptr<Instrument> instrument = file->openInstrument("d/e/f", true);
    const std::shared_ptr<ScalarData> dsDef1 = instrument->openScalarData(DataTimeFrame::Minute3, true);
    const std::shared_ptr<TradePriceOnlyData> dsDef2 = instrument->openTradePriceOnlyData(true);
    dsDef1->close();
    dsDef2->close();
    instrument->close();

    instrument = file->openInstrument("b/c", true);
    const std::shared_ptr<QuoteData> dsBc1 = instrument->openQuoteData(true);
    const std::shared_ptr<TradeData> dsBc2 = instrument->openTradeData(true);
    dsBc1->close();
    dsBc2->close();
    instrument->close();

    instrument = file->openInstrument("a", true);
    const std::shared_ptr<OhlcvData> dsA1 = instrument->openOhlcvData(OhlcvKind::Default, DataTimeFrame::Aperiodic, true);
    const std::shared_ptr<OhlcvData> dsA2 = instrument->openOhlcvAdjustedData(OhlcvKind::Default, DataTimeFrame::Day1, true);
    dsA1->close();
    dsA2->close();
    instrument->close();

    file->close();
}

const char* to_string(DuplicateTimeTicks duplicateTimeTicks)
{
    switch (duplicateTimeTicks)
    {
        case DuplicateTimeTicks::Update:
            return "update";
        case DuplicateTimeTicks::Skip:
            return "skip";
        case DuplicateTimeTicks::Fail:
            return "fail";
        default:
            return "???";
    }
}

}
