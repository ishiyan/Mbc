#include <atomic>
#include <mutex>
#include "hdf5.h"
#include "h5.h"
#include <algorithm>

namespace mbc::h5
{
    // ----------------------------------------------------------------------
    // Constants
    // ----------------------------------------------------------------------

    static const char* timeTicksTypeName = "Time ticks (100 ns) since 0001/01/01 12:00 AM";
    static const char* askPriceTypeName = "Ask price";
    static const char* bidPriceTypeName = "Bid price";
    static const char* askSizeTypeName = "Ask size";
    static const char* bidSizeTypeName = "Bid size";
    static const char* openingPriceTypeName = "Opening price";
    static const char* highestPriceTypeName = "Highest price";
    static const char* lowestPriceTypeName = "Lowest price";
    static const char* closingPriceTypeName = "Closing price";
    static const char* volumeTypeName = "Volume";
    static const char* priceTypeName = "Price";
    static const char* valueTypeName = "Value";

    // ----------------------------------------------------------------------
    // Tracers
    // ----------------------------------------------------------------------

    static std::atomic<ErrorLogger> errorLoggerFunc{ nullptr };

    template<typename T> static const char* stringFromNumber(const T& value, char* buffer, const size_t bufferLength)
    {
        T oldValue{ value };
        char* p{ buffer + bufferLength - 1 };
        *p = 0;
        do
        {
            T newValue = oldValue / 10;
            *--p = static_cast<char>(oldValue - newValue * 10 + '0');
            oldValue = newValue;
        } while (oldValue && p >= buffer);
        return p;
    }

    template<typename T> static const char* toString(const T&, char*, int)
    {
        return "";
    }

    // ----------------------------------------------------------------------
    // Tracers: duplicate ticks
    // ----------------------------------------------------------------------

    static bool traceFailedToDeleteObjectError(const char* what, const char* objectName)
    {
        const ErrorLogger log = errorLoggerFunc.load();
        if (nullptr != log)
        {
            // "h5: Error: Failed to delete {what} [{objectName}]"
            std::string text{ "h5: Error: Failed to delete " };
            text.append(what);
            text.append(" [");
            text.append(objectName);
            text.append("]");
            log(text.c_str(), true);
        }
        return false;
    }

    static bool traceFailedToMoveInstrumentError(const char* sourceInstrumentPath, const char* targetInstrumentPath)
    {
        const ErrorLogger log = errorLoggerFunc.load();
        if (nullptr != log)
        {
            std::string text{ "h5: Error: Failed to move the instrument [" };
            text.append(sourceInstrumentPath);
            text.append("] to [");
            text.append(targetInstrumentPath);
            text.append("]");
            log(text.c_str(), true);
        }
        return false;
    }

    static bool traceFailedToExtractGroupNameError(const char* instrumentPath)
    {
        const ErrorLogger log = errorLoggerFunc.load();
        if (nullptr != log)
        {
            std::string text{ "h5: Error: Failed to extract a group name from [" };
            text.append(instrumentPath);
            text.append("]");
            log(text.c_str(), true);
        }
        return false;
    }

    static void traceGroupError(const char* action, const char* groupName)
    {
        const ErrorLogger log = errorLoggerFunc.load();
        if (nullptr != log)
        {
            std::string text{ "h5: Error: Failed to " };
            text.append(action);
            text.append(" the group [");
            text.append(groupName);
            text.append("]");
            log(text.c_str(), true);
        }
    }

    static bool traceGroupOpenError(const char* groupName)
    {
        // "h5: Error: Failed to open the group [{0}]", groupName
        traceGroupError("open", groupName);
        return false;
    }

    static void traceGroupInfoError(const char* groupName)
    {
        // "h5: Error: Failed to get an info for the group [{0}]", groupName
        traceGroupError("get an info for", groupName);
    }

    static bool traceGroupCloseError(const char* groupName)
    {
        // "h5: Error: Failed to close the group [{0}]", groupName
        traceGroupError("close", groupName);
        return false;
    }

    static bool traceFailedToMoveInstrumentGroupError(const char* sourceInstrumentPath, const char* sourceName, const char* targetName)
    {
        const ErrorLogger log = errorLoggerFunc.load();
        if (nullptr != log)
        {
            std::string text{ "h5: Error: Failed to move the instrument group [" };
            text.append(sourceInstrumentPath);
            text.append("] name [");
            text.append(sourceName);
            text.append("] to name [");
            text.append(targetName);
            text.append("]");
            log(text.c_str(), true);
        }
        return false;
    }

    static void traceGroupByIndexError(const char* action, const hsize_t index, const char* groupName)
    {
        const ErrorLogger log = errorLoggerFunc.load();
        if (nullptr != log)
        {
            const int bufferSize{ 128 };
            char buffer[bufferSize];
            std::string text{ "h5: Error: Failed to get " };
            text.append(action);
            text.append(" by index=");
            text.append(stringFromNumber<hsize_t>(index, buffer, bufferSize));
            text.append(" for the group [");
            text.append(groupName);
            text.append("]");
            log(text.c_str(), true);
        }
    }

    static void traceGroupByIndexNameError(const hsize_t index, const char* groupName)
    {
        // "h5: Error: Failed to get a name by index={0} for the group [{0}]", index, groupName
        traceGroupByIndexError("a name", index, groupName);
    }

    static void traceGroupByIndexObjectInfoError(const hsize_t index, const char* groupName)
    {
        // "h5: Error: Failed to get an object info by index={0} for the group [{0}]", index, groupName
        traceGroupByIndexError("an object info", index, groupName);
    }

    // ----------------------------------------------------------------------
    // Tracers: File
    // ----------------------------------------------------------------------

    //! Error: Failed to iterate the file "{filePath}" starting from the "{groupPath}"
    static void traceFileIterateError(const char* filePath, const char* groupPath = ".")
    {
        const ErrorLogger log = errorLoggerFunc.load();
        if (nullptr != log)
        {
            std::string text{ "h5: Error: Failed to iterate the file \"" };
            text.append(filePath);
            text.append("\" starting from the \"");
            text.append(groupPath);
            text.append("\"");
            log(text.c_str(), true);
        }
    }

    //! Error: Failed to iterate the file "{filePath}" starting from the "{groupPath}"
    static void traceFileIterateError(const std::string& filePath, const char* groupPath = ".")
    {
        traceFileIterateError(filePath.c_str(), groupPath);
    }

    //! Error: Failed to {operation} the file "{filePath}" {as}
    static void traceFileError(const char* operation, const char* filePath, const char* as = nullptr)
    {
        const ErrorLogger log = errorLoggerFunc.load();
        if (nullptr != log)
        {
            std::string text{ "h5: Error: Failed to " };
            text.append(operation);
            text.append(" the file \"");
            text.append(filePath);
            text.append("\"");
            if (nullptr != as)
            {
                text.append(" ");
                text.append(as);
            }
            log(text.c_str(), true);
        }
    }

    //! Error: Failed to open the file "{filePath}" for the read-only access
    static void traceFileOpenReadOnlyError(const char* filePath)
    {
        traceFileError("open", filePath, "for the read-only access");
    }

    //! Error: Failed to {open|open or create} the file "{filePath}" for the read-write access
    static void traceFileOpenReadWriteError(const char* filePath, bool createNonExistent)
    {
        traceFileError(createNonExistent ? "open or create" : "open", filePath, "for read-write access");
    }

    //! Error: Failed to close the file "{filePath}"
    static void traceFileCloseError(const char* filePath)
    {
        traceFileError("close", filePath);
    }

    //! Error: Failed to close the file "{filePath}"
    static void traceFileCloseError(const std::string& filePath)
    {
        traceFileError("close", filePath.c_str());
    }

    //! Error: Failed to flush the file "{filePath}"
    static void traceFileFlushError(const std::string& filePath)
    {
        traceFileError("flush", filePath.c_str());
    }

    //! Error: Failed to open an instrument "{instrumentPath}" from the file "{filePath}"
    static void traceFileOpenInstrumentError(const char* instrumentPath, const std::string& filePath)
    {
        std::string text{ "open an instrument \"" };
        text.append(instrumentPath);
        text.append("\" from");
        traceFileError(text.c_str(), filePath.c_str());
    }

    // ----------------------------------------------------------------------
    // Tracers: Instrument
    // ----------------------------------------------------------------------

    //! Error: Failed to {operation} the instrument "{instrumentPath}" from the file "{filePath}"
    static void traceInstrumentError(const char* operation, const std::string& instrumentPath, const std::string& filePath)
    {
        const ErrorLogger log = errorLoggerFunc.load();
        if (nullptr != log)
        {
            std::string text{ "h5: Error: Failed to " };
            text.append(operation);
            text.append(" the instrument \"");
            text.append(instrumentPath);
            text.append("\" from the file \"");
            text.append(filePath);
            text.append("\"");
            log(text.c_str(), true);
        }
    }
    //! Error: Failed to close the instrument "{instrumentPath}" from the file "{filePath}"
    static void traceInstrumentCloseError(const std::string& instrumentPath, const std::string& filePath)
    {
        traceInstrumentError("close", instrumentPath, filePath);
    }

    //! Error: Failed to flush the instrument "{instrumentPath}" from the file "{filePath}"
    static void traceInstrumentFlushError(const std::string& instrumentPath, const std::string& filePath)
    {
        traceInstrumentError("flush", instrumentPath, filePath);
    }

    //! Error: Failed to enumerate the datasets in the instrument "{instrumentPath}" from the file "{filePath}"
    static void traceInstrumentDatasetEnumerationError(const std::string& instrumentPath, const std::string& filePath)
    {
        traceInstrumentError("enumerate the datasets in", instrumentPath, filePath);
    }

    // ----------------------------------------------------------------------
    // Tracers: Data
    // ----------------------------------------------------------------------

    //! Error: Failed to {operation} the dataset "{datasetPath}" from the file "{filePath}"
    static void traceDataError(const char* operation, const std::string& datasetPath, const std::string& filePath)
    {
        const ErrorLogger log = errorLoggerFunc.load();
        if (nullptr != log)
        {
            std::string text{ "h5: Error: Failed to " };
            text.append(operation);
            text.append(" the dataset \"");
            text.append(datasetPath);
            text.append("\" from the file \"");
            text.append(filePath);
            text.append("\"");
            log(text.c_str(), true);
        }
    }

    //! Error: Failed to close the dataset "{datasetPath}" from the file "{filePath}"
    static void traceDataCloseError(const std::string& datasetPath, const std::string& filePath)
    {
        traceDataError("close", datasetPath, filePath);
    }

    //! Error: Failed to flush the dataset "{datasetPath}" from the file "{filePath}"
    static void traceDataFlushError(const std::string& datasetPath, const std::string& filePath)
    {
        traceDataError("flush", datasetPath, filePath);
    }

    //! Error: Failed to close the native type of the dataset "{datasetPath}" from the file "{filePath}"
    static void traceDataCloseNativeTypeError(const std::string& datasetPath, const std::string& filePath)
    {
        traceDataError("close the native type of", datasetPath, filePath);
    }

    //! Error: Failed to close the standard type of the dataset "{datasetPath}" from the file "{filePath}"
    static void traceDataCloseStandardTypeError(const std::string& datasetPath, const std::string& filePath)
    {
        traceDataError("close the standard type of", datasetPath, filePath);
    }

    //! Error: Failed to get stored item count of the dataset "{datasetPath}" from the file "{filePath}"
    static void traceDataGetItemCountError(const std::string& datasetPath, const std::string& filePath)
    {
        traceDataError("get stored item count of", datasetPath, filePath);
    }

    //! Error: Failed to read the first stored item of the dataset "{datasetPath}" from the file "{filePath}"
    static void traceDataReadFirstItemError(const std::string& datasetPath, const std::string& filePath)
    {
        traceDataError("read the first stored item of", datasetPath, filePath);
    }

    //! Error: Failed to read the last stored item of the dataset "{datasetPath}" from the file "{filePath}"
    static void traceDataReadLastItemError(const std::string& datasetPath, const std::string& filePath)
    {
        traceDataError("read the last stored item of", datasetPath, filePath);
    }

    //! Error: Failed to fetch the time tick range: [{ticksFrom}, {ticksTo}] of the dataset "{datasetPath}" from the file "{filePath}"
    static bool traceDataFetchTimeTickRangeError(const long long ticksFrom, const long long ticksTo, const std::string& datasetPath, const std::string& filePath)
    {
        const size_t bufferSize{ 128 };
        char buffer[bufferSize];
        std::string text{ "fetch the time tick range: [" };
        text.append(stringFromNumber<long long>(ticksFrom, buffer, bufferSize));
        text.append(", ");
        text.append(stringFromNumber<long long>(ticksTo, buffer, bufferSize));
        text.append("] of");
        traceDataError(text.c_str(), datasetPath, filePath);
        return false;
    }

    //! Error: Failed to collect items (exception: [{e}]) of the dataset "{datasetPath}" from the file "{filePath}"
    static void traceDataExceptionWhileCollectingItemsError(const std::exception& e, const std::string& datasetPath, const std::string& filePath)
    {
        std::string text{ "collect items: (exception: [" };
        text.append(e.what());
        text.append("]) of");
        traceDataError(text.c_str(), datasetPath, filePath);
    }

    //! Error: Failed to get the time tick [{ticks}] index in the dataset "{datasetPath}" from the file "{filePath}"
    static bool traceDataGetTimeTickIndexError(const long long ticks, const std::string& datasetPath, const std::string& filePath)
    {
        const size_t bufferSize{ 128 };
        char buffer[bufferSize];
        std::string text{ "get the time tick [" };
        text.append(stringFromNumber<long long>(ticks, buffer, bufferSize));
        text.append("] index in");
        traceDataError(text.c_str(), datasetPath, filePath);
        return false;
    }

    //! Error: Failed to fetch the index range [offset {index}, count {count}] in the dataset "{datasetPath}" from the file "{filePath}"
    static bool traceDataFetchIndexRangeError(const hsize_t index, const hsize_t count, const std::string& datasetPath, const std::string& filePath)
    {
        const size_t bufferSize{ 128 };
        char buffer[bufferSize];
        std::string text{ "fetch the index range [offset " };
        text.append(stringFromNumber<hsize_t>(index, buffer, bufferSize));
        text.append(", count ");
        text.append(stringFromNumber<hsize_t>(count, buffer, bufferSize));
        text.append("] in");
        traceDataError(text.c_str(), datasetPath, filePath);
        return false;
    }

    //! Error: Failed to {append|prepend} {count} input items to the dataset "{datasetPath}" from the file "{filePath}"
    static bool traceDataAppendPrependFailError(const char* action, const hsize_t count, const std::string& datasetPath, const std::string& filePath)
    {
        const size_t bufferSize{ 128 };
        char buffer[bufferSize];
        std::string text{ action };
        text.append(stringFromNumber<hsize_t>(count, buffer, bufferSize));
        text.append(" input items to");
        traceDataError(text.c_str(), datasetPath, filePath);
        return false;
    }

    //! Error: Failed to prepend {count} input items to the dataset "{datasetPath}" from the file "{filePath}"
    static bool traceDataPrependFailError(const hsize_t count, const std::string& datasetPath, const std::string& filePath)
    {
        return traceDataAppendPrependFailError("prepend ", count, datasetPath, filePath);
    }

    //! Error: Failed to append {count} input items to the dataset "{datasetPath}" from the file "{filePath}"
    static bool traceDataAppendFailError(const hsize_t count, const std::string& datasetPath, const std::string& filePath)
    {
        return traceDataAppendPrependFailError("append ", count, datasetPath, filePath);
    }

    //! Error: Failed to append {count} input items starting from offset {offset} to the dataset "{datasetPath}" from the file "{filePath}"
    static bool traceDataAppendOffsetFailError(const hsize_t count, const hsize_t offset, const std::string& datasetPath, const std::string& filePath)
    {
        const size_t bufferSize{ 128 };
        char buffer[bufferSize];
        std::string text{ "append " };
        text.append(stringFromNumber<hsize_t>(count, buffer, bufferSize));
        text.append(" input items starting from offset ");
        text.append(stringFromNumber<hsize_t>(offset, buffer, bufferSize));
        text.append(" to");
        traceDataError(text.c_str(), datasetPath, filePath);
        return false;
    }

    //! Error: Failed to merge {count} input items to the dataset "{datasetPath}" from the file "{filePath}"
    static bool traceDataMergeFailError(const hsize_t count, const std::string& datasetPath, const std::string& filePath)
    {
        const size_t bufferSize{ 128 };
        char buffer[bufferSize];
        std::string text{ "merge " };
        text.append(stringFromNumber<hsize_t>(count, buffer, bufferSize));
        text.append(" input items to");
        traceDataError(text.c_str(), datasetPath, filePath);
        return false;
    }

    //! Error: Failed to merge {count} input items starting from offset {offset} to the dataset "{datasetPath}" from the file "{filePath}"
    static bool traceDataMergeOffsetFailError(const hsize_t count, const hsize_t offset, const std::string& datasetPath, const std::string& filePath)
    {
        const size_t bufferSize{ 128 };
        char buffer[bufferSize];
        std::string text{ "merge " };
        text.append(stringFromNumber<hsize_t>(count, buffer, bufferSize));
        text.append(" input items starting from offset ");
        text.append(stringFromNumber<hsize_t>(offset, buffer, bufferSize));
        text.append(" to");
        traceDataError(text.c_str(), datasetPath, filePath);
        return false;
    }

    //TODO:zzzzzzzzzzzzzzzzzzzzzzzzzzzzzz

    //! Info: Spreading duplicate input time tick: start input[{dataOffset}] = ({dataValue})
    template <typename T> static void traceSpreadingDuplicateInputTimeTick(const size_t dataOffset, const T& dataValue)
    {
        const ErrorLogger log = errorLoggerFunc.load();
        if (nullptr != log)
        {
            const size_t bufferSize{ 128 };
            char buffer[bufferSize];
            std::string text{ "h5: Info: Spreading duplicate input time tick: start input[" };
            text.append(stringFromNumber<size_t>(dataOffset, buffer, bufferSize));
            text.append("] = (");
            text.append(toString<T>(dataValue, buffer, bufferSize));
            text.append(")");
            log(text.c_str(), false);
        }
    }

    //! Info: Spreading duplicate input time tick: start input[{dataOffset}] = ({dataValue}) -> {newTicks}
    template <typename T> static void traceSpreadingDuplicateInputTimeTick(const size_t dataOffset, const T& dataValue, const long long newTicks)
    {
        const ErrorLogger log = errorLoggerFunc.load();
        if (nullptr != log)
        {
            const size_t bufferSize{ 128 };
            char buffer[bufferSize];
            std::string text{ "h5: Info: Spreading duplicate input time tick: start input[" };
            text.append(stringFromNumber<size_t>(dataOffset, buffer, bufferSize));
            text.append("] = (");
            text.append(toString<T>(dataValue, buffer, bufferSize));
            text.append(") -> ");
            text.append(stringFromNumber<long long>(newTicks, buffer, bufferSize));
            log(text.c_str(), false);
        }
    }

    //! Error: Non-increasing input ticks: [{dataValue1}], [{dataValue2}]
    template <typename T> static void traceNonIncreasingInputTicksError(const T& dataValue1, const T& dataValue2)
    {
        const ErrorLogger log = errorLoggerFunc.load();
        if (nullptr != log)
        {
            const size_t bufferSize{ 128 };
            char buffer[bufferSize];
            std::string text{ "h5: Error: Non-increasing input ticks: [" };
            text.append(toString<T>(dataValue1, buffer, bufferSize));
            text.append("], [");
            text.append(toString<T>(dataValue2, buffer, bufferSize));
            text.append("]");
            log(text.c_str(), true);
        }
    }

    //! {Error|Info}: Duplicate ticks: data[{dataOffset}]=({dataValue}), input[{inputOffset}]=({inputValue}), {action}
    template <typename T> static bool traceDataDuplicateTicks(const char* kind, const char* action, const hsize_t dataOffset, const T& dataValue, const hsize_t inputOffset, const T& inputValue, const bool isError = true)
    {
        const ErrorLogger log = errorLoggerFunc.load();
        if (nullptr != log)
        {
            const int bufferSize{ 128 };
            char buffer[bufferSize];
            std::string text{ "h5: " };
            text.append(kind);
            text.append(": Duplicate ticks: data[");
            text.append(stringFromNumber<hsize_t>(dataOffset, buffer, bufferSize));
            text.append("]=(");
            text.append(toString<T>(dataValue, buffer, bufferSize));
            text.append("), input[");
            text.append(stringFromNumber<hsize_t>(inputOffset, buffer, bufferSize));
            text.append("]=(");
            text.append(toString<T>(inputValue, buffer, bufferSize));
            text.append("), ");
            text.append(action);
            log(text.c_str(), isError);
        }
        return false;
    }

    //! Info: Duplicate ticks: data[{dataOffset}]=({dataValue}), input[{inputOffset}]=({inputValue}), updating
    template <typename T> static void traceDataDuplicateTicksUpdateInfo(hsize_t dataOffset, const T& dataValue, hsize_t inputOffset, const T& inputValue)
    {
        (void)traceDataDuplicateTicks("Info", "updating", dataOffset, dataValue, inputOffset, inputValue, false);
    }

    //! Error: Duplicate ticks: data[{dataOffset}]=({dataValue}), input[{inputOffset}]=({inputValue}), updating
    template <typename T> static bool traceDataDuplicateTicksUpdateError(hsize_t dataOffset, const T& dataValue, hsize_t inputOffset, const T& inputValue)
    {
        return traceDataDuplicateTicks("Error", "updating", dataOffset, dataValue, inputOffset, inputValue);
    }

    //! Error: Duplicate ticks: data[{dataOffset}]=({dataValue}), input[{inputOffset}]=({inputValue}), skipping
    template <typename T> static bool traceDataDuplicateTicksSkipError(hsize_t dataOffset, const T& dataValue, hsize_t inputOffset, const T& inputValue)
    {
        return traceDataDuplicateTicks("Error", "skipping", dataOffset, dataValue, inputOffset, inputValue);
    }

    //! Error: Duplicate ticks: data[{dataOffset}]=({dataValue}), input[{inputOffset}]=({inputValue}), aborting
    template <typename T> static bool traceDataDuplicateTicksFailError(hsize_t dataOffset, const T& dataValue, hsize_t inputOffset, const T& inputValue)
    {
        return traceDataDuplicateTicks("Error", "aborting", dataOffset, dataValue, inputOffset, inputValue);
    }

    // ----------------------------------------------------------------------
    // Types
    // ----------------------------------------------------------------------

    static hid_t makeTypeFailed(hid_t type, herr_t status)
    {
        H5Tclose(type);
        return hid_t(status);
    }

