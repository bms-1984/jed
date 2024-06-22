//  jed, a music database
//  Copyright (C) 2024  Ben M. Sutter
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef __JED_H_
#define __JED_H_
#include <stdio.h>
void error (const char *msg, int code);
int add_release_to_database (FILE *db, char *catalog);
int search_by_catalog (char *catalog);
size_t write_callback (char *data, size_t size, size_t nmemb, void *buffer);
#endif
