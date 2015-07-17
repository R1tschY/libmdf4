/*
 * main.cpp
 *
 *  This file is part of libmdf4.
 *
 *  Copyright (C) 2014 Richard Liebscher <r1tschy@yahoo.de>
 *
 *  libmdf4 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  libmdf4 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <errno.h>
#include <sys/signal.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>

#include "libmdf4.h"
#include "../config.h"

static char* command;

static bool show_file_mdf_version = false;
static bool show_generator = false;
static bool show_comment = false;
static bool show_data_structure = false;

/* GETTEXT */
#if ENABLE_NLS
# include <libintl.h>
#else
# define gettext(msgid) (msgid)
# define textdomain(domain)
# define bindtextdomain(domain, dir)
#endif

#define _(msgid) gettext (msgid)
#define gettext_noop(msgid) msgid
#define N_(msgid) gettext_noop (msgid)

static void usage() {
  printf(
      _("Usage: %s FILE\n"
          "-h, --help              print this help\n"
          "    --version           print current version\n")
          , command);
}

static void version(void) {
  printf("mdf4-info/" PACKAGE_VERSION " %s\n", mdf::version());
}

int main(int argc, char *argv[]) {
  int option_index = 1;
  static const char short_options[] = "chV";
  static const struct option long_options[] = {
      {"help", 0, 0, 'h'},
      {"version", 0, 0, 'V'},
      {"comment", 0, 0, 'c'},
      {0, 0, 0, 0}
  };
  int c;

#ifdef ENABLE_NLS
  setlocale(LC_ALL, "");
  textdomain(PACKAGE);
#endif

  command = argv[0];

  while ((c = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
    switch (c) {
    case 'h':
      usage();
      return 0;

    case 'V':
      version();
      return 0;

    case 'c':
      show_comment = true;
      break;

    default:
      fprintf(stderr, _("Try `%s --help' for more information.\n"), command);
      return 1;
    }
  }

  if (argc - optind != 1) {
    usage();
    return 0;
  }

  if (!show_file_mdf_version && !show_generator && !show_comment && !show_data_structure) {
    show_file_mdf_version = true;
    show_generator = true;
    show_comment = true;
    show_data_structure = true;
  }

  mdf::file mdf_file(argv[optind]);

  if (show_file_mdf_version)
    printf("Version: %s\n", mdf_file.get_mdf_version_string().c_str());

  if (show_generator)
    printf("Generator: %s\n", mdf_file.get_generator_name().c_str());

  if (show_comment)
    printf("Comment: \n%s\n", mdf_file.get_comment().c_str());

  if (show_data_structure) {
    int i = 0;
    for (const auto& dg : mdf_file.get_data_groups()) {
      printf("Data Group %d\n", ++i);
      printf("\tMetadata Comment: \n%s\n", dg.get_metadata_comment().c_str());

      int j = 0;
      for (const auto& cg : dg.get_channel_groups()) {
        printf("\tChannel Group %d\n", ++j);

        int k = 0;
        for (const auto& ch : cg.get_channels()) {
          printf("\tChannel %d: %s\n", ++k, ch.get_name().c_str());
          if (ch.get_source_information()) {
            printf("\t\tSource Name: %s\n", ch.get_source_information()->get_name().c_str());
            printf("\t\tSource Path: %s\n", ch.get_source_information()->get_path().c_str());
            printf("\t\tSource Metadata Comment: \n%s\n", ch.get_source_information()->get_metadata_comment().c_str());
          }
          printf("\t\tMetadata Unit: %s\n", ch.get_metadata_unit().c_str());
          printf("\t\tMetadata Comment: \n%s\n", ch.get_metadata_comment().c_str());
        }
      }
    }
  }

  return EXIT_SUCCESS;
}
