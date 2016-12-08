/*
 Stars spectral class detector
 jd to unixtimestamps and vise versa converter

 Copyright 2016 Oleg Kutkov <kutkov.o@yandex.ru>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <math.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "date_utils.h"

static const double DJMIN = -68569.5;
static const double DJMAX = 1e9;
static const int IYMIN = -4799;
static const double JD_ZERO_POINT = 2400000;

static const int mtab[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/*
 * Jd to yymmdd and vise versa conversions is based on algorithms 
 * from the SOFA (Standards Of Fundamental Astronomy) library
 * Copyright (C) 2016 IAU SOFA Board.
 */

long jd_to_unix(const double jd)
{
	if (jd < DJMIN || jd > DJMAX) {
		return -1;
	}

	double jd2 = -0.5;

	double f1 = fmod(jd, 1.0);
	double f2 = fmod(jd2, 1.0);

	double f = fmod(f1 + f2, 1.0);

	if (f < 0.0) {
		f += 1.0;
	}

	double d = floor(jd - f1) + floor(jd2 - f2) + floor(f1 + f2 - f);

	long l = (long) floor(d) + 1L + 68569L;
	long n = (4L * l) / 146097L;
	l -= (146097L * n + 3L) / 4L;
	long i = (4000L * (l + 1L)) / 1461001L;
	l -= (1461L * i) / 4L - 31L;
	long k = (80L * l) / 2447L;


	struct tm converted_time;
	memset(&converted_time, 0, sizeof(struct tm));

	converted_time.tm_mday = (int) (l - (2447L * k) / 80L);

    l = k / 11L;

	converted_time.tm_year = (int) (100L * (n - 49L) + i + l);
	converted_time.tm_year -= 1900;
	converted_time.tm_mon = (int) (k + 2L - 12L * l);
	converted_time.tm_mon -= 1; // Jan is 0

	converted_time.tm_hour = floor(f * 24.0);

	double min_f = (f * 24.0 - converted_time.tm_hour) * 60.0;
	converted_time.tm_min = floor(min_f);

	double sec_f = (min_f - converted_time.tm_min) * 60.0;
	converted_time.tm_sec = floor(sec_f);

	sec_f -= converted_time.tm_sec;

	if (sec_f > 0.5) {
		++converted_time.tm_sec;
	}

	return (long) timegm(&converted_time);
}

double unix_to_jd(const long unixtime)
{
	time_t tm_time = (time_t) unixtime;
	struct tm* utc_time = gmtime(&tm_time);

	int year = utc_time->tm_year + 1900; // restore epoch

	if (year < IYMIN) {
		return -1; // bad year
	}

	int mon = utc_time->tm_mon + 1; // Jan is 1

	if (mon < 1 || mon > 12) {
		return -2; // bad month
	}

	int leap_year = ((mon == 2) && !(year%4) && (year%100 || !(year%400)));

	/* Validate day, taking into account leap years. */
	if ((utc_time->tm_mday < 1) || (utc_time->tm_mday > (mtab[mon - 1] + leap_year))) {
		return -3; // bad day
	}

	int m_year = (mon - 14) / 12;
	long ym = (long) (m_year + year);

	double djm = (double) ((1461L * (ym + 4800L)) / 4L
							+ (367L * (long) (mon - 2 - 12 * m_year)) / 12L
							- (3L * ((ym + 4900L) / 100L)) / 4L
							+ (long) utc_time->tm_mday - 2432076L);

	djm += JD_ZERO_POINT;  // calculated julian date without time

	double day_frac = utc_time->tm_hour/24.0;

	if (day_frac < 0.0) {
		day_frac += 1.0;
	}

	double frac = day_frac + (utc_time->tm_min + utc_time->tm_sec / 60.0) / 60.0 / 24.0;

	frac = round(frac * 100000) / 100000;

	djm += frac;

	return djm;
}

