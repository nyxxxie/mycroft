/*****************************************************************************\
Copyright (C) 2016-2017 Nyxxie <github.com/nyxxxie>

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.
This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
Street, Fifth Floor, Boston, MA  02110-1301, USA.
\*****************************************************************************/

#ifndef MYCROFT_PLUGIN_H
#define MYCROFT_PLUGIN_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mc_plugin_entry_t mc_plugin_entry_t;

int mc_plugin_init();
int mc_plugin_close();

int mc_plugin_run_plugins();
int mc_plugin_addpath(const char* path);

int mc_plugin_load(const char* path);
int mc_plugin_unload(const char* path);
int mc_plugin_getloaded(const char* path);

#ifdef __cplusplus
}
#endif

#endif // MYCROFT_PLUGIN_H
