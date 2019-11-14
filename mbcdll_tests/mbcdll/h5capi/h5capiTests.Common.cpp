#include "mbc/testharness.h"
#include "h5capi.h"
#include "h5capiTestInfo.h"

namespace h5capi { namespace th = mbc::testharness;

void createEnumerationDatasets(const char* fileName)
{
    remove(fileName);
    const H5FileHandle file = h5FileOpenReadWrite(fileName, true);

    H5InstrumentHandle instrument = h5FileOpenInstrument(file, "a", true);
    const H5OhlcvDataHandle dsA1 = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Aperiodic, true);
    const H5OhlcvDataHandle dsA2 = h5InstrumentOpenOhlcvAdjustedData(instrument, OhlcvKind::Default, DataTimeFrame::Day1, true);
    h5OhlcvDataClose(dsA1);
    h5OhlcvDataClose(dsA2);
    h5InstrumentClose(instrument);

    instrument = h5FileOpenInstrument(file, "b/c", true);
    const H5QuoteDataHandle dsBc1 = h5InstrumentOpenQuoteData(instrument, true);
    const H5TradeDataHandle dsBc2 = h5InstrumentOpenTradeData(instrument, true);
    h5QuoteDataClose(dsBc1);
    h5TradeDataClose(dsBc2);
    h5InstrumentClose(instrument);

    instrument = h5FileOpenInstrument(file, "d/e/f", true);
    const H5ScalarDataHandle dsDef1 = h5InstrumentOpenScalarData(instrument, DataTimeFrame::Minute3, true);
    const H5TradePriceOnlyDataHandle dsDef2 = h5InstrumentOpenTradePriceOnlyData(instrument, true);
    h5ScalarDataClose(dsDef1);
    h5TradePriceOnlyDataClose(dsDef2);
    h5InstrumentClose(instrument);

    h5FileClose(file);
}

void createEnumerationDatasetsInReverseOrder(const char* fileName)
{
    remove(fileName);
    const H5FileHandle file = h5FileOpenReadWrite(fileName, true);

    H5InstrumentHandle instrument = h5FileOpenInstrument(file, "d/e/f", true);
    const H5ScalarDataHandle dsDef1 = h5InstrumentOpenScalarData(instrument, DataTimeFrame::Minute3, true);
    const H5TradePriceOnlyDataHandle dsDef2 = h5InstrumentOpenTradePriceOnlyData(instrument, true);
    h5ScalarDataClose(dsDef1);
    h5TradePriceOnlyDataClose(dsDef2);
    h5InstrumentClose(instrument);

    instrument = h5FileOpenInstrument(file, "b/c", true);
    const H5QuoteDataHandle dsBc1 = h5InstrumentOpenQuoteData(instrument, true);
    const H5TradeDataHandle dsBc2 = h5InstrumentOpenTradeData(instrument, true);
    h5QuoteDataClose(dsBc1);
    h5TradeDataClose(dsBc2);
    h5InstrumentClose(instrument);

    instrument = h5FileOpenInstrument(file, "a", true);
    const H5OhlcvDataHandle dsA1 = h5InstrumentOpenOhlcvData(instrument, OhlcvKind::Default, DataTimeFrame::Aperiodic, true);
    const H5OhlcvDataHandle dsA2 = h5InstrumentOpenOhlcvAdjustedData(instrument, OhlcvKind::Default, DataTimeFrame::Day1, true);
    h5OhlcvDataClose(dsA1);
    h5OhlcvDataClose(dsA2);
    h5InstrumentClose(instrument);

    h5FileClose(file);
}
}
