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

#ifndef __SPECTRAL_H__
#define __SPECTRAL_H__

#include "curve_data.h"

enum spectral_classes {
	
};

typedef struct spec {
	unsigned int type;
	char* desc;
} spectral_info;

void calculate_spectral_class_for_curve(curve_data* data);

#endif

