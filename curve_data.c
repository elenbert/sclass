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

int read_curve_file(const char* filename, curve_data* data)
{
	FILE* fp = fopen(filename, "r");

	if (!fp) {
		fprintf(stderr, "Failed to open file %s , err: %s\n", filename, strerror(errno));
		return -1;
	}

	char* line = NULL;
	size_t len;
	ssize_t read;

	while ((read = getline(&line, &len, fp)) != -1) {
		
	}

	fclose(fp);

	return 0;
}

void clean_curve_data(curve_data* data)
{
	
}

