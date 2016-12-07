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
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "date_utils.h"

long jd_to_unix(const char* jd_str)
{
	double dj1 = strtod(jd_str, NULL);
	double dj2 = 0;

	const double DJMIN = -68569.5;
	const double DJMAX = 1e9;

	long jd, l, n, i, k;
	double dj, d1, d2, f1, f2, f, d;

	dj = dj1 + dj2;
	if (dj < DJMIN || dj > DJMAX) return -1;

   if (dj1 >= dj2) {
      d1 = dj1;
      d2 = dj2;
   } else {
      d1 = dj2;
      d2 = dj1;
   }

   d2 -= 0.5;

/* Separate day and fraction. */
   f1 = fmod(d1, 1.0);
   f2 = fmod(d2, 1.0);
   f = fmod(f1 + f2, 1.0);
   if (f < 0.0) f += 1.0;
   d = floor(d1 - f1) + floor(d2 - f2) + floor(f1 + f2 - f);
   jd = (long) floor(d) + 1L;


   l = jd + 68569L;
   n = (4L * l) / 146097L;
   l -= (146097L * n + 3L) / 4L;
   i = (4000L * (l + 1L)) / 1461001L;
   l -= (1461L * i) / 4L - 31L;
   k = (80L * l) / 2447L;

	int day = (int) (l - (2447L * k) / 80L);

 l = k / 11L;

	int year = (int) (100L * (n - 49L) + i + l);
	int month = (int) (k + 2L - 12L * l);

	printf("year: %i  month: %i  day: %i  fract of the day: %f\n", year, month, day, f);

	return 0;
}

void unix_to_jd(const long unixtime, char* jd_str)
{
	
}

