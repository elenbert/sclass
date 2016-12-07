/*
 Stars spectral class detector
 Main program entry

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
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "curve_data.h"

extern const char* __progname;

void show_usage()
{
	printf("spectral class usage:\n");
	printf("\t%s -v \"vcurve_file_name\" -b \"bcurve_file_name\" -t bext_val -n vect_val -d=tm\n", __progname);
	printf("\t\t-v, --vcurve\t\t Curve file for V band\n");
	printf("\t\t-b, --bcurve\t\t Curve file for B band\n");
	printf("\t\t-t, --bextinction\t Magnitude extinction factor for B band\n");
	printf("\t\t-n, --vextinction\t Magnitude extinction factor for V band\n");
	printf("\t\t-d, --deltatime\t\t Time delta for searching nearest B-V measurements in minutes. Optional, default is 360 minutes\n\n");
}

int main(int argc, char** argv)
{
	static struct option long_options[] = {
		{ "help", no_argument, NULL, 'h' },
		{ "vcurve", required_argument, NULL, 'v' },
		{ "bcurve", required_argument, NULL, 'b' },
		{ "bextinction", required_argument, NULL, 't' },
		{ "vextinction", required_argument, NULL, 'n' },
		{ "deltatime", required_argument, NULL, 'd' }
	};

	const char *opt_str = "hv:b:t:n:d";

	char* vcurve_file = NULL;
	char* bcurve_file = NULL;

	float bextinction_factor = 0.0;
	float vextinction_factor = 0.0;

	int deltatime_min = 360; // default 6 hours

	int option_index = 0;
	int opt = getopt_long(argc, argv, opt_str, long_options, &option_index);

	char* buf;

	while (opt != -1) {
		switch (opt) {
			case 'h':
				show_usage();
				return 0;

			case 'v':
				vcurve_file = optarg;
				break;

			case 'b':
				bcurve_file = optarg;
				break;

			case 't':
				bextinction_factor = strtod(optarg, NULL);
				break;

			case 'n':
				vextinction_factor = strtod(optarg, NULL);
				break;

			case 'd':
				deltatime_min = atoi(optarg);
				break;

			default:
				show_usage();
				abort();
		}

		opt = getopt_long(argc, argv, opt_str, long_options, &option_index);
	}

	if (!vcurve_file || !bcurve_file || bextinction_factor == 0 || vextinction_factor == 0) {
		printf("Argument(s) required!\n\n");
		show_usage();
		return -1;
	}

	curve_data v_curve_data = { 0, 0 };

	if (read_curve_file(vcurve_file, &v_curve_data) < 0) {
		fprintf(stderr, "Failed to read curve data file for V band\n");
		return -1;
	}

	curve_data b_curve_data = { 0, 0 };

	if (read_curve_file(bcurve_file, &b_curve_data) < 0) {
		fprintf(stderr, "Failed to read curve data file for B band\n");
		clean_curve_data(&v_curve_data);
		return -1;
	}

	clean_curve_data(&b_curve_data);
	clean_curve_data(&v_curve_data);

	return 0;
}

