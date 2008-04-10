#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <glib.h>
#include "file.h"
#include "debug.h"


int debug_level=0,segv_level=0;
static GHashTable *debug_hash;
static char *gdb_program;

static void sigsegv(int sig)
{
	char buffer[256];
	if (segv_level > 1)
		sprintf(buffer, "gdb -ex bt %s %d", gdb_program, getpid());
	else
		sprintf(buffer, "gdb -ex bt -ex detach -ex quit %s %d", gdb_program, getpid());
	system(buffer);
	exit(1);
}

void
debug_init(const char *program_name)
{
	gdb_program=program_name;
	signal(SIGSEGV, sigsegv);
	debug_hash=g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
}


static void
debug_update_level(gpointer key, gpointer value, gpointer user_data)
{
	if (debug_level < (int) value)
		debug_level=(int) value;
}

void
debug_level_set(const char *name, int level)
{
	debug_level=0;
	if (strcmp(name,"segv")) {
		g_hash_table_insert(debug_hash, g_strdup(name), (gpointer) level);
		g_hash_table_foreach(debug_hash, debug_update_level, NULL);	
	} else {
		segv_level=level;
		if (segv_level)
			signal(SIGSEGV, sigsegv);
		else
			signal(SIGSEGV, NULL);
	}
}

int
debug_level_get(const char *name)
{
	return (int)(g_hash_table_lookup(debug_hash, name));
}

void
debug_vprintf(int level, const char *module, const int mlen, const char *function, const int flen, int prefix, const char *fmt, va_list ap)
{
	char buffer[mlen+flen+3];

	sprintf(buffer, "%s:%s", module, function);
	if (debug_level_get(module) >= level || debug_level_get(buffer) >= level) {
		if (prefix)
			printf("%s:",buffer);
		vprintf(fmt, ap);
	}
}

void
debug_printf(int level, const char *module, const int mlen,const char *function, const int flen, int prefix, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	debug_vprintf(level, module, mlen, function, flen, prefix, fmt, ap);
	va_end(ap);
}


