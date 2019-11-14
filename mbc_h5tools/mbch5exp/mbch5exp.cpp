#include <algorithm>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <regex>
#include <string>
#include "option_parser.h"
#include "mbc/h5.h"
#include "mbc/time/dotNetTicks.h"

static const char* h5Suffix{ ".h5" };
static const std::string h5Delimiter{ ".h5:/" };
static const char h5Slash{ '/' };
static const char dateSlash{ '/' };

enum class DateTimeFormat
{
    //! yyyy-MM-dd HH:mm:ss.fffffff
    YmdhmsfDash,

    //! yyyy/MM/dd HH:mm:ss.fffffff
    YmdhmsfSlash,

    //! yyyy-MM-dd HH:mm:ss
    YmdhmsDash,

    //! yyyy/MM/dd HH:mm:ss
    YmdhmsSlash,

    //! yyyy-MM-dd HH:mm
    YmdhmDash,

    //! yyyy/MM/dd HH:mm
    YmdhmSlash,

    //! yyyy-MM-dd HH
    YmdhDash,

    //! yyyy/MM/dd HH
    YmdhSlash,

    //! yyyy-MM-dd
    YmdDash,

    //! yyyy/MM/dd
    YmdSlash
};

static int help()
{
    std::cerr
        << std::endl
        << R"(Exports specified h5 dataset(s) to csv file(s).)" << std::endl
        << R"(Arguments: file.h5{:/instrumentPath{/dataset}} {-dt|--date-time format} {-v|--verbose})" << std::endl
        << R"(instrumentPath: if not specified, all instruments will be exported)" << std::endl
        << R"(dataset:        if not specified, all datasets of the instrument will be exported)" << std::endl
        << std::endl
        << "dataset naming conventions:" << std::endl
        << "t____  trade aperiodic, contains timestamp, price and volume" << std::endl
        << "T____  trade aperiodic, contains timestamp, price" << std::endl
        << "q____  quote aperiodic, contains timestamp, ask/bid prices and volumes" << std::endl
        << "Q____  quote aperiodic, contains timestamp, ask/bid prices" << std::endl
        << "ckttt  candle, contains timestamp, open/high/low/close, volume" << std::endl
        << "Ckttt  candle without volume" << std::endl
        << "jkttt  adjusted candle, contains timestamp, open/high/low/close, volume" << std::endl
        << "Jkttt  adjusted candle without volume" << std::endl
        << "where" << std::endl
        << "k    kind; the following values ar accepted:" << std::endl
        << "     _  normal" << std::endl
        << "     a  ask" << std::endl
        << "     b  bid" << std::endl
        << "ttt  time period; the following periods are accepted:" << std::endl
        << "     ___                                               aperiodic" << std::endl
        << "     s01, s02, s03, s04, s05, s06, s10, s15, s20, s30  seconds" << std::endl
        << "     m01, m02, m03, m04, m05, m06, m10, m15, m20, m30  minutes" << std::endl
        << "     h01, h02, h03, h04, h06, h08, h12                 hours" << std::endl
        << "     d01                                               days" << std::endl
        << "     w01, w02, w03                                     weeks" << std::endl
        << "     M01, M02, M03, M04, M06                           months" << std::endl
        << "     y01                                               years" << std::endl
        << std::endl
        << "-dt|--date-time format: default is ymd-hms.f" << std::endl
        << "-dt ymd-hms.f  yyyy-MM-dd HH:mm:ss.fffffff" << std::endl
        << "-dt ymd-hms    yyyy-MM-dd HH:mm:ss" << std::endl
        << "-dt ymd-hm     yyyy-MM-dd HH:mm" << std::endl
        << "-dt ymd-h      yyyy-MM-dd HH" << std::endl
        << "-dt ymd-       yyyy-MM-dd" << std::endl
        << "-dt ymd/hms.f  yyyy/MM/dd HH:mm:ss.fffffff" << std::endl
        << "-dt ymd/hms    yyyy/MM/dd HH:mm:ss" << std::endl
        << "-dt ymd/hm     yyyy/MM/dd HH:mm" << std::endl
        << "-dt ymd/h      yyyy/MM/dd HH" << std::endl
        << "-dt ymd/       yyyy/MM/dd" << std::endl
        << std::endl
        << "-v|--verbose: default is false" << std::endl
        << std::endl;
    return -1;
}

