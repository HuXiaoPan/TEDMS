#include "datetime.h"
#pragma warning(disable : 4996)
namespace utils
{
    Datetime::Datetime(bool isUTC)
    {
        timespec_get(&m_data.ts, TIME_UTC);

        //std::tm* tm;
        //time_t current = time(0);
        //char time[26];
        //ctime_s(time, sizeof time, &current);
        //// 把 time 转换为 tm 结构
        //tm gmtm;
        //gmtime_s(&gmtm, &current);       //将time_t类型的时间转换为struct tm类型的时间，存入gmtm
        //strftime(time, sizeof time, "%Y-%m-%d %H:%M:%S", &gmtm);
        std::tm* tm = isUTC ? std::gmtime((const time_t*)&m_data.ts.tv_sec) : std::localtime((const time_t*)&m_data.ts.tv_sec);
        m_data.year = tm->tm_year + 1900;
        m_data.month = tm->tm_mon + 1;
        m_data.day = tm->tm_mday;
        m_data.hour = tm->tm_hour;
        m_data.minute = tm->tm_min;
        m_data.second = tm->tm_sec;
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
    int Datetime::quarter() const
    {
        return m_data.quarter;
    }
    int64_t Datetime::timestamp() const
    {
        return m_data.ts.tv_sec;
    }
    std::string Datetime::ymd() const
    {
        char buf[9] = { 0 };
        sprintf_s(buf, "%04d%02d%02d", m_data.year, m_data.month, m_data.day);
        return buf;
    }
    std::string Datetime::ymdhmsT() const
    {
        char buf[20] = { 0 };
        sprintf_s(buf, "%04d-%02d-%02dT%02d:%02d:%02d", m_data.year, m_data.month, m_data.day, m_data.hour, m_data.minute, m_data.second);
        return buf;
    }
    std::string Datetime::ymdhmss() const
    {
        char buf[27] = { 0 };
        sprintf_s(buf, "%04d-%02d-%02d %02d:%02d:%02d.%06d", m_data.year, m_data.month, m_data.day, m_data.hour, m_data.minute, m_data.second, int(m_data.ts.tv_nsec / 1000000));
        return buf;
    }
} // namespace utils