    static hid_t makeNativeType_Quote()
    {
        const hid_t type = H5Tcreate(H5T_COMPOUND, sizeof(Quote));
        if (0 <= type)
        {
            herr_t status = H5Tinsert(type, timeTicksTypeName, HOFFSET(Quote, ticks_), H5T_NATIVE_LLONG);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, askPriceTypeName, HOFFSET(Quote, askPrice_), H5T_NATIVE_DOUBLE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, bidPriceTypeName, HOFFSET(Quote, bidPrice_), H5T_NATIVE_DOUBLE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, askSizeTypeName, HOFFSET(Quote, askSize_), H5T_NATIVE_DOUBLE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, bidSizeTypeName, HOFFSET(Quote, bidSize_), H5T_NATIVE_DOUBLE);
            if (0 > status)
                return makeTypeFailed(type, status);
        }
        return type;
    }

    static hid_t makeStandardType_Quote()
    {
        const hid_t type = H5Tcreate(H5T_COMPOUND, 40);
        if (0 <= type)
        {
            herr_t status = H5Tinsert(type, timeTicksTypeName, 0, H5T_STD_I64LE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, askPriceTypeName, 8, H5T_IEEE_F64LE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, bidPriceTypeName, 16, H5T_IEEE_F64LE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, askSizeTypeName, 24, H5T_IEEE_F64LE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, bidSizeTypeName, 32, H5T_IEEE_F64LE);
            if (0 > status)
                return makeTypeFailed(type, status);
        }
        return type;
    }

    static hid_t makeNativeType_QuotePriceOnly()
    {
        const hid_t type = H5Tcreate(H5T_COMPOUND, sizeof(QuotePriceOnly));
        if (0 <= type)
        {
            herr_t status = H5Tinsert(type, timeTicksTypeName, HOFFSET(QuotePriceOnly, ticks_), H5T_NATIVE_LLONG);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, askPriceTypeName, HOFFSET(QuotePriceOnly, askPrice_), H5T_NATIVE_DOUBLE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, bidPriceTypeName, HOFFSET(QuotePriceOnly, bidPrice_), H5T_NATIVE_DOUBLE);
            if (0 > status)
                return makeTypeFailed(type, status);
        }
        return type;
    }

    static hid_t makeStandardType_QuotePriceOnly()
    {
        const hid_t type = H5Tcreate(H5T_COMPOUND, 24);
        if (0 <= type)
        {
            herr_t status = H5Tinsert(type, timeTicksTypeName, 0, H5T_STD_I64LE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, askPriceTypeName, 8, H5T_IEEE_F64LE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, bidPriceTypeName, 16, H5T_IEEE_F64LE);
            if (0 > status)
                return makeTypeFailed(type, status);
        }
        return type;
    }

    static hid_t makeNativeType_Ohlcv()
    {
        const hid_t type = H5Tcreate(H5T_COMPOUND, sizeof(Ohlcv));
        if (0 <= type)
        {
            herr_t status = H5Tinsert(type, timeTicksTypeName, HOFFSET(Ohlcv, ticks_), H5T_NATIVE_LLONG);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, openingPriceTypeName, HOFFSET(Ohlcv, open_), H5T_NATIVE_DOUBLE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, highestPriceTypeName, HOFFSET(Ohlcv, high_), H5T_NATIVE_DOUBLE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, lowestPriceTypeName, HOFFSET(Ohlcv, low_), H5T_NATIVE_DOUBLE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, closingPriceTypeName, HOFFSET(Ohlcv, close_), H5T_NATIVE_DOUBLE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, volumeTypeName, HOFFSET(Ohlcv, volume_), H5T_NATIVE_DOUBLE);
            if (0 > status)
                return makeTypeFailed(type, status);
        }
        return type;
    }

    static hid_t makeStandardType_Ohlcv()
    {
        const hid_t type = H5Tcreate(H5T_COMPOUND, 48);
        if (0 <= type)
        {
            herr_t status = H5Tinsert(type, timeTicksTypeName, 0, H5T_STD_I64LE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, openingPriceTypeName, 8, H5T_IEEE_F64LE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, highestPriceTypeName, 16, H5T_IEEE_F64LE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, lowestPriceTypeName, 24, H5T_IEEE_F64LE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, closingPriceTypeName, 32, H5T_IEEE_F64LE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, volumeTypeName, 40, H5T_IEEE_F64LE);
            if (0 > status)
                return makeTypeFailed(type, status);
        }
        return type;
    }

    static hid_t makeNativeType_OhlcvPriceOnly()
    {
        const hid_t type = H5Tcreate(H5T_COMPOUND, sizeof(OhlcvPriceOnly));
        if (0 <= type)
        {
            herr_t status = H5Tinsert(type, timeTicksTypeName, HOFFSET(OhlcvPriceOnly, ticks_), H5T_NATIVE_LLONG);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, openingPriceTypeName, HOFFSET(OhlcvPriceOnly, open_), H5T_NATIVE_DOUBLE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, highestPriceTypeName, HOFFSET(OhlcvPriceOnly, high_), H5T_NATIVE_DOUBLE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, lowestPriceTypeName, HOFFSET(OhlcvPriceOnly, low_), H5T_NATIVE_DOUBLE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, closingPriceTypeName, HOFFSET(OhlcvPriceOnly, close_), H5T_NATIVE_DOUBLE);
            if (0 > status)
                return makeTypeFailed(type, status);
        }
        return type;
    }

    static hid_t makeStandardType_OhlcvPriceOnly()
    {
        const hid_t type = H5Tcreate(H5T_COMPOUND, 40);
        if (0 <= type)
        {
            herr_t status = H5Tinsert(type, timeTicksTypeName, 0, H5T_STD_I64LE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, openingPriceTypeName, 8, H5T_IEEE_F64LE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, highestPriceTypeName, 16, H5T_IEEE_F64LE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, lowestPriceTypeName, 24, H5T_IEEE_F64LE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, closingPriceTypeName, 32, H5T_IEEE_F64LE);
            if (0 > status)
                return makeTypeFailed(type, status);
        }
        return type;
    }

    static hid_t makeNativeType_Trade()
    {
        const hid_t type = H5Tcreate(H5T_COMPOUND, sizeof(Trade));
        if (0 <= type)
        {
            herr_t status = H5Tinsert(type, timeTicksTypeName, HOFFSET(Trade, ticks_), H5T_NATIVE_LLONG);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, priceTypeName, HOFFSET(Trade, price_), H5T_NATIVE_DOUBLE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, volumeTypeName, HOFFSET(Trade, volume_), H5T_NATIVE_DOUBLE);
            if (0 > status)
                return makeTypeFailed(type, status);
        }
        return type;
    }

    static hid_t makeStandardType_Trade()
    {
        const hid_t type = H5Tcreate(H5T_COMPOUND, 24);
        if (0 <= type)
        {
            herr_t status = H5Tinsert(type, timeTicksTypeName, 0, H5T_STD_I64LE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, priceTypeName, 8, H5T_IEEE_F64LE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, volumeTypeName, 16, H5T_IEEE_F64LE);
            if (0 > status)
                return makeTypeFailed(type, status);
        }
        return type;
    }

    static hid_t makeNativeType_TradePriceOnly()
    {
        const hid_t type = H5Tcreate(H5T_COMPOUND, sizeof(TradePriceOnly));
        if (0 <= type)
        {
            herr_t status = H5Tinsert(type, timeTicksTypeName, HOFFSET(TradePriceOnly, ticks_), H5T_NATIVE_LLONG);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, priceTypeName, HOFFSET(TradePriceOnly, price_), H5T_NATIVE_DOUBLE);
            if (0 > status)
                return makeTypeFailed(type, status);
        }
        return type;
    }

    static hid_t makeStandardType_TradePriceOnly()
    {
        const hid_t type = H5Tcreate(H5T_COMPOUND, 16);
        if (0 <= type)
        {
            herr_t status = H5Tinsert(type, timeTicksTypeName, 0, H5T_STD_I64LE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, priceTypeName, 8, H5T_IEEE_F64LE);
            if (0 > status)
                return makeTypeFailed(type, status);
        }
        return type;
    }

    static hid_t makeNativeType_Scalar()
    {
        const hid_t type = H5Tcreate(H5T_COMPOUND, sizeof(Scalar));
        if (0 <= type)
        {
            herr_t status = H5Tinsert(type, timeTicksTypeName, HOFFSET(Scalar, ticks_), H5T_NATIVE_LLONG);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, valueTypeName, HOFFSET(Scalar, value_), H5T_NATIVE_DOUBLE);
            if (0 > status)
                return makeTypeFailed(type, status);
        }
        return type;
    }

    static hid_t makeStandardType_Scalar()
    {
        const hid_t type = H5Tcreate(H5T_COMPOUND, 16);
        if (0 <= type)
        {
            herr_t status = H5Tinsert(type, timeTicksTypeName, 0, H5T_STD_I64LE);
            if (0 > status)
                return makeTypeFailed(type, status);
            status = H5Tinsert(type, valueTypeName, 8, H5T_IEEE_F64LE);
            if (0 > status)
                return makeTypeFailed(type, status);
        }
        return type;
    }

    // ----------------------------------------------------------------------
    // Operations
    // ----------------------------------------------------------------------

    static hid_t fileOpen(const char* fileName, const bool readWrite, const bool createNonexistent, const FileDriver fileDriver, const bool corkThCache)
    {
        const hid_t fapl_id = H5Pcreate(H5P_FILE_ACCESS);
        // [Hdf-forum] Preventing file corruption from power loss, Tue Jun 8 23:27:23 EDT 2010.
        // If you have memory to spare, you could "cork the cache" until you reached a suitable point
        // to update the metadata in the file, call H5Fflush(), then continue with your application.
        if (corkThCache)
        {
            H5AC_cache_config_t mdc_config;
            mdc_config.version = H5AC__CURR_CACHE_CONFIG_VERSION;
            H5Pget_mdc_config(fapl_id, &mdc_config);
            mdc_config.evictions_enabled = false;
            mdc_config.incr_mode = H5C_incr__off;
            mdc_config.decr_mode = H5C_decr__off;
            mdc_config.flash_incr_mode = H5C_flash_incr__off;
            H5Pset_mdc_config(fapl_id, &mdc_config);
        }

        // Uncomment for metadata cache logging
        // H5Pset_mdc_log_options(fapl_id, true, "h5.mdc.log", true);

        // Set file access property list to allow the latest file format.
        H5Pset_libver_bounds(fapl_id, H5F_LIBVER_LATEST, H5F_LIBVER_LATEST);

        // fileDriver = FileDriver::Log;
        switch (fileDriver)
        {
            case FileDriver::Log:
                H5Pset_fapl_log(fapl_id, "h5.fapl.log", H5FD_LOG_ALL, 1024 * 1024);
                break;
            case FileDriver::Core:
                H5Pset_fapl_core(fapl_id, 1024 * 1024, true);
                break;
            case FileDriver::StdIo:
                H5Pset_fapl_stdio(fapl_id);
                break;
            case FileDriver::Sec2:
            default:
                H5Pset_fapl_sec2(fapl_id);
        }

        // Determine how aggressively H5F_CLOSE deals with objects within a file that remain open when H5F_CLOSE is called to close that file.
        // H5Fclose behavior with open object(s) in file:
        // H5F_CLOSE_WEAK: access to file identifier is terminated; actual file close is delayed until all objects in file are closed
        // H5F_CLOSE_SEMI: function returns FAILURE
        // H5F_CLOSE_STRONG: all open objects remaining in the file are closed then file is closed
        H5Pset_fclose_degree(fapl_id, H5F_CLOSE_WEAK);

        // The library's metadata cache is fairly conservative about holding on to HDF5 object metadata (object headers, chunk index structures, etc.),
        // which can cause the cache size to grow, resulting in memory pressure on an application or system. When enabled, the "evict on close" property
        // will cause all metadata for an object to be evicted from the cache as long as metadata is not referenced by any other open object.
        // H5Pset_evict_on_close(fapl_id, true);

        H5Pset_meta_block_size(fapl_id, 1024 * 8);
        H5Pset_sieve_buf_size(fapl_id, 1024 * 1024);

        // Save old error handler.
        herr_t(*oldFunc)(hid_t, void*);
        void* oldClientData;
        H5Eget_auto2(H5E_DEFAULT, &oldFunc, &oldClientData);

        // Turn off error handling.
        H5Eset_auto2(H5E_DEFAULT, nullptr, nullptr);

        hid_t file = H5Fopen(fileName, readWrite ? H5F_ACC_RDWR : H5F_ACC_RDONLY, fapl_id); // TODO: H5F_ACC_RDWR | H5F_ACC_SWMR_WRITE : H5F_ACC_RDONLY | H5F_ACC_SWMR_READ

        // Restore previous error handler.
        H5Eset_auto2(H5E_DEFAULT, oldFunc, oldClientData);

        if (0 > file && readWrite && createNonexistent)
        {
            const hid_t fcpl_id = H5Pcreate(H5P_FILE_CREATE);
            H5Pset_sizes(fcpl_id, 8, 8); // TODO: or 16?
            file = H5Fcreate(fileName, H5F_ACC_EXCL, fcpl_id, fapl_id); // TODO: H5F_ACC_EXCL | H5F_ACC_DEBUG in debug mode
            H5Pclose(fcpl_id);
        }
        H5Pclose(fapl_id);
        return file;
    }

    static hid_t groupOpen(const char* instrumentPath, const hid_t fileId, const bool readWrite, const bool createNonexistent)
    {
        // Save old error handler.
        herr_t(*oldFunc)(hid_t, void*);
        void* oldClientData;
        H5Eget_auto2(H5E_DEFAULT, &oldFunc, &oldClientData);

        // Turn off error handling.
        H5Eset_auto2(H5E_DEFAULT, nullptr, nullptr);

        hid_t group_id = H5Gopen2(fileId, instrumentPath, H5P_DEFAULT);

        // Restore previous error handler.
        H5Eset_auto2(H5E_DEFAULT, oldFunc, oldClientData);

        if (0 > group_id && readWrite && createNonexistent)
        {
            // Create group creation property list and set it to allow creation of intermediate groups.
            const hid_t gcpl_id = H5Pcreate(H5P_LINK_CREATE);
            H5Pset_create_intermediate_group(gcpl_id, 1);

            // Create the group.
            group_id = H5Gcreate2(fileId, instrumentPath, gcpl_id, H5P_DEFAULT, H5P_DEFAULT);
            H5Pclose(gcpl_id);
        }
        return group_id;
    }

    typedef struct
    {
        hid_t nativeType_;
        hid_t standardType_;
        const char* name_;
    } DatasetTraits;

    static hid_t datasetOpen(const char* datasetName, const hid_t groupId, const hid_t externalType, const bool readWrite, const bool createNonexistent)
    {
        // Modify the dataset access properties.
        const hid_t dapl_id = H5Pcreate(H5P_DATASET_ACCESS);

        H5Pset_chunk_cache(dapl_id, 12421, 16 * 1024 * 1024, H5D_CHUNK_CACHE_W0_DEFAULT);

        // Save old error handler.
        herr_t(*oldFunc)(hid_t, void*);
        void* oldClientData;
        H5Eget_auto2(H5E_DEFAULT, &oldFunc, &oldClientData);

        // Turn off error handling.
        H5Eset_auto2(H5E_DEFAULT, nullptr, nullptr);

        hid_t dataset_id = H5Dopen2(groupId, datasetName, dapl_id);

        // Restore previous error handler.
        H5Eset_auto2(H5E_DEFAULT, oldFunc, oldClientData);

        if (0 > dataset_id && readWrite && createNonexistent)
        {
            // Create a simple dataspace with unlimited size.
            hsize_t dims[1] = { 0 };
            hsize_t max_dims[1] = { H5S_UNLIMITED };
            const hid_t space_id = H5Screate_simple(1, dims, max_dims);

            // Modify the dataset creation properties to enable chunking and compression.
            const hid_t dcpl_id = H5Pcreate(H5P_DATASET_CREATE);

            // The values of the dims_chunk array define the size of the chunks to store the raw data.
            // The unit of measure for dim values is dataset elements.
            hsize_t dims_chunk[1] = { 1024 * 20 };
            H5Pset_layout(dcpl_id, H5D_CHUNKED);
            H5Pset_chunk(dcpl_id, 1, dims_chunk);
            H5Pset_shuffle(dcpl_id);
            H5Pset_deflate(dcpl_id, 9); // TODO: review, standard is 6

            // Create the dataset.
            dataset_id = H5Dcreate2(groupId, datasetName, externalType, space_id, H5P_DEFAULT, dcpl_id, dapl_id); // H5P_DEFAULT
            H5Sclose(space_id);
            H5Pclose(dcpl_id);
        }
        H5Pclose(dapl_id);
        return dataset_id;
    }

    static bool nativeTypeFailed(DatasetTraits& datasetTraits)
    {
        datasetTraits.standardType_ = -1;
        return false;
    }

    static bool standardTypeFailed(DatasetTraits& datasetTraits)
    {
        H5Tclose(datasetTraits.nativeType_);
        datasetTraits.nativeType_ = -1;
        return false;
    }

    static bool makeOhlcv(DatasetTraits& datasetTraits, const char* name)
    {
        datasetTraits.name_ = name;
        datasetTraits.nativeType_ = makeNativeType_Ohlcv();
        if (0 > datasetTraits.nativeType_)
            return nativeTypeFailed(datasetTraits);
        datasetTraits.standardType_ = makeStandardType_Ohlcv();
        if (0 > datasetTraits.standardType_)
            return standardTypeFailed(datasetTraits);
        return true;
    }

    static bool makeOhlcvPriceOnly(DatasetTraits& datasetTraits, const char* name)
    {
        datasetTraits.name_ = name;
        datasetTraits.nativeType_ = makeNativeType_OhlcvPriceOnly();
        if (0 > datasetTraits.nativeType_)
            return nativeTypeFailed(datasetTraits);
        datasetTraits.standardType_ = makeStandardType_OhlcvPriceOnly();
        if (0 > datasetTraits.standardType_)
            return standardTypeFailed(datasetTraits);
        return true;
    }

    static bool makeScalar(DatasetTraits& datasetTraits, const char* name)
    {
        datasetTraits.name_ = name;
        datasetTraits.nativeType_ = makeNativeType_Scalar();
        if (0 > datasetTraits.nativeType_)
            return nativeTypeFailed(datasetTraits);
        datasetTraits.standardType_ = makeStandardType_Scalar();
        if (0 > datasetTraits.standardType_)
            return standardTypeFailed(datasetTraits);
        return true;
    }

    static bool dataKind2DatasetTraits(const DataKind dataKind, DatasetTraits& datasetTraits)
    {
        switch (dataKind)
        {
            case DataKind::QuoteAperiodic:
                datasetTraits.name_ = "q____";
                datasetTraits.nativeType_ = makeNativeType_Quote();
                if (0 > datasetTraits.nativeType_)
                    return nativeTypeFailed(datasetTraits);
                datasetTraits.standardType_ = makeStandardType_Quote();
                if (0 > datasetTraits.standardType_)
                    return standardTypeFailed(datasetTraits);
                return true;
            case DataKind::QuotePriceOnlyAperiodic:
                datasetTraits.name_ = "Q____";
                datasetTraits.nativeType_ = makeNativeType_QuotePriceOnly();
                if (0 > datasetTraits.nativeType_)
                    return nativeTypeFailed(datasetTraits);
                datasetTraits.standardType_ = makeStandardType_QuotePriceOnly();
                if (0 > datasetTraits.standardType_)
                    return standardTypeFailed(datasetTraits);
                return true;
            case DataKind::TradeAperiodic:
                datasetTraits.name_ = "t____";
                datasetTraits.nativeType_ = makeNativeType_Trade();
                if (0 > datasetTraits.nativeType_)
                    return nativeTypeFailed(datasetTraits);
                datasetTraits.standardType_ = makeStandardType_Trade();
                if (0 > datasetTraits.standardType_)
                    return standardTypeFailed(datasetTraits);
                return true;
            case DataKind::TradePriceOnlyAperiodic:
                datasetTraits.name_ = "T____";
                datasetTraits.nativeType_ = makeNativeType_TradePriceOnly();
                if (0 > datasetTraits.nativeType_)
                    return nativeTypeFailed(datasetTraits);
                datasetTraits.standardType_ = makeStandardType_TradePriceOnly();
                if (0 > datasetTraits.standardType_)
                    return standardTypeFailed(datasetTraits);
                return true;

            case DataKind::OhlcvYear1:     return makeOhlcv(datasetTraits, "c_y01");
            case DataKind::OhlcvMonth6:    return makeOhlcv(datasetTraits, "c_M06");
            case DataKind::OhlcvMonth4:    return makeOhlcv(datasetTraits, "c_M04");
            case DataKind::OhlcvMonth3:    return makeOhlcv(datasetTraits, "c_M03");
            case DataKind::OhlcvMonth2:    return makeOhlcv(datasetTraits, "c_M02");
            case DataKind::OhlcvMonth1:    return makeOhlcv(datasetTraits, "c_M01");
            case DataKind::OhlcvWeek3:     return makeOhlcv(datasetTraits, "c_w03");
            case DataKind::OhlcvWeek2:     return makeOhlcv(datasetTraits, "c_w02");
            case DataKind::OhlcvWeek1:     return makeOhlcv(datasetTraits, "c_w01");
            case DataKind::OhlcvDay1:      return makeOhlcv(datasetTraits, "c_d01");
            case DataKind::OhlcvHour12:    return makeOhlcv(datasetTraits, "c_h12");
            case DataKind::OhlcvHour8:     return makeOhlcv(datasetTraits, "c_h08");
            case DataKind::OhlcvHour6:     return makeOhlcv(datasetTraits, "c_h06");
            case DataKind::OhlcvHour4:     return makeOhlcv(datasetTraits, "c_h04");
            case DataKind::OhlcvHour3:     return makeOhlcv(datasetTraits, "c_h03");
            case DataKind::OhlcvHour2:     return makeOhlcv(datasetTraits, "c_h02");
            case DataKind::OhlcvHour1:     return makeOhlcv(datasetTraits, "c_h01");
            case DataKind::OhlcvMinute30:  return makeOhlcv(datasetTraits, "c_m30");
            case DataKind::OhlcvMinute20:  return makeOhlcv(datasetTraits, "c_m20");
            case DataKind::OhlcvMinute15:  return makeOhlcv(datasetTraits, "c_m15");
            case DataKind::OhlcvMinute12:  return makeOhlcv(datasetTraits, "c_m12");
            case DataKind::OhlcvMinute10:  return makeOhlcv(datasetTraits, "c_m10");
            case DataKind::OhlcvMinute6:   return makeOhlcv(datasetTraits, "c_m06");
            case DataKind::OhlcvMinute5:   return makeOhlcv(datasetTraits, "c_m05");
            case DataKind::OhlcvMinute4:   return makeOhlcv(datasetTraits, "c_m04");
            case DataKind::OhlcvMinute3:   return makeOhlcv(datasetTraits, "c_m03");
            case DataKind::OhlcvMinute2:   return makeOhlcv(datasetTraits, "c_m02");
            case DataKind::OhlcvMinute1:   return makeOhlcv(datasetTraits, "c_m01");
            case DataKind::OhlcvSecond30:  return makeOhlcv(datasetTraits, "c_s30");
            case DataKind::OhlcvSecond20:  return makeOhlcv(datasetTraits, "c_s20");
            case DataKind::OhlcvSecond15:  return makeOhlcv(datasetTraits, "c_s15");
            case DataKind::OhlcvSecond12:  return makeOhlcv(datasetTraits, "c_s12");
            case DataKind::OhlcvSecond10:  return makeOhlcv(datasetTraits, "c_s10");
            case DataKind::OhlcvSecond6:   return makeOhlcv(datasetTraits, "c_s06");
            case DataKind::OhlcvSecond5:   return makeOhlcv(datasetTraits, "c_s05");
            case DataKind::OhlcvSecond4:   return makeOhlcv(datasetTraits, "c_s04");
            case DataKind::OhlcvSecond3:   return makeOhlcv(datasetTraits, "c_s03");
            case DataKind::OhlcvSecond2:   return makeOhlcv(datasetTraits, "c_s02");
            case DataKind::OhlcvSecond1:   return makeOhlcv(datasetTraits, "c_s01");
            case DataKind::OhlcvAperiodic: return makeOhlcv(datasetTraits, "c____");

            case DataKind::OhlcvAskYear1:     return makeOhlcv(datasetTraits, "cay01");
            case DataKind::OhlcvAskMonth6:    return makeOhlcv(datasetTraits, "caM06");
            case DataKind::OhlcvAskMonth4:    return makeOhlcv(datasetTraits, "caM04");
            case DataKind::OhlcvAskMonth3:    return makeOhlcv(datasetTraits, "caM03");
            case DataKind::OhlcvAskMonth2:    return makeOhlcv(datasetTraits, "caM02");
            case DataKind::OhlcvAskMonth1:    return makeOhlcv(datasetTraits, "caM01");
            case DataKind::OhlcvAskWeek3:     return makeOhlcv(datasetTraits, "caw03");
            case DataKind::OhlcvAskWeek2:     return makeOhlcv(datasetTraits, "caw02");
            case DataKind::OhlcvAskWeek1:     return makeOhlcv(datasetTraits, "caw01");
            case DataKind::OhlcvAskDay1:      return makeOhlcv(datasetTraits, "cad01");
            case DataKind::OhlcvAskHour12:    return makeOhlcv(datasetTraits, "cah12");
            case DataKind::OhlcvAskHour8:     return makeOhlcv(datasetTraits, "cah08");
            case DataKind::OhlcvAskHour6:     return makeOhlcv(datasetTraits, "cah06");
            case DataKind::OhlcvAskHour4:     return makeOhlcv(datasetTraits, "cah04");
            case DataKind::OhlcvAskHour3:     return makeOhlcv(datasetTraits, "cah03");
            case DataKind::OhlcvAskHour2:     return makeOhlcv(datasetTraits, "cah02");
            case DataKind::OhlcvAskHour1:     return makeOhlcv(datasetTraits, "cah01");
            case DataKind::OhlcvAskMinute30:  return makeOhlcv(datasetTraits, "cam30");
            case DataKind::OhlcvAskMinute20:  return makeOhlcv(datasetTraits, "cam20");
            case DataKind::OhlcvAskMinute15:  return makeOhlcv(datasetTraits, "cam15");
            case DataKind::OhlcvAskMinute12:  return makeOhlcv(datasetTraits, "cam12");
            case DataKind::OhlcvAskMinute10:  return makeOhlcv(datasetTraits, "cam10");
            case DataKind::OhlcvAskMinute6:   return makeOhlcv(datasetTraits, "cam06");
            case DataKind::OhlcvAskMinute5:   return makeOhlcv(datasetTraits, "cam05");
            case DataKind::OhlcvAskMinute4:   return makeOhlcv(datasetTraits, "cam04");
            case DataKind::OhlcvAskMinute3:   return makeOhlcv(datasetTraits, "cam03");
            case DataKind::OhlcvAskMinute2:   return makeOhlcv(datasetTraits, "cam02");
            case DataKind::OhlcvAskMinute1:   return makeOhlcv(datasetTraits, "cam01");
            case DataKind::OhlcvAskSecond30:  return makeOhlcv(datasetTraits, "cas30");
            case DataKind::OhlcvAskSecond20:  return makeOhlcv(datasetTraits, "cas20");
            case DataKind::OhlcvAskSecond15:  return makeOhlcv(datasetTraits, "cas15");
            case DataKind::OhlcvAskSecond12:  return makeOhlcv(datasetTraits, "cas12");
            case DataKind::OhlcvAskSecond10:  return makeOhlcv(datasetTraits, "cas10");
            case DataKind::OhlcvAskSecond6:   return makeOhlcv(datasetTraits, "cas06");
            case DataKind::OhlcvAskSecond5:   return makeOhlcv(datasetTraits, "cas05");
            case DataKind::OhlcvAskSecond4:   return makeOhlcv(datasetTraits, "cas04");
            case DataKind::OhlcvAskSecond3:   return makeOhlcv(datasetTraits, "cas03");
            case DataKind::OhlcvAskSecond2:   return makeOhlcv(datasetTraits, "cas02");
            case DataKind::OhlcvAskSecond1:   return makeOhlcv(datasetTraits, "cas01");
            case DataKind::OhlcvAskAperiodic: return makeOhlcv(datasetTraits, "ca___");

            case DataKind::OhlcvBidYear1:     return makeOhlcv(datasetTraits, "cby01");
            case DataKind::OhlcvBidMonth6:    return makeOhlcv(datasetTraits, "cbM06");
            case DataKind::OhlcvBidMonth4:    return makeOhlcv(datasetTraits, "cbM04");
            case DataKind::OhlcvBidMonth3:    return makeOhlcv(datasetTraits, "cbM03");
            case DataKind::OhlcvBidMonth2:    return makeOhlcv(datasetTraits, "cbM02");
            case DataKind::OhlcvBidMonth1:    return makeOhlcv(datasetTraits, "cbM01");
            case DataKind::OhlcvBidWeek3:     return makeOhlcv(datasetTraits, "cbw03");
            case DataKind::OhlcvBidWeek2:     return makeOhlcv(datasetTraits, "cbw02");
            case DataKind::OhlcvBidWeek1:     return makeOhlcv(datasetTraits, "cbw01");
            case DataKind::OhlcvBidDay1:      return makeOhlcv(datasetTraits, "cbd01");
            case DataKind::OhlcvBidHour12:    return makeOhlcv(datasetTraits, "cbh12");
            case DataKind::OhlcvBidHour8:     return makeOhlcv(datasetTraits, "cbh08");
            case DataKind::OhlcvBidHour6:     return makeOhlcv(datasetTraits, "cbh06");
            case DataKind::OhlcvBidHour4:     return makeOhlcv(datasetTraits, "cbh04");
            case DataKind::OhlcvBidHour3:     return makeOhlcv(datasetTraits, "cbh03");
            case DataKind::OhlcvBidHour2:     return makeOhlcv(datasetTraits, "cbh02");
            case DataKind::OhlcvBidHour1:     return makeOhlcv(datasetTraits, "cbh01");
            case DataKind::OhlcvBidMinute30:  return makeOhlcv(datasetTraits, "cbm30");
            case DataKind::OhlcvBidMinute20:  return makeOhlcv(datasetTraits, "cbm20");
            case DataKind::OhlcvBidMinute15:  return makeOhlcv(datasetTraits, "cbm15");
            case DataKind::OhlcvBidMinute12:  return makeOhlcv(datasetTraits, "cbm12");
            case DataKind::OhlcvBidMinute10:  return makeOhlcv(datasetTraits, "cbm10");
            case DataKind::OhlcvBidMinute6:   return makeOhlcv(datasetTraits, "cbm06");
            case DataKind::OhlcvBidMinute5:   return makeOhlcv(datasetTraits, "cbm05");
            case DataKind::OhlcvBidMinute4:   return makeOhlcv(datasetTraits, "cbm04");
            case DataKind::OhlcvBidMinute3:   return makeOhlcv(datasetTraits, "cbm03");
            case DataKind::OhlcvBidMinute2:   return makeOhlcv(datasetTraits, "cbm02");
            case DataKind::OhlcvBidMinute1:   return makeOhlcv(datasetTraits, "cbm01");
            case DataKind::OhlcvBidSecond30:  return makeOhlcv(datasetTraits, "cbs30");
            case DataKind::OhlcvBidSecond20:  return makeOhlcv(datasetTraits, "cbs20");
            case DataKind::OhlcvBidSecond15:  return makeOhlcv(datasetTraits, "cbs15");
            case DataKind::OhlcvBidSecond12:  return makeOhlcv(datasetTraits, "cbs12");
            case DataKind::OhlcvBidSecond10:  return makeOhlcv(datasetTraits, "cbs10");
            case DataKind::OhlcvBidSecond6:   return makeOhlcv(datasetTraits, "cbs06");
            case DataKind::OhlcvBidSecond5:   return makeOhlcv(datasetTraits, "cbs05");
            case DataKind::OhlcvBidSecond4:   return makeOhlcv(datasetTraits, "cbs04");
            case DataKind::OhlcvBidSecond3:   return makeOhlcv(datasetTraits, "cbs03");
            case DataKind::OhlcvBidSecond2:   return makeOhlcv(datasetTraits, "cbs02");
            case DataKind::OhlcvBidSecond1:   return makeOhlcv(datasetTraits, "cbs01");
            case DataKind::OhlcvBidAperiodic: return makeOhlcv(datasetTraits, "cb___");

            case DataKind::OhlcvAdjustedYear1:     return makeOhlcv(datasetTraits, "j_y01");
            case DataKind::OhlcvAdjustedMonth6:    return makeOhlcv(datasetTraits, "j_M06");
            case DataKind::OhlcvAdjustedMonth4:    return makeOhlcv(datasetTraits, "j_M04");
            case DataKind::OhlcvAdjustedMonth3:    return makeOhlcv(datasetTraits, "j_M03");
            case DataKind::OhlcvAdjustedMonth2:    return makeOhlcv(datasetTraits, "j_M02");
            case DataKind::OhlcvAdjustedMonth1:    return makeOhlcv(datasetTraits, "j_M01");
            case DataKind::OhlcvAdjustedWeek3:     return makeOhlcv(datasetTraits, "j_w03");
            case DataKind::OhlcvAdjustedWeek2:     return makeOhlcv(datasetTraits, "j_w02");
            case DataKind::OhlcvAdjustedWeek1:     return makeOhlcv(datasetTraits, "j_w01");
            case DataKind::OhlcvAdjustedDay1:      return makeOhlcv(datasetTraits, "j_d01");
            case DataKind::OhlcvAdjustedHour12:    return makeOhlcv(datasetTraits, "j_h12");
            case DataKind::OhlcvAdjustedHour8:     return makeOhlcv(datasetTraits, "j_h08");
            case DataKind::OhlcvAdjustedHour6:     return makeOhlcv(datasetTraits, "j_h06");
            case DataKind::OhlcvAdjustedHour4:     return makeOhlcv(datasetTraits, "j_h04");
            case DataKind::OhlcvAdjustedHour3:     return makeOhlcv(datasetTraits, "j_h03");
            case DataKind::OhlcvAdjustedHour2:     return makeOhlcv(datasetTraits, "j_h02");
            case DataKind::OhlcvAdjustedHour1:     return makeOhlcv(datasetTraits, "j_h01");
            case DataKind::OhlcvAdjustedMinute30:  return makeOhlcv(datasetTraits, "j_m30");
            case DataKind::OhlcvAdjustedMinute20:  return makeOhlcv(datasetTraits, "j_m20");
            case DataKind::OhlcvAdjustedMinute15:  return makeOhlcv(datasetTraits, "j_m15");
            case DataKind::OhlcvAdjustedMinute12:  return makeOhlcv(datasetTraits, "j_m12");
            case DataKind::OhlcvAdjustedMinute10:  return makeOhlcv(datasetTraits, "j_m10");
            case DataKind::OhlcvAdjustedMinute6:   return makeOhlcv(datasetTraits, "j_m06");
            case DataKind::OhlcvAdjustedMinute5:   return makeOhlcv(datasetTraits, "j_m05");
            case DataKind::OhlcvAdjustedMinute4:   return makeOhlcv(datasetTraits, "j_m04");
            case DataKind::OhlcvAdjustedMinute3:   return makeOhlcv(datasetTraits, "j_m03");
            case DataKind::OhlcvAdjustedMinute2:   return makeOhlcv(datasetTraits, "j_m02");
            case DataKind::OhlcvAdjustedMinute1:   return makeOhlcv(datasetTraits, "j_m01");
            case DataKind::OhlcvAdjustedSecond30:  return makeOhlcv(datasetTraits, "j_s30");
            case DataKind::OhlcvAdjustedSecond20:  return makeOhlcv(datasetTraits, "j_s20");
            case DataKind::OhlcvAdjustedSecond15:  return makeOhlcv(datasetTraits, "j_s15");
            case DataKind::OhlcvAdjustedSecond12:  return makeOhlcv(datasetTraits, "j_s12");
            case DataKind::OhlcvAdjustedSecond10:  return makeOhlcv(datasetTraits, "j_s10");
            case DataKind::OhlcvAdjustedSecond6:   return makeOhlcv(datasetTraits, "j_s06");
            case DataKind::OhlcvAdjustedSecond5:   return makeOhlcv(datasetTraits, "j_s05");
            case DataKind::OhlcvAdjustedSecond4:   return makeOhlcv(datasetTraits, "j_s04");
            case DataKind::OhlcvAdjustedSecond3:   return makeOhlcv(datasetTraits, "j_s03");
            case DataKind::OhlcvAdjustedSecond2:   return makeOhlcv(datasetTraits, "j_s02");
            case DataKind::OhlcvAdjustedSecond1:   return makeOhlcv(datasetTraits, "j_s01");
            case DataKind::OhlcvAdjustedAperiodic: return makeOhlcv(datasetTraits, "j____");

            case DataKind::OhlcvPriceOnlyYear1:     return makeOhlcvPriceOnly(datasetTraits, "C_y01");
            case DataKind::OhlcvPriceOnlyMonth6:    return makeOhlcvPriceOnly(datasetTraits, "C_M06");
            case DataKind::OhlcvPriceOnlyMonth4:    return makeOhlcvPriceOnly(datasetTraits, "C_M04");
            case DataKind::OhlcvPriceOnlyMonth3:    return makeOhlcvPriceOnly(datasetTraits, "C_M03");
            case DataKind::OhlcvPriceOnlyMonth2:    return makeOhlcvPriceOnly(datasetTraits, "C_M02");
            case DataKind::OhlcvPriceOnlyMonth1:    return makeOhlcvPriceOnly(datasetTraits, "C_M01");
            case DataKind::OhlcvPriceOnlyWeek3:     return makeOhlcvPriceOnly(datasetTraits, "C_w03");
            case DataKind::OhlcvPriceOnlyWeek2:     return makeOhlcvPriceOnly(datasetTraits, "C_w02");
            case DataKind::OhlcvPriceOnlyWeek1:     return makeOhlcvPriceOnly(datasetTraits, "C_w01");
            case DataKind::OhlcvPriceOnlyDay1:      return makeOhlcvPriceOnly(datasetTraits, "C_d01");
            case DataKind::OhlcvPriceOnlyHour12:    return makeOhlcvPriceOnly(datasetTraits, "C_h12");
            case DataKind::OhlcvPriceOnlyHour8:     return makeOhlcvPriceOnly(datasetTraits, "C_h08");
            case DataKind::OhlcvPriceOnlyHour6:     return makeOhlcvPriceOnly(datasetTraits, "C_h06");
            case DataKind::OhlcvPriceOnlyHour4:     return makeOhlcvPriceOnly(datasetTraits, "C_h04");
            case DataKind::OhlcvPriceOnlyHour3:     return makeOhlcvPriceOnly(datasetTraits, "C_h03");
            case DataKind::OhlcvPriceOnlyHour2:     return makeOhlcvPriceOnly(datasetTraits, "C_h02");
            case DataKind::OhlcvPriceOnlyHour1:     return makeOhlcvPriceOnly(datasetTraits, "C_h01");
            case DataKind::OhlcvPriceOnlyMinute30:  return makeOhlcvPriceOnly(datasetTraits, "C_m30");
            case DataKind::OhlcvPriceOnlyMinute20:  return makeOhlcvPriceOnly(datasetTraits, "C_m20");
            case DataKind::OhlcvPriceOnlyMinute15:  return makeOhlcvPriceOnly(datasetTraits, "C_m15");
            case DataKind::OhlcvPriceOnlyMinute12:  return makeOhlcvPriceOnly(datasetTraits, "C_m12");
            case DataKind::OhlcvPriceOnlyMinute10:  return makeOhlcvPriceOnly(datasetTraits, "C_m10");
            case DataKind::OhlcvPriceOnlyMinute6:   return makeOhlcvPriceOnly(datasetTraits, "C_m06");
            case DataKind::OhlcvPriceOnlyMinute5:   return makeOhlcvPriceOnly(datasetTraits, "C_m05");
            case DataKind::OhlcvPriceOnlyMinute4:   return makeOhlcvPriceOnly(datasetTraits, "C_m04");
            case DataKind::OhlcvPriceOnlyMinute3:   return makeOhlcvPriceOnly(datasetTraits, "C_m03");
            case DataKind::OhlcvPriceOnlyMinute2:   return makeOhlcvPriceOnly(datasetTraits, "C_m02");
            case DataKind::OhlcvPriceOnlyMinute1:   return makeOhlcvPriceOnly(datasetTraits, "C_m01");
            case DataKind::OhlcvPriceOnlySecond30:  return makeOhlcvPriceOnly(datasetTraits, "C_s30");
            case DataKind::OhlcvPriceOnlySecond20:  return makeOhlcvPriceOnly(datasetTraits, "C_s20");
            case DataKind::OhlcvPriceOnlySecond15:  return makeOhlcvPriceOnly(datasetTraits, "C_s15");
            case DataKind::OhlcvPriceOnlySecond12:  return makeOhlcvPriceOnly(datasetTraits, "C_s12");
            case DataKind::OhlcvPriceOnlySecond10:  return makeOhlcvPriceOnly(datasetTraits, "C_s10");
            case DataKind::OhlcvPriceOnlySecond6:   return makeOhlcvPriceOnly(datasetTraits, "C_s06");
            case DataKind::OhlcvPriceOnlySecond5:   return makeOhlcvPriceOnly(datasetTraits, "C_s05");
            case DataKind::OhlcvPriceOnlySecond4:   return makeOhlcvPriceOnly(datasetTraits, "C_s04");
            case DataKind::OhlcvPriceOnlySecond3:   return makeOhlcvPriceOnly(datasetTraits, "C_s03");
            case DataKind::OhlcvPriceOnlySecond2:   return makeOhlcvPriceOnly(datasetTraits, "C_s02");
            case DataKind::OhlcvPriceOnlySecond1:   return makeOhlcvPriceOnly(datasetTraits, "C_s01");
            case DataKind::OhlcvPriceOnlyAperiodic: return makeOhlcvPriceOnly(datasetTraits, "C____");

            case DataKind::OhlcvPriceOnlyAskYear1:     return makeOhlcvPriceOnly(datasetTraits, "Cay01");
            case DataKind::OhlcvPriceOnlyAskMonth6:    return makeOhlcvPriceOnly(datasetTraits, "CaM06");
            case DataKind::OhlcvPriceOnlyAskMonth4:    return makeOhlcvPriceOnly(datasetTraits, "CaM04");
            case DataKind::OhlcvPriceOnlyAskMonth3:    return makeOhlcvPriceOnly(datasetTraits, "CaM03");
            case DataKind::OhlcvPriceOnlyAskMonth2:    return makeOhlcvPriceOnly(datasetTraits, "CaM02");
            case DataKind::OhlcvPriceOnlyAskMonth1:    return makeOhlcvPriceOnly(datasetTraits, "CaM01");
            case DataKind::OhlcvPriceOnlyAskWeek3:     return makeOhlcvPriceOnly(datasetTraits, "Caw03");
            case DataKind::OhlcvPriceOnlyAskWeek2:     return makeOhlcvPriceOnly(datasetTraits, "Caw02");
            case DataKind::OhlcvPriceOnlyAskWeek1:     return makeOhlcvPriceOnly(datasetTraits, "Caw01");
            case DataKind::OhlcvPriceOnlyAskDay1:      return makeOhlcvPriceOnly(datasetTraits, "Cad01");
            case DataKind::OhlcvPriceOnlyAskHour12:    return makeOhlcvPriceOnly(datasetTraits, "Cah12");
            case DataKind::OhlcvPriceOnlyAskHour8:     return makeOhlcvPriceOnly(datasetTraits, "Cah08");
            case DataKind::OhlcvPriceOnlyAskHour6:     return makeOhlcvPriceOnly(datasetTraits, "Cah06");
            case DataKind::OhlcvPriceOnlyAskHour4:     return makeOhlcvPriceOnly(datasetTraits, "Cah04");
            case DataKind::OhlcvPriceOnlyAskHour3:     return makeOhlcvPriceOnly(datasetTraits, "Cah03");
            case DataKind::OhlcvPriceOnlyAskHour2:     return makeOhlcvPriceOnly(datasetTraits, "Cah02");
            case DataKind::OhlcvPriceOnlyAskHour1:     return makeOhlcvPriceOnly(datasetTraits, "Cah01");
            case DataKind::OhlcvPriceOnlyAskMinute30:  return makeOhlcvPriceOnly(datasetTraits, "Cam30");
            case DataKind::OhlcvPriceOnlyAskMinute20:  return makeOhlcvPriceOnly(datasetTraits, "Cam20");
            case DataKind::OhlcvPriceOnlyAskMinute15:  return makeOhlcvPriceOnly(datasetTraits, "Cam15");
            case DataKind::OhlcvPriceOnlyAskMinute12:  return makeOhlcvPriceOnly(datasetTraits, "Cam12");
            case DataKind::OhlcvPriceOnlyAskMinute10:  return makeOhlcvPriceOnly(datasetTraits, "Cam10");
            case DataKind::OhlcvPriceOnlyAskMinute6:   return makeOhlcvPriceOnly(datasetTraits, "Cam06");
            case DataKind::OhlcvPriceOnlyAskMinute5:   return makeOhlcvPriceOnly(datasetTraits, "Cam05");
            case DataKind::OhlcvPriceOnlyAskMinute4:   return makeOhlcvPriceOnly(datasetTraits, "Cam04");
            case DataKind::OhlcvPriceOnlyAskMinute3:   return makeOhlcvPriceOnly(datasetTraits, "Cam03");
            case DataKind::OhlcvPriceOnlyAskMinute2:   return makeOhlcvPriceOnly(datasetTraits, "Cam02");
            case DataKind::OhlcvPriceOnlyAskMinute1:   return makeOhlcvPriceOnly(datasetTraits, "Cam01");
            case DataKind::OhlcvPriceOnlyAskSecond30:  return makeOhlcvPriceOnly(datasetTraits, "Cas30");
            case DataKind::OhlcvPriceOnlyAskSecond20:  return makeOhlcvPriceOnly(datasetTraits, "Cas20");
            case DataKind::OhlcvPriceOnlyAskSecond15:  return makeOhlcvPriceOnly(datasetTraits, "Cas15");
            case DataKind::OhlcvPriceOnlyAskSecond12:  return makeOhlcvPriceOnly(datasetTraits, "Cas12");
            case DataKind::OhlcvPriceOnlyAskSecond10:  return makeOhlcvPriceOnly(datasetTraits, "Cas10");
            case DataKind::OhlcvPriceOnlyAskSecond6:   return makeOhlcvPriceOnly(datasetTraits, "Cas06");
            case DataKind::OhlcvPriceOnlyAskSecond5:   return makeOhlcvPriceOnly(datasetTraits, "Cas05");
            case DataKind::OhlcvPriceOnlyAskSecond4:   return makeOhlcvPriceOnly(datasetTraits, "Cas04");
            case DataKind::OhlcvPriceOnlyAskSecond3:   return makeOhlcvPriceOnly(datasetTraits, "Cas03");
            case DataKind::OhlcvPriceOnlyAskSecond2:   return makeOhlcvPriceOnly(datasetTraits, "Cas02");
            case DataKind::OhlcvPriceOnlyAskSecond1:   return makeOhlcvPriceOnly(datasetTraits, "Cas01");
            case DataKind::OhlcvPriceOnlyAskAperiodic: return makeOhlcvPriceOnly(datasetTraits, "Ca___");

            case DataKind::OhlcvPriceOnlyBidYear1:     return makeOhlcvPriceOnly(datasetTraits, "Cby01");
            case DataKind::OhlcvPriceOnlyBidMonth6:    return makeOhlcvPriceOnly(datasetTraits, "CbM06");
            case DataKind::OhlcvPriceOnlyBidMonth4:    return makeOhlcvPriceOnly(datasetTraits, "CbM04");
            case DataKind::OhlcvPriceOnlyBidMonth3:    return makeOhlcvPriceOnly(datasetTraits, "CbM03");
            case DataKind::OhlcvPriceOnlyBidMonth2:    return makeOhlcvPriceOnly(datasetTraits, "CbM02");
            case DataKind::OhlcvPriceOnlyBidMonth1:    return makeOhlcvPriceOnly(datasetTraits, "CbM01");
            case DataKind::OhlcvPriceOnlyBidWeek3:     return makeOhlcvPriceOnly(datasetTraits, "Cbw03");
            case DataKind::OhlcvPriceOnlyBidWeek2:     return makeOhlcvPriceOnly(datasetTraits, "Cbw02");
            case DataKind::OhlcvPriceOnlyBidWeek1:     return makeOhlcvPriceOnly(datasetTraits, "Cbw01");
            case DataKind::OhlcvPriceOnlyBidDay1:      return makeOhlcvPriceOnly(datasetTraits, "Cbd01");
            case DataKind::OhlcvPriceOnlyBidHour12:    return makeOhlcvPriceOnly(datasetTraits, "Cbh12");
            case DataKind::OhlcvPriceOnlyBidHour8:     return makeOhlcvPriceOnly(datasetTraits, "Cbh08");
            case DataKind::OhlcvPriceOnlyBidHour6:     return makeOhlcvPriceOnly(datasetTraits, "Cbh06");
            case DataKind::OhlcvPriceOnlyBidHour4:     return makeOhlcvPriceOnly(datasetTraits, "Cbh04");
            case DataKind::OhlcvPriceOnlyBidHour3:     return makeOhlcvPriceOnly(datasetTraits, "Cbh03");
            case DataKind::OhlcvPriceOnlyBidHour2:     return makeOhlcvPriceOnly(datasetTraits, "Cbh02");
            case DataKind::OhlcvPriceOnlyBidHour1:     return makeOhlcvPriceOnly(datasetTraits, "Cbh01");
            case DataKind::OhlcvPriceOnlyBidMinute30:  return makeOhlcvPriceOnly(datasetTraits, "Cbm30");
            case DataKind::OhlcvPriceOnlyBidMinute20:  return makeOhlcvPriceOnly(datasetTraits, "Cbm20");
            case DataKind::OhlcvPriceOnlyBidMinute15:  return makeOhlcvPriceOnly(datasetTraits, "Cbm15");
            case DataKind::OhlcvPriceOnlyBidMinute12:  return makeOhlcvPriceOnly(datasetTraits, "Cbm12");
            case DataKind::OhlcvPriceOnlyBidMinute10:  return makeOhlcvPriceOnly(datasetTraits, "Cbm10");
            case DataKind::OhlcvPriceOnlyBidMinute6:   return makeOhlcvPriceOnly(datasetTraits, "Cbm06");
            case DataKind::OhlcvPriceOnlyBidMinute5:   return makeOhlcvPriceOnly(datasetTraits, "Cbm05");
            case DataKind::OhlcvPriceOnlyBidMinute4:   return makeOhlcvPriceOnly(datasetTraits, "Cbm04");
            case DataKind::OhlcvPriceOnlyBidMinute3:   return makeOhlcvPriceOnly(datasetTraits, "Cbm03");
            case DataKind::OhlcvPriceOnlyBidMinute2:   return makeOhlcvPriceOnly(datasetTraits, "Cbm02");
            case DataKind::OhlcvPriceOnlyBidMinute1:   return makeOhlcvPriceOnly(datasetTraits, "Cbm01");
            case DataKind::OhlcvPriceOnlyBidSecond30:  return makeOhlcvPriceOnly(datasetTraits, "Cbs30");
            case DataKind::OhlcvPriceOnlyBidSecond20:  return makeOhlcvPriceOnly(datasetTraits, "Cbs20");
            case DataKind::OhlcvPriceOnlyBidSecond15:  return makeOhlcvPriceOnly(datasetTraits, "Cbs15");
            case DataKind::OhlcvPriceOnlyBidSecond12:  return makeOhlcvPriceOnly(datasetTraits, "Cbs12");
            case DataKind::OhlcvPriceOnlyBidSecond10:  return makeOhlcvPriceOnly(datasetTraits, "Cbs10");
            case DataKind::OhlcvPriceOnlyBidSecond6:   return makeOhlcvPriceOnly(datasetTraits, "Cbs06");
            case DataKind::OhlcvPriceOnlyBidSecond5:   return makeOhlcvPriceOnly(datasetTraits, "Cbs05");
            case DataKind::OhlcvPriceOnlyBidSecond4:   return makeOhlcvPriceOnly(datasetTraits, "Cbs04");
            case DataKind::OhlcvPriceOnlyBidSecond3:   return makeOhlcvPriceOnly(datasetTraits, "Cbs03");
            case DataKind::OhlcvPriceOnlyBidSecond2:   return makeOhlcvPriceOnly(datasetTraits, "Cbs02");
            case DataKind::OhlcvPriceOnlyBidSecond1:   return makeOhlcvPriceOnly(datasetTraits, "Cbs01");
            case DataKind::OhlcvPriceOnlyBidAperiodic: return makeOhlcvPriceOnly(datasetTraits, "Cb___");

            case DataKind::OhlcvAdjustedPriceOnlyYear1:     return makeOhlcvPriceOnly(datasetTraits, "J_y01");
            case DataKind::OhlcvAdjustedPriceOnlyMonth6:    return makeOhlcvPriceOnly(datasetTraits, "J_M06");
            case DataKind::OhlcvAdjustedPriceOnlyMonth4:    return makeOhlcvPriceOnly(datasetTraits, "J_M04");
            case DataKind::OhlcvAdjustedPriceOnlyMonth3:    return makeOhlcvPriceOnly(datasetTraits, "J_M03");
            case DataKind::OhlcvAdjustedPriceOnlyMonth2:    return makeOhlcvPriceOnly(datasetTraits, "J_M02");
            case DataKind::OhlcvAdjustedPriceOnlyMonth1:    return makeOhlcvPriceOnly(datasetTraits, "J_M01");
            case DataKind::OhlcvAdjustedPriceOnlyWeek3:     return makeOhlcvPriceOnly(datasetTraits, "J_w03");
            case DataKind::OhlcvAdjustedPriceOnlyWeek2:     return makeOhlcvPriceOnly(datasetTraits, "J_w02");
            case DataKind::OhlcvAdjustedPriceOnlyWeek1:     return makeOhlcvPriceOnly(datasetTraits, "J_w01");
            case DataKind::OhlcvAdjustedPriceOnlyDay1:      return makeOhlcvPriceOnly(datasetTraits, "J_d01");
            case DataKind::OhlcvAdjustedPriceOnlyHour12:    return makeOhlcvPriceOnly(datasetTraits, "J_h12");
            case DataKind::OhlcvAdjustedPriceOnlyHour8:     return makeOhlcvPriceOnly(datasetTraits, "J_h08");
            case DataKind::OhlcvAdjustedPriceOnlyHour6:     return makeOhlcvPriceOnly(datasetTraits, "J_h06");
            case DataKind::OhlcvAdjustedPriceOnlyHour4:     return makeOhlcvPriceOnly(datasetTraits, "J_h04");
            case DataKind::OhlcvAdjustedPriceOnlyHour3:     return makeOhlcvPriceOnly(datasetTraits, "J_h03");
            case DataKind::OhlcvAdjustedPriceOnlyHour2:     return makeOhlcvPriceOnly(datasetTraits, "J_h02");
            case DataKind::OhlcvAdjustedPriceOnlyHour1:     return makeOhlcvPriceOnly(datasetTraits, "J_h01");
            case DataKind::OhlcvAdjustedPriceOnlyMinute30:  return makeOhlcvPriceOnly(datasetTraits, "J_m30");
            case DataKind::OhlcvAdjustedPriceOnlyMinute20:  return makeOhlcvPriceOnly(datasetTraits, "J_m20");
            case DataKind::OhlcvAdjustedPriceOnlyMinute15:  return makeOhlcvPriceOnly(datasetTraits, "J_m15");
            case DataKind::OhlcvAdjustedPriceOnlyMinute12:  return makeOhlcvPriceOnly(datasetTraits, "J_m12");
            case DataKind::OhlcvAdjustedPriceOnlyMinute10:  return makeOhlcvPriceOnly(datasetTraits, "J_m10");
            case DataKind::OhlcvAdjustedPriceOnlyMinute6:   return makeOhlcvPriceOnly(datasetTraits, "J_m06");
            case DataKind::OhlcvAdjustedPriceOnlyMinute5:   return makeOhlcvPriceOnly(datasetTraits, "J_m05");
            case DataKind::OhlcvAdjustedPriceOnlyMinute4:   return makeOhlcvPriceOnly(datasetTraits, "J_m04");
            case DataKind::OhlcvAdjustedPriceOnlyMinute3:   return makeOhlcvPriceOnly(datasetTraits, "J_m03");
            case DataKind::OhlcvAdjustedPriceOnlyMinute2:   return makeOhlcvPriceOnly(datasetTraits, "J_m02");
            case DataKind::OhlcvAdjustedPriceOnlyMinute1:   return makeOhlcvPriceOnly(datasetTraits, "J_m01");
            case DataKind::OhlcvAdjustedPriceOnlySecond30:  return makeOhlcvPriceOnly(datasetTraits, "J_s30");
            case DataKind::OhlcvAdjustedPriceOnlySecond20:  return makeOhlcvPriceOnly(datasetTraits, "J_s20");
            case DataKind::OhlcvAdjustedPriceOnlySecond15:  return makeOhlcvPriceOnly(datasetTraits, "J_s15");
            case DataKind::OhlcvAdjustedPriceOnlySecond12:  return makeOhlcvPriceOnly(datasetTraits, "J_s12");
            case DataKind::OhlcvAdjustedPriceOnlySecond10:  return makeOhlcvPriceOnly(datasetTraits, "J_s10");
            case DataKind::OhlcvAdjustedPriceOnlySecond6:   return makeOhlcvPriceOnly(datasetTraits, "J_s06");
            case DataKind::OhlcvAdjustedPriceOnlySecond5:   return makeOhlcvPriceOnly(datasetTraits, "J_s05");
            case DataKind::OhlcvAdjustedPriceOnlySecond4:   return makeOhlcvPriceOnly(datasetTraits, "J_s04");
            case DataKind::OhlcvAdjustedPriceOnlySecond3:   return makeOhlcvPriceOnly(datasetTraits, "J_s03");
            case DataKind::OhlcvAdjustedPriceOnlySecond2:   return makeOhlcvPriceOnly(datasetTraits, "J_s02");
            case DataKind::OhlcvAdjustedPriceOnlySecond1:   return makeOhlcvPriceOnly(datasetTraits, "J_s01");
            case DataKind::OhlcvAdjustedPriceOnlyAperiodic: return makeOhlcvPriceOnly(datasetTraits, "J____");

            case DataKind::ScalarYear1:     return makeScalar(datasetTraits, "s_y01");
            case DataKind::ScalarMonth6:    return makeScalar(datasetTraits, "s_M06");
            case DataKind::ScalarMonth4:    return makeScalar(datasetTraits, "s_M04");
            case DataKind::ScalarMonth3:    return makeScalar(datasetTraits, "s_M03");
            case DataKind::ScalarMonth2:    return makeScalar(datasetTraits, "s_M02");
            case DataKind::ScalarMonth1:    return makeScalar(datasetTraits, "s_M01");
            case DataKind::ScalarWeek3:     return makeScalar(datasetTraits, "s_w03");
            case DataKind::ScalarWeek2:     return makeScalar(datasetTraits, "s_w02");
            case DataKind::ScalarWeek1:     return makeScalar(datasetTraits, "s_w01");
            case DataKind::ScalarDay1:      return makeScalar(datasetTraits, "s_d01");
            case DataKind::ScalarHour12:    return makeScalar(datasetTraits, "s_h12");
            case DataKind::ScalarHour8:     return makeScalar(datasetTraits, "s_h08");
            case DataKind::ScalarHour6:     return makeScalar(datasetTraits, "s_h06");
            case DataKind::ScalarHour4:     return makeScalar(datasetTraits, "s_h04");
            case DataKind::ScalarHour3:     return makeScalar(datasetTraits, "s_h03");
            case DataKind::ScalarHour2:     return makeScalar(datasetTraits, "s_h02");
            case DataKind::ScalarHour1:     return makeScalar(datasetTraits, "s_h01");
            case DataKind::ScalarMinute30:  return makeScalar(datasetTraits, "s_m30");
            case DataKind::ScalarMinute20:  return makeScalar(datasetTraits, "s_m20");
            case DataKind::ScalarMinute15:  return makeScalar(datasetTraits, "s_m15");
            case DataKind::ScalarMinute12:  return makeScalar(datasetTraits, "s_m12");
            case DataKind::ScalarMinute10:  return makeScalar(datasetTraits, "s_m10");
            case DataKind::ScalarMinute6:   return makeScalar(datasetTraits, "s_m06");
            case DataKind::ScalarMinute5:   return makeScalar(datasetTraits, "s_m05");
            case DataKind::ScalarMinute4:   return makeScalar(datasetTraits, "s_m04");
            case DataKind::ScalarMinute3:   return makeScalar(datasetTraits, "s_m03");
            case DataKind::ScalarMinute2:   return makeScalar(datasetTraits, "s_m02");
            case DataKind::ScalarMinute1:   return makeScalar(datasetTraits, "s_m01");
            case DataKind::ScalarSecond30:  return makeScalar(datasetTraits, "s_s30");
            case DataKind::ScalarSecond20:  return makeScalar(datasetTraits, "s_s20");
            case DataKind::ScalarSecond15:  return makeScalar(datasetTraits, "s_s15");
            case DataKind::ScalarSecond12:  return makeScalar(datasetTraits, "s_s12");
            case DataKind::ScalarSecond10:  return makeScalar(datasetTraits, "s_s10");
            case DataKind::ScalarSecond6:   return makeScalar(datasetTraits, "s_s06");
            case DataKind::ScalarSecond5:   return makeScalar(datasetTraits, "s_s05");
            case DataKind::ScalarSecond4:   return makeScalar(datasetTraits, "s_s04");
            case DataKind::ScalarSecond3:   return makeScalar(datasetTraits, "s_s03");
            case DataKind::ScalarSecond2:   return makeScalar(datasetTraits, "s_s02");
            case DataKind::ScalarSecond1:   return makeScalar(datasetTraits, "s_s01");
            case DataKind::ScalarAperiodic: return makeScalar(datasetTraits, "s____");

            default: break;
        }
        return false;
    }

    typedef struct
    {
        DataType dataType_;
        OhlcvKind ohlcvKind_;
        ScalarKind scalarKind_;
        TradeKind tradeKind_;
        QuoteKind quoteKind_;
        DataTimeFrame timeFrame_;
    } ParsedDatasetName;

    static bool parseTimeFrame(char c3, char c4, char c5, ParsedDatasetName& pdn)
    {
        if ('d' == c3)
        {
            if ('0' == c4 && '1' == c5)
            {
                pdn.timeFrame_ = DataTimeFrame::Day1;
                return true;
            }
        }
        else if ('h' == c3)
        {
            if ('0' == c4)
            {
                switch (c5)
                {
                    case '1':
                        pdn.timeFrame_ = DataTimeFrame::Hour1;
                        return true;
                    case '2':
                        pdn.timeFrame_ = DataTimeFrame::Hour2;
                        return true;
                    case '3':
                        pdn.timeFrame_ = DataTimeFrame::Hour3;
                        return true;
                    case '4':
                        pdn.timeFrame_ = DataTimeFrame::Hour4;
                        return true;
                    case '6':
                        pdn.timeFrame_ = DataTimeFrame::Hour6;
                        return true;
                    case '8':
                        pdn.timeFrame_ = DataTimeFrame::Hour8;
                        return true;
                    default:;
                }
            }
            else if ('1' == c4 && '2' == c5)
            {
                pdn.timeFrame_ = DataTimeFrame::Hour12;
                return true;
            }
        }
        else if ('m' == c3)
        {
            if ('0' == c4)
            {
                switch (c5)
                {
                    case '1':
                        pdn.timeFrame_ = DataTimeFrame::Minute1;
                        return true;
                    case '2':
                        pdn.timeFrame_ = DataTimeFrame::Minute2;
                        return true;
                    case '3':
                        pdn.timeFrame_ = DataTimeFrame::Minute3;
                        return true;
                    case '4':
                        pdn.timeFrame_ = DataTimeFrame::Minute4;
                        return true;
                    case '5':
                        pdn.timeFrame_ = DataTimeFrame::Minute5;
                        return true;
                    case '6':
                        pdn.timeFrame_ = DataTimeFrame::Minute6;
                        return true;
                    default:;
                }
            }
            else if ('1' == c4)
            {
                switch (c5)
                {
                    case '0':
                        pdn.timeFrame_ = DataTimeFrame::Minute10;
                        return true;
                    case '2':
                        pdn.timeFrame_ = DataTimeFrame::Minute12;
                        return true;
                    case '5':
                        pdn.timeFrame_ = DataTimeFrame::Minute15;
                        return true;
                    default:;
                }
            }
            else if ('2' == c4 && '0' == c5)
            {
                pdn.timeFrame_ = DataTimeFrame::Minute20;
                return true;
            }
            else if ('3' == c4 && '0' == c5)
            {
                pdn.timeFrame_ = DataTimeFrame::Minute30;
                return true;
            }
        }
        else if ('s' == c3)
        {
            if ('0' == c4)
            {
                switch (c5)
                {
                    case '1':
                        pdn.timeFrame_ = DataTimeFrame::Second1;
                        return true;
                    case '2':
                        pdn.timeFrame_ = DataTimeFrame::Second2;
                        return true;
                    case '3':
                        pdn.timeFrame_ = DataTimeFrame::Second3;
                        return true;
                    case '4':
                        pdn.timeFrame_ = DataTimeFrame::Second4;
                        return true;
                    case '5':
                        pdn.timeFrame_ = DataTimeFrame::Second5;
                        return true;
                    case '6':
                        pdn.timeFrame_ = DataTimeFrame::Second6;
                        return true;
                    default:;
                }
            }
            else if ('1' == c4)
            {
                switch (c5)
                {
                    case '0':
                        pdn.timeFrame_ = DataTimeFrame::Second10;
                        return true;
                    case '2':
                        pdn.timeFrame_ = DataTimeFrame::Second12;
                        return true;
                    case '5':
                        pdn.timeFrame_ = DataTimeFrame::Second15;
                        return true;
                    default:;
                }
            }
            else if ('2' == c4 && '0' == c5)
            {
                pdn.timeFrame_ = DataTimeFrame::Second20;
                return true;
            }
            else if ('3' == c4 && '0' == c5)
            {
                pdn.timeFrame_ = DataTimeFrame::Second30;
                return true;
            }
        }
        else if ('w' == c3)
        {
            if ('0' == c4)
            {
                switch (c5)
                {
                    case '1':
                        pdn.timeFrame_ = DataTimeFrame::Week1;
                        return true;
                    case '2':
                        pdn.timeFrame_ = DataTimeFrame::Week2;
                        return true;
                    case '3':
                        pdn.timeFrame_ = DataTimeFrame::Week3;
                        return true;
                    default:;
                }
            }
        }
        else if ('M' == c3)
        {
            if ('0' == c4)
            {
                switch (c5)
                {
                    case '1':
                        pdn.timeFrame_ = DataTimeFrame::Month1;
                        return true;
                    case '2':
                        pdn.timeFrame_ = DataTimeFrame::Month2;
                        return true;
                    case '3':
                        pdn.timeFrame_ = DataTimeFrame::Month3;
                        return true;
                    case '4':
                        pdn.timeFrame_ = DataTimeFrame::Month4;
                        return true;
                    case '6':
                        pdn.timeFrame_ = DataTimeFrame::Month6;
                        return true;
                    default:;
                }
            }
        }
        else if ('y' == c3 && '0' == c4 && '1' == c5)
        {
            pdn.timeFrame_ = DataTimeFrame::Year1;
            return true;
        }
        else if ('_' == c3 && '_' == c4 && '_' == c5)
            return true;
        return false;
    }

    static bool parseOhlcvKind(char c2, char c3, char c4, char c5, ParsedDatasetName& pdn)
    {
        if ('_' != c2)
        {
            if ('a' == c2)
                pdn.ohlcvKind_ = OhlcvKind::Ask;
            else if ('b' == c2)
                pdn.ohlcvKind_ = OhlcvKind::Bid;
            else
                return false;
        }
        return parseTimeFrame(c3, c4, c5, pdn);
    }

    static bool parseDatasetName(const char* name, ParsedDatasetName& pdn)
    {
        pdn.dataType_ = DataType::Trade;
        pdn.ohlcvKind_ = OhlcvKind::Default;
        pdn.scalarKind_ = ScalarKind::Default;
        pdn.tradeKind_ = TradeKind::Default;
        pdn.quoteKind_ = QuoteKind::Default;
        pdn.timeFrame_ = DataTimeFrame::Aperiodic;
        const char c1 = *name;
        if (0 == c1)
            return false;
        const char c2 = name[1];
        if (0 == c2)
            return false;
        const char c3 = name[2];
        if (0 == c3)
            return false;
        const char c4 = name[3];
        if (0 == c4)
            return false;
        const char c5 = name[4];
        if (0 == c5)
            return false;
        const char c6 = name[5];
        if (0 != c6)
            return false;
        if ('t' == c1)
        {
            if ('_' == c2 && '_' == c3 && '_' == c4 && '_' == c5)
                return true;
        }
        else if ('T' == c1)
        {
            pdn.dataType_ = DataType::TradePriceOnly;
            if ('_' == c2 && '_' == c3 && '_' == c4 && '_' == c5)
                return true;
        }
        else if ('q' == c1)
        {
            pdn.dataType_ = DataType::Quote;
            if ('_' == c2 && '_' == c3 && '_' == c4 && '_' == c5)
                return true;
        }
        else if ('Q' == c1)
        {
            pdn.dataType_ = DataType::QuotePriceOnly;
            if ('_' == c2 && '_' == c3 && '_' == c4 && '_' == c5)
                return true;
        }
        else if ('c' == c1)
        {
            pdn.dataType_ = DataType::Ohlcv;
            return parseOhlcvKind(c2, c3, c4, c5, pdn);
        }
        else if ('C' == c1)
        {
            pdn.dataType_ = DataType::OhlcvPriceOnly;
            return parseOhlcvKind(c2, c3, c4, c5, pdn);
        }
        else if ('j' == c1)
        {
            pdn.dataType_ = DataType::OhlcvAdjusted;
            return parseOhlcvKind(c2, c3, c4, c5, pdn);
        }
        else if ('J' == c1)
        {
            pdn.dataType_ = DataType::OhlcvAdjustedPriceOnly;
            return parseOhlcvKind(c2, c3, c4, c5, pdn);
        }
        else if ('s' == c1)
        {
            pdn.dataType_ = DataType::Scalar;
            if ('_' != c2)
                return false;
            return parseTimeFrame(c3, c4, c5, pdn);
        }
        return false;
    }

    static DataKind ohlcvKind2DataKind(const OhlcvKind kind, const DataTimeFrame timeFrame, const bool adjusted)
    {
        switch (kind)
        {
            case OhlcvKind::Default:
            {
                if (adjusted)
                {
                    switch (timeFrame)
                    {
                        case DataTimeFrame::Aperiodic: return DataKind::OhlcvAdjustedAperiodic;
                        case DataTimeFrame::Second1:   return DataKind::OhlcvAdjustedSecond1;
                        case DataTimeFrame::Second2:   return DataKind::OhlcvAdjustedSecond2;
                        case DataTimeFrame::Second3:   return DataKind::OhlcvAdjustedSecond3;
                        case DataTimeFrame::Second4:   return DataKind::OhlcvAdjustedSecond4;
                        case DataTimeFrame::Second5:   return DataKind::OhlcvAdjustedSecond5;
                        case DataTimeFrame::Second6:   return DataKind::OhlcvAdjustedSecond6;
                        case DataTimeFrame::Second10:  return DataKind::OhlcvAdjustedSecond10;
                        case DataTimeFrame::Second12:  return DataKind::OhlcvAdjustedSecond12;
                        case DataTimeFrame::Second15:  return DataKind::OhlcvAdjustedSecond15;
                        case DataTimeFrame::Second20:  return DataKind::OhlcvAdjustedSecond20;
                        case DataTimeFrame::Second30:  return DataKind::OhlcvAdjustedSecond30;
                        case DataTimeFrame::Minute1:   return DataKind::OhlcvAdjustedMinute1;
                        case DataTimeFrame::Minute2:   return DataKind::OhlcvAdjustedMinute2;
                        case DataTimeFrame::Minute3:   return DataKind::OhlcvAdjustedMinute3;
                        case DataTimeFrame::Minute4:   return DataKind::OhlcvAdjustedMinute4;
                        case DataTimeFrame::Minute5:   return DataKind::OhlcvAdjustedMinute5;
                        case DataTimeFrame::Minute6:   return DataKind::OhlcvAdjustedMinute6;
                        case DataTimeFrame::Minute10:  return DataKind::OhlcvAdjustedMinute10;
                        case DataTimeFrame::Minute12:  return DataKind::OhlcvAdjustedMinute12;
                        case DataTimeFrame::Minute15:  return DataKind::OhlcvAdjustedMinute15;
                        case DataTimeFrame::Minute20:  return DataKind::OhlcvAdjustedMinute20;
                        case DataTimeFrame::Minute30:  return DataKind::OhlcvAdjustedMinute30;
                        case DataTimeFrame::Hour1:     return DataKind::OhlcvAdjustedHour1;
                        case DataTimeFrame::Hour2:     return DataKind::OhlcvAdjustedHour2;
                        case DataTimeFrame::Hour3:     return DataKind::OhlcvAdjustedHour3;
                        case DataTimeFrame::Hour4:     return DataKind::OhlcvAdjustedHour4;
                        case DataTimeFrame::Hour6:     return DataKind::OhlcvAdjustedHour6;
                        case DataTimeFrame::Hour8:     return DataKind::OhlcvAdjustedHour8;
                        case DataTimeFrame::Hour12:    return DataKind::OhlcvAdjustedHour12;
                        case DataTimeFrame::Day1:      return DataKind::OhlcvAdjustedDay1;
                        case DataTimeFrame::Week1:     return DataKind::OhlcvAdjustedWeek1;
                        case DataTimeFrame::Week2:     return DataKind::OhlcvAdjustedWeek2;
                        case DataTimeFrame::Week3:     return DataKind::OhlcvAdjustedWeek3;
                        case DataTimeFrame::Month1:    return DataKind::OhlcvAdjustedMonth1;
                        case DataTimeFrame::Month2:    return DataKind::OhlcvAdjustedMonth2;
                        case DataTimeFrame::Month3:    return DataKind::OhlcvAdjustedMonth3;
                        case DataTimeFrame::Month4:    return DataKind::OhlcvAdjustedMonth4;
                        case DataTimeFrame::Month6:    return DataKind::OhlcvAdjustedMonth6;
                        case DataTimeFrame::Year1:     return DataKind::OhlcvAdjustedYear1;
                        default: break;
                    }
                    return DataKind::OhlcvAdjustedAperiodic;
                }
                switch (timeFrame)
                {
                    case DataTimeFrame::Aperiodic: return DataKind::OhlcvAperiodic;
                    case DataTimeFrame::Second1:   return DataKind::OhlcvSecond1;
                    case DataTimeFrame::Second2:   return DataKind::OhlcvSecond2;
                    case DataTimeFrame::Second3:   return DataKind::OhlcvSecond3;
                    case DataTimeFrame::Second4:   return DataKind::OhlcvSecond4;
                    case DataTimeFrame::Second5:   return DataKind::OhlcvSecond5;
                    case DataTimeFrame::Second6:   return DataKind::OhlcvSecond6;
                    case DataTimeFrame::Second10:  return DataKind::OhlcvSecond10;
                    case DataTimeFrame::Second12:  return DataKind::OhlcvSecond12;
                    case DataTimeFrame::Second15:  return DataKind::OhlcvSecond15;
                    case DataTimeFrame::Second20:  return DataKind::OhlcvSecond20;
                    case DataTimeFrame::Second30:  return DataKind::OhlcvSecond30;
                    case DataTimeFrame::Minute1:   return DataKind::OhlcvMinute1;
                    case DataTimeFrame::Minute2:   return DataKind::OhlcvMinute2;
                    case DataTimeFrame::Minute3:   return DataKind::OhlcvMinute3;
                    case DataTimeFrame::Minute4:   return DataKind::OhlcvMinute4;
                    case DataTimeFrame::Minute5:   return DataKind::OhlcvMinute5;
                    case DataTimeFrame::Minute6:   return DataKind::OhlcvMinute6;
                    case DataTimeFrame::Minute10:  return DataKind::OhlcvMinute10;
                    case DataTimeFrame::Minute12:  return DataKind::OhlcvMinute12;
                    case DataTimeFrame::Minute15:  return DataKind::OhlcvMinute15;
                    case DataTimeFrame::Minute20:  return DataKind::OhlcvMinute20;
                    case DataTimeFrame::Minute30:  return DataKind::OhlcvMinute30;
                    case DataTimeFrame::Hour1:     return DataKind::OhlcvHour1;
                    case DataTimeFrame::Hour2:     return DataKind::OhlcvHour2;
                    case DataTimeFrame::Hour3:     return DataKind::OhlcvHour3;
                    case DataTimeFrame::Hour4:     return DataKind::OhlcvHour4;
                    case DataTimeFrame::Hour6:     return DataKind::OhlcvHour6;
                    case DataTimeFrame::Hour8:     return DataKind::OhlcvHour8;
                    case DataTimeFrame::Hour12:    return DataKind::OhlcvHour12;
                    case DataTimeFrame::Day1:      return DataKind::OhlcvDay1;
                    case DataTimeFrame::Week1:     return DataKind::OhlcvWeek1;
                    case DataTimeFrame::Week2:     return DataKind::OhlcvWeek2;
                    case DataTimeFrame::Week3:     return DataKind::OhlcvWeek3;
                    case DataTimeFrame::Month1:    return DataKind::OhlcvMonth1;
                    case DataTimeFrame::Month2:    return DataKind::OhlcvMonth2;
                    case DataTimeFrame::Month3:    return DataKind::OhlcvMonth3;
                    case DataTimeFrame::Month4:    return DataKind::OhlcvMonth4;
                    case DataTimeFrame::Month6:    return DataKind::OhlcvMonth6;
                    case DataTimeFrame::Year1:     return DataKind::OhlcvYear1;
                    default: break;
                }
                return DataKind::OhlcvAperiodic;
            }
            case OhlcvKind::Ask:
            {
                switch (timeFrame)
                {
                    case DataTimeFrame::Aperiodic: return DataKind::OhlcvAskAperiodic;
                    case DataTimeFrame::Second1:   return DataKind::OhlcvAskSecond1;
                    case DataTimeFrame::Second2:   return DataKind::OhlcvAskSecond2;
                    case DataTimeFrame::Second3:   return DataKind::OhlcvAskSecond3;
                    case DataTimeFrame::Second4:   return DataKind::OhlcvAskSecond4;
                    case DataTimeFrame::Second5:   return DataKind::OhlcvAskSecond5;
                    case DataTimeFrame::Second6:   return DataKind::OhlcvAskSecond6;
                    case DataTimeFrame::Second10:  return DataKind::OhlcvAskSecond10;
                    case DataTimeFrame::Second12:  return DataKind::OhlcvAskSecond12;
                    case DataTimeFrame::Second15:  return DataKind::OhlcvAskSecond15;
                    case DataTimeFrame::Second20:  return DataKind::OhlcvAskSecond20;
                    case DataTimeFrame::Second30:  return DataKind::OhlcvAskSecond30;
                    case DataTimeFrame::Minute1:   return DataKind::OhlcvAskMinute1;
                    case DataTimeFrame::Minute2:   return DataKind::OhlcvAskMinute2;
                    case DataTimeFrame::Minute3:   return DataKind::OhlcvAskMinute3;
                    case DataTimeFrame::Minute4:   return DataKind::OhlcvAskMinute4;
                    case DataTimeFrame::Minute5:   return DataKind::OhlcvAskMinute5;
                    case DataTimeFrame::Minute6:   return DataKind::OhlcvAskMinute6;
                    case DataTimeFrame::Minute10:  return DataKind::OhlcvAskMinute10;
                    case DataTimeFrame::Minute12:  return DataKind::OhlcvAskMinute12;
                    case DataTimeFrame::Minute15:  return DataKind::OhlcvAskMinute15;
                    case DataTimeFrame::Minute20:  return DataKind::OhlcvAskMinute20;
                    case DataTimeFrame::Minute30:  return DataKind::OhlcvAskMinute30;
                    case DataTimeFrame::Hour1:     return DataKind::OhlcvAskHour1;
                    case DataTimeFrame::Hour2:     return DataKind::OhlcvAskHour2;
                    case DataTimeFrame::Hour3:     return DataKind::OhlcvAskHour3;
                    case DataTimeFrame::Hour4:     return DataKind::OhlcvAskHour4;
                    case DataTimeFrame::Hour6:     return DataKind::OhlcvAskHour6;
                    case DataTimeFrame::Hour8:     return DataKind::OhlcvAskHour8;
                    case DataTimeFrame::Hour12:    return DataKind::OhlcvAskHour12;
                    case DataTimeFrame::Day1:      return DataKind::OhlcvAskDay1;
                    case DataTimeFrame::Week1:     return DataKind::OhlcvAskWeek1;
                    case DataTimeFrame::Week2:     return DataKind::OhlcvAskWeek2;
                    case DataTimeFrame::Week3:     return DataKind::OhlcvAskWeek3;
                    case DataTimeFrame::Month1:    return DataKind::OhlcvAskMonth1;
                    case DataTimeFrame::Month2:    return DataKind::OhlcvAskMonth2;
                    case DataTimeFrame::Month3:    return DataKind::OhlcvAskMonth3;
                    case DataTimeFrame::Month4:    return DataKind::OhlcvAskMonth4;
                    case DataTimeFrame::Month6:    return DataKind::OhlcvAskMonth6;
                    case DataTimeFrame::Year1:     return DataKind::OhlcvAskYear1;
                    default: break;
                }
                return DataKind::OhlcvAskAperiodic;
            }
            case OhlcvKind::Bid:
            {
                switch (timeFrame)
                {
                    case DataTimeFrame::Aperiodic: return DataKind::OhlcvBidAperiodic;
                    case DataTimeFrame::Second1:   return DataKind::OhlcvBidSecond1;
                    case DataTimeFrame::Second2:   return DataKind::OhlcvBidSecond2;
                    case DataTimeFrame::Second3:   return DataKind::OhlcvBidSecond3;
                    case DataTimeFrame::Second4:   return DataKind::OhlcvBidSecond4;
                    case DataTimeFrame::Second5:   return DataKind::OhlcvBidSecond5;
                    case DataTimeFrame::Second6:   return DataKind::OhlcvBidSecond6;
                    case DataTimeFrame::Second10:  return DataKind::OhlcvBidSecond10;
                    case DataTimeFrame::Second12:  return DataKind::OhlcvBidSecond12;
                    case DataTimeFrame::Second15:  return DataKind::OhlcvBidSecond15;
                    case DataTimeFrame::Second20:  return DataKind::OhlcvBidSecond20;
                    case DataTimeFrame::Second30:  return DataKind::OhlcvBidSecond30;
                    case DataTimeFrame::Minute1:   return DataKind::OhlcvBidMinute1;
                    case DataTimeFrame::Minute2:   return DataKind::OhlcvBidMinute2;
                    case DataTimeFrame::Minute3:   return DataKind::OhlcvBidMinute3;
                    case DataTimeFrame::Minute4:   return DataKind::OhlcvBidMinute4;
                    case DataTimeFrame::Minute5:   return DataKind::OhlcvBidMinute5;
                    case DataTimeFrame::Minute6:   return DataKind::OhlcvBidMinute6;
                    case DataTimeFrame::Minute10:  return DataKind::OhlcvBidMinute10;
                    case DataTimeFrame::Minute12:  return DataKind::OhlcvBidMinute12;
                    case DataTimeFrame::Minute15:  return DataKind::OhlcvBidMinute15;
                    case DataTimeFrame::Minute20:  return DataKind::OhlcvBidMinute20;
                    case DataTimeFrame::Minute30:  return DataKind::OhlcvBidMinute30;
                    case DataTimeFrame::Hour1:     return DataKind::OhlcvBidHour1;
                    case DataTimeFrame::Hour2:     return DataKind::OhlcvBidHour2;
                    case DataTimeFrame::Hour3:     return DataKind::OhlcvBidHour3;
                    case DataTimeFrame::Hour4:     return DataKind::OhlcvBidHour4;
                    case DataTimeFrame::Hour6:     return DataKind::OhlcvBidHour6;
                    case DataTimeFrame::Hour8:     return DataKind::OhlcvBidHour8;
                    case DataTimeFrame::Hour12:    return DataKind::OhlcvBidHour12;
                    case DataTimeFrame::Day1:      return DataKind::OhlcvBidDay1;
                    case DataTimeFrame::Week1:     return DataKind::OhlcvBidWeek1;
                    case DataTimeFrame::Week2:     return DataKind::OhlcvBidWeek2;
                    case DataTimeFrame::Week3:     return DataKind::OhlcvBidWeek3;
                    case DataTimeFrame::Month1:    return DataKind::OhlcvBidMonth1;
                    case DataTimeFrame::Month2:    return DataKind::OhlcvBidMonth2;
                    case DataTimeFrame::Month3:    return DataKind::OhlcvBidMonth3;
                    case DataTimeFrame::Month4:    return DataKind::OhlcvBidMonth4;
                    case DataTimeFrame::Month6:    return DataKind::OhlcvBidMonth6;
                    case DataTimeFrame::Year1:     return DataKind::OhlcvBidYear1;
                    default: break;
                }
                return DataKind::OhlcvBidAperiodic;
            }
            default: break;
        }
        return DataKind::OhlcvAperiodic;
    }

    static DataKind ohlcvPriceOnlyKind2DataKind(const OhlcvKind kind, const DataTimeFrame timeFrame, const bool adjusted)
    {
        switch (kind)
        {
            case OhlcvKind::Default:
            {
                if (adjusted)
                {
                    switch (timeFrame)
                    {
                        case DataTimeFrame::Aperiodic: return DataKind::OhlcvAdjustedPriceOnlyAperiodic;
                        case DataTimeFrame::Second1:   return DataKind::OhlcvAdjustedPriceOnlySecond1;
                        case DataTimeFrame::Second2:   return DataKind::OhlcvAdjustedPriceOnlySecond2;
                        case DataTimeFrame::Second3:   return DataKind::OhlcvAdjustedPriceOnlySecond3;
                        case DataTimeFrame::Second4:   return DataKind::OhlcvAdjustedPriceOnlySecond4;
                        case DataTimeFrame::Second5:   return DataKind::OhlcvAdjustedPriceOnlySecond5;
                        case DataTimeFrame::Second6:   return DataKind::OhlcvAdjustedPriceOnlySecond6;
                        case DataTimeFrame::Second10:  return DataKind::OhlcvAdjustedPriceOnlySecond10;
                        case DataTimeFrame::Second12:  return DataKind::OhlcvAdjustedPriceOnlySecond12;
                        case DataTimeFrame::Second15:  return DataKind::OhlcvAdjustedPriceOnlySecond15;
                        case DataTimeFrame::Second20:  return DataKind::OhlcvAdjustedPriceOnlySecond20;
                        case DataTimeFrame::Second30:  return DataKind::OhlcvAdjustedPriceOnlySecond30;
                        case DataTimeFrame::Minute1:   return DataKind::OhlcvAdjustedPriceOnlyMinute1;
                        case DataTimeFrame::Minute2:   return DataKind::OhlcvAdjustedPriceOnlyMinute2;
                        case DataTimeFrame::Minute3:   return DataKind::OhlcvAdjustedPriceOnlyMinute3;
                        case DataTimeFrame::Minute4:   return DataKind::OhlcvAdjustedPriceOnlyMinute4;
                        case DataTimeFrame::Minute5:   return DataKind::OhlcvAdjustedPriceOnlyMinute5;
                        case DataTimeFrame::Minute6:   return DataKind::OhlcvAdjustedPriceOnlyMinute6;
                        case DataTimeFrame::Minute10:  return DataKind::OhlcvAdjustedPriceOnlyMinute10;
                        case DataTimeFrame::Minute12:  return DataKind::OhlcvAdjustedPriceOnlyMinute12;
                        case DataTimeFrame::Minute15:  return DataKind::OhlcvAdjustedPriceOnlyMinute15;
                        case DataTimeFrame::Minute20:  return DataKind::OhlcvAdjustedPriceOnlyMinute20;
                        case DataTimeFrame::Minute30:  return DataKind::OhlcvAdjustedPriceOnlyMinute30;
                        case DataTimeFrame::Hour1:     return DataKind::OhlcvAdjustedPriceOnlyHour1;
                        case DataTimeFrame::Hour2:     return DataKind::OhlcvAdjustedPriceOnlyHour2;
                        case DataTimeFrame::Hour3:     return DataKind::OhlcvAdjustedPriceOnlyHour3;
                        case DataTimeFrame::Hour4:     return DataKind::OhlcvAdjustedPriceOnlyHour4;
                        case DataTimeFrame::Hour6:     return DataKind::OhlcvAdjustedPriceOnlyHour6;
                        case DataTimeFrame::Hour8:     return DataKind::OhlcvAdjustedPriceOnlyHour8;
                        case DataTimeFrame::Hour12:    return DataKind::OhlcvAdjustedPriceOnlyHour12;
                        case DataTimeFrame::Day1:      return DataKind::OhlcvAdjustedPriceOnlyDay1;
                        case DataTimeFrame::Week1:     return DataKind::OhlcvAdjustedPriceOnlyWeek1;
                        case DataTimeFrame::Week2:     return DataKind::OhlcvAdjustedPriceOnlyWeek2;
                        case DataTimeFrame::Week3:     return DataKind::OhlcvAdjustedPriceOnlyWeek3;
                        case DataTimeFrame::Month1:    return DataKind::OhlcvAdjustedPriceOnlyMonth1;
                        case DataTimeFrame::Month2:    return DataKind::OhlcvAdjustedPriceOnlyMonth2;
                        case DataTimeFrame::Month3:    return DataKind::OhlcvAdjustedPriceOnlyMonth3;
                        case DataTimeFrame::Month4:    return DataKind::OhlcvAdjustedPriceOnlyMonth4;
                        case DataTimeFrame::Month6:    return DataKind::OhlcvAdjustedPriceOnlyMonth6;
                        case DataTimeFrame::Year1:     return DataKind::OhlcvAdjustedPriceOnlyYear1;
                        default: break;
                    }
                    return DataKind::OhlcvAdjustedPriceOnlyAperiodic;
                }
                switch (timeFrame)
                {
                    case DataTimeFrame::Aperiodic: return DataKind::OhlcvPriceOnlyAperiodic;
                    case DataTimeFrame::Second1:   return DataKind::OhlcvPriceOnlySecond1;
                    case DataTimeFrame::Second2:   return DataKind::OhlcvPriceOnlySecond2;
                    case DataTimeFrame::Second3:   return DataKind::OhlcvPriceOnlySecond3;
                    case DataTimeFrame::Second4:   return DataKind::OhlcvPriceOnlySecond4;
                    case DataTimeFrame::Second5:   return DataKind::OhlcvPriceOnlySecond5;
                    case DataTimeFrame::Second6:   return DataKind::OhlcvPriceOnlySecond6;
                    case DataTimeFrame::Second10:  return DataKind::OhlcvPriceOnlySecond10;
                    case DataTimeFrame::Second12:  return DataKind::OhlcvPriceOnlySecond12;
                    case DataTimeFrame::Second15:  return DataKind::OhlcvPriceOnlySecond15;
                    case DataTimeFrame::Second20:  return DataKind::OhlcvPriceOnlySecond20;
                    case DataTimeFrame::Second30:  return DataKind::OhlcvPriceOnlySecond30;
                    case DataTimeFrame::Minute1:   return DataKind::OhlcvPriceOnlyMinute1;
                    case DataTimeFrame::Minute2:   return DataKind::OhlcvPriceOnlyMinute2;
                    case DataTimeFrame::Minute3:   return DataKind::OhlcvPriceOnlyMinute3;
                    case DataTimeFrame::Minute4:   return DataKind::OhlcvPriceOnlyMinute4;
                    case DataTimeFrame::Minute5:   return DataKind::OhlcvPriceOnlyMinute5;
                    case DataTimeFrame::Minute6:   return DataKind::OhlcvPriceOnlyMinute6;
                    case DataTimeFrame::Minute10:  return DataKind::OhlcvPriceOnlyMinute10;
                    case DataTimeFrame::Minute12:  return DataKind::OhlcvPriceOnlyMinute12;
                    case DataTimeFrame::Minute15:  return DataKind::OhlcvPriceOnlyMinute15;
                    case DataTimeFrame::Minute20:  return DataKind::OhlcvPriceOnlyMinute20;
                    case DataTimeFrame::Minute30:  return DataKind::OhlcvPriceOnlyMinute30;
                    case DataTimeFrame::Hour1:     return DataKind::OhlcvPriceOnlyHour1;
                    case DataTimeFrame::Hour2:     return DataKind::OhlcvPriceOnlyHour2;
                    case DataTimeFrame::Hour3:     return DataKind::OhlcvPriceOnlyHour3;
                    case DataTimeFrame::Hour4:     return DataKind::OhlcvPriceOnlyHour4;
                    case DataTimeFrame::Hour6:     return DataKind::OhlcvPriceOnlyHour6;
                    case DataTimeFrame::Hour8:     return DataKind::OhlcvPriceOnlyHour8;
                    case DataTimeFrame::Hour12:    return DataKind::OhlcvPriceOnlyHour12;
                    case DataTimeFrame::Day1:      return DataKind::OhlcvPriceOnlyDay1;
                    case DataTimeFrame::Week1:     return DataKind::OhlcvPriceOnlyWeek1;
                    case DataTimeFrame::Week2:     return DataKind::OhlcvPriceOnlyWeek2;
                    case DataTimeFrame::Week3:     return DataKind::OhlcvPriceOnlyWeek3;
                    case DataTimeFrame::Month1:    return DataKind::OhlcvPriceOnlyMonth1;
                    case DataTimeFrame::Month2:    return DataKind::OhlcvPriceOnlyMonth2;
                    case DataTimeFrame::Month3:    return DataKind::OhlcvPriceOnlyMonth3;
                    case DataTimeFrame::Month4:    return DataKind::OhlcvPriceOnlyMonth4;
                    case DataTimeFrame::Month6:    return DataKind::OhlcvPriceOnlyMonth6;
                    case DataTimeFrame::Year1:     return DataKind::OhlcvPriceOnlyYear1;
                    default: break;
                }
                return DataKind::OhlcvPriceOnlyAperiodic;
            }
            case OhlcvKind::Ask:
            {
                switch (timeFrame)
                {
                    case DataTimeFrame::Aperiodic: return DataKind::OhlcvPriceOnlyAskAperiodic;
                    case DataTimeFrame::Second1:   return DataKind::OhlcvPriceOnlyAskSecond1;
                    case DataTimeFrame::Second2:   return DataKind::OhlcvPriceOnlyAskSecond2;
                    case DataTimeFrame::Second3:   return DataKind::OhlcvPriceOnlyAskSecond3;
                    case DataTimeFrame::Second4:   return DataKind::OhlcvPriceOnlyAskSecond4;
                    case DataTimeFrame::Second5:   return DataKind::OhlcvPriceOnlyAskSecond5;
                    case DataTimeFrame::Second6:   return DataKind::OhlcvPriceOnlyAskSecond6;
                    case DataTimeFrame::Second10:  return DataKind::OhlcvPriceOnlyAskSecond10;
                    case DataTimeFrame::Second12:  return DataKind::OhlcvPriceOnlyAskSecond12;
                    case DataTimeFrame::Second15:  return DataKind::OhlcvPriceOnlyAskSecond15;
                    case DataTimeFrame::Second20:  return DataKind::OhlcvPriceOnlyAskSecond20;
                    case DataTimeFrame::Second30:  return DataKind::OhlcvPriceOnlyAskSecond30;
                    case DataTimeFrame::Minute1:   return DataKind::OhlcvPriceOnlyAskMinute1;
                    case DataTimeFrame::Minute2:   return DataKind::OhlcvPriceOnlyAskMinute2;
                    case DataTimeFrame::Minute3:   return DataKind::OhlcvPriceOnlyAskMinute3;
                    case DataTimeFrame::Minute4:   return DataKind::OhlcvPriceOnlyAskMinute4;
                    case DataTimeFrame::Minute5:   return DataKind::OhlcvPriceOnlyAskMinute5;
                    case DataTimeFrame::Minute6:   return DataKind::OhlcvPriceOnlyAskMinute6;
                    case DataTimeFrame::Minute10:  return DataKind::OhlcvPriceOnlyAskMinute10;
                    case DataTimeFrame::Minute12:  return DataKind::OhlcvPriceOnlyAskMinute12;
                    case DataTimeFrame::Minute15:  return DataKind::OhlcvPriceOnlyAskMinute15;
                    case DataTimeFrame::Minute20:  return DataKind::OhlcvPriceOnlyAskMinute20;
                    case DataTimeFrame::Minute30:  return DataKind::OhlcvPriceOnlyAskMinute30;
                    case DataTimeFrame::Hour1:     return DataKind::OhlcvPriceOnlyAskHour1;
                    case DataTimeFrame::Hour2:     return DataKind::OhlcvPriceOnlyAskHour2;
                    case DataTimeFrame::Hour3:     return DataKind::OhlcvPriceOnlyAskHour3;
                    case DataTimeFrame::Hour4:     return DataKind::OhlcvPriceOnlyAskHour4;
                    case DataTimeFrame::Hour6:     return DataKind::OhlcvPriceOnlyAskHour6;
                    case DataTimeFrame::Hour8:     return DataKind::OhlcvPriceOnlyAskHour8;
                    case DataTimeFrame::Hour12:    return DataKind::OhlcvPriceOnlyAskHour12;
                    case DataTimeFrame::Day1:      return DataKind::OhlcvPriceOnlyAskDay1;
                    case DataTimeFrame::Week1:     return DataKind::OhlcvPriceOnlyAskWeek1;
                    case DataTimeFrame::Week2:     return DataKind::OhlcvPriceOnlyAskWeek2;
                    case DataTimeFrame::Week3:     return DataKind::OhlcvPriceOnlyAskWeek3;
                    case DataTimeFrame::Month1:    return DataKind::OhlcvPriceOnlyAskMonth1;
                    case DataTimeFrame::Month2:    return DataKind::OhlcvPriceOnlyAskMonth2;
                    case DataTimeFrame::Month3:    return DataKind::OhlcvPriceOnlyAskMonth3;
                    case DataTimeFrame::Month4:    return DataKind::OhlcvPriceOnlyAskMonth4;
                    case DataTimeFrame::Month6:    return DataKind::OhlcvPriceOnlyAskMonth6;
                    case DataTimeFrame::Year1:     return DataKind::OhlcvPriceOnlyAskYear1;
                    default: break;
                }
                return DataKind::OhlcvPriceOnlyAskAperiodic;
            }
            case OhlcvKind::Bid:
            {
                switch (timeFrame)
                {
                    case DataTimeFrame::Aperiodic: return DataKind::OhlcvPriceOnlyBidAperiodic;
                    case DataTimeFrame::Second1:   return DataKind::OhlcvPriceOnlyBidSecond1;
                    case DataTimeFrame::Second2:   return DataKind::OhlcvPriceOnlyBidSecond2;
                    case DataTimeFrame::Second3:   return DataKind::OhlcvPriceOnlyBidSecond3;
                    case DataTimeFrame::Second4:   return DataKind::OhlcvPriceOnlyBidSecond4;
                    case DataTimeFrame::Second5:   return DataKind::OhlcvPriceOnlyBidSecond5;
                    case DataTimeFrame::Second6:   return DataKind::OhlcvPriceOnlyBidSecond6;
                    case DataTimeFrame::Second10:  return DataKind::OhlcvPriceOnlyBidSecond10;
                    case DataTimeFrame::Second12:  return DataKind::OhlcvPriceOnlyBidSecond12;
                    case DataTimeFrame::Second15:  return DataKind::OhlcvPriceOnlyBidSecond15;
                    case DataTimeFrame::Second20:  return DataKind::OhlcvPriceOnlyBidSecond20;
                    case DataTimeFrame::Second30:  return DataKind::OhlcvPriceOnlyBidSecond30;
                    case DataTimeFrame::Minute1:   return DataKind::OhlcvPriceOnlyBidMinute1;
                    case DataTimeFrame::Minute2:   return DataKind::OhlcvPriceOnlyBidMinute2;
                    case DataTimeFrame::Minute3:   return DataKind::OhlcvPriceOnlyBidMinute3;
                    case DataTimeFrame::Minute4:   return DataKind::OhlcvPriceOnlyBidMinute4;
                    case DataTimeFrame::Minute5:   return DataKind::OhlcvPriceOnlyBidMinute5;
                    case DataTimeFrame::Minute6:   return DataKind::OhlcvPriceOnlyBidMinute6;
                    case DataTimeFrame::Minute10:  return DataKind::OhlcvPriceOnlyBidMinute10;
                    case DataTimeFrame::Minute12:  return DataKind::OhlcvPriceOnlyBidMinute12;
                    case DataTimeFrame::Minute15:  return DataKind::OhlcvPriceOnlyBidMinute15;
                    case DataTimeFrame::Minute20:  return DataKind::OhlcvPriceOnlyBidMinute20;
                    case DataTimeFrame::Minute30:  return DataKind::OhlcvPriceOnlyBidMinute30;
                    case DataTimeFrame::Hour1:     return DataKind::OhlcvPriceOnlyBidHour1;
                    case DataTimeFrame::Hour2:     return DataKind::OhlcvPriceOnlyBidHour2;
                    case DataTimeFrame::Hour3:     return DataKind::OhlcvPriceOnlyBidHour3;
                    case DataTimeFrame::Hour4:     return DataKind::OhlcvPriceOnlyBidHour4;
                    case DataTimeFrame::Hour6:     return DataKind::OhlcvPriceOnlyBidHour6;
                    case DataTimeFrame::Hour8:     return DataKind::OhlcvPriceOnlyBidHour8;
                    case DataTimeFrame::Hour12:    return DataKind::OhlcvPriceOnlyBidHour12;
                    case DataTimeFrame::Day1:      return DataKind::OhlcvPriceOnlyBidDay1;
                    case DataTimeFrame::Week1:     return DataKind::OhlcvPriceOnlyBidWeek1;
                    case DataTimeFrame::Week2:     return DataKind::OhlcvPriceOnlyBidWeek2;
                    case DataTimeFrame::Week3:     return DataKind::OhlcvPriceOnlyBidWeek3;
                    case DataTimeFrame::Month1:    return DataKind::OhlcvPriceOnlyBidMonth1;
                    case DataTimeFrame::Month2:    return DataKind::OhlcvPriceOnlyBidMonth2;
                    case DataTimeFrame::Month3:    return DataKind::OhlcvPriceOnlyBidMonth3;
                    case DataTimeFrame::Month4:    return DataKind::OhlcvPriceOnlyBidMonth4;
                    case DataTimeFrame::Month6:    return DataKind::OhlcvPriceOnlyBidMonth6;
                    case DataTimeFrame::Year1:     return DataKind::OhlcvPriceOnlyBidYear1;
                    default: break;
                }
                return DataKind::OhlcvPriceOnlyBidAperiodic;
            }
            default: break;
        }
        return DataKind::OhlcvPriceOnlyAperiodic;
    }

    static DataKind scalarKind2DataKind(const ScalarKind kind, const DataTimeFrame timeFrame)
    {
        switch (kind)
        {
            case ScalarKind::Default:
            {
                switch (timeFrame)
                {
                    case DataTimeFrame::Aperiodic: return DataKind::ScalarAperiodic;
                    case DataTimeFrame::Second1:   return DataKind::ScalarSecond1;
                    case DataTimeFrame::Second2:   return DataKind::ScalarSecond2;
                    case DataTimeFrame::Second3:   return DataKind::ScalarSecond3;
                    case DataTimeFrame::Second4:   return DataKind::ScalarSecond4;
                    case DataTimeFrame::Second5:   return DataKind::ScalarSecond5;
                    case DataTimeFrame::Second6:   return DataKind::ScalarSecond6;
                    case DataTimeFrame::Second10:  return DataKind::ScalarSecond10;
                    case DataTimeFrame::Second12:  return DataKind::ScalarSecond12;
                    case DataTimeFrame::Second15:  return DataKind::ScalarSecond15;
                    case DataTimeFrame::Second20:  return DataKind::ScalarSecond20;
                    case DataTimeFrame::Second30:  return DataKind::ScalarSecond30;
                    case DataTimeFrame::Minute1:   return DataKind::ScalarMinute1;
                    case DataTimeFrame::Minute2:   return DataKind::ScalarMinute2;
                    case DataTimeFrame::Minute3:   return DataKind::ScalarMinute3;
                    case DataTimeFrame::Minute4:   return DataKind::ScalarMinute4;
                    case DataTimeFrame::Minute5:   return DataKind::ScalarMinute5;
                    case DataTimeFrame::Minute6:   return DataKind::ScalarMinute6;
                    case DataTimeFrame::Minute10:  return DataKind::ScalarMinute10;
                    case DataTimeFrame::Minute12:  return DataKind::ScalarMinute12;
                    case DataTimeFrame::Minute15:  return DataKind::ScalarMinute15;
                    case DataTimeFrame::Minute20:  return DataKind::ScalarMinute20;
                    case DataTimeFrame::Minute30:  return DataKind::ScalarMinute30;
                    case DataTimeFrame::Hour1:     return DataKind::ScalarHour1;
                    case DataTimeFrame::Hour2:     return DataKind::ScalarHour2;
                    case DataTimeFrame::Hour3:     return DataKind::ScalarHour3;
                    case DataTimeFrame::Hour4:     return DataKind::ScalarHour4;
                    case DataTimeFrame::Hour6:     return DataKind::ScalarHour6;
                    case DataTimeFrame::Hour8:     return DataKind::ScalarHour8;
                    case DataTimeFrame::Hour12:    return DataKind::ScalarHour12;
                    case DataTimeFrame::Day1:      return DataKind::ScalarDay1;
                    case DataTimeFrame::Week1:     return DataKind::ScalarWeek1;
                    case DataTimeFrame::Week2:     return DataKind::ScalarWeek2;
                    case DataTimeFrame::Week3:     return DataKind::ScalarWeek3;
                    case DataTimeFrame::Month1:    return DataKind::ScalarMonth1;
                    case DataTimeFrame::Month2:    return DataKind::ScalarMonth2;
                    case DataTimeFrame::Month3:    return DataKind::ScalarMonth3;
                    case DataTimeFrame::Month4:    return DataKind::ScalarMonth4;
                    case DataTimeFrame::Month6:    return DataKind::ScalarMonth6;
                    case DataTimeFrame::Year1:     return DataKind::ScalarYear1;
                    default: break;
                }
                return DataKind::ScalarAperiodic;
            }
            default: break;
        }
        return DataKind::ScalarAperiodic;
    }

    static bool countItems(const hid_t datasetId, hsize_t& itemCount)
    {
        // Get the dataspace handle.
        const hid_t space_id = H5Dget_space(datasetId);
        if (0 > space_id)
            return false;

        // Get the original dimension.
        hsize_t dims[1];
        if (0 > H5Sget_simple_extent_dims(space_id, dims, nullptr))
        {
            H5Sclose(space_id);
            return false;
        }

        // Terminate an access to the dataspace.
        if (0 > H5Sclose(space_id))
            return false;
        itemCount = dims[0];
        return true;
    }

    static bool closeSpace(hid_t space_id)
    {
        H5Sclose(space_id);
        return false;
    }

    static bool readItem(const hid_t datasetId, const hid_t nativeTypeId, const hsize_t itemOffset, void* itemBuffer)
    {
        // Get the dataspace handle.
        const hid_t space_id = H5Dget_space(datasetId);
        if (0 > space_id)
            return false;

        // Define a hyperslab in the dataset of the size of the records.
        hsize_t counts[1], offsets[1];
        counts[0] = 1;
        offsets[0] = itemOffset;
        if (0 > H5Sselect_hyperslab(space_id, H5S_SELECT_SET, offsets, nullptr, counts, nullptr))
            return closeSpace(space_id);

        // Create a memory dataspace handle.
        const hid_t memorySpace_id = H5Screate_simple(1, counts, nullptr);
        if (0 > memorySpace_id)
            return closeSpace(space_id);

        // Read the item.
        if (0 > H5Dread(datasetId, nativeTypeId, memorySpace_id, space_id, H5P_DEFAULT, itemBuffer))
        {
            H5Sclose(memorySpace_id);
            return closeSpace(space_id);
        }

        // Terminate an access to the dataspace.
        if (0 > H5Sclose(memorySpace_id))
            return closeSpace(space_id);
        return 0 <= H5Sclose(space_id);
    }

    static bool readItemRange(const hid_t datasetId, const hid_t nativeTypeId, const hsize_t itemOffset, const hsize_t itemCount, void* itemBuffer)
    {
        // Get the dataspace handle.
        const hid_t space_id = H5Dget_space(datasetId);
        if (0 > space_id)
            return false;

        // Define a hyperslab in the dataset of the size of the records.
        hsize_t counts[1], offsets[1];
        counts[0] = itemCount;
        offsets[0] = itemOffset;
        if (0 > H5Sselect_hyperslab(space_id, H5S_SELECT_SET, offsets, nullptr, counts, nullptr))
            return closeSpace(space_id);

        // Create a memory dataspace handle.
        const hid_t memorySpace_id = H5Screate_simple(1, counts, nullptr);
        if (0 > memorySpace_id)
            return closeSpace(space_id);

        // Read the item.
        if (0 > H5Dread(datasetId, nativeTypeId, memorySpace_id, space_id, H5P_DEFAULT, itemBuffer))
        {
            H5Sclose(memorySpace_id);
            return closeSpace(space_id);
        }

        // Terminate an access to the dataspace.
        if (0 > H5Sclose(memorySpace_id))
            return closeSpace(space_id);
        return 0 <= H5Sclose(space_id);
    }

    static bool writeItem(const hid_t datasetId, const hid_t nativeTypeId, const hsize_t itemOffset, const void* itemBuffer)
    {
        // Get the dataspace handle.
        const hid_t space_id = H5Dget_space(datasetId);
        if (0 > space_id)
            return false;

        // Define a hyperslab in the dataset of the size of the records.
        hsize_t counts[1], offsets[1];
        counts[0] = 1;
        offsets[0] = itemOffset;
        if (0 > H5Sselect_hyperslab(space_id, H5S_SELECT_SET, offsets, nullptr, counts, nullptr))
            return closeSpace(space_id);

        // Create a memory dataspace handle.
        const hid_t memorySpace_id = H5Screate_simple(1, counts, nullptr);
        if (0 > memorySpace_id)
            return closeSpace(space_id);

        // Write the record.
        if (0 > H5Dwrite(datasetId, nativeTypeId, memorySpace_id, space_id, H5P_DEFAULT, itemBuffer))
        {
            H5Sclose(memorySpace_id);
            return closeSpace(space_id);
        }

        // Terminate an access to the dataspace.
        if (0 > H5Sclose(space_id))
        {
            H5Sclose(memorySpace_id);
            return false;
        }
        return 0 <= H5Sclose(memorySpace_id);
    }

    static bool closeSpace2(hid_t space1_id, hid_t space2_id)
    {
        H5Sclose(space1_id);
        H5Sclose(space2_id);
        return false;
    }

    static bool appendItems(const hid_t datasetId, const hid_t nativeTypeId, const hsize_t itemCount, const void* itemBuffer)
    {
        // Get the dataspace handle.
        hid_t space_id = H5Dget_space(datasetId);
        if (0 > space_id)
            return false;

        // Get the original dimension.
        hsize_t dims[1];
        if (0 > H5Sget_simple_extent_dims(space_id, dims, nullptr))
        {
            H5Sclose(space_id);
            return false;
        }

        // Terminate an access to the dataspace.
        if (0 > H5Sclose(space_id))
            return false;

        // Extend the dataset.
        hsize_t counts[1];
        counts[0] = dims[0] + itemCount;
        if (0 > H5Dset_extent(datasetId, counts))
            return false;

        // Create a simple memory dataspace.
        counts[0] = itemCount;
        const hid_t memorySpace_id = H5Screate_simple(1, counts, nullptr);
        if (0 > memorySpace_id)
            return false;

        // Get a copy of the new file data space for writing.
        space_id = H5Dget_space(datasetId);
        if (0 > space_id)
            return closeSpace(memorySpace_id);

        // Define a hyperslab in the dataset.
        if (0 > H5Sselect_hyperslab(space_id, H5S_SELECT_SET, dims, nullptr, counts, nullptr))
            return closeSpace2(space_id, memorySpace_id);

        // Write the items.
        if (0 > H5Dwrite(datasetId, nativeTypeId, memorySpace_id, space_id, H5P_DEFAULT, itemBuffer))
            return closeSpace2(space_id, memorySpace_id);

        // Terminate an access to the dataspace.
        if (0 > H5Sclose(space_id))
            return closeSpace(memorySpace_id);
        return 0 <= H5Sclose(memorySpace_id);
    }

    static bool deallocate(void* ptr)
    {
        free(ptr);
        return false;
    }

    static bool closeSpaceAndDeallocate(hid_t memorySpace_id, void* ptr)
    {
        H5Sclose(memorySpace_id);
        return deallocate(ptr);
    }

    static bool closeSpace2AndDeallocate(hid_t space_id, hid_t memorySpace_id, void* ptr)
    {
        H5Sclose(space_id);
        return closeSpaceAndDeallocate(memorySpace_id, ptr);
    }

    static bool prependItems(const hid_t datasetId, const hid_t nativeTypeId, const hsize_t itemCount, const void* itemBuffer, const size_t itemSize)
    {
        // Get the dataspace handle.
        hid_t space_id = H5Dget_space(datasetId);
        if (0 > space_id)
            return false;

        // Get the original dimension.
        hsize_t dims[1];
        if (0 > H5Sget_simple_extent_dims(space_id, dims, nullptr))
            return closeSpace(space_id);

        // Terminate an access to the dataspace.
        if (0 > H5Sclose(space_id))
            return false;

        // Allocate memory block for stored data.
        void* ptr{ static_cast<void*>(calloc(size_t(dims[0]), itemSize)) };
        if (nullptr == ptr)
            return false;

        // Create a simple memory dataspace.
        hsize_t counts[1], offsets[1];
        //counts[0] = dims[0];
        hid_t memorySpace_id = H5Screate_simple(1, dims, nullptr);
        if (0 > memorySpace_id)
            return deallocate(ptr);

        // Get a copy of the new file data space for reading.
        space_id = H5Dget_space(datasetId);
        if (0 > space_id)
            return closeSpaceAndDeallocate(memorySpace_id, ptr);

        // Define a hyperslab in the dataset.
        offsets[0] = 0;
        if (0 > H5Sselect_hyperslab(space_id, H5S_SELECT_SET, offsets, nullptr, dims, nullptr))
            return closeSpace2AndDeallocate(space_id, memorySpace_id, ptr);

        // Read in the stored items.
        if (0 > H5Dread(datasetId, nativeTypeId, memorySpace_id, space_id, H5P_DEFAULT, ptr))
            return closeSpace2AndDeallocate(space_id, memorySpace_id, ptr);

        // Terminate an access to the dataspace.
        if (0 > H5Sclose(space_id))
            return closeSpaceAndDeallocate(memorySpace_id, ptr);

        if (0 > H5Sclose(memorySpace_id))
            return deallocate(ptr);

        // Extend the dataset.
        counts[0] = dims[0] + itemCount;
        if (0 > H5Dset_extent(datasetId, counts))
            return deallocate(ptr);

        // Create a simple memory dataspace.
        counts[0] = itemCount;
        memorySpace_id = H5Screate_simple(1, counts, nullptr);
        if (0 > memorySpace_id)
            return deallocate(ptr);

        // Get a copy of the new file data space for writing.
        space_id = H5Dget_space(datasetId);
        if (0 > space_id)
            return closeSpaceAndDeallocate(memorySpace_id, ptr);

        // Define a hyperslab in the dataset.
        if (0 > H5Sselect_hyperslab(space_id, H5S_SELECT_SET, offsets, nullptr, counts, nullptr))
            return closeSpaceAndDeallocate(memorySpace_id, ptr);

        // Write the new items.
        if (0 > H5Dwrite(datasetId, nativeTypeId, memorySpace_id, space_id, H5P_DEFAULT, itemBuffer))
            return closeSpaceAndDeallocate(memorySpace_id, ptr);

        // Terminate access to the memory dataspace.
        if (0 > H5Sclose(memorySpace_id))
            return closeSpaceAndDeallocate(space_id, ptr);

        // Create a simple memory dataspace.
        memorySpace_id = H5Screate_simple(1, dims, nullptr);
        if (0 > memorySpace_id)
            return closeSpaceAndDeallocate(space_id, ptr);

        // Define a hyperslab in the dataset.
        offsets[0] = itemCount;
        if (0 > H5Sselect_hyperslab(space_id, H5S_SELECT_SET, offsets, nullptr, dims, nullptr))
            return closeSpace2AndDeallocate(space_id, memorySpace_id, ptr);

        // Write the stored items.
        if (0 > H5Dwrite(datasetId, nativeTypeId, memorySpace_id, space_id, H5P_DEFAULT, ptr))
            return closeSpace2AndDeallocate(space_id, memorySpace_id, ptr);

        if (0 > H5Sclose(space_id))
            return closeSpaceAndDeallocate(memorySpace_id, ptr);
        if (0 > H5Sclose(memorySpace_id))
            return deallocate(ptr);
        free(ptr);
        return true;
    }

    static bool insertItems(const hid_t datasetId, const hid_t nativeTypeId, const hsize_t offset, const hsize_t itemCount, const void* itemBuffer, const size_t itemSize)
    {
        // Get the dataspace handle.
        hid_t space_id = H5Dget_space(datasetId);
        if (0 > space_id)
            return false;

        // Get the original dimension.
        hsize_t dims[1];
        if (0 > H5Sget_simple_extent_dims(space_id, dims, nullptr))
            return closeSpace(space_id);

        // Terminate an access to the dataspace.
        if (0 > H5Sclose(space_id))
            return false;

        // Allocate memory block for stored data.
        hsize_t counts[1];
        counts[0] = dims[0] - offset;
        const auto ptr = static_cast<void*>(calloc(size_t(counts[0]), itemSize));
        if (nullptr == ptr)
            return false;

        // Create a simple memory dataspace.
        hid_t memorySpace_id = H5Screate_simple(1, counts, nullptr);
        if (0 > memorySpace_id)
            return deallocate(ptr);

        // Get a copy of the new file data space for reading.
        space_id = H5Dget_space(datasetId);
        if (0 > space_id)
            return closeSpaceAndDeallocate(memorySpace_id, ptr);

        // Define a hyperslab in the dataset.
        hsize_t offsets[1];
        offsets[0] = offset;
        if (0 > H5Sselect_hyperslab(space_id, H5S_SELECT_SET, offsets, nullptr, counts, nullptr))
            return closeSpace2AndDeallocate(space_id, memorySpace_id, ptr);

        // Read in the stored items.
        if (0 > H5Dread(datasetId, nativeTypeId, memorySpace_id, space_id, H5P_DEFAULT, ptr))
            return closeSpace2AndDeallocate(space_id, memorySpace_id, ptr);

        // Terminate an access to the dataspace.
        if (0 > H5Sclose(space_id))
            return closeSpaceAndDeallocate(memorySpace_id, ptr);
        if (0 > H5Sclose(memorySpace_id))
            return deallocate(ptr);

        // Extend the dataset.
        counts[0] = dims[0] + itemCount;
        if (0 > H5Dset_extent(datasetId, counts))
            return deallocate(ptr);

        // Create a simple memory dataspace.
        counts[0] = itemCount;
        memorySpace_id = H5Screate_simple(1, counts, nullptr);
        if (0 > memorySpace_id)
            return deallocate(ptr);

        // Get a copy of the new file data space for writing.
        space_id = H5Dget_space(datasetId);
        if (0 > space_id)
            return closeSpaceAndDeallocate(memorySpace_id, ptr);

        // Define a hyperslab in the dataset.
        if (0 > H5Sselect_hyperslab(space_id, H5S_SELECT_SET, offsets, nullptr, counts, nullptr))
            return closeSpaceAndDeallocate(memorySpace_id, ptr);

        // Write the new items.
        if (0 > H5Dwrite(datasetId, nativeTypeId, memorySpace_id, space_id, H5P_DEFAULT, itemBuffer))
            return closeSpaceAndDeallocate(memorySpace_id, ptr);

        // Terminate an access to the memory dataspace.
        if (0 > H5Sclose(memorySpace_id))
            return closeSpaceAndDeallocate(space_id, ptr);

        // Create a simple memory dataspace.
        counts[0] = dims[0] - offset;
        memorySpace_id = H5Screate_simple(1, counts, nullptr);
        if (0 > memorySpace_id)
            return closeSpaceAndDeallocate(space_id, ptr);

        // Define a hyperslab in the dataset.
        offsets[0] += itemCount;
        if (0 > H5Sselect_hyperslab(space_id, H5S_SELECT_SET, offsets, nullptr, counts, nullptr))
            return closeSpace2AndDeallocate(space_id, memorySpace_id, ptr);

        // Write the stored items.
        if (0 > H5Dwrite(datasetId, nativeTypeId, memorySpace_id, space_id, H5P_DEFAULT, ptr))
            return closeSpace2AndDeallocate(space_id, memorySpace_id, ptr);

        if (0 > H5Sclose(space_id))
            return closeSpaceAndDeallocate(memorySpace_id, ptr);
        if (0 > H5Sclose(memorySpace_id))
            return deallocate(ptr);
        free(ptr);
        return true;
    }