static bool hasSuffix(const std::string& str, const std::string& suffix)
{
    const auto size = str.size();
    const auto suffixSize = suffix.size();

    return size >= suffixSize &&
        str.compare(size - suffixSize, suffixSize, suffix) == 0;
}

static std::vector<std::string> split(const std::string& phrase, const std::string& delimiter)
{
    size_t pos;
    std::vector<std::string> list;
    std::string s{ phrase };

    while ((pos = s.find(delimiter)) != std::string::npos) {
        list.push_back(s.substr(0, pos));
        s.erase(0, pos + delimiter.length());
    }

    list.push_back(s);
    return list;
}

static bool malformedPatternError(const std::string& pattern)
{
    std::cerr << "file pattern should have either ':'-separated file and instrument path or end with .h5: " << pattern << std::endl;
    return false;
}

static bool parsePattern(const std::string& fileAndPath, std::string& fileName, std::string& instrumentPath, std::string& datasetName)
{
    const auto parts = split(fileAndPath, h5Delimiter);
    if (parts.size() == 1U) {
        if (!hasSuffix(parts[0], h5Suffix)) {
            return malformedPatternError(fileAndPath);
        }
        fileName = parts[0];
    }
    else if (parts.size() == 2U) {
        fileName = parts[0] + h5Suffix;

        // at this point instrument path is without starting slash
        instrumentPath = parts[1];
        if (!instrumentPath.empty()) {
            const std::size_t pos = instrumentPath.find_last_of(h5Slash);
            if (pos != std::string::npos) {
                datasetName = instrumentPath.substr(pos + 1);
                instrumentPath = instrumentPath.substr(0, pos);
            }
            instrumentPath = h5Slash + instrumentPath;
        }
    }
    else {
        return malformedPatternError(fileAndPath);
    }

    return true;
}

static bool parseDateTimeFormat(const std::string& format, DateTimeFormat& dateTimeFormat)
{
    if (format.empty() || format == "ymd-hms.f") {
        // noop
    }
    else if (format == "ymd/hms.f") {
        dateTimeFormat = DateTimeFormat::YmdhmsfSlash;
    }
    else if (format == "ymd-") {
        dateTimeFormat = DateTimeFormat::YmdDash;
    }
    else if (format == "ymd/") {
        dateTimeFormat = DateTimeFormat::YmdSlash;
    }
    else if (format == "ymd-hms") {
        dateTimeFormat = DateTimeFormat::YmdhmsDash;
    }
    else if (format == "ymd/hms") {
        dateTimeFormat = DateTimeFormat::YmdhmsSlash;
    }
    else if (format == "ymd-hm") {
        dateTimeFormat = DateTimeFormat::YmdhmDash;
    }
    else if (format == "ymd/hm") {
        dateTimeFormat = DateTimeFormat::YmdhmSlash;
    }
    else if (format == "ymd-h") {
        dateTimeFormat = DateTimeFormat::YmdhDash;
    }
    else if (format == "ymd/h") {
        dateTimeFormat = DateTimeFormat::YmdhSlash;
    }
    else {
        std::cerr << "unknown date-time format " << format << std::endl;
        return false;
    }

    return true;
}

