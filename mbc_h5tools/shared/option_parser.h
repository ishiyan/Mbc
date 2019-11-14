#pragma once

#include <string>
#include <vector>

class option_parser
{
public:
    option_parser(const int& argc, const char** argv)
    {
        for (int i = 1; i < argc; ++i)
            this->tokens_.emplace_back(argv[i]);
    }

    /*
    usage:

    const std::string& filename = parser.get_option("-f");
    if (!filename.empty()) {
        // Do interesting things ...
        std::cout << "-f found: " << filename << std::endl;
    }
    */

    [[nodiscard]] const std::string& get_option(const char* option) const
    {
        auto itr = std::find(this->tokens_.begin(), this->tokens_.end(), option);
        if (itr != this->tokens_.end() && ++itr != this->tokens_.end())
        {
            return *itr;
        }
        return empty_string_;
    }

    [[nodiscard]] const std::string& get_option(const char* optionShort, const char* optionLong) const
    {
        auto itr = std::find(this->tokens_.begin(), this->tokens_.end(), optionShort);
        if (itr != this->tokens_.end() && ++itr != this->tokens_.end())
            return *itr;
        itr = std::find(this->tokens_.begin(), this->tokens_.end(), optionLong);
        if (itr != this->tokens_.end() && ++itr != this->tokens_.end())
            return *itr;
        return empty_string_;
    }

    [[nodiscard]] const std::string& get_option(unsigned int index) const
    {
        if (index >= count())
            return empty_string_;
        return tokens_[index];
    }

    [[nodiscard]] bool is_option(const char* option) const
    {
        return std::find(this->tokens_.begin(), this->tokens_.end(), option) != this->tokens_.end();
    }

    [[nodiscard]] bool is_option(const char* optionShort, const char* optionLong) const
    {
        if (std::find(this->tokens_.begin(), this->tokens_.end(), optionShort) != this->tokens_.end())
            return true;

        return std::find(this->tokens_.begin(), this->tokens_.end(), optionLong) != this->tokens_.end();
    }

    [[nodiscard]] bool is_help(const unsigned int minOptionCount = 0U) const
    {
        return tokens_.size() < minOptionCount
            || is_option("-h")
            || is_option("--help");
    }

    [[nodiscard]] unsigned int count() const
    {
        return static_cast<unsigned int>(tokens_.size());
    }

private:
    const std::string empty_string_;
    std::vector<std::string> tokens_;
};