#ifdef _why_they_are_not_used_
    static bool deleteItems(const hid_t datasetId, hid_t nativeTypeId, const hsize_t offset)
    {
        // Get the dataspace handle.
        const hid_t space_id = H5Dget_space(datasetId);
        if (0 > space_id)
            return false;

        // Get the original dimension.
        hsize_t dims[1];
        if (0 > H5Sget_simple_extent_dims(space_id, dims, nullptr))
            return closeSpace(space_id);

        // Terminate an access to the dataspace.
        if (0 > H5Sclose(space_id))
            return false;

        // Check if we have something to delete.
        if (offset >= dims[0])
            return true;

        hsize_t counts[1];
        counts[0] = offset;
        // Truncate the dataset.
        return 0 <= H5Dset_extent(datasetId, counts);
    }
#endif

    static bool deleteItems(const hid_t datasetId, const hid_t nativeTypeId, const hsize_t offset, const hsize_t itemCount, const size_t itemSize)
    {
        // Get the dataspace handle.
        hid_t space_id = H5Dget_space(datasetId);
        if (0 > space_id)
            return false;

        // Get the original dimension.
        hsize_t dims[1];
        if (0 > H5Sget_simple_extent_dims(space_id, dims, nullptr))
        {
            H5Sclose(space_id);
            return false;
        }

        // Terminate an access to the dataspace.
        if (0 > H5Sclose(space_id))
            return false;

        // Check if we need to delete.
        if (offset >= dims[0])
            return true;

        // Allocate memory block for stored data after the deletion range.
        hsize_t counts[1];
        counts[0] = dims[0] - offset - itemCount;
        if (counts[0] > 0)
        {
            void* ptr{ static_cast<void*>(calloc(size_t(counts[0]), itemSize)) };
            if (nullptr == ptr)
                return false;

            // Create a simple memory dataspace.
            const hid_t memorySpace_id = H5Screate_simple(1, counts, nullptr);
            if (0 > memorySpace_id)
                return deallocate(ptr);

            // Get a copy of the new file data space for reading.
            space_id = H5Dget_space(datasetId);
            if (0 > space_id)
                return closeSpaceAndDeallocate(memorySpace_id, ptr);

            // Define a hyperslab in the dataset.
            hsize_t offsets[1];
            offsets[0] = offset + itemCount;
            if (0 > H5Sselect_hyperslab(space_id, H5S_SELECT_SET, offsets, nullptr, counts, nullptr))
                return closeSpace2AndDeallocate(space_id, memorySpace_id, ptr);

            // Read in the stored items.
            if (0 > H5Dread(datasetId, nativeTypeId, memorySpace_id, space_id, H5P_DEFAULT, ptr))
                return closeSpace2AndDeallocate(space_id, memorySpace_id, ptr);

            // Define a hyperslab in the dataset.
            offsets[0] = offset;
            if (0 > H5Sselect_hyperslab(space_id, H5S_SELECT_SET, offsets, nullptr, counts, nullptr))
                return closeSpace2AndDeallocate(space_id, memorySpace_id, ptr);

            // Write the stored items.
            if (0 > H5Dwrite(datasetId, nativeTypeId, memorySpace_id, space_id, H5P_DEFAULT, ptr))
                return closeSpace2AndDeallocate(space_id, memorySpace_id, ptr);

            if (0 > H5Sclose(space_id))
                return closeSpaceAndDeallocate(memorySpace_id, ptr);
            if (0 > H5Sclose(memorySpace_id))
                return deallocate(ptr);
            free(ptr);

            // Extend the dataset.
            counts[0] = dims[0] - itemCount;
            if (0 > H5Dset_extent(datasetId, counts))
                return false;
        }
        else // Delete the rest of the items.
        {
            // Truncate the dataset.
            counts[0] = offset;
            if (0 > H5Dset_extent(datasetId, counts))
                return false;
        }
        return true;
    }

    template <typename T> static hsize_t lowerNeighborIndex(const T* source, const hsize_t length, const long long targetTicks, bool& match)
    {
        match = false;
        hsize_t indexLow = 0, indexHigh = length;
        while (indexLow < indexHigh)
        {
            hsize_t indexMiddle = indexLow + (indexHigh - indexLow) / 2;
            const long long ticks = (source + indexMiddle)->ticks_;
            if (targetTicks > ticks)
            {
                if (indexLow == indexMiddle)
                    break;
                indexLow = indexMiddle;
            }
            else if (targetTicks < ticks)
                indexHigh = indexMiddle;
            else // targetTicks == ticks
            {
                match = true;
                return indexMiddle;
            }
        }
        return indexLow;
    }

    template <typename T> static bool lowerNeighborIndex(const hid_t datasetId, const hid_t nativeTypeId, const long long targetTicks, hsize_t& index, bool& match)
    {
        match = false;
        index = 0;

        // Get the dataspace handle.
        const hid_t space_id = H5Dget_space(datasetId);
        if (0 > space_id)
            return false;

        // Get the original dimension.
        hsize_t counts[1];
        if (0 > H5Sget_simple_extent_dims(space_id, counts, nullptr))
            return closeSpace(space_id);
        hsize_t indexLow = 0, indexHigh = counts[0];
        T t{};

        // Create a memory dataspace handle.
        counts[0] = 1;
        const hid_t memorySpace_id = H5Screate_simple(1, counts, nullptr);
        if (0 > memorySpace_id)
            return closeSpace(space_id);

        hsize_t offsets[1];
        while (indexLow < indexHigh)
        {
            const hsize_t indexMiddle = indexLow + (indexHigh - indexLow) / 2;

            // Define a hyperslab in the dataset.
            offsets[0] = indexMiddle;
            if (0 > H5Sselect_hyperslab(space_id, H5S_SELECT_SET, offsets, nullptr, counts, nullptr))
                return closeSpace2(memorySpace_id, space_id);

            // Read the item.
            if (0 > H5Dread(datasetId, nativeTypeId, memorySpace_id, space_id, H5P_DEFAULT, static_cast<void*>(&t)))
                return closeSpace2(memorySpace_id, space_id);

            const long long ticks = t.ticks_;
            if (targetTicks > ticks)
            {
                if (indexLow == indexMiddle)
                    break;
                indexLow = indexMiddle;
            }
            else if (targetTicks < ticks)
                indexHigh = indexMiddle;
            else // targetTicks == ticks
            {
                match = true;
                index = indexMiddle;
                goto done; // NOLINT
            }
        }
        index = indexLow;
done:
        // Terminate an access to the dataspace.
        if (0 > H5Sclose(memorySpace_id))
            return closeSpace(space_id);
        return 0 <= H5Sclose(space_id);
    }

    template <typename T> static bool mergeItems(const hid_t datasetId, const hid_t nativeTypeId, hsize_t inputStart, const hsize_t inputCount, const T* inputBuffer, const DuplicateTimeTicks duplicateTimeTicks, const bool verbose)
    {
        hsize_t index;
        const hsize_t inputStop = inputStart + inputCount;
        bool match;
        T dataValue{};
        while (inputStart < inputStop)
        {
            T inputValue = *(inputBuffer + inputStart);
            if (!lowerNeighborIndex<T>(datasetId, nativeTypeId, inputValue.ticks_, index, match))
                return false;
            if (match)
            {
                if (!readItem(datasetId, nativeTypeId, index, static_cast<void*>(&dataValue)))
                    return false;
                if (inputValue.isDifferent(dataValue))
                {
                    if (DuplicateTimeTicks::Update == duplicateTimeTicks)
                    {
                        if (verbose)
                            traceDataDuplicateTicksUpdateInfo<T>(index, dataValue, inputStart, inputValue);
                        if (!writeItem(datasetId, nativeTypeId, index, static_cast<void*>(&inputValue)))
                            return traceDataDuplicateTicksUpdateError<T>(index, dataValue, inputStart, inputValue);
                    }
                    else if (DuplicateTimeTicks::Skip == duplicateTimeTicks)
                        traceDataDuplicateTicksSkipError<T>(index, dataValue, inputStart, inputValue);
                    else //if (DuplicateTimeTicks::Fail == duplicateTimeTicks)
                        return traceDataDuplicateTicksFailError<T>(index, dataValue, inputStart, inputValue);
                }
                ++inputStart;
            }
            else
            {
                ++index; // Upper neighbor.
                if (!readItem(datasetId, nativeTypeId, index, static_cast<void*>(&dataValue)))
                    return false;
                hsize_t mergeCount = 1;
                hsize_t mergeStart = inputStart;
                for (++inputStart; inputStart < inputStop; ++inputStart)
                {
                    if ((inputBuffer + inputStart)->ticks_ < dataValue.ticks_)
                        ++mergeCount;
                    else
                        break;
                }
                if (!insertItems(datasetId, nativeTypeId, index, mergeCount, static_cast<const void*>(inputBuffer + mergeStart), sizeof(T)))
                    return false;
            }
        }
        return true;
    }

    static bool closeGroup(hid_t id)
    {
        H5Gclose(id);
        return false;
    }
	
    static bool isEmptyGroup(const hid_t fileId, const char* groupPath)
    {
        const hid_t group_id = H5Gopen2(fileId, groupPath, H5P_DEFAULT);
        if (0 > group_id)
            return traceGroupOpenError(groupPath);
        H5G_info_t g_info;
        const herr_t status = H5Gget_info(group_id, &g_info);
        if (0 > status)
        {
            traceGroupInfoError(groupPath);
            return closeGroup(group_id);
        }
        if (0 > H5Gclose(group_id))
            return traceGroupCloseError(groupPath);
        return g_info.nlinks == 0;
    }

    // ----------------------------------------------------------------------
    // ObjectInfo
    // ----------------------------------------------------------------------

    ObjectInfo::ObjectInfo(const char* name, const GroupInfo* parent) :
        name_(name),
        path_(nullptr == parent ? name : parent->path()),
        parent_(parent)
    {
        if (nullptr != parent)
        {
            path_.append("/");
            path_.append(name);
        }
    }

    ObjectInfo::ObjectInfo() :
        name_("/"),
        path_(""),
        parent_(nullptr)
    {
    }

    // ----------------------------------------------------------------------
    // DataInfo
    // ----------------------------------------------------------------------

    DataInfo::DataInfo(const char* name, const GroupInfo* parent) :
        ObjectInfo(name, parent)
    {
        dataKind_ = DataKind::TradeAperiodic;
        ParsedDatasetName pdn;
        isValidName_ = parseDatasetName(name, pdn);
        dataType_ = pdn.dataType_;
        ohlcvKind_ = pdn.ohlcvKind_;
        scalarKind_ = pdn.scalarKind_;
        tradeKind_ = pdn.tradeKind_;
        quoteKind_ = pdn.quoteKind_;
        timeFrame_ = pdn.timeFrame_;
        if (isValidName_)
        {
            switch (dataType_)
            {
                case DataType::TradePriceOnly:
                    dataKind_ = DataKind::TradePriceOnlyAperiodic;
                    break;
                case DataType::Quote:
                    dataKind_ = DataKind::QuoteAperiodic;
                    break;
                case DataType::QuotePriceOnly:
                    dataKind_ = DataKind::QuotePriceOnlyAperiodic;
                    break;
                case DataType::Ohlcv:
                    dataKind_ = ohlcvKind2DataKind(ohlcvKind_, timeFrame_, false);
                    break;
                case DataType::OhlcvPriceOnly:
                    dataKind_ = ohlcvPriceOnlyKind2DataKind(ohlcvKind_, timeFrame_, false);
                    break;
                case DataType::Scalar:
                    dataKind_ = scalarKind2DataKind(scalarKind_, timeFrame_);
                    break;
                case DataType::OhlcvAdjusted:
                    dataKind_ = ohlcvKind2DataKind(ohlcvKind_, timeFrame_, true);
                    break;
                case DataType::OhlcvAdjustedPriceOnly:
                    dataKind_ = ohlcvPriceOnlyKind2DataKind(ohlcvKind_, timeFrame_, true);
                    break;
                case DataType::Trade:
                default:
                    break;
            }
        }
    }

    DataInfo::DataInfo(const DataInfo& other) :
        DataInfo(other.name(), other.parent_)
    {
    }

    DataInfo::DataInfo(const DataInfo&& other) noexcept :
        DataInfo(other.name(), other.parent_)
    {
    }

    int DataInfo::compare(const void* first, const void* second) noexcept
    {
        // ReSharper disable CppCStyleCast
        const auto p1 = *(const DataInfo**)first;
        const auto p2 = *(const DataInfo**)second;
        // ReSharper restore CppCStyleCast

        return strcmp(p1->path(), p2->path());
    }

    // ----------------------------------------------------------------------
    // GroupInfo
    // ----------------------------------------------------------------------

    GroupInfo::GroupInfo() :
        ObjectInfo()
    {
    }

    GroupInfo::GroupInfo(const char* name, const GroupInfo* parent) :
        ObjectInfo(name, parent)
    {
    }

    void GroupInfo::enumerateDatasets(const DataInfoEnumerator& enumerate) const
    {
        const DataInfo** ptr{ datasets_.data() };
        const size_t size = datasets_.size();
        for (size_t i = 0; i != size; ++i)
            enumerate(*ptr[i]);
    }

    void GroupInfo::enumerateGroups(const GroupInfoEnumerator& enumerate) const
    {
        const GroupInfo** ptr{ groups_.data() };
        const size_t size = groups_.size();
        for (size_t i = 0; i != size; ++i)
            enumerate(ptr[i]);
    }

    bool GroupInfo::iterateGroup(const hid_t groupOrFileId, const char* groupName, GroupInfo* parentGroupInfo)
    {
        const hid_t id = H5Gopen2(groupOrFileId, groupName, H5P_DEFAULT);
        if (0 > id)
            return traceGroupOpenError(groupName);

        H5G_info_t g_info;
        herr_t status = H5Gget_info(id, &g_info);
        if (0 > status)
        {
            traceGroupInfoError(groupName);
            return closeGroup(id);
        }

        const hsize_t numberOfLinks = g_info.nlinks;
        const int maxObjectNameLength = 1024;
        char name[maxObjectNameLength];
        H5O_info_t o_info;
        for (hsize_t index = 0; index < numberOfLinks; ++index)
        {
            status = herr_t(H5Lget_name_by_idx(id, ".", H5_INDEX_NAME, H5_ITER_NATIVE, index, &name[0], maxObjectNameLength, H5P_DEFAULT));
            if (0 > status)
            {
                traceGroupByIndexNameError(index, groupName);
                return closeGroup(id);
            }

            status = H5Oget_info_by_idx(id, ".", H5_INDEX_NAME, H5_ITER_NATIVE, index, &o_info, H5P_DEFAULT);
            if (0 > status)
            {
                traceGroupByIndexObjectInfoError(index, groupName);
                return closeGroup(id);
            }

            switch (o_info.type)
            {
                case H5O_TYPE_GROUP:
                {
                    const auto ptr = new GroupInfo(name, parentGroupInfo);
                    parentGroupInfo->groups_.append(ptr);
                    if (!iterateGroup(id, name, ptr))
                        return closeGroup(id);
                    break;
                }
                case H5O_TYPE_DATASET:
                {
                    const auto ptr = new DataInfo(name, parentGroupInfo);
                    parentGroupInfo->datasets_.append(ptr);
                    break;
                }
                case H5O_TYPE_NAMED_DATATYPE:
                case H5O_TYPE_NTYPES:
                case H5O_TYPE_UNKNOWN:
                default: break;
            }
        }
        if (0 > H5Gclose(id))
            return traceGroupCloseError(groupName);
        return true;
    }

    void GroupInfo::moveDatasetsTo(DataInfoList* datasets)
    {
        size_t size = datasets_.size();
        if (size > 0)
        {
            const DataInfo** ptr{ datasets_.data() };
            for (size_t i = 0; i != size; ++i)
            {
                datasets->append(ptr[i]);
                ptr[i] = nullptr;
            }
            datasets_.size_ = 0;
        }

        size = groups_.size();
        if (size > 0)
        {
            GroupInfo** ptr{ const_cast<GroupInfo * *>(groups_.data()) };
            for (size_t i = 0; i != size; ++i)
                ptr[i]->moveDatasetsTo(datasets);
        }
    }

    void GroupInfo::enumerateDatasetsRecursive(const DataInfoEnumerator& enumerate) const
    {
        size_t size = datasets_.size();
        if (size > 0)
        {
            const DataInfo** ptr{ datasets_.data() };
            for (size_t i = 0; i != size; ++i)
                enumerate(*ptr[i]);
        }

        size = groups_.size();
        if (size > 0)
        {
            GroupInfo** ptr{ const_cast<GroupInfo * *>(groups_.data()) };
            for (size_t i = 0; i != size; ++i)
                ptr[i]->enumerateDatasetsRecursive(enumerate);
        }
    }

    int GroupInfo::compare(const void* first, const void* second) noexcept
    {
        // ReSharper disable CppCStyleCast
        const auto p1 = *(const GroupInfo**)first;
        const auto p2 = *(const GroupInfo**)second;
        // ReSharper restore CppCStyleCast

        return strcmp(p1->name(), p2->name());
    }

    void GroupInfo::sort() const
    {
        size_t size{ datasets_.size() };
        if (size > 1)
            std::qsort(static_cast<void*>(datasets_.data()), size, sizeof(DataInfo*), DataInfo::compare);
        size = groups_.size();
        if (size > 0)
        {
            const GroupInfo** ptr{ groups_.data() };
            if (size > 1)
                std::qsort(static_cast<void*>(ptr), size, sizeof(GroupInfo*), GroupInfo::compare);

            for (size_t i = 0; i != size; ++i)
                ptr[i]->sort();
        }
    }

    static std::mutex theMutex;

    // ----------------------------------------------------------------------
    // File
    // ----------------------------------------------------------------------

    std::shared_ptr<GroupInfo> File::contentTree(const bool sort) const
    {
        GroupInfo* root{ new GroupInfo() };
        {
            std::lock_guard<std::mutex> lock(theMutex);
            if (!root->iterateGroup(fileId_, ".", root))
                traceFileIterateError(filePath_);
        }
        if (sort)
            root->sort();
        return std::shared_ptr<GroupInfo>(root);
    }

    std::shared_ptr<GroupInfo> File::contentTree(const char* filePath, const FileDriver fileDriver, const bool sort)
    {
        GroupInfo* root{ new GroupInfo() };
        {
            std::lock_guard<std::mutex> lock(theMutex);
            const hid_t fileId{ fileOpen(filePath, false, false, fileDriver, false) };
            if (0 <= fileId)
            {
                if (!root->iterateGroup(fileId, ".", root))
                    traceFileIterateError(filePath);
                if (0 > H5Fclose(fileId))
                    traceFileCloseError(filePath);
            }
            else
                traceFileOpenReadOnlyError(filePath);
        }
        if (sort)
            root->sort();
        return std::shared_ptr<GroupInfo>(root);
    }

    bool File::enumerateDatasets(const DataInfoEnumerator& enumerate, bool sort) const
    {
        GroupInfo root{};
        {
            std::lock_guard<std::mutex> lock(theMutex);
            if (!root.iterateGroup(fileId_, ".", &root))
            {
                traceFileIterateError(filePath_);
                return false;
            }
        }
        if (sort)
        {
            DataInfoList* list{ new DataInfoList() };
            root.moveDatasetsTo(list);
            const size_t size{ list->size() };
            const DataInfo** const data = list->data();
            if (size > 1)
                std::qsort(static_cast<void*>(data), size, sizeof(DataInfo*), DataInfo::compare);
            for (size_t i = 0; i < size; ++i)
                enumerate(*data[i]);
            delete list;
        }
        else
        {
            root.enumerateDatasetsRecursive(enumerate);
        }
        return true;
    }

    bool File::enumerateDatasets(const DataInfoEnumerator& enumerate, const char* instrumentPath, const bool sort) const
    {
        GroupInfo root{};
        {
            std::lock_guard<std::mutex> lock(theMutex);
            if (!root.iterateGroup(fileId_, instrumentPath, &root))
            {
                traceFileIterateError(filePath_);
                return false;
            }
        }
        if (sort)
            root.sort();
        root.enumerateDatasets(enumerate);
        return true;
    }

    bool File::enumerateDatasets(const DataInfoEnumerator& enumerate, const char* filePath, FileDriver fileDriver, bool sort)
    {
        const std::shared_ptr<File> f = openReadOnly(filePath, fileDriver);
        if (!f.operator bool())
            return false;
        return f->enumerateDatasets(enumerate, sort);
    }

    bool File::enumerateDatasets(const DataInfoEnumerator& enumerate, const char* filePath, FileDriver fileDriver, const char* instrumentPath, bool sort)
    {
        const std::shared_ptr<File> f = openReadOnly(filePath, fileDriver);
        if (!f.operator bool())
            return false;
        return f->enumerateDatasets(enumerate, instrumentPath, sort);
    }

    static herr_t traceCallback(const unsigned number, const H5E_error2_t* errorDescription, void* clientData)
    {
        // ReSharper disable once CppCStyleCast
        const auto log = (ErrorLogger)clientData;
        if (nullptr != log)
        {
            const int bufferSize{ 512 };
            char buffer[bufferSize];
            std::string text{ "h5: HDF5 error #" };
            text.append(stringFromNumber(number, buffer, bufferSize));
            text.append(": class: ");
            if (0 > H5Eget_class_name(errorDescription->cls_id, buffer, bufferSize))
                return herr_t(-1);

            text.append(buffer);
            text.append(", major: ");
            if (0 > H5Eget_msg(errorDescription->maj_num, nullptr, buffer, bufferSize))
                return herr_t(-1);

            text.append(buffer);
            text.append(", minor: ");
            if (0 > H5Eget_msg(errorDescription->min_num, nullptr, buffer, bufferSize))
                return herr_t(-1);

            text.append(buffer);
            text.append(" (file:");
            text.append(errorDescription->file_name);
            text.append(", func:");
            text.append(errorDescription->func_name);
            text.append(", line:");
            text.append(stringFromNumber(errorDescription->line, buffer, bufferSize));
            text.append(")");
            log(text.c_str(), true);
        }
        return herr_t(0);
    }

    static herr_t autoTrace(const hid_t errorStackId, void* clientData)
    {
        return H5Ewalk2(errorStackId, H5E_WALK_UPWARD, traceCallback, clientData);
    }

    void File::interceptErrorStack(const ErrorLogger errorLogger)
    {
        errorLoggerFunc.store(errorLogger);
        // ReSharper disable CppCStyleCast
        if (0 > H5Eset_auto(H5E_DEFAULT, autoTrace, (void*)errorLogger))
            // ReSharper restore CppCStyleCast
            if (nullptr != errorLogger)
                errorLogger("h5: Error: H5Eset_auto() failed", true);
    }

    File::File(const char* filePath, const hid_t fileId, const bool readOnly) :
        filePath_{ filePath },
        fileId_{ fileId },
        readOnly_{ readOnly }
    {
    }

    File::~File() noexcept
    {
    	try
    	{
			close();
		}
    	catch(...)
    	{
    	}
    }

    void File::close()
    {
        std::lock_guard<std::mutex> lock(theMutex);
        if (0 <= fileId_)
        {
            if (!readOnly_)
                if (0 > H5Fflush(fileId_, H5F_SCOPE_LOCAL))
                    traceFileFlushError(filePath_);
            if (0 > H5Fclose(fileId_))
                traceFileCloseError(filePath_);
            fileId_ = -1;
        }
    }

    void File::flush() const
    {
        std::lock_guard<std::mutex> lock(theMutex);
        if (!readOnly_ && 0 <= fileId_)
            if (0 > H5Fflush(fileId_, H5F_SCOPE_LOCAL))
                traceFileFlushError(filePath_);
    }

    std::shared_ptr<File> File::openReadOnly(const char* filePath, const FileDriver fileDriver)
    {
        std::shared_ptr<File> repository(nullptr);
        const std::string path(filePath);
        std::lock_guard<std::mutex> lock(theMutex);
        hid_t fileId = fileOpen(path.c_str(), false, false, fileDriver, false);
        if (0 > fileId)
            traceFileOpenReadOnlyError(filePath);
        else
            repository = std::make_shared<File>(path.c_str(), fileId, true);
        return repository;
    }

    std::shared_ptr<File> File::openReadWrite(const char* filePath, const bool createNonexistent, const FileDriver fileDriver, const bool corkTheCache)
    {
        std::shared_ptr<File> repository(nullptr);
        const std::string path(filePath);
        std::lock_guard<std::mutex> lock(theMutex);
        hid_t fileId = fileOpen(path.c_str(), true, createNonexistent, fileDriver, corkTheCache);
        if (0 > fileId)
            traceFileOpenReadWriteError(filePath, createNonexistent);
        else
            repository = std::make_shared<File>(path.c_str(), fileId, false);
        return repository;
    }

    bool File::deleteObject(const char* objectPath, const bool deleteEmptyParentPath) const
    {
        std::lock_guard<std::mutex> lock(theMutex);
        herr_t err = H5Ldelete(fileId_, objectPath, H5P_DEFAULT);
        if (err < 0)
            return traceFailedToDeleteObjectError("an object", objectPath);
        if (deleteEmptyParentPath)
        {
            std::string path{ objectPath };
            while (true)
            {
                const std::string::size_type index = path.find_last_of('/');
                if (std::string::npos != index && index > 0)
                {
                    path = path.substr(0, index);
                    const char* groupPath = path.c_str();
                    if (isEmptyGroup(fileId_, groupPath))
                    {
                        err = H5Ldelete(fileId_, groupPath, H5P_DEFAULT);
                        if (err < 0)
                        {
                            traceFailedToDeleteObjectError("an empty group", groupPath);
                            break;
                        }
                        continue;
                    }
                }
                break;
            }
        }
        return true;
    }

    bool File::moveInstrument(const char* instrumentPathOld, const char* instrumentPathNew) const
    {
        std::lock_guard<std::mutex> lock(theMutex);
        // Create group creation property list and set it to allow creation of intermediate groups.
        const hid_t lcpl_id = H5Pcreate(H5P_LINK_CREATE);
        H5Pset_create_intermediate_group(lcpl_id, 1);
        const herr_t status = H5Lmove(fileId_, instrumentPathOld, fileId_, instrumentPathNew, lcpl_id, H5P_DEFAULT);
        H5Pclose(lcpl_id);
        if (0 > status)
            return traceFailedToMoveInstrumentError(instrumentPathOld, instrumentPathNew);
        return true;
    }

    bool File::renameInstrument(const char* instrumentPath, const char* instrumentNewName) const
    {
        std::string path{ instrumentPath };
        const auto index = path.find_last_of('/');
        if (std::string::npos == index || path.length() <= index)
            return traceFailedToExtractGroupNameError(instrumentPath);
        const std::string instrumentOldName = path.substr(index + 1);
        if (instrumentOldName.length() < 1)
            return traceFailedToExtractGroupNameError(instrumentPath);
        if (0 == index)
            path = "/";
        else
        {
            path = path.substr(0, index);
            const auto length = path.length();
            if (1 < length && path[length - 1] == '/')
                path = path.substr(0, length - 1);
        }

        std::lock_guard<std::mutex> lock(theMutex);
        const hid_t idPath = groupOpen(path.c_str(), fileId_, false, false);
        if (0 > idPath)
            return traceGroupOpenError(path.c_str());
        const herr_t status = H5Lmove(idPath, instrumentOldName.c_str(), idPath, instrumentNewName, H5P_DEFAULT, H5P_DEFAULT);
        H5Gclose(idPath);
        if (0 > status)
            return traceFailedToMoveInstrumentGroupError(path.c_str(), instrumentOldName.c_str(), instrumentNewName);
        return true;
    }

    std::shared_ptr<Instrument> File::openInstrument(const char* instrumentPath, const bool createNonexistent)
    {
        std::shared_ptr<Instrument> instrument(nullptr);
        std::lock_guard<std::mutex> lock(theMutex);
        hid_t groupId = groupOpen(instrumentPath, fileId_, !readOnly_, createNonexistent);
        if (0 > groupId)
            traceFileOpenInstrumentError(instrumentPath, filePath_);
        else
            instrument = std::make_shared<Instrument>(instrumentPath, groupId, readOnly_, filePath_);
        return instrument;
    }

    // ----------------------------------------------------------------------
    // Instrument
    // ----------------------------------------------------------------------

    Instrument::Instrument(const char* instrumentPath, const hid_t groupId, const bool readOnly, std::string filePath) :
        filePath_{std::move(filePath)},
        instrumentPath_{ instrumentPath },
        groupId_{ groupId },
        readOnly_{ readOnly }
    {
    }

    Instrument::~Instrument()
    {
		try
		{
			close();
		}
		catch (...)
		{
		}
	}

    void Instrument::close()
    {
        std::lock_guard<std::mutex> lock(theMutex);
        if (0 <= groupId_)
        {
            if (!readOnly_)
                if (0 > H5Gflush(groupId_))
                    traceInstrumentFlushError(instrumentPath_, filePath_);
            if (0 > H5Gclose(groupId_))
                traceInstrumentCloseError(instrumentPath_, filePath_);
            groupId_ = -1;
        }
    }

    void Instrument::flush() const
    {
        std::lock_guard<std::mutex> lock(theMutex);
        if (!readOnly_ && 0 <= groupId_)
            if (0 > H5Gflush(groupId_))
                traceInstrumentFlushError(instrumentPath_, filePath_);
    }

    int Instrument::compareDataset(const void* first, const void* second) noexcept
    {
		// ReSharper disable CppCStyleCast
		const auto p1 = (const DataInfo*)first;
		const auto p2 = (const DataInfo*)second;
		// ReSharper restore CppCStyleCast

		return strcmp(p1->name(), p2->name());
    }

    bool Instrument::enumerateDatasets(const DataInfoEnumerator& enumerate, const bool sort) const
    {
        bool status;
		GroupInfo root{};
		{
			std::lock_guard<std::mutex> lock(theMutex);
			status = root.iterateGroup(groupId_, instrumentPath_.c_str(), &root);
		}
    	if (status)
        {
	        if (sort)
		        root.sort();
	        root.enumerateDatasets(enumerate);
        }
    	else
			traceInstrumentDatasetEnumerationError(instrumentPath_, filePath_);

    	return status;
    }

    static void closeTypes(DatasetTraits& datasetTraits)
    {
        H5Tclose(datasetTraits.standardType_);
        H5Tclose(datasetTraits.nativeType_);
    }

    std::shared_ptr<QuoteData> Instrument::openQuoteData(const bool createNonexistent) const
    {
        std::shared_ptr<QuoteData> ptr;
        DatasetTraits datasetTraits;
        hid_t datasetId;
        {
            std::lock_guard<std::mutex> lock(theMutex);
            if (!dataKind2DatasetTraits(DataKind::QuoteAperiodic, datasetTraits))
                return ptr;
            datasetId = datasetOpen(datasetTraits.name_, groupId_, datasetTraits.standardType_, !readOnly_, createNonexistent);
            if (0 > datasetId)
            {
                closeTypes(datasetTraits);
                return ptr;
            }
        }
        ptr.reset(new QuoteData(this, datasetTraits.name_, datasetId, datasetTraits.nativeType_, datasetTraits.standardType_, readOnly_));
        return ptr;
    }

    std::shared_ptr<QuotePriceOnlyData> Instrument::openQuotePriceOnlyData(const bool createNonexistent) const
    {
        std::shared_ptr<QuotePriceOnlyData> ptr;
        DatasetTraits datasetTraits;
        hid_t datasetId;
        {
            std::lock_guard<std::mutex> lock(theMutex);
            if (!dataKind2DatasetTraits(DataKind::QuotePriceOnlyAperiodic, datasetTraits))
                return ptr;
            datasetId = datasetOpen(datasetTraits.name_, groupId_, datasetTraits.standardType_, !readOnly_, createNonexistent);
            if (0 > datasetId)
            {
                closeTypes(datasetTraits);
                return ptr;
            }
        }
        ptr.reset(new QuotePriceOnlyData(this, datasetTraits.name_, datasetId, datasetTraits.nativeType_, datasetTraits.standardType_, readOnly_));
        return ptr;
    }

    std::shared_ptr<TradeData> Instrument::openTradeData(const bool createNonexistent) const
    {
        std::shared_ptr<TradeData> ptr;
        DatasetTraits datasetTraits;
        hid_t datasetId;
        {
            std::lock_guard<std::mutex> lock(theMutex);
            if (!dataKind2DatasetTraits(DataKind::TradeAperiodic, datasetTraits))
                return ptr;
            datasetId = datasetOpen(datasetTraits.name_, groupId_, datasetTraits.standardType_, !readOnly_, createNonexistent);
            if (0 > datasetId)
            {
                closeTypes(datasetTraits);
                return ptr;
            }
        }
        ptr.reset(new TradeData(this, datasetTraits.name_, datasetId, datasetTraits.nativeType_, datasetTraits.standardType_, readOnly_));
        return ptr;
    }

    std::shared_ptr<TradePriceOnlyData> Instrument::openTradePriceOnlyData(const bool createNonexistent) const
    {
        std::shared_ptr<TradePriceOnlyData> ptr;
        DatasetTraits datasetTraits;
        hid_t datasetId;
        {
            std::lock_guard<std::mutex> lock(theMutex);
            if (!dataKind2DatasetTraits(DataKind::TradePriceOnlyAperiodic, datasetTraits))
                return ptr;
            datasetId = datasetOpen(datasetTraits.name_, groupId_, datasetTraits.standardType_, !readOnly_, createNonexistent);
            if (0 > datasetId)
            {
                closeTypes(datasetTraits);
                return ptr;
            }
        }
        ptr.reset(new TradePriceOnlyData(this, datasetTraits.name_, datasetId, datasetTraits.nativeType_, datasetTraits.standardType_, readOnly_));
        return ptr;
    }

    inline std::shared_ptr<OhlcvData> Instrument::openOhlcvData(const OhlcvKind kind, const DataTimeFrame timeFrame, const bool adjusted, const bool createNonexistent) const
    {
        std::shared_ptr<OhlcvData> ptr;
        DatasetTraits datasetTraits;
        hid_t datasetId;
        {
            std::lock_guard<std::mutex> lock(theMutex);
            const DataKind dataKind = ohlcvKind2DataKind(kind, timeFrame, adjusted);
            if (!dataKind2DatasetTraits(dataKind, datasetTraits))
                return ptr;
            datasetId = datasetOpen(datasetTraits.name_, groupId_, datasetTraits.standardType_, !readOnly_, createNonexistent);
            if (0 > datasetId)
            {
                closeTypes(datasetTraits);
                return ptr;
            }
        }
        ptr.reset(new OhlcvData(this, datasetTraits.name_, datasetId, datasetTraits.nativeType_, datasetTraits.standardType_, readOnly_));
        return ptr;
    }

    std::shared_ptr<OhlcvData> Instrument::openOhlcvData(const OhlcvKind kind, const DataTimeFrame timeFrame, const bool createNonexistent) const
    {
        return openOhlcvData(kind, timeFrame, false, createNonexistent);
    }

    std::shared_ptr<OhlcvData> Instrument::openOhlcvAdjustedData(const OhlcvKind kind, const DataTimeFrame timeFrame, const bool createNonexistent) const
    {
        return openOhlcvData(kind, timeFrame, true, createNonexistent);
    }

    inline std::shared_ptr<OhlcvPriceOnlyData> Instrument::openOhlcvPriceOnlyData(const OhlcvKind kind, const DataTimeFrame timeFrame, const bool adjusted, const bool createNonexistent) const
    {
        std::shared_ptr<OhlcvPriceOnlyData> ptr;
        DatasetTraits datasetTraits;
        hid_t datasetId;
        {
            std::lock_guard<std::mutex> lock(theMutex);
            const DataKind dataKind = ohlcvPriceOnlyKind2DataKind(kind, timeFrame, adjusted);
            if (!dataKind2DatasetTraits(dataKind, datasetTraits))
                return ptr;
            datasetId = datasetOpen(datasetTraits.name_, groupId_, datasetTraits.standardType_, !readOnly_, createNonexistent);
            if (0 > datasetId)
            {
                closeTypes(datasetTraits);
                return ptr;
            }
        }
        ptr.reset(new OhlcvPriceOnlyData(this, datasetTraits.name_, datasetId, datasetTraits.nativeType_, datasetTraits.standardType_, readOnly_));
        return ptr;
    }

    std::shared_ptr<OhlcvPriceOnlyData> Instrument::openOhlcvPriceOnlyData(const OhlcvKind kind, const DataTimeFrame timeFrame, const bool createNonexistent) const
    {
        return openOhlcvPriceOnlyData(kind, timeFrame, false, createNonexistent);
    }

    std::shared_ptr<OhlcvPriceOnlyData> Instrument::openOhlcvAdjustedPriceOnlyData(const OhlcvKind kind, const DataTimeFrame timeFrame, const bool createNonexistent) const
    {
        return openOhlcvPriceOnlyData(kind, timeFrame, true, createNonexistent);
    }

    std::shared_ptr<ScalarData> Instrument::openScalarData(const DataTimeFrame timeFrame, const bool createNonexistent) const
    {
        std::shared_ptr<ScalarData> ptr;
        DatasetTraits datasetTraits;
        hid_t datasetId;
        {
            std::lock_guard<std::mutex> lock(theMutex);
            const DataKind dataKind = scalarKind2DataKind(ScalarKind::Default, timeFrame);
            if (!dataKind2DatasetTraits(dataKind, datasetTraits))
                return ptr;
            datasetId = datasetOpen(datasetTraits.name_, groupId_, datasetTraits.standardType_, !readOnly_, createNonexistent);
            if (0 > datasetId)
            {
                closeTypes(datasetTraits);
                return ptr;
            }
        }
        ptr.reset(new ScalarData(this, datasetTraits.name_, datasetId, datasetTraits.nativeType_, datasetTraits.standardType_, readOnly_));
        return ptr;
    }

    // ----------------------------------------------------------------------
    // Data
    // ----------------------------------------------------------------------

    Data::Data(const Instrument* instrument, const char* name, const hid_t datasetId, const hid_t nativeTypeId, const hid_t standardTypeId, const bool readOnly) :
        maximumReadBufferBytes_{ defaultMaximumReadBufferBytes_ },
        filePath_(instrument->filePath_),
        datasetPath_(instrument->instrumentPath_),
        datasetId_{ datasetId },
        nativeTypeId_{ nativeTypeId },
        standardTypeId_{ standardTypeId },
        readOnly_{ readOnly }
    {
        datasetPath_.append("/");
        datasetPath_.append(name);
    }

    Data::~Data()
    {
		try
		{
			close();
		}
		catch (...)
		{
		}
	}

    hsize_t Data::defaultMaximumReadBufferBytes_{ 1024L * 1024L };

    void Data::close()
    {
        std::lock_guard<std::mutex> lock(theMutex);
        if (0 <= datasetId_)
        {
            if (!readOnly_)
                if (0 > H5Dflush(datasetId_))
                    traceDataFlushError(datasetPath_, filePath_);
            if (0 > H5Dclose(datasetId_))
                traceDataCloseError(datasetPath_, filePath_);
            datasetId_ = -1;
        }
        if (0 <= nativeTypeId_)
        {
            if (0 > H5Tclose(nativeTypeId_))
                traceDataCloseNativeTypeError(datasetPath_, filePath_);
            nativeTypeId_ = -1;
        }
        if (0 <= standardTypeId_)
        {
            if (0 > H5Tclose(standardTypeId_))
                traceDataCloseStandardTypeError(datasetPath_, filePath_);
            standardTypeId_ = -1;
        }
    }

    void Data::flush() const
    {
        std::lock_guard<std::mutex> lock(theMutex);
        if (!readOnly_ && 0 <= datasetId_)
            if (0 > H5Dflush(datasetId_))
                traceDataFlushError(datasetPath_, filePath_);
    }

    long long Data::firstTicks() const
    {
        long long ticks{0L};
        hsize_t dataCount;
        std::lock_guard<std::mutex> lock(theMutex);
        if (!countItems(datasetId_, dataCount))
            traceDataGetItemCountError(datasetPath_, filePath_);
        else if (0 < dataCount)
        {
            unsigned char buffer[256];
            if (!readItem(datasetId_, nativeTypeId_, 0, static_cast<void*>(buffer)))
                traceDataReadFirstItemError(datasetPath_, filePath_);
            else
                ticks = reinterpret_cast<Scalar*>(buffer)->ticks_;
        }
        return ticks;
    }

    long long Data::lastTicks() const
    {
        long long ticks{0L};
        hsize_t dataCount;
        std::lock_guard<std::mutex> lock(theMutex);
        if (!countItems(datasetId_, dataCount))
            traceDataGetItemCountError(datasetPath_, filePath_);
        else if (0 < dataCount)
        {
            unsigned char buffer[256];
            if (!readItem(datasetId_, nativeTypeId_, dataCount - 1, static_cast<void*>(buffer)))
                traceDataReadLastItemError(datasetPath_, filePath_);
            else
                ticks = *reinterpret_cast<long long*>(buffer);
        }
        return ticks;
    }

    hsize_t Data::count() const
    {
        hsize_t dataCount{0UL};
        std::lock_guard<std::mutex> lock(theMutex);
        if (!countItems(datasetId_, dataCount))
            traceDataGetItemCountError(datasetPath_, filePath_);
        return dataCount;
    }

    template <typename T> bool Data::spreadList(std::vector<T>& input, const bool verbose)
    {
        return spreadList(const_cast<T*>(input.data()), static_cast<size_t>(input.size()), verbose);
    }

    template <typename T> bool Data::spreadList(T* input, const size_t inputCount, const bool verbose)
    {
        size_t iStart = 0, iCount = 0, j;
        long long lStart = input[0].ticks_;
        bool spread = false;
        T t{};
        for (size_t i = 1; i < inputCount; ++i)
        {
            const long long l = input[i].ticks_;
            if (lStart == l)
                ++iCount;
            else
            {
                if (0 < iCount)
                {
                    if (verbose)
                    {
                        traceSpreadingDuplicateInputTimeTick<T>(iStart, input[iStart]);
                        for (j = iStart + 1; j < i; ++j)
                        {
                            traceSpreadingDuplicateInputTimeTick<T>(j, input[j], lStart + 1);
                            t = input[j];
                            t.ticks_ = ++lStart;
                            input[j] = t;
                        }
                    }
                    else
                    {
                        for (j = iStart + 1; j < i; ++j)
                        {
                            t = input[j];
                            t.ticks_ = ++lStart;
                            input[j] = t;
                        }
                    }
                    iCount = 0;
                    spread = true;
                }
                iStart = i;
                lStart = input[i].ticks_;
            }
        }
        if (0 < iCount)
        {
            iCount += iStart;
            if (verbose)
            {
                traceSpreadingDuplicateInputTimeTick<T>(iStart, input[iStart]);
                for (j = iStart + 1; j <= iCount; ++j)
                {
                    traceSpreadingDuplicateInputTimeTick<T>(j, input[j], lStart + 1);
                    t = input[j];
                    t.ticks_ = ++lStart;
                    input[j] = t;
                }
            }
            else
            {
                for (j = iStart + 1; j <= iCount; ++j)
                {
                    t = input[j];
                    t.ticks_ = ++lStart;
                    input[j] = t;
                }
            }
            spread = true;
        }
        return spread;
    }

    template <typename T> bool Data::deleteIndexRange(const hsize_t indexFrom, const hsize_t indexTo) const
    {
        if (readOnly_)
            return false;
        if (indexTo < indexFrom)
            return true;
        const hsize_t count = indexTo - indexFrom + 1;
        if (count > 0)
        {
            std::lock_guard<std::mutex> lock(theMutex);
            return deleteItems(datasetId_, nativeTypeId_, indexFrom, count, sizeof(T));
        }
        return true;
    }

    template <typename T> bool Data::deleteTickRange(const long long ticksFrom, const long long ticksTo) const
    {
        if (readOnly_)
            return false;
        if (ticksTo < ticksFrom)
            return true;
        hsize_t indexFrom, indexTo;
        bool matchFrom, matchTo;
        {
            std::lock_guard<std::mutex> lock(theMutex);
            if (!ticksToIndexNoLock<T>(ticksFrom, indexFrom, matchFrom) ||
                !ticksToIndexNoLock<T>(ticksTo, indexTo, matchTo))
                return false;
        }
        if (!matchFrom)
            ++indexFrom;
        const hsize_t count = indexTo - indexFrom + 1;
        if (count > 0)
        {
            std::lock_guard<std::mutex> lock(theMutex);
            return deleteItems(datasetId_, nativeTypeId_, indexFrom, count, sizeof(T));
        }
        return true;
    }

    template <typename T> bool Data::addData(const T* input, const hsize_t inputCount, DuplicateTimeTicks duplicateTimeTicks, bool verbose) const
    {
        if (readOnly_)
            return false;
        T inputFirst = input[0], inputLast = input[inputCount - 1];
        const long long inputFirstTicks = inputFirst.ticks_;
        const long long inputLastTicks = inputLast.ticks_;
        {
            long long l1 = inputFirstTicks;
            for (hsize_t i = 1; i < inputCount; ++i)
            {
                const long long l2 = input[i].ticks_;
                if (l1 >= l2)
                {
                    traceNonIncreasingInputTicksError<T>(input[i - 1], input[i]);
                    return false;
                }
                l1 = l2;
            }
        }
        std::lock_guard<std::mutex> lock(theMutex);
        hsize_t dataCount;
        if (!countItems(datasetId_, dataCount))
        {
            traceDataGetItemCountError(datasetPath_, filePath_);
            return false;
        }
        if (0 == dataCount) // Empty dataset
        {
            if (!appendItems(datasetId_, nativeTypeId_, inputCount, static_cast<void*>(const_cast<T*>(input))))
                return traceDataAppendFailError(inputCount, datasetPath_, filePath_);
            return true;
        }
        T dataFirst{}, dataLast{};
        if (!readItem(datasetId_, nativeTypeId_, 0, static_cast<void*>(&dataFirst)))
        {
            traceDataReadFirstItemError(datasetPath_, filePath_);
            return false;
        }
        if (!readItem(datasetId_, nativeTypeId_, dataCount - 1, static_cast<void*>(&dataLast)))
        {
            traceDataReadLastItemError(datasetPath_, filePath_);
            return false;
        }
        const long long dataFirstTicks = dataFirst.ticks_;
        const long long dataLastTicks = dataLast.ticks_;
        if (inputFirstTicks > dataLastTicks) //      dddddddd
        {                                    //               iiii
            if (!appendItems(datasetId_, nativeTypeId_, inputCount, static_cast<void*>(const_cast<T*>(input))))
                return traceDataAppendFailError(inputCount, datasetPath_, filePath_);
            return true;
        }
        if (inputFirstTicks == dataLastTicks) //      ddddddd[d]
        {                                     //             [i]iii
            if (inputFirst.isDifferent(dataLast))
            {
                const hsize_t count1 = dataCount - 1;
                if (DuplicateTimeTicks::Update == duplicateTimeTicks)
                {
                    if (verbose)
                        traceDataDuplicateTicksUpdateInfo<T>(count1, dataLast, 0, inputFirst);
                    if (!writeItem(datasetId_, nativeTypeId_, count1, static_cast<void*>(&inputFirst)))
                        return traceDataDuplicateTicksUpdateError<T>(count1, dataLast, 0, inputFirst);
                }
                else if (DuplicateTimeTicks::Skip == duplicateTimeTicks)
                    traceDataDuplicateTicksSkipError<T>(count1, dataLast, 0, inputFirst);
                else //if (DuplicateTimeTicks::Fail == duplicateTimeTicks)
                    return traceDataDuplicateTicksFailError<T>(count1, dataLast, 0, inputFirst);
            }
            if (1 < inputCount)
            {
                if (!appendItems(datasetId_, nativeTypeId_, inputCount - 1, static_cast<void*>(const_cast<T*>(input + 1))))
                    traceDataAppendOffsetFailError(inputCount - 1, 1, datasetPath_, filePath_);
            }
            return true;
        }
        if (inputLastTicks < dataFirstTicks) //      dddddddd
        {                                    // iiii
            if (!prependItems(datasetId_, nativeTypeId_, inputCount, static_cast<void*>(const_cast<T*>(input)), sizeof(T)))
                return traceDataPrependFailError(inputCount, datasetPath_, filePath_);
            return true;
        }
        if (inputLastTicks == dataFirstTicks) //      [d]ddddddd
        {                                     //   iii[i]
            const hsize_t count1 = inputCount - 1;
            if (inputFirst.isDifferent(dataFirst))
            {
                if (DuplicateTimeTicks::Update == duplicateTimeTicks)
                {
                    if (verbose)
                        traceDataDuplicateTicksUpdateInfo<T>(0, dataFirst, count1, inputLast);
                    if (!writeItem(datasetId_, nativeTypeId_, 0, static_cast<void*>(&inputLast)))
                        return traceDataDuplicateTicksUpdateError<T>(0, dataFirst, count1, inputLast);
                }
                else if (DuplicateTimeTicks::Skip == duplicateTimeTicks)
                    traceDataDuplicateTicksSkipError<T>(0, dataFirst, count1, inputLast);
                else //if (DuplicateTimeTicks::Fail == duplicateTimeTicks)
                    return traceDataDuplicateTicksFailError<T>(0, dataFirst, count1, inputLast);
            }
            if (0 < count1)
            {
                if (!prependItems(datasetId_, nativeTypeId_, count1, static_cast<void*>(const_cast<T*>(input)), sizeof(T)))
                    return traceDataPrependFailError(count1, datasetPath_, filePath_);
            }
            return true;
        }
        if (inputLastTicks > dataLastTicks)
        {
            hsize_t index; bool match;
            if (inputFirstTicks > dataFirstTicks) // dddddddddddd[d]
            {                                     //          iii[i]iiiiiii
                index = lowerNeighborIndex<T>(input, inputCount, dataLastTicks, match);
                if (match)
                {
                    T t = *(input + index);
                    if (t.isDifferent(dataLast))
                    {
                        const hsize_t count1 = dataCount - 1;
                        if (DuplicateTimeTicks::Update == duplicateTimeTicks)
                        {
                            if (verbose)
                                traceDataDuplicateTicksUpdateInfo<T>(count1, dataLast, index, t);
                            if (!writeItem(datasetId_, nativeTypeId_, count1, static_cast<void*>(&t)))
                                return traceDataDuplicateTicksUpdateError<T>(count1, dataLast, index, t);
                        }
                        else if (DuplicateTimeTicks::Skip == duplicateTimeTicks)
                            traceDataDuplicateTicksSkipError<T>(count1, dataLast, index, t);
                        else //if (DuplicateTimeTicks::Fail == duplicateTimeTicks)
                            return traceDataDuplicateTicksFailError<T>(count1, dataLast, index, t);
                    }
                    if (0 < index)
                    {
                        if (!mergeItems(datasetId_, nativeTypeId_, 0, index, input, duplicateTimeTicks, verbose))
                            return traceDataMergeFailError(index, datasetPath_, filePath_);
                    }
                }
                else if (0 <= index)
                {
                    if (!mergeItems(datasetId_, nativeTypeId_, 0, index + 1, input, duplicateTimeTicks, verbose))
                        return traceDataMergeFailError(index, datasetPath_, filePath_);
                }
                if (++index < inputCount)
                {
                    if (!appendItems(datasetId_, nativeTypeId_, inputCount - index, static_cast<void*>(const_cast<T*>(input + index))))
                        return traceDataAppendOffsetFailError(inputCount - index, index, datasetPath_, filePath_);
                }
            }
            else //     [d]ddddddd[d]
            {    // iiii[i]iiiiiii[i]iiiiiii
                index = lowerNeighborIndex<T>(input, inputCount, dataFirstTicks, match);
                if (match)
                {
                    T t = *(input + index);
                    if (t.isDifferent(dataFirst))
                    {
                        if (DuplicateTimeTicks::Update == duplicateTimeTicks)
                        {
                            if (verbose)
                                traceDataDuplicateTicksUpdateInfo<T>(0, dataFirst, index, t);
                            if (!writeItem(datasetId_, nativeTypeId_, 0, static_cast<void*>(&t)))
                                return traceDataDuplicateTicksUpdateError<T>(0, dataFirst, index, t);
                        }
                        else if (DuplicateTimeTicks::Skip == duplicateTimeTicks)
                            traceDataDuplicateTicksSkipError<T>(0, dataFirst, index, t);
                        else //if (DuplicateTimeTicks::Fail == duplicateTimeTicks)
                            return traceDataDuplicateTicksFailError<T>(0, dataFirst, index, t);
                    }
                    if (0 < index)
                    {
                        if (!prependItems(datasetId_, nativeTypeId_, index, static_cast<void*>(const_cast<T*>(input)), sizeof(T)))
                            return traceDataPrependFailError(index, datasetPath_, filePath_);
                    }
                }
                else
                {
                    if (!prependItems(datasetId_, nativeTypeId_, index + 1, static_cast<void*>(const_cast<T*>(input)), sizeof(T)))
                        return traceDataPrependFailError(index, datasetPath_, filePath_);
                }
                ++index; // The upper neighbor.
                hsize_t index2 = lowerNeighborIndex<T>(input, inputCount, dataLastTicks, match);
                if (match)
                {
                    T t = *(input + index2);
                    if (t.isDifferent(dataLast))
                    {
                        const hsize_t count1 = dataCount - 1;
                        if (DuplicateTimeTicks::Update == duplicateTimeTicks)
                        {
                            if (verbose)
                                traceDataDuplicateTicksUpdateInfo<T>(count1, dataLast, index, t);
                            if (!writeItem(datasetId_, nativeTypeId_, count1, static_cast<void*>(&t)))
                                return traceDataDuplicateTicksUpdateError<T>(count1, dataLast, index, t);
                        }
                        else if (DuplicateTimeTicks::Skip == duplicateTimeTicks)
                            traceDataDuplicateTicksSkipError<T>(count1, dataLast, index, t);
                        else //if (DuplicateTimeTicks::Fail == duplicateTimeTicks)
                            return traceDataDuplicateTicksFailError<T>(count1, dataLast, index, t);
                    }
                }
                if (index <= index2)
                {
                    if (!mergeItems(datasetId_, nativeTypeId_, index, index2 - index + 1, input, duplicateTimeTicks, verbose))
                        return traceDataMergeOffsetFailError(index2 - index + 1, index, datasetPath_, filePath_);
                }
                ++index2; // The upper neighbor.
                if (index2 < inputCount)
                {
                    if (!appendItems(datasetId_, nativeTypeId_, inputCount - index2, static_cast<void*>(const_cast<T*>(input + index2))))
                        return traceDataAppendOffsetFailError(inputCount - index2, index2, datasetPath_, filePath_);
                }
            }
        }
        else
        {
            if (inputFirstTicks > dataFirstTicks) // ddd[d]ddd[d]ddddd
            {                                     //    [i]iii[i]
                if (!mergeItems<T>(datasetId_, nativeTypeId_, 0, inputCount, input, duplicateTimeTicks, verbose))
                    return traceDataMergeFailError(inputCount, datasetPath_, filePath_);
            }
            else //      [d]dddddddd
            {    // iiiii[i]ii
                bool match;
                hsize_t index = lowerNeighborIndex<T>(input, inputCount, dataFirstTicks, match);
                if (match)
                {
                    T t = *(input + index);
                    if (t.isDifferent(dataFirst))
                    {
                        if (DuplicateTimeTicks::Update == duplicateTimeTicks)
                        {
                            if (verbose)
                                traceDataDuplicateTicksUpdateInfo<T>(0, dataFirst, index, t);
                            if (!writeItem(datasetId_, nativeTypeId_, 0, static_cast<void*>(&t)))
                                return traceDataDuplicateTicksUpdateError<T>(0, dataFirst, index, t);
                        }
                        else if (DuplicateTimeTicks::Skip == duplicateTimeTicks)
                            traceDataDuplicateTicksSkipError<T>(0, dataFirst, index, t);
                        else //if (DuplicateTimeTicks::Fail == duplicateTimeTicks)
                            return traceDataDuplicateTicksFailError<T>(0, dataFirst, index, t);
                    }
                    if (0 < index)
                    {
                        if (!prependItems(datasetId_, nativeTypeId_, index, static_cast<void*>(const_cast<T*>(input)), sizeof(T)))
                            return traceDataPrependFailError(index, datasetPath_, filePath_);
                    }
                }
                else if (0 <= index)
                {
                    if (!prependItems(datasetId_, nativeTypeId_, index + 1, static_cast<void*>(const_cast<T*>(input)), sizeof(T)))
                        return traceDataPrependFailError(index, datasetPath_, filePath_);
                }
                if (++index < inputCount)
                {
                    if (!mergeItems(datasetId_, nativeTypeId_, index, inputCount - index, input, duplicateTimeTicks, verbose))
                        return traceDataMergeOffsetFailError(inputCount - index, index, datasetPath_, filePath_);
                }
            }
        }
        return true;
    }

    template <typename T> bool Data::addData(std::vector<T>& input, const DuplicateTimeTicks duplicateTimeTicks, const bool verbose) const
    {
        return addData<T>(input.data(), hsize_t(input.size()), duplicateTimeTicks, verbose);
    }

    template <typename T> bool Data::fetchData(Collect<T> collect, const long long ticksFrom, const long long ticksTo) const
    {
        std::lock_guard<std::mutex> lock(theMutex);
        return fetchDataNoLock<T>(collect, ticksFrom, ticksTo);
    }

    template <typename T> bool Data::fetchDataNoLock(Collect<T> collect, const long long ticksFrom, long long ticksTo) const
    {
        hsize_t itemOffset, itemCount;
        long long lastTicks;
        T t{};
        if (0L >= ticksFrom)
        {
lessThanFirst:
            itemOffset = 0L;
            if (!countItems(datasetId_, itemCount))
                return traceDataFetchTimeTickRangeError(ticksFrom, ticksTo, datasetPath_, filePath_);
            if (0L >= itemCount)
                return true;
            if (0L < ticksTo)
            {
                if (!readItem(datasetId_, nativeTypeId_, itemCount - 1, static_cast<void*>(&t)))
                    return traceDataFetchTimeTickRangeError(ticksFrom, ticksTo, datasetPath_, filePath_);
                lastTicks = t.ticks_;
                if (ticksTo < lastTicks)
                {
                    hsize_t index;
                    bool match;
                    if (!lowerNeighborIndex<T>(datasetId_, nativeTypeId_, ticksTo, index, match))
                        return traceDataFetchTimeTickRangeError(ticksFrom, ticksTo, datasetPath_, filePath_);
                    itemCount = ++index;
                }
            }
        }
        else // ticksFrom > 0
        {
            if (!readItem(datasetId_, nativeTypeId_, 0, static_cast<void*>(&t)))
                return traceDataFetchTimeTickRangeError(ticksFrom, ticksTo, datasetPath_, filePath_);
            const long long firstTicks = t.ticks_;
            if (0L < ticksTo)
            {
                if (ticksTo < firstTicks)
                    return true;
            }
            if (ticksFrom <= firstTicks)
                goto lessThanFirst; // NOLINT
            if (!countItems(datasetId_, itemCount))
                return traceDataFetchTimeTickRangeError(ticksFrom, ticksTo, datasetPath_, filePath_);
            if (0L >= itemCount)
                return true;
            if (!readItem(datasetId_, nativeTypeId_, itemCount - 1, static_cast<void*>(&t)))
                return traceDataFetchTimeTickRangeError(ticksFrom, ticksTo, datasetPath_, filePath_);
            lastTicks = t.ticks_;
            if (ticksFrom > lastTicks)
                return true;
            if (0L >= ticksTo)
            {
                ticksTo = lastTicks;
                if (ticksTo < firstTicks)
                    return true;
            }
            bool match;
            if (!lowerNeighborIndex<T>(datasetId_, nativeTypeId_, ticksFrom, itemOffset, match))
                return traceDataFetchTimeTickRangeError(ticksFrom, ticksTo, datasetPath_, filePath_);
            if (!match)
                ++itemOffset;
            if (ticksTo < lastTicks)
            {
                hsize_t index;
                if (!lowerNeighborIndex<T>(datasetId_, nativeTypeId_, ticksTo, index, match))
                    return traceDataFetchTimeTickRangeError(ticksFrom, ticksTo, datasetPath_, filePath_);
                itemCount = ++index - itemOffset;
            }
            else // ticksTo >= lastTicks
                itemCount -= itemOffset;
        }
        if (0L >= itemCount)
            return true;
        // Get the dataspace handle.
        hid_t memorySpace_id;
        const hid_t space_id = H5Dget_space(datasetId_);
        if (0 > space_id)
            return traceDataFetchTimeTickRangeError(ticksFrom, ticksTo, datasetPath_, filePath_);
        hsize_t dims[1], offsets[1];
        void* ptr;
        const hsize_t bytesTotal = itemCount * sizeof(T);
        const hsize_t bytesLimit = maximumReadBufferBytes();
        if (0 == bytesLimit || bytesLimit > bytesTotal)
        {
            dims[0] = itemCount;
            // Allocate memory block for stored data.
            ptr = static_cast<void*>(calloc(size_t(itemCount), sizeof(T)));
            if (nullptr == ptr)
            {
closeSpace:
                H5Sclose(space_id);
                return traceDataFetchTimeTickRangeError(ticksFrom, ticksTo, datasetPath_, filePath_);
            }
            // Create a simple memory dataspace.
            memorySpace_id = H5Screate_simple(1, dims, nullptr);
            if (0 > memorySpace_id)
            {
deallocate:
                free(ptr);
                goto closeSpace; // NOLINT
            }
            // Define a hyperslab in the dataset.
            offsets[0] = itemOffset;
            if (0 > H5Sselect_hyperslab(space_id, H5S_SELECT_SET, offsets, nullptr, dims, nullptr))
            {
closeMemorySpace:
                H5Sclose(memorySpace_id);
                goto deallocate; // NOLINT
            }
            // Read in the stored items.
            if (0 > H5Dread(datasetId_, nativeTypeId_, memorySpace_id, space_id, H5P_DEFAULT, ptr))
                goto closeMemorySpace; // NOLINT
            try
            {
                collect(static_cast<const T*>(ptr), dims[0]);
            }
            catch (const std::exception& e)
            {
                free(ptr);
                H5Sclose(memorySpace_id);
                H5Sclose(space_id);
                traceDataExceptionWhileCollectingItemsError(e, datasetPath_, filePath_);
                return traceDataFetchTimeTickRangeError(ticksFrom, ticksTo, datasetPath_, filePath_);
            }
        }
        else
        {
            hsize_t itemLimit = bytesLimit / sizeof(T);
            if (0 >= itemLimit)
                goto closeSpace; // NOLINT
            // Allocate memory block for stored data.
            ptr = static_cast<void*>(calloc(size_t(itemLimit), sizeof(T)));
            if (nullptr == ptr)
                goto closeSpace; // NOLINT
            dims[0] = itemLimit;
            // Create a simple memory dataspace.
            memorySpace_id = H5Screate_simple(1, dims, nullptr);
            if (0 > memorySpace_id)
                goto deallocate; // NOLINT
            for (hsize_t i = itemOffset;;)
            {
                // Define a hyperslab in the dataset.
                offsets[0] = i;
                if (0 > H5Sselect_hyperslab(space_id, H5S_SELECT_SET, offsets, nullptr, dims, nullptr))
                    goto closeMemorySpace; // NOLINT
                // Read in the <indexLimit> stored items starting from <i>.
                if (0 > H5Dread(datasetId_, nativeTypeId_, memorySpace_id, space_id, H5P_DEFAULT, ptr))
                    goto closeMemorySpace; // NOLINT
                // Add items to the list.
                try
                {
                    collect(static_cast<const T*>(ptr), itemLimit);
                }
                catch (const std::exception& e)
                {
                    free(ptr);
                    H5Sclose(memorySpace_id);
                    H5Sclose(space_id);
                    traceDataExceptionWhileCollectingItemsError(e, datasetPath_, filePath_);
                    return traceDataFetchTimeTickRangeError(ticksFrom, ticksTo, datasetPath_, filePath_);
                }
                itemCount -= itemLimit;
                i += itemLimit;
                if (itemCount < itemLimit)
                {
                    itemLimit = itemCount;
                    if (0 >= itemLimit)
                        break;
                    dims[0] = itemLimit;
                    if (0 > H5Sclose(memorySpace_id))
                        goto deallocate; // NOLINT
                    memorySpace_id = H5Screate_simple(1, dims, nullptr);
                    if (0 > memorySpace_id)
                        goto deallocate; // NOLINT
                }
            }
        }
        free(ptr);
        if (0 > H5Sclose(memorySpace_id))
        {
            H5Sclose(space_id);
            return traceDataFetchTimeTickRangeError(ticksFrom, ticksTo, datasetPath_, filePath_);
        }
        if (0 > H5Sclose(space_id))
            return traceDataFetchTimeTickRangeError(ticksFrom, ticksTo, datasetPath_, filePath_);
        return true;
    }

    template <typename T> bool Data::ticksToIndexNoLock(const long long ticks, hsize_t& index, bool& match) const
    {
        if (0L >= ticks)
        {
zeroIndex:
            index = 0L;
            match = true;
            return true;
        }
        T t{};
        if (!readItem(datasetId_, nativeTypeId_, 0, static_cast<void*>(&t)))
        {
failed:
            index = 0L;
            match = true;
            return traceDataGetTimeTickIndexError(ticks, datasetPath_, filePath_);
        }
        const long long firstTicks = t.ticks_;
        if (ticks <= firstTicks)
            goto zeroIndex; // NOLINT
        hsize_t itemCount;
        if (!countItems(datasetId_, itemCount))
            goto failed; // NOLINT
        if (0L >= itemCount)
            goto zeroIndex; // NOLINT
        if (!readItem(datasetId_, nativeTypeId_, itemCount - 1, static_cast<void*>(&t)))
            goto failed; // NOLINT
        const long long lastTicks = t.ticks_;
        if (ticks >= lastTicks)
        {
            index = itemCount - 1;
            match = true;
            return true;
        }
        if (!lowerNeighborIndex<T>(datasetId_, nativeTypeId_, ticks, itemCount, match))
            goto failed; // NOLINT
        index = itemCount;
        return true;
    }

    template <typename T> bool Data::ticksToIndex(const long long ticks, hsize_t& index, bool& match) const
    {
        std::lock_guard<std::mutex> lock(theMutex);
        return ticksToIndexNoLock<T>(ticks, index, match);
    }

    template <typename T> bool Data::ticksToIndexRange(const long long ticksFrom, const long long ticksTo, hsize_t& indexFrom, hsize_t& indexTo, bool& matchFrom, bool& matchTo) const
    {
        std::lock_guard<std::mutex> lock(theMutex);
        const bool statusFrom = ticksToIndexNoLock<T>(ticksFrom, indexFrom, matchFrom);
        const bool statusTo = ticksToIndexNoLock<T>(ticksTo, indexTo, matchTo);
        return statusFrom && statusTo;
    }

    template <typename T> bool Data::fetchFromIndexRange(Collect<T> collect, const hsize_t index, hsize_t count) const
    {
        void* ptr{ nullptr };
        try
        {
            hsize_t itemCount{ hsize_t(0) };
            std::lock_guard<std::mutex> lock(theMutex);
            if (!countItems(datasetId_, itemCount))
                return traceDataFetchIndexRangeError(index, count, datasetPath_, filePath_);
            if (index < itemCount)
            {
                if (index + count > itemCount)
                    count = itemCount - index;
                ptr = static_cast<void*>(calloc(size_t(count + 1), sizeof(T)));
                if (nullptr == ptr)
                    return traceDataFetchIndexRangeError(index, count, datasetPath_, filePath_);
                if (!readItemRange(datasetId_, nativeTypeId_, index, count, ptr))
                    return traceDataFetchIndexRangeError(index, count, datasetPath_, filePath_);
                collect(static_cast<const T*>(ptr), count);
                free(ptr);
                ptr = nullptr;
                return true;
            }
        }
        catch (const std::exception& e)
        {
            if (nullptr != ptr)
                free(ptr);
            traceDataExceptionWhileCollectingItemsError(e, datasetPath_, filePath_);
            return traceDataFetchIndexRangeError(index, count, datasetPath_, filePath_);
        }
        return false;
    }

    // ----------------------------------------------------------------------
    // QuoteData
    // ----------------------------------------------------------------------

    QuoteData::QuoteData(const Instrument* instrument, const char* name, const hid_t datasetId, const hid_t nativeTypeId, const hid_t standardTypeId, const bool readOnly) :
        Data(instrument, name, datasetId, nativeTypeId, standardTypeId, readOnly)
    {
    }

    bool QuoteData::add(const Quote* input, const hsize_t inputCount, const DuplicateTimeTicks duplicateTimeTicks, const bool verbose) const
    {
        return addData<Quote>(input, inputCount, duplicateTimeTicks, verbose);
    }

    bool QuoteData::spreadDuplicateTimeTicks(std::vector<Quote>& input, const bool verbose)
    {
        return spreadList<Quote>(input, verbose);
    }

    bool QuoteData::spreadDuplicateTimeTicks(Quote* input, const size_t inputCount, const bool verbose)
    {
        return spreadList<Quote>(input, inputCount, verbose);
    }

    bool QuoteData::fetchAll(const CollectQuote& collect) const
    {
        return fetchData<Quote>(collect, hsize_t(0), hsize_t(0));
    }

    bool QuoteData::fetchTickRange(const CollectQuote& collect, const long long from, const long long to) const
    {
        return fetchData<Quote>(collect, from, to);
    }

    bool QuoteData::fetchIndexRange(const CollectQuote& collect, const hsize_t index, const hsize_t count) const
    {
        return fetchFromIndexRange<Quote>(collect, index, count);
    }

    bool QuoteData::ticksIndex(const long long ticks, hsize_t& index, bool& match) const
    {
        return ticksToIndex<Quote>(ticks, index, match);
    }

    bool QuoteData::ticksIndexRange(const long long ticksFrom, const long long ticksTo, hsize_t& indexFrom, hsize_t& indexTo, bool& matchFrom, bool& matchTo) const
    {
        return ticksToIndexRange<Quote>(ticksFrom, ticksTo, indexFrom, indexTo, matchFrom, matchTo);
    }

    bool QuoteData::deleteIndexRange(const hsize_t indexFrom, const hsize_t indexTo) const
    {
        return Data::deleteIndexRange<Quote>(indexFrom, indexTo);
    }

    bool QuoteData::deleteTickRange(const long long ticksFrom, const long long ticksTo) const
    {
        return Data::deleteTickRange<Quote>(ticksFrom, ticksTo);
    }

    bool QuoteData::deleteAllBeforeTick(const long long ticks) const
    {
        return Data::deleteTickRange<Quote>(0L, ticks);
    }

    bool QuoteData::deleteAllBeforeIndex(const hsize_t index) const
    {
        return Data::deleteIndexRange<Quote>(0UL, index);
    }

    bool QuoteData::deleteAllAfterTick(const long long ticks) const
    {
        return Data::deleteTickRange<Quote>(ticks, lastTicks());
    }

    bool QuoteData::deleteAllAfterIndex(const hsize_t index) const
    {
        return Data::deleteIndexRange<Quote>(index, count() - 1);
    }

    // ----------------------------------------------------------------------
    // QuotePriceOnlyData
    // ----------------------------------------------------------------------

    QuotePriceOnlyData::QuotePriceOnlyData(const Instrument* instrument, const char* name, const hid_t datasetId, const hid_t nativeTypeId, const hid_t standardTypeId, const bool readOnly) :
        Data(instrument, name, datasetId, nativeTypeId, standardTypeId, readOnly)
    {
    }

    bool QuotePriceOnlyData::add(const QuotePriceOnly* input, const hsize_t inputCount, const DuplicateTimeTicks duplicateTimeTicks, const bool verbose) const
    {
        return addData<QuotePriceOnly>(input, inputCount, duplicateTimeTicks, verbose);
    }

    bool QuotePriceOnlyData::spreadDuplicateTimeTicks(std::vector<QuotePriceOnly>& input, const bool verbose)
    {
        return spreadList<QuotePriceOnly>(input, verbose);
    }

    bool QuotePriceOnlyData::spreadDuplicateTimeTicks(QuotePriceOnly* input, const size_t inputCount, const bool verbose)
    {
        return spreadList<QuotePriceOnly>(input, inputCount, verbose);
    }

    bool QuotePriceOnlyData::fetchAll(const CollectQuotePriceOnly& collect) const
    {
        return fetchData<QuotePriceOnly>(collect, hsize_t(0), hsize_t(0));
    }

    bool QuotePriceOnlyData::fetchTickRange(const CollectQuotePriceOnly& collect, const long long from, const long long to) const
    {
        return fetchData<QuotePriceOnly>(collect, from, to);
    }

    bool QuotePriceOnlyData::fetchIndexRange(const CollectQuotePriceOnly& collect, const hsize_t index, const hsize_t count) const
    {
        return fetchFromIndexRange<QuotePriceOnly>(collect, index, count);
    }

    bool QuotePriceOnlyData::ticksIndex(const long long ticks, hsize_t& index, bool& match) const
    {
        return ticksToIndex<QuotePriceOnly>(ticks, index, match);
    }

    bool QuotePriceOnlyData::ticksIndexRange(const long long ticksFrom, const long long ticksTo, hsize_t& indexFrom, hsize_t& indexTo, bool& matchFrom, bool& matchTo) const
    {
        return ticksToIndexRange<QuotePriceOnly>(ticksFrom, ticksTo, indexFrom, indexTo, matchFrom, matchTo);
    }

    bool QuotePriceOnlyData::deleteIndexRange(const hsize_t indexFrom, const hsize_t indexTo) const
    {
        return Data::deleteIndexRange<QuotePriceOnly>(indexFrom, indexTo);
    }

    bool QuotePriceOnlyData::deleteTickRange(const long long ticksFrom, const long long ticksTo) const
    {
        return Data::deleteTickRange<QuotePriceOnly>(ticksFrom, ticksTo);
    }

    bool QuotePriceOnlyData::deleteAllBeforeTick(const long long ticks) const
    {
        return Data::deleteTickRange<QuotePriceOnly>(0L, ticks);
    }

    bool QuotePriceOnlyData::deleteAllBeforeIndex(const hsize_t index) const
    {
        return Data::deleteIndexRange<QuotePriceOnly>(0UL, index);
    }

    bool QuotePriceOnlyData::deleteAllAfterTick(const long long ticks) const
    {
        return Data::deleteTickRange<QuotePriceOnly>(ticks, lastTicks());
    }

    bool QuotePriceOnlyData::deleteAllAfterIndex(const hsize_t index) const
    {
        return Data::deleteIndexRange<QuotePriceOnly>(index, count() - 1);
    }

    // ----------------------------------------------------------------------
    // TradeData
    // ----------------------------------------------------------------------

    TradeData::TradeData(const Instrument* instrument, const char* name, const hid_t datasetId, const hid_t nativeTypeId, const hid_t standardTypeId, const bool readOnly) :
        Data(instrument, name, datasetId, nativeTypeId, standardTypeId, readOnly)
    {
    }

    bool TradeData::add(const Trade* input, const hsize_t inputCount, const DuplicateTimeTicks duplicateTimeTicks, const bool verbose) const
    {
        return addData<Trade>(input, inputCount, duplicateTimeTicks, verbose);
    }

    bool TradeData::spreadDuplicateTimeTicks(std::vector<Trade>& input, const bool verbose)
    {
        return spreadList<Trade>(input, verbose);
    }

    bool TradeData::spreadDuplicateTimeTicks(Trade* input, const size_t inputCount, const bool verbose)
    {
        return spreadList<Trade>(input, inputCount, verbose);
    }

    bool TradeData::fetchAll(const CollectTrade& collect) const
    {
        return fetchData<Trade>(collect, hsize_t(0), hsize_t(0));
    }

    bool TradeData::fetchTickRange(const CollectTrade& collect, const long long from, const long long to) const
    {
        return fetchData<Trade>(collect, from, to);
    }

    bool TradeData::fetchIndexRange(const CollectTrade& collect, const hsize_t index, const hsize_t count) const
    {
        return fetchFromIndexRange<Trade>(collect, index, count);
    }

    bool TradeData::ticksIndex(const long long ticks, hsize_t& index, bool& match) const
    {
        return ticksToIndex<Trade>(ticks, index, match);
    }

    bool TradeData::ticksIndexRange(const long long ticksFrom, const long long ticksTo, hsize_t& indexFrom, hsize_t& indexTo, bool& matchFrom, bool& matchTo) const
    {
        return ticksToIndexRange<Trade>(ticksFrom, ticksTo, indexFrom, indexTo, matchFrom, matchTo);
    }

    bool TradeData::deleteIndexRange(const hsize_t indexFrom, const hsize_t indexTo) const
    {
        return Data::deleteIndexRange<Trade>(indexFrom, indexTo);
    }

    bool TradeData::deleteTickRange(const long long ticksFrom, const long long ticksTo) const
    {
        return Data::deleteTickRange<Trade>(ticksFrom, ticksTo);
    }

    bool TradeData::deleteAllBeforeTick(const long long ticks) const
    {
        return Data::deleteTickRange<Trade>(0L, ticks);
    }

    bool TradeData::deleteAllBeforeIndex(const hsize_t index) const
    {
        return Data::deleteIndexRange<Trade>(0UL, index);
    }

    bool TradeData::deleteAllAfterTick(const long long ticks) const
    {
        return Data::deleteTickRange<Trade>(ticks, lastTicks());
    }

    bool TradeData::deleteAllAfterIndex(const hsize_t index) const
    {
        return Data::deleteIndexRange<Trade>(index, count() - 1);
    }

    // ----------------------------------------------------------------------
    // TradePriceOnlyData
    // ----------------------------------------------------------------------

    TradePriceOnlyData::TradePriceOnlyData(const Instrument* instrument, const char* name, const hid_t datasetId, const hid_t nativeTypeId, const hid_t standardTypeId, const bool readOnly) :
        Data(instrument, name, datasetId, nativeTypeId, standardTypeId, readOnly)
    {
    }

    bool TradePriceOnlyData::add(const TradePriceOnly* input, const hsize_t inputCount, const DuplicateTimeTicks duplicateTimeTicks, const bool verbose) const
    {
        return addData<TradePriceOnly>(input, inputCount, duplicateTimeTicks, verbose);
    }

    bool TradePriceOnlyData::spreadDuplicateTimeTicks(std::vector<TradePriceOnly>& input, const bool verbose)
    {
        return spreadList<TradePriceOnly>(input, verbose);
    }

    bool TradePriceOnlyData::spreadDuplicateTimeTicks(TradePriceOnly* input, const size_t inputCount, const bool verbose)
    {
        return spreadList<TradePriceOnly>(input, inputCount, verbose);
    }

    bool TradePriceOnlyData::fetchAll(const CollectTradePriceOnly& collect) const
    {
        return fetchData<TradePriceOnly>(collect, hsize_t(0), hsize_t(0));
    }

    bool TradePriceOnlyData::fetchTickRange(const CollectTradePriceOnly& collect, const long long from, const long long to) const
    {
        return fetchData<TradePriceOnly>(collect, from, to);
    }

    bool TradePriceOnlyData::fetchIndexRange(const CollectTradePriceOnly& collect, const hsize_t index, const hsize_t count) const
    {
        return fetchFromIndexRange<TradePriceOnly>(collect, index, count);
    }

    bool TradePriceOnlyData::ticksIndex(const long long ticks, hsize_t& index, bool& match) const
    {
        return ticksToIndex<TradePriceOnly>(ticks, index, match);
    }

    bool TradePriceOnlyData::ticksIndexRange(const long long ticksFrom, const long long ticksTo, hsize_t& indexFrom, hsize_t& indexTo, bool& matchFrom, bool& matchTo) const
    {
        return ticksToIndexRange<TradePriceOnly>(ticksFrom, ticksTo, indexFrom, indexTo, matchFrom, matchTo);
    }

    bool TradePriceOnlyData::deleteIndexRange(const hsize_t indexFrom, const hsize_t indexTo) const
    {
        return Data::deleteIndexRange<TradePriceOnly>(indexFrom, indexTo);
    }

    bool TradePriceOnlyData::deleteTickRange(const long long ticksFrom, const long long ticksTo) const
    {
        return Data::deleteTickRange<TradePriceOnly>(ticksFrom, ticksTo);
    }

    bool TradePriceOnlyData::deleteAllBeforeTick(const long long ticks) const
    {
        return Data::deleteTickRange<TradePriceOnly>(0L, ticks);
    }

    bool TradePriceOnlyData::deleteAllBeforeIndex(const hsize_t index) const
    {
        return Data::deleteIndexRange<TradePriceOnly>(0UL, index);
    }

    bool TradePriceOnlyData::deleteAllAfterTick(const long long ticks) const
    {
        return Data::deleteTickRange<TradePriceOnly>(ticks, lastTicks());
    }

    bool TradePriceOnlyData::deleteAllAfterIndex(const hsize_t index) const
    {
        return Data::deleteIndexRange<TradePriceOnly>(index, count() - 1);
    }

    // ----------------------------------------------------------------------
    // OhlcvData
    // ----------------------------------------------------------------------

    OhlcvData::OhlcvData(const Instrument* instrument, const char* name, const hid_t datasetId, const hid_t nativeTypeId, const hid_t standardTypeId, const bool readOnly) :
        Data(instrument, name, datasetId, nativeTypeId, standardTypeId, readOnly)
    {
    }

    bool OhlcvData::add(const Ohlcv* input, const hsize_t inputCount, const DuplicateTimeTicks duplicateTimeTicks, const bool verbose) const
    {
        return addData<Ohlcv>(input, inputCount, duplicateTimeTicks, verbose);
    }

    bool OhlcvData::spreadDuplicateTimeTicks(std::vector<Ohlcv>& input, const bool verbose)
    {
        return spreadList<Ohlcv>(input, verbose);
    }

    bool OhlcvData::spreadDuplicateTimeTicks(Ohlcv* input, const size_t inputCount, const bool verbose)
    {
        return spreadList<Ohlcv>(input, inputCount, verbose);
    }

    bool OhlcvData::fetchAll(const CollectOhlcv& collect) const
    {
        return fetchData<Ohlcv>(collect, hsize_t(0), hsize_t(0));
    }

    bool OhlcvData::fetchTickRange(const CollectOhlcv& collect, const long long from, const long long to) const
    {
        return fetchData<Ohlcv>(collect, from, to);
    }

    bool OhlcvData::fetchIndexRange(const CollectOhlcv& collect, const hsize_t index, const hsize_t count) const
    {
        return fetchFromIndexRange<Ohlcv>(collect, index, count);
    }

    bool OhlcvData::ticksIndex(const long long ticks, hsize_t& index, bool& match) const
    {
        return ticksToIndex<Ohlcv>(ticks, index, match);
    }

    bool OhlcvData::ticksIndexRange(const long long ticksFrom, const long long ticksTo, hsize_t& indexFrom, hsize_t& indexTo, bool& matchFrom, bool& matchTo) const
    {
        return ticksToIndexRange<Ohlcv>(ticksFrom, ticksTo, indexFrom, indexTo, matchFrom, matchTo);
    }

    bool OhlcvData::deleteIndexRange(const hsize_t indexFrom, const hsize_t indexTo) const
    {
        return Data::deleteIndexRange<Ohlcv>(indexFrom, indexTo);
    }

    bool OhlcvData::deleteTickRange(const long long ticksFrom, const long long ticksTo) const
    {
        return Data::deleteTickRange<Ohlcv>(ticksFrom, ticksTo);
    }

    bool OhlcvData::deleteAllBeforeTick(const long long ticks) const
    {
        return Data::deleteTickRange<Ohlcv>(0L, ticks);
    }

    bool OhlcvData::deleteAllBeforeIndex(const hsize_t index) const
    {
        return Data::deleteIndexRange<Ohlcv>(0UL, index);
    }

    bool OhlcvData::deleteAllAfterTick(const long long ticks) const
    {
        return Data::deleteTickRange<Ohlcv>(ticks, lastTicks());
    }

    bool OhlcvData::deleteAllAfterIndex(const hsize_t index) const
    {
        return Data::deleteIndexRange<Ohlcv>(index, count() - 1);
    }

    // ----------------------------------------------------------------------
    // OhlcvPriceOnlyData
    // ----------------------------------------------------------------------

    OhlcvPriceOnlyData::OhlcvPriceOnlyData(const Instrument* instrument, const char* name, const hid_t datasetId, const hid_t nativeTypeId, const hid_t standardTypeId, const bool readOnly) :
        Data(instrument, name, datasetId, nativeTypeId, standardTypeId, readOnly)
    {
    }

    bool OhlcvPriceOnlyData::add(const OhlcvPriceOnly* input, const hsize_t inputCount, const DuplicateTimeTicks duplicateTimeTicks, const bool verbose) const
    {
        return addData<OhlcvPriceOnly>(input, inputCount, duplicateTimeTicks, verbose);
    }

    bool OhlcvPriceOnlyData::spreadDuplicateTimeTicks(std::vector<OhlcvPriceOnly>& input, const bool verbose)
    {
        return spreadList<OhlcvPriceOnly>(input, verbose);
    }

    bool OhlcvPriceOnlyData::spreadDuplicateTimeTicks(OhlcvPriceOnly* input, const size_t inputCount, const bool verbose)
    {
        return spreadList<OhlcvPriceOnly>(input, inputCount, verbose);
    }

    bool OhlcvPriceOnlyData::fetchAll(const CollectOhlcvPriceOnly& collect) const
    {
        return fetchData<OhlcvPriceOnly>(collect, hsize_t(0), hsize_t(0));
    }

    bool OhlcvPriceOnlyData::fetchTickRange(const CollectOhlcvPriceOnly& collect, const long long from, const long long to) const
    {
        return fetchData<OhlcvPriceOnly>(collect, from, to);
    }

    bool OhlcvPriceOnlyData::fetchIndexRange(const CollectOhlcvPriceOnly& collect, const hsize_t index, const hsize_t count) const
    {
        return fetchFromIndexRange<OhlcvPriceOnly>(collect, index, count);
    }

    bool OhlcvPriceOnlyData::ticksIndex(const long long ticks, hsize_t& index, bool& match) const
    {
        return ticksToIndex<OhlcvPriceOnly>(ticks, index, match);
    }

    bool OhlcvPriceOnlyData::ticksIndexRange(const long long ticksFrom, const long long ticksTo, hsize_t& indexFrom, hsize_t& indexTo, bool& matchFrom, bool& matchTo) const
    {
        return ticksToIndexRange<OhlcvPriceOnly>(ticksFrom, ticksTo, indexFrom, indexTo, matchFrom, matchTo);
    }

    bool OhlcvPriceOnlyData::deleteIndexRange(const hsize_t indexFrom, const hsize_t indexTo) const
    {
        return Data::deleteIndexRange<OhlcvPriceOnly>(indexFrom, indexTo);
    }

    bool OhlcvPriceOnlyData::deleteTickRange(const long long ticksFrom, const long long ticksTo) const
    {
        return Data::deleteTickRange<OhlcvPriceOnly>(ticksFrom, ticksTo);
    }

    bool OhlcvPriceOnlyData::deleteAllBeforeTick(const long long ticks) const
    {
        return Data::deleteTickRange<OhlcvPriceOnly>(0L, ticks);
    }

    bool OhlcvPriceOnlyData::deleteAllBeforeIndex(const hsize_t index) const
    {
        return Data::deleteIndexRange<OhlcvPriceOnly>(0UL, index);
    }

    bool OhlcvPriceOnlyData::deleteAllAfterTick(const long long ticks) const
    {
        return Data::deleteTickRange<OhlcvPriceOnly>(ticks, lastTicks());
    }

    bool OhlcvPriceOnlyData::deleteAllAfterIndex(const hsize_t index) const
    {
        return Data::deleteIndexRange<OhlcvPriceOnly>(index, count() - 1);
    }

    // ----------------------------------------------------------------------
    // ScalarData
    // ----------------------------------------------------------------------

    ScalarData::ScalarData(const Instrument* instrument, const char* name, const hid_t datasetId, const hid_t nativeTypeId, const hid_t standardTypeId, const bool readOnly) :
        Data(instrument, name, datasetId, nativeTypeId, standardTypeId, readOnly)
    {
    }

    bool ScalarData::add(const Scalar* input, const hsize_t inputCount, const DuplicateTimeTicks duplicateTimeTicks, const bool verbose) const
    {
        return addData<Scalar>(input, inputCount, duplicateTimeTicks, verbose);
    }

    bool ScalarData::spreadDuplicateTimeTicks(std::vector<Scalar>& input, const bool verbose)
    {
        return spreadList<Scalar>(input, verbose);
    }

    bool ScalarData::spreadDuplicateTimeTicks(Scalar* input, const size_t inputCount, const bool verbose)
    {
        return spreadList<Scalar>(input, inputCount, verbose);
    }

    bool ScalarData::fetchAll(const CollectScalar& collect) const
    {
        return fetchData<Scalar>(collect, hsize_t(0), hsize_t(0));
    }

    bool ScalarData::fetchTickRange(const CollectScalar& collect, const long long from, const long long to) const
    {
        return fetchData<Scalar>(collect, from, to);
    }

    bool ScalarData::fetchIndexRange(const CollectScalar& collect, const hsize_t index, const hsize_t count) const
    {
        return fetchFromIndexRange<Scalar>(collect, index, count);
    }

    bool ScalarData::ticksIndex(const long long ticks, hsize_t& index, bool& match) const
    {
        return ticksToIndex<Scalar>(ticks, index, match);
    }

    bool ScalarData::ticksIndexRange(const long long ticksFrom, const long long ticksTo, hsize_t& indexFrom, hsize_t& indexTo, bool& matchFrom, bool& matchTo) const
    {
        return ticksToIndexRange<Scalar>(ticksFrom, ticksTo, indexFrom, indexTo, matchFrom, matchTo);
    }

    bool ScalarData::deleteIndexRange(const hsize_t indexFrom, const hsize_t indexTo) const
    {
        return Data::deleteIndexRange<Scalar>(indexFrom, indexTo);
    }

    bool ScalarData::deleteTickRange(const long long ticksFrom, const long long ticksTo) const
    {
        return Data::deleteTickRange<Scalar>(ticksFrom, ticksTo);
    }

    bool ScalarData::deleteAllBeforeTick(const long long ticks) const
    {
        return Data::deleteTickRange<Scalar>(0L, ticks);
    }

    bool ScalarData::deleteAllBeforeIndex(const hsize_t index) const
    {
        return Data::deleteIndexRange<Scalar>(0UL, index);
    }

    bool ScalarData::deleteAllAfterTick(const long long ticks) const
    {
        return Data::deleteTickRange<Scalar>(ticks, lastTicks());
    }

    bool ScalarData::deleteAllAfterIndex(const hsize_t index) const
    {
        return Data::deleteIndexRange<Scalar>(index, count() - 1);
    }
}
