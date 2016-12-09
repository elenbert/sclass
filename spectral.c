/*
 Stars spectral class detector
 Spectral calculation routines

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

#include <stdio.h>
#include "spectral.h"

static const int COLOR_TEMP_BASE = 4600;

static inline int get_color_index(const float b_v)
{
	// using Ballesteros' formula
	return COLOR_TEMP_BASE * ((1/(0.92 * b_v + 1.7)) + (1/(0.92 * b_v + 0.62)));
}

void calculate_spectral_class_for_curve(curve_data* data)
{
	unsigned int mag_count = data->ent_num;
	float total_magnitude_v = 0;
	float total_magnitude_b = 0;

	for (unsigned int i = 0; i < mag_count; i++) {
		total_magnitude_v += data->ent[i].magnitude_a;
		total_magnitude_b += data->ent[i].magnitude_b;
	}

	float median_magnitude_v = total_magnitude_v / mag_count;
	float median_magnitude_b = total_magnitude_b / mag_count;

	printf("Median magnitude for V: %f\n", median_magnitude_v);
	printf("Median magnitude for B: %f\n", median_magnitude_b);

	float mag_diff = median_magnitude_b - median_magnitude_v;

	printf("Magnitudes difference: %f\n", mag_diff);

	int color_idx = get_color_index(mag_diff);

	printf("Star color temperature: %i K\n", color_idx);
}

