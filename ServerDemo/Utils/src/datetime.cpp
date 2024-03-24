#include "datetime.h"

#include <ctime>
#include <sys/time.h>

namespace utils
{

Datetime::Datetime(bool isUTC)
{
    gettimeofday(&m_data.tv, nullptr);
    std::tm *tm = isUTC ? std::gmtime(&m_data.tv.tv_sec) : std::localtime(&m_data.tv.tv_sec);

    m_data.year = tm->tm_year + 1900;
    m_data.month = tm->tm_mon + 1;
    m_data.day = tm->tm_mday;
    m_data.hour = tm->tm_hour;
    m_data.minute = tm->tm_min;
    m_data.second = tm->tm_sec;
    m_data.usec = m_data.tv.tv_usec;

    if (m_data.month >= 1 && m_data.month < 4)
    {
        m_data.quarter = 1;
    }
    else if (m_data.month >= 4 && m_data.month < 7)
    {
        m_data.quarter = 2;
    }
    else if (m_data.month >= 7 && m_data.month < 10)
    {
        m_data.quarter = 3;
    }
    else
    {
        m_data.quarter = 4;
    }
}

Datetime::~Datetime()
{}

int Datetime::year() const
{
    return m_data.year;
}

int Datetime::month() const
{
    return m_data.month;
}

int Datetime::day() const
{
    return m_data.day;
}

int Datetime::hour() const
{
    return m_data.hour;
}

int Datetime::minute() const
{
    return m_data.minute;
}

int Datetime::second() const
{
    return m_data.second;
}

int Datetime::usec() const
{
    return m_data.usec;
}

int Datetime::quarter() const
{
    return m_data.quarter;
}

int64_t Datetime::timestamp() const
{
    return m_data.tv.tv_sec;
}

std::string Datetime::ymd() const
{
    char buf[9] = {0};
    sprintf(buf, "%04d%02d%02d", m_data.year, m_data.month, m_data.day);
    return buf;
}

std::string Datetime::ymdhmsT() const
{
    char buf[20] = {0};
    sprintf(buf, "%04d-%02d-%02dT%02d:%02d:%02d", m_data.year, m_data.month, m_data.day, m_data.hour, m_data.minute, m_data.second);
    return buf;
}

std::string Datetime::ymdhmss() const
{
    char buf[27] = {0};
    sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d.%06d", m_data.year, m_data.month, m_data.day, m_data.hour, m_data.minute, m_data.second, m_data.usec);
    return buf;
}

} // namespace utils
