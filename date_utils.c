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

/*
 * Based on algorithms from the SOFA (Standards Of Fundamental Astronomy) library
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
	return 0;
}

