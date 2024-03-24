#ifndef UTILS_DATETIME_H
#define UTILS_DATETIME_H

#include <string>

namespace utils
{

class Datetime
{
public:
    explicit Datetime(bool isUTC = false);
    virtual ~Datetime();

    int year() const;
    int month() const;
    int day() const;
    int hour() const;
    int minute() const;
    int second() const;
    int usec() const;
    int quarter() const;
    int64_t timestamp() const;

    std::string ymd() const;
    std::string ymdhmsT() const;
    std::string ymdhmss() const;

private:
    struct
    {
        int year;
        int month;
        int day;
        int hour;
        int minute;
        int second;
        int usec;
        int quarter;
        struct timeval tv;
    } m_data;
};

} // namespace utils

#endif // UTILS_DATETIME_H