#include "../../mbc/include/mbc/h5.h"
#include "../include/h5capi.h"

#ifdef __cplusplus
extern "C" {
#endif

    // ----------------------------------------------------------------------
    // Utilities
    // ----------------------------------------------------------------------

    static inline H5FileHandle castToFileHandle(const std::shared_ptr<mbc::h5::File>&& pointer)
    {
        return static_cast<H5FileHandle>(pointer.operator bool() ? new const std::shared_ptr<mbc::h5::File>(pointer) : nullptr);
    }

    static inline H5InstrumentHandle castToInstrumentHandle(const std::shared_ptr<mbc::h5::Instrument>&& pointer)
    {
        return static_cast<H5InstrumentHandle>(pointer.operator bool() ? new const std::shared_ptr<mbc::h5::Instrument>(pointer) : nullptr);
    }

    static inline H5ScalarDataHandle castToScalarDataHandle(const std::shared_ptr<mbc::h5::ScalarData>&& pointer)
    {
        return static_cast<H5ScalarDataHandle>(pointer.operator bool() ? new const std::shared_ptr<mbc::h5::ScalarData>(pointer) : nullptr);
    }

    static inline H5TradeDataHandle castToTradeDataHandle(const std::shared_ptr<mbc::h5::TradeData>&& pointer)
    {
        return static_cast<H5TradeDataHandle>(pointer.operator bool() ? new const std::shared_ptr<mbc::h5::TradeData>(pointer) : nullptr);
    }

    static inline H5TradePriceOnlyDataHandle castToTradePriceOnlyDataHandle(const std::shared_ptr<mbc::h5::TradePriceOnlyData>&& pointer)
    {
        return static_cast<H5TradePriceOnlyDataHandle>(pointer.operator bool() ? new const std::shared_ptr<mbc::h5::TradePriceOnlyData>(pointer) : nullptr);
    }

    static inline H5OhlcvDataHandle castToOhlcvDataHandle(const std::shared_ptr<mbc::h5::OhlcvData>&& pointer)
    {
        return static_cast<H5OhlcvDataHandle>(pointer.operator bool() ? new const std::shared_ptr<mbc::h5::OhlcvData>(pointer) : nullptr);
    }

    static inline H5OhlcvPriceOnlyDataHandle castToOhlcvPriceOnlyDataHandle(const std::shared_ptr<mbc::h5::OhlcvPriceOnlyData>&& pointer)
    {
        return static_cast<H5OhlcvPriceOnlyDataHandle>(pointer.operator bool() ? new const std::shared_ptr<mbc::h5::OhlcvPriceOnlyData>(pointer) : nullptr);
    }

    static inline H5QuoteDataHandle castToQuoteDataHandle(const std::shared_ptr<mbc::h5::QuoteData>&& pointer)
    {
        return static_cast<H5QuoteDataHandle>(pointer.operator bool() ? new const std::shared_ptr<mbc::h5::QuoteData>(pointer) : nullptr);
    }

    static inline H5QuotePriceOnlyDataHandle castToQuotePriceOnlyDataHandle(const std::shared_ptr<mbc::h5::QuotePriceOnlyData>&& pointer)
    {
        return static_cast<H5QuotePriceOnlyDataHandle>(pointer.operator bool() ? new const std::shared_ptr<mbc::h5::QuotePriceOnlyData>(pointer) : nullptr);
    }

    static inline const std::shared_ptr<mbc::h5::File>* castFromFileHandle(H5FileHandle fileHandle)
    {
		if (fileHandle == nullptr)
			return nullptr;
		const auto ptr = static_cast<const std::shared_ptr<mbc::h5::File>*>(fileHandle);
		return *ptr == nullptr ? nullptr : ptr;
    }

    static inline const std::shared_ptr<mbc::h5::Instrument>* castFromInstrumentHandle(H5InstrumentHandle instrumentHandle)
    {
		if (instrumentHandle == nullptr)
			return nullptr;
		const auto ptr = static_cast<const std::shared_ptr<mbc::h5::Instrument>*>(instrumentHandle);
		return *ptr == nullptr ? nullptr : ptr;
    }

    static inline const std::shared_ptr<mbc::h5::ScalarData>* castFromScalarDataHandle(H5ScalarDataHandle dataHandle)
    {
        if (dataHandle == nullptr)
            return nullptr;
        const auto ptr = static_cast<const std::shared_ptr<mbc::h5::ScalarData>*>(dataHandle);
        return *ptr == nullptr ? nullptr : ptr;
    }

    static inline const std::shared_ptr<mbc::h5::TradeData>* castFromTradeDataHandle(H5TradeDataHandle dataHandle)
    {
        if (dataHandle == nullptr)
            return nullptr;
        const auto ptr = static_cast<const std::shared_ptr<mbc::h5::TradeData>*>(dataHandle);
        return *ptr == nullptr ? nullptr : ptr;
    }

    static inline const std::shared_ptr<mbc::h5::TradePriceOnlyData>* castFromTradePriceOnlyDataHandle(H5TradePriceOnlyDataHandle dataHandle)
    {
        if (dataHandle == nullptr)
            return nullptr;
        const auto ptr = static_cast<const std::shared_ptr<mbc::h5::TradePriceOnlyData>*>(dataHandle);
        return *ptr == nullptr ? nullptr : ptr;
    }

    static inline const std::shared_ptr<mbc::h5::OhlcvData>* castFromOhlcvDataHandle(H5OhlcvDataHandle dataHandle)
    {
        if (dataHandle == nullptr)
            return nullptr;
        const auto ptr = static_cast<const std::shared_ptr<mbc::h5::OhlcvData>*>(dataHandle);
        return *ptr == nullptr ? nullptr : ptr;
    }

    static inline const std::shared_ptr<mbc::h5::OhlcvPriceOnlyData>* castFromOhlcvPriceOnlyDataHandle(H5OhlcvPriceOnlyDataHandle dataHandle)
    {
        if (dataHandle == nullptr)
            return nullptr;
        const auto ptr = static_cast<const std::shared_ptr<mbc::h5::OhlcvPriceOnlyData>*>(dataHandle);
        return *ptr == nullptr ? nullptr : ptr;
    }

    static inline const std::shared_ptr<mbc::h5::QuoteData>* castFromQuoteDataHandle(H5QuoteDataHandle dataHandle)
    {
        if (dataHandle == nullptr)
            return nullptr;
        const auto ptr = static_cast<const std::shared_ptr<mbc::h5::QuoteData>*>(dataHandle);
        return *ptr == nullptr ? nullptr : ptr;
    }

    static inline const std::shared_ptr<mbc::h5::QuotePriceOnlyData>* castFromQuotePriceOnlyDataHandle(H5QuotePriceOnlyDataHandle dataHandle)
    {
        if (dataHandle == nullptr)
            return nullptr;
        const auto ptr = static_cast<const std::shared_ptr<mbc::h5::QuotePriceOnlyData>*>(dataHandle);
        return *ptr == nullptr ? nullptr : ptr;
    }

    // ----------------------------------------------------------------------
    // File
    // ----------------------------------------------------------------------

    DLL_PUBLIC void DLL_CC h5InterceptErrorStack(ErrorLogger errorLogger)
    {
        mbc::h5::File::interceptErrorStack(errorLogger);
    }

    DLL_PUBLIC H5FileHandle DLL_CC h5FileOpenReadOnly(const char* filePath, FileDriver fileDriver)
    {
        return castToFileHandle(mbc::h5::File::openReadOnly(filePath, static_cast<mbc::h5::FileDriver>(fileDriver)));
    }

    DLL_PUBLIC H5FileHandle DLL_CC h5FileOpenReadWrite(const char* filePath, bool createNonexistent, FileDriver fileDriver, bool corkTheCache)
    {
        return castToFileHandle(mbc::h5::File::openReadWrite(filePath, createNonexistent, static_cast<mbc::h5::FileDriver>(fileDriver), corkTheCache));
    }

    DLL_PUBLIC void DLL_CC h5FileClose(H5FileHandle fileHandle)
    {
		if (fileHandle == nullptr)
			return;
		// ReSharper disable once CppCStyleCast
		((std::shared_ptr<mbc::h5::File>*)fileHandle)->reset();
    }

    DLL_PUBLIC void DLL_CC h5FileFlush(H5FileHandle fileHandle)
    {
        const auto fp = castFromFileHandle(fileHandle);
        if (fp != nullptr)
            (*fp)->flush();
    }

    DLL_PUBLIC bool DLL_CC h5FileDeleteObject(H5FileHandle fileHandle, const char* objectPath, bool deleteEmptyParentPath) noexcept
    {
        const auto fp = castFromFileHandle(fileHandle);
        return fp == nullptr ? false : (*fp)->deleteObject(objectPath, deleteEmptyParentPath);
    }

    DLL_PUBLIC bool DLL_CC h5FileMoveInstrument(H5FileHandle fileHandle, const char* instrumentPathOld, const char* instrumentPathNew) noexcept
    {
        const auto fp = castFromFileHandle(fileHandle);
        return fp == nullptr ? false : (*fp)->moveInstrument(instrumentPathOld, instrumentPathNew);
    }

    DLL_PUBLIC bool DLL_CC h5FileRenameInstrument(H5FileHandle fileHandle, const char* instrumentPath, const char* instrumentNewName) noexcept
    {
        const auto fp = castFromFileHandle(fileHandle);
        return fp == nullptr ? false : (*fp)->renameInstrument(instrumentPath, instrumentNewName);
    }

    DLL_PUBLIC H5InstrumentHandle DLL_CC h5FileOpenInstrument(H5FileHandle fileHandle, const char* instrumentPath, bool createNonexistent)
    {
        const auto fp = castFromFileHandle(fileHandle);
        return fp == nullptr ? nullptr : castToInstrumentHandle((*fp)->openInstrument(instrumentPath, createNonexistent));
    }

    DLL_PUBLIC bool DLL_CC h5FileEnumerateDatasets(H5FileHandle fileHandle, DatasetEnumerator enumerate, bool sort)
    {
        const auto fp = castFromFileHandle(fileHandle);
        if (fp == nullptr)
            return false;
        return (*fp)->enumerateDatasets([enumerate](const mbc::h5::DataInfo& di)
        {
            enumerate(static_cast<DataType>(di.parsedDataType()), static_cast<DataKind>(di.parsedDataKind()),
                static_cast<OhlcvKind>(di.parsedOhlcvKind()), static_cast<ScalarKind>(di.parsedScalarKind()),
                static_cast<TradeKind>(di.parsedTradeKind()), static_cast<QuoteKind>(di.parsedQuoteKind()),
                static_cast<DataTimeFrame>(di.parsedTimeFrame()), di.isValidName(), di.name(), di.path());
        }, sort);
    }

    DLL_PUBLIC bool DLL_CC h5FileEnumerateInstrumentDatasets(H5FileHandle fileHandle, DatasetEnumerator enumerate, const char* instrumentPath, bool sort)
    {
        const auto fp = castFromFileHandle(fileHandle);
        if (fp == nullptr)
            return false;
        return (*fp)->enumerateDatasets([enumerate](const mbc::h5::DataInfo& di)
        {
            enumerate(static_cast<DataType>(di.parsedDataType()), static_cast<DataKind>(di.parsedDataKind()),
                static_cast<OhlcvKind>(di.parsedOhlcvKind()), static_cast<ScalarKind>(di.parsedScalarKind()),
                static_cast<TradeKind>(di.parsedTradeKind()), static_cast<QuoteKind>(di.parsedQuoteKind()),
                static_cast<DataTimeFrame>(di.parsedTimeFrame()), di.isValidName(), di.name(), di.path());
        }, instrumentPath, sort);
    }

    DLL_PUBLIC bool DLL_CC h5FileEnumerateDatasetsStatic(DatasetEnumerator enumerate, const char* filePath, FileDriver fileDriver, bool sort)
    {
        return mbc::h5::File::enumerateDatasets([enumerate](const mbc::h5::DataInfo& di)
        {
            enumerate(static_cast<DataType>(di.parsedDataType()), static_cast<DataKind>(di.parsedDataKind()),
                static_cast<OhlcvKind>(di.parsedOhlcvKind()), static_cast<ScalarKind>(di.parsedScalarKind()),
                static_cast<TradeKind>(di.parsedTradeKind()), static_cast<QuoteKind>(di.parsedQuoteKind()),
                static_cast<DataTimeFrame>(di.parsedTimeFrame()), di.isValidName(), di.name(), di.path());
        }, filePath, static_cast<mbc::h5::FileDriver>(fileDriver), sort);
    }

    DLL_PUBLIC bool DLL_CC h5FileEnumerateInstrumentDatasetsStatic(DatasetEnumerator enumerate, const char* filePath, FileDriver fileDriver, const char* instrumentPath, bool sort)
    {
        return mbc::h5::File::enumerateDatasets([enumerate](const mbc::h5::DataInfo& di)
        {
            enumerate(static_cast<DataType>(di.parsedDataType()), static_cast<DataKind>(di.parsedDataKind()),
                static_cast<OhlcvKind>(di.parsedOhlcvKind()), static_cast<ScalarKind>(di.parsedScalarKind()),
                static_cast<TradeKind>(di.parsedTradeKind()), static_cast<QuoteKind>(di.parsedQuoteKind()),
                static_cast<DataTimeFrame>(di.parsedTimeFrame()), di.isValidName(), di.name(), di.path());
        }, filePath, static_cast<mbc::h5::FileDriver>(fileDriver), instrumentPath, sort);
    }

    static void enumerateGroup(const mbc::h5::GroupInfo* groupInfo, const void* groupInfoHandle, GroupInfoAddDataInfo addDataInfo, GroupInfoAddGroupInfo addGroupInfo)
    {
        size_t size{ groupInfo->datasetsCount() };
        if (size > 0)
        {
            const mbc::h5::DataInfo** ptr{ groupInfo->datasets() };
            for (size_t i = 0; i != size; ++i)
            {
                const auto p = ptr[i];
                addDataInfo(groupInfoHandle, static_cast<DataType>(p->parsedDataType()), static_cast<DataKind>(p->parsedDataKind()),
                    static_cast<OhlcvKind>(p->parsedOhlcvKind()), static_cast<ScalarKind>(p->parsedScalarKind()),
                    static_cast<TradeKind>(p->parsedTradeKind()), static_cast<QuoteKind>(p->parsedQuoteKind()),
                    static_cast<DataTimeFrame>(p->parsedTimeFrame()), p->isValidName(), p->name(), p->path());
            }
        }

        size = groupInfo->groupsCount();
        if (size > 0)
        {
            const mbc::h5::GroupInfo** ptr{ groupInfo->groups() };
            for (size_t i = 0; i != size; ++i)
            {
                const auto p = ptr[i];
                const auto giChild = addGroupInfo(groupInfoHandle, p->name(), p->path());
                enumerateGroup(p, giChild, addDataInfo, addGroupInfo);
            }
        }
    }

    DLL_PUBLIC bool DLL_CC h5FileEnumerateContentTree(H5FileHandle fileHandle, const void* groupInfoHandle, GroupInfoAddDataInfo addDataInfo, GroupInfoAddGroupInfo addGroupInfo, bool sort)
    {
        const auto fp = castFromFileHandle(fileHandle);
        if (fp == nullptr)
            return false;

        const auto gi = (*fp)->contentTree(sort);
        if (gi.operator bool() == false)
            return false;

        enumerateGroup(gi.get(), groupInfoHandle, addDataInfo, addGroupInfo);
        return true;
    }

    DLL_PUBLIC bool DLL_CC h5FileEnumerateContentTreeStatic(const char* filePath, FileDriver fileDriver, const void* groupInfoHandle, GroupInfoAddDataInfo addDataInfo, GroupInfoAddGroupInfo addGroupInfo, bool sort)
    {
        const auto gi = mbc::h5::File::contentTree(filePath, static_cast<mbc::h5::FileDriver>(fileDriver), sort);
        if (gi == nullptr)
            return false;

        enumerateGroup(gi.get(), groupInfoHandle, addDataInfo, addGroupInfo);
        return true;
    }

    // ----------------------------------------------------------------------
    // Instrument
    // ----------------------------------------------------------------------

    DLL_PUBLIC void DLL_CC h5InstrumentClose(H5InstrumentHandle instrumentHandle)
    {
		if (instrumentHandle == nullptr)
			return;
		// ReSharper disable once CppCStyleCast
		((std::shared_ptr<mbc::h5::Instrument>*)instrumentHandle)->reset();
	}

    DLL_PUBLIC void DLL_CC h5InstrumentFlush(H5InstrumentHandle instrumentHandle)
    {
        const auto ip = castFromInstrumentHandle(instrumentHandle);
        if (ip != nullptr)
            (*ip)->flush();
    }

    DLL_PUBLIC bool DLL_CC h5InstrumentEnumerateDatasets(H5InstrumentHandle instrumentHandle, DatasetEnumerator enumerate, bool sort)
    {
        const auto ip = castFromInstrumentHandle(instrumentHandle);
        return ip != nullptr ? (*ip)->enumerateDatasets([enumerate](const mbc::h5::DataInfo& di)
        {
            enumerate(static_cast<DataType>(di.parsedDataType()), static_cast<DataKind>(di.parsedDataKind()),
                static_cast<OhlcvKind>(di.parsedOhlcvKind()), static_cast<ScalarKind>(di.parsedScalarKind()),
                static_cast<TradeKind>(di.parsedTradeKind()), static_cast<QuoteKind>(di.parsedQuoteKind()),
                static_cast<DataTimeFrame>(di.parsedTimeFrame()), di.isValidName(), di.name(), di.path());
        }, sort) : false;
    }

    DLL_PUBLIC H5ScalarDataHandle DLL_CC h5InstrumentOpenScalarData(H5InstrumentHandle instrumentHandle, DataTimeFrame timeFrame, bool createNonexistent)
    {
        const auto ip = castFromInstrumentHandle(instrumentHandle);
        return ip != nullptr ? castToScalarDataHandle((*ip)->openScalarData(static_cast<mbc::h5::DataTimeFrame>(timeFrame), createNonexistent)) : nullptr;
    }

    DLL_PUBLIC H5TradeDataHandle DLL_CC h5InstrumentOpenTradeData(H5InstrumentHandle instrumentHandle, bool createNonexistent)
    {
        const auto ip = castFromInstrumentHandle(instrumentHandle);
        return ip != nullptr ? castToTradeDataHandle((*ip)->openTradeData(createNonexistent)) : nullptr;
    }

    DLL_PUBLIC H5TradePriceOnlyDataHandle DLL_CC h5InstrumentOpenTradePriceOnlyData(H5InstrumentHandle instrumentHandle, bool createNonexistent)
    {
        const auto ip = castFromInstrumentHandle(instrumentHandle);
        return ip != nullptr ? castToTradePriceOnlyDataHandle((*ip)->openTradePriceOnlyData(createNonexistent)) : nullptr;
    }

    DLL_PUBLIC H5OhlcvDataHandle DLL_CC h5InstrumentOpenOhlcvData(H5InstrumentHandle instrumentHandle, OhlcvKind kind, DataTimeFrame timeFrame, bool createNonexistent)
    {
        const auto ip = castFromInstrumentHandle(instrumentHandle);
        return ip != nullptr ? castToOhlcvDataHandle((*ip)->openOhlcvData(static_cast<mbc::h5::OhlcvKind>(kind),
            static_cast<mbc::h5::DataTimeFrame>(timeFrame), createNonexistent)) : nullptr;
    }

    DLL_PUBLIC H5OhlcvPriceOnlyDataHandle DLL_CC h5InstrumentOpenOhlcvPriceOnlyData(H5InstrumentHandle instrumentHandle, OhlcvKind kind, DataTimeFrame timeFrame, bool createNonexistent)
    {
        const auto ip = castFromInstrumentHandle(instrumentHandle);
        return ip != nullptr ? castToOhlcvPriceOnlyDataHandle((*ip)->openOhlcvPriceOnlyData(static_cast<mbc::h5::OhlcvKind>(kind),
            static_cast<mbc::h5::DataTimeFrame>(timeFrame), createNonexistent)) : nullptr;
    }

    DLL_PUBLIC H5OhlcvDataHandle DLL_CC h5InstrumentOpenOhlcvAdjustedData(H5InstrumentHandle instrumentHandle, OhlcvKind kind, DataTimeFrame timeFrame, bool createNonexistent)
    {
        const auto ip = castFromInstrumentHandle(instrumentHandle);
        return ip != nullptr ? castToOhlcvDataHandle((*ip)->openOhlcvAdjustedData(static_cast<mbc::h5::OhlcvKind>(kind),
            static_cast<mbc::h5::DataTimeFrame>(timeFrame), createNonexistent)) : nullptr;
    }

    DLL_PUBLIC H5OhlcvPriceOnlyDataHandle DLL_CC h5InstrumentOpenOhlcvAdjustedPriceOnlyData(H5InstrumentHandle instrumentHandle, OhlcvKind kind, DataTimeFrame timeFrame, bool createNonexistent)
    {
        const auto ip = castFromInstrumentHandle(instrumentHandle);
        return ip != nullptr ? castToOhlcvPriceOnlyDataHandle((*ip)->openOhlcvAdjustedPriceOnlyData(static_cast<mbc::h5::OhlcvKind>(kind),
            static_cast<mbc::h5::DataTimeFrame>(timeFrame), createNonexistent)) : nullptr;
    }

    DLL_PUBLIC H5QuoteDataHandle DLL_CC h5InstrumentOpenQuoteData(H5InstrumentHandle instrumentHandle, bool createNonexistent)
    {
        const auto ip = castFromInstrumentHandle(instrumentHandle);
        return ip != nullptr ? castToQuoteDataHandle((*ip)->openQuoteData(createNonexistent)) : nullptr;
    }

    DLL_PUBLIC H5QuotePriceOnlyDataHandle DLL_CC h5InstrumentOpenQuotePriceOnlyData(H5InstrumentHandle instrumentHandle, bool createNonexistent)
    {
        const auto ip = castFromInstrumentHandle(instrumentHandle);
        return ip == nullptr ? nullptr : castToQuotePriceOnlyDataHandle((*ip)->openQuotePriceOnlyData(createNonexistent));
    }

    // ----------------------------------------------------------------------
    // Data
    // ----------------------------------------------------------------------

    DLL_PUBLIC unsigned long long DLL_CC h5DataGetDefaultMaximumReadBufferBytes()
    {
        return static_cast<unsigned long long>(mbc::h5::Data::defaultMaximumReadBufferBytes());
    }

    DLL_PUBLIC void DLL_CC h5DataSetDefaultMaximumReadBufferBytes(unsigned long long value)
    {
        mbc::h5::Data::setDefaultMaximumReadBufferBytes(static_cast<const hsize_t>(value));
    }

    // ----------------------------------------------------------------------
    // ScalarData
    // ----------------------------------------------------------------------

    DLL_PUBLIC void DLL_CC h5ScalarDataClose(H5ScalarDataHandle dataHandle)
    {
        if (dataHandle == nullptr)
            return;
        // ReSharper disable once CppCStyleCast
        ((std::shared_ptr<mbc::h5::ScalarData>*)dataHandle)->reset();
    }

    DLL_PUBLIC void DLL_CC h5ScalarDataFlush(H5ScalarDataHandle dataHandle)
    {
        const auto dp = castFromScalarDataHandle(dataHandle);
        if (dp != nullptr)
            (*dp)->flush();
    }

    DLL_PUBLIC long long DLL_CC h5ScalarDataFirstTicks(H5ScalarDataHandle dataHandle)
    {
        const auto dp = castFromScalarDataHandle(dataHandle);
        return dp == nullptr ? 0L : (*dp)->firstTicks();
    }

    DLL_PUBLIC long long DLL_CC h5ScalarDataLastTicks(H5ScalarDataHandle dataHandle)
    {
        const auto dp = castFromScalarDataHandle(dataHandle);
        return dp == nullptr ? 0L : (*dp)->lastTicks();
    }

    DLL_PUBLIC unsigned long long DLL_CC h5ScalarDataCount(H5ScalarDataHandle dataHandle)
    {
        const auto dp = castFromScalarDataHandle(dataHandle);
        return dp == nullptr ? 0UL : (*dp)->count();
    }

    DLL_PUBLIC unsigned long long DLL_CC h5ScalarDataGetMaximumReadBufferBytes(H5ScalarDataHandle dataHandle)
    {
        const auto dp = castFromScalarDataHandle(dataHandle);
        return dp == nullptr ? 0UL : static_cast<unsigned long long>((*dp)->maximumReadBufferBytes());
    }

    DLL_PUBLIC void DLL_CC h5ScalarDataSetMaximumReadBufferBytes(H5ScalarDataHandle dataHandle, unsigned long long value)
    {
        const auto dp = castFromScalarDataHandle(dataHandle);
        if (dp != nullptr)
            (*dp)->setMaximumReadBufferBytes(static_cast<const hsize_t>(value));
    }

    DLL_PUBLIC bool DLL_CC h5ScalarDataTicksIndex(H5ScalarDataHandle dataHandle, long long ticks, unsigned long long* index, bool* match)
    {
        bool status{ false };
        const auto dp = castFromScalarDataHandle(dataHandle);
        if (dp != nullptr)
        {
            hsize_t i;
            bool m;
            status = (*dp)->ticksIndex(ticks, i, m);
            if (status)
            {
                *index = i;
                *match = m;
            }
        }
        return status;
    }

    DLL_PUBLIC bool DLL_CC h5ScalarDataTicksIndexRange(H5ScalarDataHandle dataHandle, long long ticksFrom, long long ticksTo, unsigned long long* indexFrom, unsigned long long* indexTo, bool* matchFrom, bool* matchTo)
    {
        bool status{ false };
        const auto dp = castFromScalarDataHandle(dataHandle);
        if (dp != nullptr)
        {
            hsize_t i1, i2;
            bool m1, m2;
            status = (*dp)->ticksIndexRange(ticksFrom, ticksTo, i1, i2, m1, m2);
            if (status)
            {
                *indexFrom = i1;
                *indexTo = i2;
                *matchFrom = m1;
                *matchTo = m2;
            }
        }
        return status;
    }

    DLL_PUBLIC bool DLL_CC h5ScalarDataFetchAll(H5ScalarDataHandle dataHandle, ScalarDataAppender collect)
    {
        const auto dp = castFromScalarDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->fetchAll([collect](const mbc::h5::Scalar* data, hsize_t dataCount)
        {
            for (hsize_t i = 0; i < dataCount; ++i, ++data)
                collect(data->ticks_, data->value_);
        });
    }

    DLL_PUBLIC bool DLL_CC h5ScalarDataFetchTickRange(H5ScalarDataHandle dataHandle, ScalarDataAppender collect, long long from, long long to)
    {
        const auto dp = castFromScalarDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->fetchTickRange([collect](const mbc::h5::Scalar* data, hsize_t dataCount)
        {
            for (hsize_t i = 0; i < dataCount; ++i, ++data)
                collect(data->ticks_, data->value_);
        }, from, to);
    }

    DLL_PUBLIC bool DLL_CC h5ScalarDataFetchIndexRange(H5ScalarDataHandle dataHandle, ScalarDataAppender collect, unsigned long long index, unsigned long long count)
    {
        const auto dp = castFromScalarDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->fetchIndexRange([collect](const mbc::h5::Scalar* data, hsize_t dataCount)
        {
            for (hsize_t i = 0; i < dataCount; ++i, ++data)
                collect(data->ticks_, data->value_);
        }, index, count);
    }

    DLL_PUBLIC bool DLL_CC h5ScalarDataAdd(H5ScalarDataHandle dataHandle, const Scalar* input, unsigned long long inputCount, DuplicateTimeTicks duplicateTimeTicks, bool verbose)
    {
        const auto dp = castFromScalarDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->add(reinterpret_cast<const mbc::h5::Scalar*>(input), static_cast<hsize_t>(inputCount), static_cast<mbc::h5::DuplicateTimeTicks>(duplicateTimeTicks), verbose);
    }

    DLL_PUBLIC bool DLL_CC h5ScalarDataSpreadDuplicateTimeTicks(Scalar* input, size_t inputCount, bool verbose)
    {
        return mbc::h5::ScalarData::spreadDuplicateTimeTicks(reinterpret_cast<mbc::h5::Scalar*>(input), inputCount, verbose);
    }

    DLL_PUBLIC bool DLL_CC h5ScalarDataDeleteIndexRange(H5ScalarDataHandle dataHandle, hsize_t indexFrom, hsize_t indexTo)
    {
        const auto dp = castFromScalarDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteIndexRange(indexFrom, indexTo);
    }

    DLL_PUBLIC bool DLL_CC h5ScalarDataDeleteTickRange(H5ScalarDataHandle dataHandle, long long ticksFrom, long long ticksTo)
    {
        const auto dp = castFromScalarDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteTickRange(ticksFrom, ticksTo);
    }

    DLL_PUBLIC bool DLL_CC h5ScalarDataDeleteAllBeforeTick(H5ScalarDataHandle dataHandle, long long ticks)
    {
        const auto dp = castFromScalarDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllBeforeTick(ticks);
    }

    DLL_PUBLIC bool DLL_CC h5ScalarDataDeleteAllBeforeIndex(H5ScalarDataHandle dataHandle, hsize_t index)
    {
        const auto dp = castFromScalarDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllBeforeIndex(index);
    }

    DLL_PUBLIC bool DLL_CC h5ScalarDataDeleteAllAfterTick(H5ScalarDataHandle dataHandle, long long ticks)
    {
        const auto dp = castFromScalarDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllAfterTick(ticks);
    }

    DLL_PUBLIC bool DLL_CC h5ScalarDataDeleteAllAfterIndex(H5ScalarDataHandle dataHandle, hsize_t index)
    {
        const auto dp = castFromScalarDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllAfterIndex(index);
    }

    // ----------------------------------------------------------------------
    // TradeData
    // ----------------------------------------------------------------------

    DLL_PUBLIC void DLL_CC h5TradeDataClose(H5TradeDataHandle dataHandle)
    {
        if (dataHandle == nullptr)
            return;
        // ReSharper disable once CppCStyleCast
        ((std::shared_ptr<mbc::h5::TradeData>*)dataHandle)->reset();
    }

    DLL_PUBLIC void DLL_CC h5TradeDataFlush(H5TradeDataHandle dataHandle)
    {
        const auto dp = castFromTradeDataHandle(dataHandle);
        if (dp != nullptr)
            (*dp)->flush();
    }

    DLL_PUBLIC long long DLL_CC h5TradeDataFirstTicks(H5TradeDataHandle dataHandle)
    {
        const auto dp = castFromTradeDataHandle(dataHandle);
        return dp == nullptr ? 0L : (*dp)->firstTicks();
    }

    DLL_PUBLIC long long DLL_CC h5TradeDataLastTicks(H5TradeDataHandle dataHandle)
    {
        const auto dp = castFromTradeDataHandle(dataHandle);
        return dp == nullptr ? 0L : (*dp)->lastTicks();
    }

    DLL_PUBLIC unsigned long long DLL_CC h5TradeDataCount(H5TradeDataHandle dataHandle)
    {
        const auto dp = castFromTradeDataHandle(dataHandle);
        return dp == nullptr ? 0UL : (*dp)->count();
    }

    DLL_PUBLIC unsigned long long DLL_CC h5TradeDataGetMaximumReadBufferBytes(H5TradeDataHandle dataHandle)
    {
        const auto dp = castFromTradeDataHandle(dataHandle);
        return dp == nullptr ? 0UL : static_cast<unsigned long long>((*dp)->maximumReadBufferBytes());
    }

    DLL_PUBLIC void DLL_CC h5TradeDataSetMaximumReadBufferBytes(H5TradeDataHandle dataHandle, unsigned long long value)
    {
        const auto dp = castFromTradeDataHandle(dataHandle);
        if (dp != nullptr)
            (*dp)->setMaximumReadBufferBytes(static_cast<const hsize_t>(value));
    }

    DLL_PUBLIC bool DLL_CC h5TradeDataTicksIndex(H5TradeDataHandle dataHandle, long long ticks, unsigned long long* index, bool* match)
    {
        bool status{ false };
        const auto dp = castFromTradeDataHandle(dataHandle);
        if (dp != nullptr)
        {
            hsize_t i;
            bool m;
            status = (*dp)->ticksIndex(ticks, i, m);
            if (status)
            {
                *index = i;
                *match = m;
            }
        }
        return status;
    }

    DLL_PUBLIC bool DLL_CC h5TradeDataTicksIndexRange(H5TradeDataHandle dataHandle, long long ticksFrom, long long ticksTo, unsigned long long* indexFrom, unsigned long long* indexTo, bool* matchFrom, bool* matchTo)
    {
        bool status{ false };
        const auto dp = castFromTradeDataHandle(dataHandle);
        if (dp != nullptr)
        {
            hsize_t i1, i2;
            bool m1, m2;
            status = (*dp)->ticksIndexRange(ticksFrom, ticksTo, i1, i2, m1, m2);
            if (status)
            {
                *indexFrom = i1;
                *indexTo = i2;
                *matchFrom = m1;
                *matchTo = m2;
            }
        }
        return status;
    }

    DLL_PUBLIC bool DLL_CC h5TradeDataFetchAll(H5TradeDataHandle dataHandle, TradeDataAppender collect)
    {
        const auto dp = castFromTradeDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->fetchAll([collect](const mbc::h5::Trade* data, hsize_t dataCount)
        {
            for (hsize_t i = 0; i < dataCount; ++i, ++data)
                collect(data->ticks_, data->price_, data->volume_);
        });
    }

    DLL_PUBLIC bool DLL_CC h5TradeDataFetchTickRange(H5TradeDataHandle dataHandle, TradeDataAppender collect, long long from, long long to)
    {
        const auto dp = castFromTradeDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->fetchTickRange([collect](const mbc::h5::Trade* data, hsize_t dataCount)
        {
            for (hsize_t i = 0; i < dataCount; ++i, ++data)
                collect(data->ticks_, data->price_, data->volume_);
        }, from, to);
    }

    DLL_PUBLIC bool DLL_CC h5TradeDataFetchIndexRange(H5TradeDataHandle dataHandle, TradeDataAppender collect, unsigned long long index, unsigned long long count)
    {
        const auto dp = castFromTradeDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->fetchIndexRange([collect](const mbc::h5::Trade* data, hsize_t dataCount)
        {
            for (hsize_t i = 0; i < dataCount; ++i, ++data)
                collect(data->ticks_, data->price_, data->volume_);
        }, index, count);
    }

    DLL_PUBLIC bool DLL_CC h5TradeDataAdd(H5TradeDataHandle dataHandle, const Trade* input, unsigned long long inputCount, DuplicateTimeTicks duplicateTimeTicks, bool verbose)
    {
        const auto dp = castFromTradeDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->add(reinterpret_cast<const mbc::h5::Trade*>(input), static_cast<hsize_t>(inputCount), static_cast<mbc::h5::DuplicateTimeTicks>(duplicateTimeTicks), verbose);
    }

    DLL_PUBLIC bool DLL_CC h5TradeDataSpreadDuplicateTimeTicks(Trade* input, size_t inputCount, bool verbose)
    {
        return mbc::h5::TradeData::spreadDuplicateTimeTicks(reinterpret_cast<mbc::h5::Trade*>(input), inputCount, verbose);
    }

    DLL_PUBLIC bool DLL_CC h5TradeDataDeleteIndexRange(H5TradeDataHandle dataHandle, hsize_t indexFrom, hsize_t indexTo)
    {
        const auto dp = castFromTradeDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteIndexRange(indexFrom, indexTo);
    }

    DLL_PUBLIC bool DLL_CC h5TradeDataDeleteTickRange(H5TradeDataHandle dataHandle, long long ticksFrom, long long ticksTo)
    {
        const auto dp = castFromTradeDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteTickRange(ticksFrom, ticksTo);
    }

    DLL_PUBLIC bool DLL_CC h5TradeDataDeleteAllBeforeTick(H5TradeDataHandle dataHandle, long long ticks)
    {
        const auto dp = castFromTradeDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllBeforeTick(ticks);
    }

    DLL_PUBLIC bool DLL_CC h5TradeDataDeleteAllBeforeIndex(H5TradeDataHandle dataHandle, hsize_t index)
    {
        const auto dp = castFromTradeDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllBeforeIndex(index);
    }

    DLL_PUBLIC bool DLL_CC h5TradeDataDeleteAllAfterTick(H5TradeDataHandle dataHandle, long long ticks)
    {
        const auto dp = castFromTradeDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllAfterTick(ticks);
    }

    DLL_PUBLIC bool DLL_CC h5TradeDataDeleteAllAfterIndex(H5TradeDataHandle dataHandle, hsize_t index)
    {
        const auto dp = castFromTradeDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllAfterIndex(index);
    }

    // ----------------------------------------------------------------------
    // TradePriceOnlyData
    // ----------------------------------------------------------------------

    DLL_PUBLIC void DLL_CC h5TradePriceOnlyDataClose(H5TradePriceOnlyDataHandle dataHandle)
    {
        if (dataHandle == nullptr)
            return;
        // ReSharper disable once CppCStyleCast
        ((std::shared_ptr<mbc::h5::TradePriceOnlyData>*)dataHandle)->reset();
    }

    DLL_PUBLIC void DLL_CC h5TradePriceOnlyDataFlush(H5TradePriceOnlyDataHandle dataHandle)
    {
        const auto dp = castFromTradePriceOnlyDataHandle(dataHandle);
        if (dp != nullptr)
            (*dp)->flush();
    }

    DLL_PUBLIC long long DLL_CC h5TradePriceOnlyDataFirstTicks(H5TradePriceOnlyDataHandle dataHandle)
    {
        const auto dp = castFromTradePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? 0L : (*dp)->firstTicks();
    }

    DLL_PUBLIC long long DLL_CC h5TradePriceOnlyDataLastTicks(H5TradePriceOnlyDataHandle dataHandle)
    {
        const auto dp = castFromTradePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? 0L : (*dp)->lastTicks();
    }

    DLL_PUBLIC unsigned long long DLL_CC h5TradePriceOnlyDataCount(H5TradePriceOnlyDataHandle dataHandle)
    {
        const auto dp = castFromTradePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? 0UL : (*dp)->count();
    }

    DLL_PUBLIC unsigned long long DLL_CC h5TradePriceOnlyDataGetMaximumReadBufferBytes(H5TradePriceOnlyDataHandle dataHandle)
    {
        const auto dp = castFromTradePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? 0UL : static_cast<unsigned long long>((*dp)->maximumReadBufferBytes());
    }

    DLL_PUBLIC void DLL_CC h5TradePriceOnlyDataSetMaximumReadBufferBytes(H5TradePriceOnlyDataHandle dataHandle, unsigned long long value)
    {
        const auto dp = castFromTradePriceOnlyDataHandle(dataHandle);
        if (dp != nullptr)
            (*dp)->setMaximumReadBufferBytes(static_cast<const hsize_t>(value));
    }

    DLL_PUBLIC bool DLL_CC h5TradePriceOnlyDataTicksIndex(H5TradePriceOnlyDataHandle dataHandle, long long ticks, unsigned long long* index, bool* match)
    {
        bool status{ false };
        const auto dp = castFromTradePriceOnlyDataHandle(dataHandle);
        if (dp != nullptr)
        {
            hsize_t i;
            bool m;
            status = (*dp)->ticksIndex(ticks, i, m);
            if (status)
            {
                *index = i;
                *match = m;
            }
        }
        return status;
    }

    DLL_PUBLIC bool DLL_CC h5TradePriceOnlyDataTicksIndexRange(H5TradePriceOnlyDataHandle dataHandle, long long ticksFrom, long long ticksTo, unsigned long long* indexFrom, unsigned long long* indexTo, bool* matchFrom, bool* matchTo)
    {
        bool status{ false };
        const auto dp = castFromTradePriceOnlyDataHandle(dataHandle);
        if (dp != nullptr)
        {
            hsize_t i1, i2;
            bool m1, m2;
            status = (*dp)->ticksIndexRange(ticksFrom, ticksTo, i1, i2, m1, m2);
            if (status)
            {
                *indexFrom = i1;
                *indexTo = i2;
                *matchFrom = m1;
                *matchTo = m2;
            }
        }
        return status;
    }

    DLL_PUBLIC bool DLL_CC h5TradePriceOnlyDataFetchAll(H5TradePriceOnlyDataHandle dataHandle, TradePriceOnlyDataAppender collect)
    {
        const auto dp = castFromTradePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->fetchAll([collect](const mbc::h5::TradePriceOnly* data, hsize_t dataCount)
        {
            for (hsize_t i = 0; i < dataCount; ++i, ++data)
                collect(data->ticks_, data->price_);
        });
    }

    DLL_PUBLIC bool DLL_CC h5TradePriceOnlyDataFetchTickRange(H5TradePriceOnlyDataHandle dataHandle, TradePriceOnlyDataAppender collect, long long from, long long to)
    {
        const auto dp = castFromTradePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->fetchTickRange([collect](const mbc::h5::TradePriceOnly* data, hsize_t dataCount)
        {
            for (hsize_t i = 0; i < dataCount; ++i, ++data)
                collect(data->ticks_, data->price_);
        }, from, to);
    }

    DLL_PUBLIC bool DLL_CC h5TradePriceOnlyDataFetchIndexRange(H5TradePriceOnlyDataHandle dataHandle, TradePriceOnlyDataAppender collect, unsigned long long index, unsigned long long count)
    {
        const auto dp = castFromTradePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->fetchIndexRange([collect](const mbc::h5::TradePriceOnly* data, hsize_t dataCount)
        {
            for (hsize_t i = 0; i < dataCount; ++i, ++data)
                collect(data->ticks_, data->price_);
        }, index, count);
    }

    DLL_PUBLIC bool DLL_CC h5TradePriceOnlyDataAdd(H5TradePriceOnlyDataHandle dataHandle, const TradePriceOnly* input, unsigned long long inputCount, DuplicateTimeTicks duplicateTimeTicks, bool verbose)
    {
        const auto dp = castFromTradePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->add(reinterpret_cast<const mbc::h5::TradePriceOnly*>(input), static_cast<hsize_t>(inputCount), static_cast<mbc::h5::DuplicateTimeTicks>(duplicateTimeTicks), verbose);
    }

    DLL_PUBLIC bool DLL_CC h5TradePriceOnlyDataSpreadDuplicateTimeTicks(TradePriceOnly* input, size_t inputCount, bool verbose)
    {
        return mbc::h5::TradePriceOnlyData::spreadDuplicateTimeTicks(reinterpret_cast<mbc::h5::TradePriceOnly*>(input), inputCount, verbose);
    }

    DLL_PUBLIC bool DLL_CC h5TradePriceOnlyDataDeleteIndexRange(H5TradePriceOnlyDataHandle dataHandle, hsize_t indexFrom, hsize_t indexTo)
    {
        const auto dp = castFromTradePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteIndexRange(indexFrom, indexTo);
    }

    DLL_PUBLIC bool DLL_CC h5TradePriceOnlyDataDeleteTickRange(H5TradePriceOnlyDataHandle dataHandle, long long ticksFrom, long long ticksTo)
    {
        const auto dp = castFromTradePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteTickRange(ticksFrom, ticksTo);
    }

    DLL_PUBLIC bool DLL_CC h5TradePriceOnlyDataDeleteAllBeforeTick(H5TradePriceOnlyDataHandle dataHandle, long long ticks)
    {
        const auto dp = castFromTradePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllBeforeTick(ticks);
    }

    DLL_PUBLIC bool DLL_CC h5TradePriceOnlyDataDeleteAllBeforeIndex(H5TradePriceOnlyDataHandle dataHandle, hsize_t index)
    {
        const auto dp = castFromTradePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllBeforeIndex(index);
    }

    DLL_PUBLIC bool DLL_CC h5TradePriceOnlyDataDeleteAllAfterTick(H5TradePriceOnlyDataHandle dataHandle, long long ticks)
    {
        const auto dp = castFromTradePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllAfterTick(ticks);
    }

    DLL_PUBLIC bool DLL_CC h5TradePriceOnlyDataDeleteAllAfterIndex(H5TradePriceOnlyDataHandle dataHandle, hsize_t index)
    {
        const auto dp = castFromTradePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllAfterIndex(index);
    }

    // ----------------------------------------------------------------------
    // OhlcvData
    // ----------------------------------------------------------------------

    DLL_PUBLIC void DLL_CC h5OhlcvDataClose(H5OhlcvDataHandle dataHandle)
    {
        if (dataHandle == nullptr)
            return;
        // ReSharper disable once CppCStyleCast
        ((std::shared_ptr<mbc::h5::OhlcvData>*)dataHandle)->reset();
    }

    DLL_PUBLIC void DLL_CC h5OhlcvDataFlush(H5OhlcvDataHandle dataHandle)
    {
        const auto dp = castFromOhlcvDataHandle(dataHandle);
        if (dp != nullptr)
            (*dp)->flush();
    }

    DLL_PUBLIC long long DLL_CC h5OhlcvDataFirstTicks(H5OhlcvDataHandle dataHandle)
    {
        const auto dp = castFromOhlcvDataHandle(dataHandle);
        return dp == nullptr ? 0L : (*dp)->firstTicks();
    }

    DLL_PUBLIC long long DLL_CC h5OhlcvDataLastTicks(H5OhlcvDataHandle dataHandle)
    {
        const auto dp = castFromOhlcvDataHandle(dataHandle);
        return dp == nullptr ? 0L : (*dp)->lastTicks();
    }

    DLL_PUBLIC unsigned long long DLL_CC h5OhlcvDataCount(H5OhlcvDataHandle dataHandle)
    {
        const auto dp = castFromOhlcvDataHandle(dataHandle);
        return dp == nullptr ? 0UL : (*dp)->count();
    }

    DLL_PUBLIC unsigned long long DLL_CC h5OhlcvDataGetMaximumReadBufferBytes(H5OhlcvDataHandle dataHandle)
    {
        const auto dp = castFromOhlcvDataHandle(dataHandle);
        return dp == nullptr ? 0UL : static_cast<unsigned long long>((*dp)->maximumReadBufferBytes());
    }

    DLL_PUBLIC void DLL_CC h5OhlcvDataSetMaximumReadBufferBytes(H5OhlcvDataHandle dataHandle, unsigned long long value)
    {
        const auto dp = castFromOhlcvDataHandle(dataHandle);
        if (dp != nullptr)
            (*dp)->setMaximumReadBufferBytes(static_cast<const hsize_t>(value));
    }

    DLL_PUBLIC bool DLL_CC h5OhlcvDataTicksIndex(H5OhlcvDataHandle dataHandle, long long ticks, unsigned long long* index, bool* match)
    {
        bool status{ false };
        const auto dp = castFromOhlcvDataHandle(dataHandle);
        if (dp != nullptr)
        {
            hsize_t i;
            bool m;
            status = (*dp)->ticksIndex(ticks, i, m);
            if (status)
            {
                *index = i;
                *match = m;
            }
        }
        return status;
    }

    DLL_PUBLIC bool DLL_CC h5OhlcvDataTicksIndexRange(H5OhlcvDataHandle dataHandle, long long ticksFrom, long long ticksTo, unsigned long long* indexFrom, unsigned long long* indexTo, bool* matchFrom, bool* matchTo)
    {
        bool status{ false };
        const auto dp = castFromOhlcvDataHandle(dataHandle);
        if (dp != nullptr)
        {
            hsize_t i1, i2;
            bool m1, m2;
            status = (*dp)->ticksIndexRange(ticksFrom, ticksTo, i1, i2, m1, m2);
            if (status)
            {
                *indexFrom = i1;
                *indexTo = i2;
                *matchFrom = m1;
                *matchTo = m2;
            }
        }
        return status;
    }

    DLL_PUBLIC bool DLL_CC h5OhlcvDataFetchAll(H5OhlcvDataHandle dataHandle, OhlcvDataAppender collect)
    {
        const auto dp = castFromOhlcvDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->fetchAll([collect](const mbc::h5::Ohlcv* data, hsize_t dataCount)
        {
            for (hsize_t i = 0; i < dataCount; ++i, ++data)
                collect(data->ticks_, data->open_, data->high_, data->low_, data->close_, data->volume_);
        });
    }

    DLL_PUBLIC bool DLL_CC h5OhlcvDataFetchTickRange(H5OhlcvDataHandle dataHandle, OhlcvDataAppender collect, long long from, long long to)
    {
        const auto dp = castFromOhlcvDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->fetchTickRange([collect](const mbc::h5::Ohlcv* data, hsize_t dataCount)
        {
            for (hsize_t i = 0; i < dataCount; ++i, ++data)
                collect(data->ticks_, data->open_, data->high_, data->low_, data->close_, data->volume_);
        }, from, to);
    }

    DLL_PUBLIC bool DLL_CC h5OhlcvDataFetchIndexRange(H5OhlcvDataHandle dataHandle, OhlcvDataAppender collect, unsigned long long index, unsigned long long count)
    {
        const auto dp = castFromOhlcvDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->fetchIndexRange([collect](const mbc::h5::Ohlcv* data, hsize_t dataCount)
        {
            for (hsize_t i = 0; i < dataCount; ++i, ++data)
                collect(data->ticks_, data->open_, data->high_, data->low_, data->close_, data->volume_);
        }, index, count);
    }

    DLL_PUBLIC bool DLL_CC h5OhlcvDataAdd(H5OhlcvDataHandle dataHandle, const Ohlcv* input, unsigned long long inputCount, DuplicateTimeTicks duplicateTimeTicks, bool verbose)
    {
        const auto dp = castFromOhlcvDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->add(reinterpret_cast<const mbc::h5::Ohlcv*>(input), static_cast<hsize_t>(inputCount), static_cast<mbc::h5::DuplicateTimeTicks>(duplicateTimeTicks), verbose);
    }

    DLL_PUBLIC bool DLL_CC h5OhlcvDataSpreadDuplicateTimeTicks(Ohlcv* input, size_t inputCount, bool verbose)
    {
        return mbc::h5::OhlcvData::spreadDuplicateTimeTicks(reinterpret_cast<mbc::h5::Ohlcv*>(input), inputCount, verbose);
    }

    DLL_PUBLIC bool DLL_CC h5OhlcvDataDeleteIndexRange(H5OhlcvDataHandle dataHandle, hsize_t indexFrom, hsize_t indexTo)
    {
        const auto dp = castFromOhlcvDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteIndexRange(indexFrom, indexTo);
    }

    DLL_PUBLIC bool DLL_CC h5OhlcvDataDeleteTickRange(H5OhlcvDataHandle dataHandle, long long ticksFrom, long long ticksTo)
    {
        const auto dp = castFromOhlcvDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteTickRange(ticksFrom, ticksTo);
    }

    DLL_PUBLIC bool DLL_CC h5OhlcvDataDeleteAllBeforeTick(H5OhlcvDataHandle dataHandle, long long ticks)
    {
        const auto dp = castFromOhlcvDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllBeforeTick(ticks);
    }

    DLL_PUBLIC bool DLL_CC h5OhlcvDataDeleteAllBeforeIndex(H5OhlcvDataHandle dataHandle, hsize_t index)
    {
        const auto dp = castFromOhlcvDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllBeforeIndex(index);
    }

    DLL_PUBLIC bool DLL_CC h5OhlcvDataDeleteAllAfterTick(H5OhlcvDataHandle dataHandle, long long ticks)
    {
        const auto dp = castFromOhlcvDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllAfterTick(ticks);
    }

    DLL_PUBLIC bool DLL_CC h5OhlcvDataDeleteAllAfterIndex(H5OhlcvDataHandle dataHandle, hsize_t index)
    {
        const auto dp = castFromOhlcvDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllAfterIndex(index);
    }

    // ----------------------------------------------------------------------
    // OhlcvPriceOnlyData
    // ----------------------------------------------------------------------

    DLL_PUBLIC void DLL_CC h5OhlcvPriceOnlyDataClose(H5OhlcvPriceOnlyDataHandle dataHandle)
    {
        if (dataHandle == nullptr)
            return;
        // ReSharper disable once CppCStyleCast
        ((std::shared_ptr<mbc::h5::OhlcvPriceOnlyData>*)dataHandle)->reset();
    }

    DLL_PUBLIC void DLL_CC h5OhlcvPriceOnlyDataFlush(H5OhlcvPriceOnlyDataHandle dataHandle)
    {
        const auto dp = castFromOhlcvPriceOnlyDataHandle(dataHandle);
        if (dp != nullptr)
            (*dp)->flush();
    }

    DLL_PUBLIC long long DLL_CC h5OhlcvPriceOnlyDataFirstTicks(H5OhlcvPriceOnlyDataHandle dataHandle)
    {
        const auto dp = castFromOhlcvPriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? 0L : (*dp)->firstTicks();
    }

    DLL_PUBLIC long long DLL_CC h5OhlcvPriceOnlyDataLastTicks(H5OhlcvPriceOnlyDataHandle dataHandle)
    {
        const auto dp = castFromOhlcvPriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? 0L : (*dp)->lastTicks();
    }

    DLL_PUBLIC unsigned long long DLL_CC h5OhlcvPriceOnlyDataCount(H5OhlcvPriceOnlyDataHandle dataHandle)
    {
        const auto dp = castFromOhlcvPriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? 0UL : (*dp)->count();
    }

    DLL_PUBLIC unsigned long long DLL_CC h5OhlcvPriceOnlyDataGetMaximumReadBufferBytes(H5OhlcvPriceOnlyDataHandle dataHandle)
    {
        const auto dp = castFromOhlcvPriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? 0UL : static_cast<unsigned long long>((*dp)->maximumReadBufferBytes());
    }

    DLL_PUBLIC void DLL_CC h5OhlcvPriceOnlyDataSetMaximumReadBufferBytes(H5OhlcvPriceOnlyDataHandle dataHandle, unsigned long long value)
    {
        const auto dp = castFromOhlcvPriceOnlyDataHandle(dataHandle);
        if (dp != nullptr)
            (*dp)->setMaximumReadBufferBytes(static_cast<const hsize_t>(value));
    }

    DLL_PUBLIC bool DLL_CC h5OhlcvPriceOnlyDataTicksIndex(H5OhlcvPriceOnlyDataHandle dataHandle, long long ticks, unsigned long long* index, bool* match)
    {
        bool status{ false };
        const auto dp = castFromOhlcvPriceOnlyDataHandle(dataHandle);
        if (dp != nullptr)
        {
            hsize_t i;
            bool m;
            status = (*dp)->ticksIndex(ticks, i, m);
            if (status)
            {
                *index = i;
                *match = m;
            }
        }
        return status;
    }

    DLL_PUBLIC bool DLL_CC h5OhlcvPriceOnlyDataTicksIndexRange(H5OhlcvPriceOnlyDataHandle dataHandle, long long ticksFrom, long long ticksTo, unsigned long long* indexFrom, unsigned long long* indexTo, bool* matchFrom, bool* matchTo)
    {
        bool status{ false };
        const auto dp = castFromOhlcvPriceOnlyDataHandle(dataHandle);
        if (dp != nullptr)
        {
            hsize_t i1, i2;
            bool m1, m2;
            status = (*dp)->ticksIndexRange(ticksFrom, ticksTo, i1, i2, m1, m2);
            if (status)
            {
                *indexFrom = i1;
                *indexTo = i2;
                *matchFrom = m1;
                *matchTo = m2;
            }
        }
        return status;
    }

    DLL_PUBLIC bool DLL_CC h5OhlcvPriceOnlyDataFetchAll(H5OhlcvPriceOnlyDataHandle dataHandle, OhlcvPriceOnlyDataAppender collect)
    {
        const auto dp = castFromOhlcvPriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->fetchAll([collect](const mbc::h5::OhlcvPriceOnly* data, hsize_t dataCount)
        {
            for (hsize_t i = 0; i < dataCount; ++i, ++data)
                collect(data->ticks_, data->open_, data->high_, data->low_, data->close_);
        });
    }

    DLL_PUBLIC bool DLL_CC h5OhlcvPriceOnlyDataFetchTickRange(H5OhlcvPriceOnlyDataHandle dataHandle, OhlcvPriceOnlyDataAppender collect, long long from, long long to)
    {
        const auto dp = castFromOhlcvPriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->fetchTickRange([collect](const mbc::h5::OhlcvPriceOnly* data, hsize_t dataCount)
        {
            for (hsize_t i = 0; i < dataCount; ++i, ++data)
                collect(data->ticks_, data->open_, data->high_, data->low_, data->close_);
        }, from, to);
    }

    DLL_PUBLIC bool DLL_CC h5OhlcvPriceOnlyDataFetchIndexRange(H5OhlcvPriceOnlyDataHandle dataHandle, OhlcvPriceOnlyDataAppender collect, unsigned long long index, unsigned long long count)
    {
        const auto dp = castFromOhlcvPriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->fetchIndexRange([collect](const mbc::h5::OhlcvPriceOnly* data, hsize_t dataCount)
        {
            for (hsize_t i = 0; i < dataCount; ++i, ++data)
                collect(data->ticks_, data->open_, data->high_, data->low_, data->close_);
        }, index, count);
    }

    DLL_PUBLIC bool DLL_CC h5OhlcvPriceOnlyDataAdd(H5OhlcvPriceOnlyDataHandle dataHandle, const OhlcvPriceOnly* input, unsigned long long inputCount, DuplicateTimeTicks duplicateTimeTicks, bool verbose)
    {
        const auto dp = castFromOhlcvPriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->add(reinterpret_cast<const mbc::h5::OhlcvPriceOnly*>(input), static_cast<hsize_t>(inputCount), static_cast<mbc::h5::DuplicateTimeTicks>(duplicateTimeTicks), verbose);
    }

    DLL_PUBLIC bool DLL_CC h5OhlcvPriceOnlyDataSpreadDuplicateTimeTicks(OhlcvPriceOnly* input, size_t inputCount, bool verbose)
    {
        return mbc::h5::OhlcvPriceOnlyData::spreadDuplicateTimeTicks(reinterpret_cast<mbc::h5::OhlcvPriceOnly*>(input), inputCount, verbose);
    }

    DLL_PUBLIC bool DLL_CC h5OhlcvPriceOnlyDataDeleteIndexRange(H5OhlcvPriceOnlyDataHandle dataHandle, hsize_t indexFrom, hsize_t indexTo)
    {
        const auto dp = castFromOhlcvPriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteIndexRange(indexFrom, indexTo);
    }

    DLL_PUBLIC bool DLL_CC h5OhlcvPriceOnlyDataDeleteTickRange(H5OhlcvPriceOnlyDataHandle dataHandle, long long ticksFrom, long long ticksTo)
    {
        const auto dp = castFromOhlcvPriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteTickRange(ticksFrom, ticksTo);
    }

    DLL_PUBLIC bool DLL_CC h5OhlcvPriceOnlyDataDeleteAllBeforeTick(H5OhlcvPriceOnlyDataHandle dataHandle, long long ticks)
    {
        const auto dp = castFromOhlcvPriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllBeforeTick(ticks);
    }

    DLL_PUBLIC bool DLL_CC h5OhlcvPriceOnlyDataDeleteAllBeforeIndex(H5OhlcvPriceOnlyDataHandle dataHandle, hsize_t index)
    {
        const auto dp = castFromOhlcvPriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllBeforeIndex(index);
    }

    DLL_PUBLIC bool DLL_CC h5OhlcvPriceOnlyDataDeleteAllAfterTick(H5OhlcvPriceOnlyDataHandle dataHandle, long long ticks)
    {
        const auto dp = castFromOhlcvPriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllAfterTick(ticks);
    }

    DLL_PUBLIC bool DLL_CC h5OhlcvPriceOnlyDataDeleteAllAfterIndex(H5OhlcvPriceOnlyDataHandle dataHandle, hsize_t index)
    {
        const auto dp = castFromOhlcvPriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllAfterIndex(index);
    }

    // ----------------------------------------------------------------------
    // QuoteData
    // ----------------------------------------------------------------------

    DLL_PUBLIC void DLL_CC h5QuoteDataClose(H5QuoteDataHandle dataHandle)
    {
        if (dataHandle == nullptr)
            return;
        // ReSharper disable once CppCStyleCast
        ((std::shared_ptr<mbc::h5::QuoteData>*)dataHandle)->reset();
    }

    DLL_PUBLIC void DLL_CC h5QuoteDataFlush(H5QuoteDataHandle dataHandle)
    {
        const auto dp = castFromQuoteDataHandle(dataHandle);
        if (dp != nullptr)
            (*dp)->flush();
    }

    DLL_PUBLIC long long DLL_CC h5QuoteDataFirstTicks(H5QuoteDataHandle dataHandle)
    {
        const auto dp = castFromQuoteDataHandle(dataHandle);
        return dp == nullptr ? 0L : (*dp)->firstTicks();
    }

    DLL_PUBLIC long long DLL_CC h5QuoteDataLastTicks(H5QuoteDataHandle dataHandle)
    {
        const auto dp = castFromQuoteDataHandle(dataHandle);
        return dp == nullptr ? 0L : (*dp)->lastTicks();
    }

    DLL_PUBLIC unsigned long long DLL_CC h5QuoteDataCount(H5QuoteDataHandle dataHandle)
    {
        const auto dp = castFromQuoteDataHandle(dataHandle);
        return dp == nullptr ? 0UL : (*dp)->count();
    }

    DLL_PUBLIC unsigned long long DLL_CC h5QuoteDataGetMaximumReadBufferBytes(H5QuoteDataHandle dataHandle)
    {
        const auto dp = castFromQuoteDataHandle(dataHandle);
        return dp == nullptr ? 0UL : static_cast<unsigned long long>((*dp)->maximumReadBufferBytes());
    }

    DLL_PUBLIC void DLL_CC h5QuoteDataSetMaximumReadBufferBytes(H5QuoteDataHandle dataHandle, unsigned long long value)
    {
        const auto dp = castFromQuoteDataHandle(dataHandle);
        if (dp != nullptr)
            (*dp)->setMaximumReadBufferBytes(static_cast<const hsize_t>(value));
    }

    DLL_PUBLIC bool DLL_CC h5QuoteDataTicksIndex(H5QuoteDataHandle dataHandle, long long ticks, unsigned long long* index, bool* match)
    {
        bool status{ false };
        const auto dp = castFromQuoteDataHandle(dataHandle);
        if (dp != nullptr)
        {
            hsize_t i;
            bool m;
            status = (*dp)->ticksIndex(ticks, i, m);
            if (status)
            {
                *index = i;
                *match = m;
            }
        }
        return status;
    }

    DLL_PUBLIC bool DLL_CC h5QuoteDataTicksIndexRange(H5QuoteDataHandle dataHandle, long long ticksFrom, long long ticksTo, unsigned long long* indexFrom, unsigned long long* indexTo, bool* matchFrom, bool* matchTo)
    {
        bool status{ false };
        const auto dp = castFromQuoteDataHandle(dataHandle);
        if (dp != nullptr)
        {
            hsize_t i1, i2;
            bool m1, m2;
            status = (*dp)->ticksIndexRange(ticksFrom, ticksTo, i1, i2, m1, m2);
            if (status)
            {
                *indexFrom = i1;
                *indexTo = i2;
                *matchFrom = m1;
                *matchTo = m2;
            }
        }
        return status;
    }

    DLL_PUBLIC bool DLL_CC h5QuoteDataFetchAll(H5QuoteDataHandle dataHandle, QuoteDataAppender collect)
    {
        const auto dp = castFromQuoteDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->fetchAll([collect](const mbc::h5::Quote* data, hsize_t dataCount)
        {
            for (hsize_t i = 0; i < dataCount; ++i, ++data)
                collect(data->ticks_, data->askPrice_, data->bidPrice_, data->askSize_, data->askSize_);
        });
    }

    DLL_PUBLIC bool DLL_CC h5QuoteDataFetchTickRange(H5QuoteDataHandle dataHandle, QuoteDataAppender collect, long long from, long long to)
    {
        const auto dp = castFromQuoteDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->fetchTickRange([collect](const mbc::h5::Quote* data, hsize_t dataCount)
        {
            for (hsize_t i = 0; i < dataCount; ++i, ++data)
                collect(data->ticks_, data->askPrice_, data->bidPrice_, data->askSize_, data->askSize_);
        }, from, to);
    }

    DLL_PUBLIC bool DLL_CC h5QuoteDataFetchIndexRange(H5QuoteDataHandle dataHandle, QuoteDataAppender collect, unsigned long long index, unsigned long long count)
    {
        const auto dp = castFromQuoteDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->fetchIndexRange([collect](const mbc::h5::Quote* data, hsize_t dataCount)
        {
            for (hsize_t i = 0; i < dataCount; ++i, ++data)
                collect(data->ticks_, data->askPrice_, data->bidPrice_, data->askSize_, data->askSize_);
        }, index, count);
    }

    DLL_PUBLIC bool DLL_CC h5QuoteDataAdd(H5QuoteDataHandle dataHandle, const Quote* input, unsigned long long inputCount, DuplicateTimeTicks duplicateTimeTicks, bool verbose)
    {
        const auto dp = castFromQuoteDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->add(reinterpret_cast<const mbc::h5::Quote*>(input), static_cast<hsize_t>(inputCount), static_cast<mbc::h5::DuplicateTimeTicks>(duplicateTimeTicks), verbose);
    }

    DLL_PUBLIC bool DLL_CC h5QuoteDataSpreadDuplicateTimeTicks(Quote* input, size_t inputCount, bool verbose)
    {
        return mbc::h5::QuoteData::spreadDuplicateTimeTicks(reinterpret_cast<mbc::h5::Quote*>(input), inputCount, verbose);
    }

    DLL_PUBLIC bool DLL_CC h5QuoteDataDeleteIndexRange(H5QuoteDataHandle dataHandle, hsize_t indexFrom, hsize_t indexTo)
    {
        const auto dp = castFromQuoteDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteIndexRange(indexFrom, indexTo);
    }

    DLL_PUBLIC bool DLL_CC h5QuoteDataDeleteTickRange(H5QuoteDataHandle dataHandle, long long ticksFrom, long long ticksTo)
    {
        const auto dp = castFromQuoteDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteTickRange(ticksFrom, ticksTo);
    }

    DLL_PUBLIC bool DLL_CC h5QuoteDataDeleteAllBeforeTick(H5QuoteDataHandle dataHandle, long long ticks)
    {
        const auto dp = castFromQuoteDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllBeforeTick(ticks);
    }

    DLL_PUBLIC bool DLL_CC h5QuoteDataDeleteAllBeforeIndex(H5QuoteDataHandle dataHandle, hsize_t index)
    {
        const auto dp = castFromQuoteDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllBeforeIndex(index);
    }

    DLL_PUBLIC bool DLL_CC h5QuoteDataDeleteAllAfterTick(H5QuoteDataHandle dataHandle, long long ticks)
    {
        const auto dp = castFromQuoteDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllAfterTick(ticks);
    }

    DLL_PUBLIC bool DLL_CC h5QuoteDataDeleteAllAfterIndex(H5QuoteDataHandle dataHandle, hsize_t index)
    {
        const auto dp = castFromQuoteDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllAfterIndex(index);
    }

    // ----------------------------------------------------------------------
    // QuotePriceOnlyData
    // ----------------------------------------------------------------------

    DLL_PUBLIC void DLL_CC h5QuotePriceOnlyDataClose(H5QuotePriceOnlyDataHandle dataHandle)
    {
        if (dataHandle == nullptr)
            return;
        // ReSharper disable once CppCStyleCast
        ((std::shared_ptr<mbc::h5::QuotePriceOnlyData>*)dataHandle)->reset();
    }

    DLL_PUBLIC void DLL_CC h5QuotePriceOnlyDataFlush(H5QuotePriceOnlyDataHandle dataHandle)
    {
        const auto dp = castFromQuotePriceOnlyDataHandle(dataHandle);
        if (dp != nullptr)
            (*dp)->flush();
    }

    DLL_PUBLIC long long DLL_CC h5QuotePriceOnlyDataFirstTicks(H5QuotePriceOnlyDataHandle dataHandle)
    {
        const auto dp = castFromQuotePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? 0L : (*dp)->firstTicks();
    }

    DLL_PUBLIC long long DLL_CC h5QuotePriceOnlyDataLastTicks(H5QuotePriceOnlyDataHandle dataHandle)
    {
        const auto dp = castFromQuotePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? 0L : (*dp)->lastTicks();
    }

    DLL_PUBLIC unsigned long long DLL_CC h5QuotePriceOnlyDataCount(H5QuotePriceOnlyDataHandle dataHandle)
    {
        const auto dp = castFromQuotePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? 0UL : (*dp)->count();
    }

    DLL_PUBLIC unsigned long long DLL_CC h5QuotePriceOnlyDataGetMaximumReadBufferBytes(H5QuotePriceOnlyDataHandle dataHandle)
    {
        const auto dp = castFromQuotePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? 0UL : static_cast<unsigned long long>((*dp)->maximumReadBufferBytes());
    }

    DLL_PUBLIC void DLL_CC h5QuotePriceOnlyDataSetMaximumReadBufferBytes(H5QuotePriceOnlyDataHandle dataHandle, unsigned long long value)
    {
        const auto dp = castFromQuotePriceOnlyDataHandle(dataHandle);
        if (dp != nullptr)
            (*dp)->setMaximumReadBufferBytes(static_cast<const hsize_t>(value));
    }

    DLL_PUBLIC bool DLL_CC h5QuotePriceOnlyDataTicksIndex(H5QuotePriceOnlyDataHandle dataHandle, long long ticks, unsigned long long* index, bool* match)
    {
        bool status{ false };
        const auto dp = castFromQuotePriceOnlyDataHandle(dataHandle);
        if (dp != nullptr)
        {
            hsize_t i;
            bool m;
            status = (*dp)->ticksIndex(ticks, i, m);
            if (status)
            {
                *index = i;
                *match = m;
            }
        }
        return status;
    }

    DLL_PUBLIC bool DLL_CC h5QuotePriceOnlyDataTicksIndexRange(H5QuotePriceOnlyDataHandle dataHandle, long long ticksFrom, long long ticksTo, unsigned long long* indexFrom, unsigned long long* indexTo, bool* matchFrom, bool* matchTo)
    {
        bool status{ false };
        const auto dp = castFromQuotePriceOnlyDataHandle(dataHandle);
        if (dp != nullptr)
        {
            hsize_t i1, i2;
            bool m1, m2;
            status = (*dp)->ticksIndexRange(ticksFrom, ticksTo, i1, i2, m1, m2);
            if (status)
            {
                *indexFrom = i1;
                *indexTo = i2;
                *matchFrom = m1;
                *matchTo = m2;
            }
        }
        return status;
    }

    DLL_PUBLIC bool DLL_CC h5QuotePriceOnlyDataFetchAll(H5QuotePriceOnlyDataHandle dataHandle, QuotePriceOnlyDataAppender collect)
    {
        const auto dp = castFromQuotePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->fetchAll([collect](const mbc::h5::QuotePriceOnly* data, hsize_t dataCount)
        {
            for (hsize_t i = 0; i < dataCount; ++i, ++data)
                collect(data->ticks_, data->askPrice_, data->bidPrice_);
        });
    }

    DLL_PUBLIC bool DLL_CC h5QuotePriceOnlyDataFetchTickRange(H5QuotePriceOnlyDataHandle dataHandle, QuotePriceOnlyDataAppender collect, long long from, long long to)
    {
        const auto dp = castFromQuotePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->fetchTickRange([collect](const mbc::h5::QuotePriceOnly* data, hsize_t dataCount)
        {
            for (hsize_t i = 0; i < dataCount; ++i, ++data)
                collect(data->ticks_, data->askPrice_, data->bidPrice_);
        }, from, to);
    }

    DLL_PUBLIC bool DLL_CC h5QuotePriceOnlyDataFetchIndexRange(H5QuotePriceOnlyDataHandle dataHandle, QuotePriceOnlyDataAppender collect, unsigned long long index, unsigned long long count)
    {
        const auto dp = castFromQuotePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->fetchIndexRange([collect](const mbc::h5::QuotePriceOnly* data, hsize_t dataCount)
        {
            for (hsize_t i = 0; i < dataCount; ++i, ++data)
                collect(data->ticks_, data->askPrice_, data->bidPrice_);
        }, index, count);
    }

    DLL_PUBLIC bool DLL_CC h5QuotePriceOnlyDataAdd(H5QuotePriceOnlyDataHandle dataHandle, const QuotePriceOnly* input, unsigned long long inputCount, DuplicateTimeTicks duplicateTimeTicks, bool verbose)
    {
        const auto dp = castFromQuotePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->add(reinterpret_cast<const mbc::h5::QuotePriceOnly*>(input), static_cast<hsize_t>(inputCount), static_cast<mbc::h5::DuplicateTimeTicks>(duplicateTimeTicks), verbose);
    }

    DLL_PUBLIC bool DLL_CC h5QuotePriceOnlyDataSpreadDuplicateTimeTicks(QuotePriceOnly* input, size_t inputCount, bool verbose)
    {
        return mbc::h5::QuotePriceOnlyData::spreadDuplicateTimeTicks(reinterpret_cast<mbc::h5::QuotePriceOnly*>(input), inputCount, verbose);
    }

    DLL_PUBLIC bool DLL_CC h5QuotePriceOnlyDataDeleteIndexRange(H5QuotePriceOnlyDataHandle dataHandle, hsize_t indexFrom, hsize_t indexTo)
    {
        const auto dp = castFromQuotePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteIndexRange(indexFrom, indexTo);
    }

    DLL_PUBLIC bool DLL_CC h5QuotePriceOnlyDataDeleteTickRange(H5QuotePriceOnlyDataHandle dataHandle, long long ticksFrom, long long ticksTo)
    {
        const auto dp = castFromQuotePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteTickRange(ticksFrom, ticksTo);
    }

    DLL_PUBLIC bool DLL_CC h5QuotePriceOnlyDataDeleteAllBeforeTick(H5QuotePriceOnlyDataHandle dataHandle, long long ticks)
    {
        const auto dp = castFromQuotePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllBeforeTick(ticks);
    }

    DLL_PUBLIC bool DLL_CC h5QuotePriceOnlyDataDeleteAllBeforeIndex(H5QuotePriceOnlyDataHandle dataHandle, hsize_t index)
    {
        const auto dp = castFromQuotePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllBeforeIndex(index);
    }

    DLL_PUBLIC bool DLL_CC h5QuotePriceOnlyDataDeleteAllAfterTick(H5QuotePriceOnlyDataHandle dataHandle, long long ticks)
    {
        const auto dp = castFromQuotePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllAfterTick(ticks);
    }

    DLL_PUBLIC bool DLL_CC h5QuotePriceOnlyDataDeleteAllAfterIndex(H5QuotePriceOnlyDataHandle dataHandle, hsize_t index)
    {
        const auto dp = castFromQuotePriceOnlyDataHandle(dataHandle);
        return dp == nullptr ? false : (*dp)->deleteAllAfterIndex(index);
    }

#ifdef __cplusplus
}
#endif