//! the length of the buffer is mbc::time::DotNetTicks::toStringFullBufferLength
static const char* formatTicks(char* buffer, const DateTimeFormat format, const long long ticks)
{
    switch (format)
    {
        case DateTimeFormat::YmdDash:
            return mbc::time::DotNetTicks::toStringDate(buffer, mbc::time::DotNetTicks::toStringFullBufferLength, ticks);
        case DateTimeFormat::YmdSlash:
            return mbc::time::DotNetTicks::toStringDate(buffer, mbc::time::DotNetTicks::toStringFullBufferLength, ticks, dateSlash);
        case DateTimeFormat::YmdhmsfDash:
            return mbc::time::DotNetTicks::toStringFull(buffer, mbc::time::DotNetTicks::toStringFullBufferLength, ticks);
        case DateTimeFormat::YmdhmsfSlash:
            return mbc::time::DotNetTicks::toStringFull(buffer, mbc::time::DotNetTicks::toStringFullBufferLength, ticks, dateSlash);
        case DateTimeFormat::YmdhmsDash:
            return mbc::time::DotNetTicks::toStringSeconds(buffer, mbc::time::DotNetTicks::toStringFullBufferLength, ticks);
        case DateTimeFormat::YmdhmsSlash:
            return mbc::time::DotNetTicks::toStringSeconds(buffer, mbc::time::DotNetTicks::toStringFullBufferLength, ticks, dateSlash);
        case DateTimeFormat::YmdhmDash:
            return mbc::time::DotNetTicks::toStringMinutes(buffer, mbc::time::DotNetTicks::toStringFullBufferLength, ticks);
        case DateTimeFormat::YmdhmSlash:
            return mbc::time::DotNetTicks::toStringMinutes(buffer, mbc::time::DotNetTicks::toStringFullBufferLength, ticks, dateSlash);
        case DateTimeFormat::YmdhDash:
            return mbc::time::DotNetTicks::toStringHours(buffer, mbc::time::DotNetTicks::toStringFullBufferLength, ticks);
        case DateTimeFormat::YmdhSlash:
            return mbc::time::DotNetTicks::toStringHours(buffer, mbc::time::DotNetTicks::toStringFullBufferLength, ticks, dateSlash);
        default:
            std::cerr << "unknown date-time format: " << static_cast<int>(format) << std::endl;
            return "???";
    }
}

static std::string createFileName(const mbc::h5::DataInfo& dataInfo)
{
    // /stock/AMS_TNLA_US17275R1023/c_d01 -> stock-AMS_TNLA_US17275R1023-c_d01.csv
    std::string csvFileName{ dataInfo.path() };
    csvFileName += ".csv";
    if (csvFileName[0] == h5Slash)
        csvFileName = csvFileName.substr(1U);
    std::replace(csvFileName.begin(), csvFileName.end(), h5Slash, '-');
    //std::replace(csvFileName.begin(), csvFileName.end(), '*', 's');
    //std::replace(csvFileName.begin(), csvFileName.end(), '?', 'q');
    return csvFileName;
}

static void configureNumericFormatting(std::ofstream& out)
{
    out << std::setprecision(std::numeric_limits<double>::digits10 + 1) << std::defaultfloat;
    struct numeric final : std::numpunct<char>
    {
        [[nodiscard]] char do_decimal_point() const override { return '.'; }
        [[nodiscard]] std::string do_grouping() const override { return ""; }
    };

    out.imbue(std::locale(std::locale::classic(), new numeric));
}

static bool failedToOpenData(const char* type, const std::string& h5FileName, const mbc::h5::DataInfo& dataInfo)
{
    std::cerr << "failed to open " << type << " data from " << h5FileName << ":" << dataInfo.path() << std::endl;
    return false;
}

static bool failedToExportData(const char* type, const std::string& csvFileName)
{
    std::cerr << "failed to export " << type << " data to the csv file stream " << csvFileName << std::endl;
    return false;
}

static void failedToWriteData(const char* type, const std::string& csvFileName)
{
    std::cerr << "failed to write " << type << " data to the csv file stream " << csvFileName << std::endl;
}

