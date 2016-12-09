/*
 Stars spectral class detector
 Data curve reader and storage

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
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "curve_data.h"
#include "date_utils.h"

int read_curve_file(const char* filename, curve_data* data, const float mag_correction)
{
	printf("\nLoading data from %s\n", filename);

	FILE* fp = fopen(filename, "r");

	if (!fp) {
		fprintf(stderr, "Failed to open file %s , err: %s\n", filename, strerror(errno));
		return -1;
	}

	char* line = NULL;
	size_t len;
	ssize_t read;

	data->ent = (curve_data_entry* ) malloc(sizeof(curve_data_entry));

	if (!data->ent) {
		fprintf(stderr, "Memory allocation failed (curve_data_entry), err: %s\n", strerror(errno));
		fclose(fp);
		return -1;
	}

	while ((read = getline(&line, &len, fp)) != -1) {
		if (strstr(line, "#") != NULL) { // skip comments
			continue;
		}

		char* jd_str = strtok(line, " ");
		char* magnitude = strtok(NULL, " ");

		if (!jd_str || !magnitude) {
			continue;
		}

		data->ent = (curve_data_entry* ) realloc(data->ent, sizeof(curve_data_entry) * (data->ent_num + 1));

		data->ent[data->ent_num].magnitude_a = strtod(magnitude, NULL);
		double jd = strtod(jd_str, NULL);

		if (errno == ERANGE || data->ent[data->ent_num].magnitude_a == 0 || jd == 0) {
			fprintf(stderr, "Failed to parse line: %s Skipping...\n", line);
			continue;
		}

		data->ent[data->ent_num].timestamp = jd_to_unix(jd);
		data->ent[data->ent_num].magnitude_a += mag_correction;
		data->ent[data->ent_num].magnitude_b = 0;

		unix_to_jd(data->ent[data->ent_num].timestamp);

		data->ent_num++;
	}

	fclose(fp);

	printf("Total measurements loaded: %u\n", data->ent_num);

	return !(data->ent_num > 0);
}

void clean_curve_data(curve_data* data)
{
	free(data->ent);
	data->ent = NULL;
}

int is_timediff_in_range(const long timestamp1, const long timestamp2, const int delta)
{
	long timediff = abs(timestamp1 - timestamp2);

	return delta <= timediff;
}

void generate_common_curve_data_set(curve_data* list1, curve_data* list2, curve_data* commlist, const int time_delta)
{
	unsigned int min_entry_num = 0;
	unsigned int search_entry_num = 0;

	curve_data* base_list, * search_list;

	if (list1->ent_num < list2->ent_num) {
		min_entry_num = list1->ent_num;
		search_entry_num = list2->ent_num;
		base_list = list1;
		search_list = list2;
	} else {
		min_entry_num = list2->ent_num;
		search_entry_num = list1->ent_num;
		base_list = list1;
		search_list = list2;
	}

	printf("\nMerging both measurements sets.");
	printf("Total data to process: %u  Time delta for records searching: %i", min_entry_num, time_delta);

	commlist->ent = (curve_data_entry* ) malloc(sizeof(curve_data_entry));

	for (unsigned int i = 0; i < min_entry_num; i++) {
		long tm1 = base_list->ent[i].timestamp;
		float mag1 = base_list->ent[i].magnitude_a;

		long closest_time = 0;
		float closest_sec_mag = 0;

		for (unsigned int k = 0; k < search_entry_num; k++) {
			long tm2 = search_list->ent[k].timestamp;

			long timediff = abs(tm1 - tm2);

			if (timediff <= time_delta) {
				if (closest_time == 0 || tm2 < closest_time) {
					closest_time = tm2;
					closest_sec_mag = search_list->ent[k].magnitude_a;
					continue;
				}
			}			
		}

		if (closest_sec_mag == 0) {
			printf("\nWarning! Unable to find any closest point in specified time range! JD1: %f  MAG1: %f\n", unix_to_jd(tm1), mag1);
		} else {
			commlist->ent = (curve_data_entry* ) realloc(commlist->ent, sizeof(curve_data_entry) * (commlist->ent_num + 1));

			commlist->ent[commlist->ent_num].timestamp = tm1;
			commlist->ent[commlist->ent_num].magnitude_a = mag1;
			commlist->ent[commlist->ent_num].magnitude_b = closest_sec_mag;

			commlist->ent_num++;
			//printf("time 1: %li time 2: %li   mag 1: %f  mag 2: %f\n", tm1, closest_time, mag1, closest_sec_mag);
		}
	}

	printf("\nTotal measurements in work: %u\n", commlist->ent_num);
}

