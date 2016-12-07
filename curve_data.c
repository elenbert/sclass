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
	FILE* fp = fopen(filename, "r");

	if (!fp) {
		fprintf(stderr, "Failed to open file %s , err: %s\n", filename, strerror(errno));
		return -1;
	}

	char* line = NULL;
	size_t len;
	ssize_t read;

	data->ent = (curve_data_entry* ) malloc(sizeof(curve_data_entry));

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

		data->ent[data->ent_num].magnitude = strtod(magnitude, NULL);

		if (errno == ERANGE || data->ent[data->ent_num].magnitude == 0) {
			fprintf(stderr, "Failed to parse line: %s Skipping...\n", line);
			continue;
		}

		data->ent[data->ent_num].timestamp = jd_to_unix(jd_str);
		data->ent[data->ent_num].magnitude += mag_correction;

		data->ent_num++;
	}

	fclose(fp);

	return 0;
}

void clean_curve_data(curve_data* data)
{
	free(data->ent);
	data->ent = NULL;
}