static bool exportDataset(const std::shared_ptr<mbc::h5::Instrument>& instrument, const mbc::h5::DataInfo& dataInfo, DateTimeFormat format, std::string& h5FileName, bool verbose)
{
    if (verbose) {
        std::cout << "exporting from " << h5FileName << ":" << dataInfo.path() << std::endl;
    }
    const std::string csvFileName{ createFileName(dataInfo) };
    std::ofstream out(csvFileName);
    if (out.fail()) {
        std::cerr << "failed to open csv file stream " << csvFileName << std::endl;
        return false;
    }
    configureNumericFormatting(out);

    char timeBuf[mbc::time::DotNetTicks::toStringFullBufferLength];
    bool status{ false };
    switch (dataInfo.parsedDataType())
    {
        case mbc::h5::DataType::Trade:
        {
            const char* type{ "trade" };
            std::shared_ptr<mbc::h5::TradeData> data = instrument->openTradeData();
            if (data == nullptr) {
                out.close();
                return failedToOpenData(type, h5FileName, dataInfo);
            }
            status = data->fetchAll([&out, &timeBuf, &format, &type, &csvFileName](const mbc::h5::Trade* ptr, unsigned long long cnt) noexcept {
                try {
                    while (cnt-- > 0UL) {
                        out << formatTicks(timeBuf, format, ptr->ticks_);
                        out << ";" << ptr->price_ << ";" << ptr->volume_ << std::endl;
                        ++ptr;
                    }
                }
                catch (...) {
                    failedToWriteData(type, csvFileName);
                }
            });
            data->close();
            out.close();
            if (!status)
                return failedToExportData(type, csvFileName);
            break;
        }
        case mbc::h5::DataType::TradePriceOnly:
        {
            const char* type{ "trade price-only" };
            std::shared_ptr<mbc::h5::TradePriceOnlyData> data = instrument->openTradePriceOnlyData();
            if (data == nullptr) {
                out.close();
                return failedToOpenData(type, h5FileName, dataInfo);
            }
            status = data->fetchAll([&out, &timeBuf, &format, &type, &csvFileName](const mbc::h5::TradePriceOnly* ptr, unsigned long long cnt) noexcept {
                try {
                    while (cnt-- > 0UL) {
                        out << formatTicks(timeBuf, format, ptr->ticks_);
                        out << ";" << ptr->price_ << std::endl;
                        ++ptr;
                    }
                }
                catch (...) {
                    failedToWriteData(type, csvFileName);
                }
            });
            data->close();
            out.close();
            if (!status)
                return failedToExportData(type, csvFileName);
            break;
        }
        case mbc::h5::DataType::Quote:;
        {
            const char* type{ "quote" };
            std::shared_ptr<mbc::h5::QuoteData> data = instrument->openQuoteData();
            if (data == nullptr) {
                out.close();
                return failedToOpenData(type, h5FileName, dataInfo);
            }
            status = data->fetchAll([&out, &timeBuf, &format, &type, &csvFileName](const mbc::h5::Quote* ptr, unsigned long long cnt) noexcept {
                try {
                    while (cnt-- > 0UL) {
                        out << formatTicks(timeBuf, format, ptr->ticks_);
                        out << ";" << ptr->askPrice_ << ";" << ptr->bidPrice_ << ";" << ptr->askSize_ << ";" << ptr->bidSize_ << std::endl;
                        ++ptr;
                    }
                }
                catch (...) {
                    failedToWriteData(type, csvFileName);
                }
            });
            data->close();
            out.close();
            if (!status)
                return failedToExportData(type, csvFileName);
            break;
        }
        case mbc::h5::DataType::QuotePriceOnly:
        {
            const char* type{ "quote price-only" };
            std::shared_ptr<mbc::h5::QuotePriceOnlyData> data = instrument->openQuotePriceOnlyData();
            if (data == nullptr) {
                out.close();
                return failedToOpenData(type, h5FileName, dataInfo);
            }
            status = data->fetchAll([&out, &timeBuf, &format, &type, &csvFileName](const mbc::h5::QuotePriceOnly* ptr, unsigned long long cnt) noexcept {
                try {
                    while (cnt-- > 0UL) {
                        out << formatTicks(timeBuf, format, ptr->ticks_);
                        out << ";" << ptr->askPrice_ << ";" << ptr->bidPrice_ << std::endl;
                        ++ptr;
                    }
                }
                catch (...) {
                    failedToWriteData(type, csvFileName);
                }
            });
            data->close();
            out.close();
            if (!status)
                return failedToExportData(type, csvFileName);
            break;
        }
        case mbc::h5::DataType::Ohlcv:
        {
            const char* type{ "ohlcv" };
            std::shared_ptr<mbc::h5::OhlcvData> data = instrument->openOhlcvData(dataInfo.parsedOhlcvKind(), dataInfo.parsedTimeFrame());
            if (data == nullptr) {
                out.close();
                return failedToOpenData(type, h5FileName, dataInfo);
            }
            status = data->fetchAll([&out, &timeBuf, &format, &type, &csvFileName](const mbc::h5::Ohlcv* ptr, unsigned long long cnt) noexcept {
                try {
                    while (cnt-- > 0UL) {
                        out << formatTicks(timeBuf, format, ptr->ticks_);
                        out << ";" << ptr->open_ << ";" << ptr->high_ << ";" << ptr->low_ << ";" << ptr->close_ << ";" << ptr->volume_ << std::endl;
                        ++ptr;
                    }
                }
                catch (...) {
                    failedToWriteData(type, csvFileName);
                }
            });
            data->close();
            out.close();
            if (!status)
                return failedToExportData(type, csvFileName);
            break;
        }
        case mbc::h5::DataType::OhlcvPriceOnly:
        {
            const char* type{ "ohlcv price-only" };
            std::shared_ptr<mbc::h5::OhlcvPriceOnlyData> data = instrument->openOhlcvPriceOnlyData(dataInfo.parsedOhlcvKind(), dataInfo.parsedTimeFrame());
            if (data == nullptr) {
                out.close();
                return failedToOpenData(type, h5FileName, dataInfo);
            }
            status = data->fetchAll([&out, &timeBuf, &format, &type, &csvFileName](const mbc::h5::OhlcvPriceOnly* ptr, unsigned long long cnt) noexcept {
                try {
                    while (cnt-- > 0UL) {
                        out << formatTicks(timeBuf, format, ptr->ticks_);
                        out << ";" << ptr->open_ << ";" << ptr->high_ << ";" << ptr->low_ << ";" << ptr->close_ << std::endl;
                        ++ptr;
                    }
                }
                catch (...) {
                    failedToWriteData(type, csvFileName);
                }
            });
            data->close();
            out.close();
            if (!status)
                return failedToExportData(type, csvFileName);
            break;
        }
        case mbc::h5::DataType::Scalar:
        {
            const char* type{ "scalar" };
            std::shared_ptr<mbc::h5::ScalarData> data = instrument->openScalarData(dataInfo.parsedTimeFrame());
            if (data == nullptr) {
                out.close();
                return failedToOpenData(type, h5FileName, dataInfo);
            }
            status = data->fetchAll([&out, &timeBuf, &format, &type, &csvFileName](const mbc::h5::Scalar* ptr, unsigned long long cnt) noexcept {
                try {
                    while (cnt-- > 0UL) {
                        out << formatTicks(timeBuf, format, ptr->ticks_);
                        out << ";" << ptr->value_ << std::endl;
                        ++ptr;
                    }
                }
                catch (...) {
                    failedToWriteData(type, csvFileName);
                }
            });
            data->close();
            out.close();
            if (!status)
                return failedToExportData(type, csvFileName);
            break;
        }
        case mbc::h5::DataType::OhlcvAdjusted:
        {
            const char* type{ "ohlcv adjusted" };
            std::shared_ptr<mbc::h5::OhlcvData> data = instrument->openOhlcvAdjustedData(dataInfo.parsedOhlcvKind(), dataInfo.parsedTimeFrame());
            if (data == nullptr) {
                out.close();
                return failedToOpenData(type, h5FileName, dataInfo);
            }
            status = data->fetchAll([&out, &timeBuf, &format, &type, &csvFileName](const mbc::h5::Ohlcv* ptr, unsigned long long cnt) noexcept {
                try {
                    while (cnt-- > 0UL) {
                        out << formatTicks(timeBuf, format, ptr->ticks_);
                        out << ";" << ptr->open_ << ";" << ptr->high_ << ";" << ptr->low_ << ";" << ptr->close_ << ";" << ptr->volume_ << std::endl;
                        ++ptr;
                    }
                }
                catch (...) {
                    failedToWriteData(type, csvFileName);
                }
            });
            data->close();
            out.close();
            if (!status)
                return failedToExportData(type, csvFileName);
            break;
        }
        case mbc::h5::DataType::OhlcvAdjustedPriceOnly:
        {
            const char* type{ "ohlcv adjusted price-only" };
            std::shared_ptr<mbc::h5::OhlcvPriceOnlyData> data = instrument->openOhlcvAdjustedPriceOnlyData(dataInfo.parsedOhlcvKind(), dataInfo.parsedTimeFrame());
            if (data == nullptr) {
                out.close();
                return failedToOpenData(type, h5FileName, dataInfo);
            }
            status = data->fetchAll([&out, &timeBuf, &format, &type, &csvFileName](const mbc::h5::OhlcvPriceOnly* ptr, unsigned long long cnt) noexcept {
                try {
                    while (cnt-- > 0UL) {
                        out << formatTicks(timeBuf, format, ptr->ticks_);
                        out << ";" << ptr->open_ << ";" << ptr->high_ << ";" << ptr->low_ << ";" << ptr->close_ << std::endl;
                        ++ptr;
                    }
                }
                catch (...) {
                    failedToWriteData(type, csvFileName);
                }
            });
            data->close();
            out.close();
            if (!status)
                return failedToExportData(type, csvFileName);
            break;
        }
        default:
        {
            std::cerr << "unknown data type " << static_cast<int>(dataInfo.parsedDataType()) << std::endl;
        }
    }
    out.close();
    return status;
}

