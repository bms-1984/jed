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
static FILE* database;
static CURL* handle;

struct buffer {
    char *response;
    size_t size;
};

struct buffer response;

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
                {"output", required_argument, 0, 'o'},
                {0, 0, 0, 0}
            };
        int option_index = 0;
        c = getopt_long (argc, argv, "vhVa:do:", long_options, &option_index);
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
                      " -d, --dry-run                    Make no changes on disk\n"
                      " -o, --output <file>              Use a database other than the default\n"));
            exit (0);
        case 'd':
            dryrun = 1;
            break;
        case 'a':
            len = strlen (optarg);
            catalog = malloc (len * sizeof(char));
            memcpy (catalog, optarg, len);
            break;
        case 'o':
            database = fopen (optarg, "a+");
            break;
        }
    }
    if (!database)
        database = fopen ("~/.jed/db.json", "a+");
    handle = curl_easy_init ();
    curl_easy_setopt (handle, CURLOPT_HEADER, 0L);
    curl_easy_setopt (handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt (handle, CURLOPT_WRITEDATA, (void *)&response);
    if (catalog) {
        int err = add_release_to_database (database, catalog);
        if (err)
            error (_("could not add release to database"), err);
    }
    free (catalog);
    fclose (database);
    curl_easy_cleanup (handle);
    return 0;
}

void error (const char *msg, int code) {
    printf (_("error: %s\n"), msg);
    exit (code);
}

size_t write_callback (char *data, size_t size, size_t nmemb, void *buffer) {
    size_t realsize = size * nmemb;
    struct buffer *buf = (struct buffer *)buffer;
    char *ptr = realloc (buf->response, buf->size + realsize + 1);
    if (!ptr)
        return 0;
    buf->response = ptr;
    memcpy (&(buf->response[buf->size]), data, realsize);
    buf->size += realsize;
    buf->response[buf->size] = 0;

    return realsize;
}

int search_by_catalog (char *catalog) {
    char *url = malloc (200);
    sprintf (url, "https://api.discogs.com/database/search?catno=%s&key=%s&secret=%s", catalog, KEY, SECRET);
    curl_easy_setopt(handle, CURLOPT_URL, url);
    CURLcode res = curl_easy_perform (handle);
    free (url);
    return res;
}

int add_release_to_database (FILE *db, char *catalog) {
    int err = search_by_catalog (catalog);
    if (err)
        error(_("catalog number search failed"), err);
    for (int i = 0; i < (int) response.size; i++)
        fputc(response.response[i], db);
    return 0;
}
