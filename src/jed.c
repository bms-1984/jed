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

#include <stdio.h>
#include <getopt.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <jansson.h>

#include "config.h"
#include "gettext.h"
#include "jed.h"
#define _(String) gettext (String)

#ifndef LOCALEDIR
#define LOCALEDIR 0
#endif

static int verbose;
static int dryrun;
static char* catalog;

int main (int argc, char **argv) {
#ifdef ENABLE_NLS
    if (LOCALEDIR == 0)
        error (_("LOCALEDIR not defined, but NLS was built."), 3);
    setlocale (LC_ALL, "");
    bindtextdomain (PACKAGE, LOCALEDIR);
    textdomain (PACKAGE);
#endif
    int c;
    int len;
    while (1) {
        static struct option long_options[] =
            {
                {"verbose", no_argument, 0, 'V'},
                {"version", no_argument, 0, 'v'},
                {"help", no_argument, 0, 'h'},
                {"add", required_argument, 0, 'a'},
                {"dry-run", no_argument, 0, 'd'},
                {0, 0, 0, 0}
            };
        int option_index = 0;
        c = getopt_long (argc, argv, "vhVa:d", long_options, &option_index);
        if (c == -1)
            break;
        switch (c) {
        case 'V':
            verbose = 1;
            break;
        case 'v':
            printf ("%s\n", PACKAGE_STRING);
            exit (0);
        case 'h':
            printf (_("Usage: jed [options...]\n"
                      " -h, --help                       Get help\n"
                      " -v, --version                    Show version\n"
                      " -V, --verbose                    Show more information\n"
                      " -a, --add <catalog_number>       Add release\n"
                      " -d, --dry-run                    Make no changes on disk\n"));
            exit (0);
        case 'd':
            dryrun = 1;
            break;
        case 'a':
            len = strlen (optarg);
            catalog = malloc (len * sizeof(char));
            memcpy (catalog, optarg, len);
            break;
        }
    }

    free (catalog);
    return 0;
}

void error (const char *msg, int code) {
    printf ("error: %s\n", msg);
    exit (code);
}