int main(const int argc, const char* argv[])  // NOLINT(bugprone-exception-escape)
{
    const option_parser parser{ argc, argv };
    if (parser.is_help(1U))
        return help();

    const bool verbose{ !parser.get_option("-v", "--verbose").empty() };

    // The first argument is a h5 file name with optional instrument path.
    const std::string& fileAndPath = parser.get_option(0U);
    std::string fileName;
    std::string instrumentPath;
    std::string datasetName;
    if (!parsePattern(fileAndPath, fileName, instrumentPath, datasetName))
        return -1;
    if (verbose) {
        std::cout << "file            : " << fileName << std::endl;
        std::cout << "instrument      : " << (instrumentPath.empty() ? "not specified" : instrumentPath) << std::endl;
        std::cout << "dataset         : " << (datasetName.empty() ? "not specified" : datasetName) << std::endl;
    }

    if (!std::filesystem::exists(fileName)) {
        std::cerr << fileName << " does not exist" << std::endl;
        return -1;
    }
    if (!std::filesystem::is_regular_file(fileName)) {
        std::cerr << fileName << " is not a regular file" << std::endl;
        return -1;
    }

    const std::string& format = parser.get_option("-dt", "--date-time");
    auto dateTimeFormat{ DateTimeFormat::YmdhmsfDash };
    if (!parseDateTimeFormat(format, dateTimeFormat))
        return -1;
    if (verbose) {
        std::cout << "date-time format: " << static_cast<int>(dateTimeFormat) << "  (" << (format.empty() ? "not specified" : format) << ")" << std::endl;
    }

    mbc::h5::File::interceptErrorStack([](const char* message, bool isError) noexcept
    {
        try {
            if (isError)
                std::cerr << message << std::endl;
            else
                std::cout << message << std::endl;
        }
        catch (...) {
        }
    });

    const std::shared_ptr<mbc::h5::File> file = mbc::h5::File::openReadOnly(fileName.c_str());
    if (file == nullptr) {
        std::cerr << "failed to open file " << fileName << "for read-only access" << std::endl;
        return -1;
    }

    std::vector<mbc::h5::DataInfo> dataEnumeratorVector;
    bool status = file->enumerateDatasets([&dataEnumeratorVector](const mbc::h5::DataInfo& dataInfo) noexcept
    {
        try {
            dataEnumeratorVector.push_back(dataInfo);
        }
        catch (...) {
        }
    }, false);
    if (status == false) {
        file->close();
        std::cerr << "failed to enumerate datasets in file " << fileName << std::endl;
        return -1;
    }

    for (auto const& dataInfo : dataEnumeratorVector) {
        std::string path{ dataInfo.path() };
        if (verbose) {
            std::cout << "processing " << fileName << ":" << path << std::endl;
        }
        if (instrumentPath.empty() || path.rfind(instrumentPath, 0) == 0) {
            if (!datasetName.empty() && datasetName.rfind(dataInfo.name(), 0) != 0)
                continue;
            if (!dataInfo.isValidName()) {
                std::cerr << "invalid dataset name " << dataInfo.name() << " in " << fileName << ":" << path << std::endl;
                continue;
            }

            const std::size_t pos = path.find_last_of(h5Slash);
            if (pos != std::string::npos)
                path = path.substr(0, pos);

            const std::shared_ptr<mbc::h5::Instrument> instrument = file->openInstrument(path.c_str());
            if (instrument == nullptr) {
                std::cerr << "failed to open instrument " << fileName << ":" << path << " for read-only access" << std::endl;
                file->close();
                return -1;
            }
            try {
                status = exportDataset(instrument, dataInfo, dateTimeFormat, fileName, verbose);
            }
            catch (...) {
            }
            instrument->close();
            if (!status) {
                std::cerr << "failed to export dataset " << datasetName << " from " <<fileName << ":" << path << std::endl;
                return -1;
            }
        }
    }
}
