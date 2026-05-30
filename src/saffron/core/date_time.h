#pragma once

#include "saffron/base.h"

namespace saffron
{
class DateTime
{
	using TimePoint = std::chrono::system_clock::time_point;

public:
	struct Date
	{
		int Seconds;
		int Minutes;
		int Hour;
		int Weekday;
		int Day;
		int Month;
		int Year;
	};

public:
	DateTime();
	explicit DateTime(Date date);
	explicit DateTime(TimePoint timePoint);
	DateTime(int year, int month, int day, int weekday, int hour, int minutes, int seconds);

	auto IsBefore(const DateTime& rhs) const -> bool;
	auto IsAfter(const DateTime& rhs) const -> bool;

	auto Seconds() const -> int;
	auto Minutes() const -> int;
	auto Hour() const -> int;
	auto Weekday() const -> int;
	auto Day() const -> int;
	auto Month() const -> int;
	auto Year() const -> int;

	auto WeekdayString(bool abbreviation) const -> std::string;
	auto MonthString(bool abbreviation) const -> std::string;
	auto TimeString() const -> std::string;
	auto ANSIDateString() const -> std::string;

private:
	void Clamp();

private:
	Date _date{};
};
}
