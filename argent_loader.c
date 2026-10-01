#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/unistd.h>

#include <libretro.h>
#include <core_api.h>
#include <argent_loader.h>

extern void frontend_log_cb(enum retro_log_level level, const char *tag, const char *fmt, ...);
extern uint32_t get_time_ms(void);

void *core_buffer = (void*)CORE_LOAD_ADDR;
struct retro_header_t core_header;
struct retro_core_t core_api;

bool load_core(const char *core_path) {
    memset(core_buffer, 0, CORE_LOAD_SIZE);  // Clear the 16 MB core section
	FILE *hfile = fopen(core_path, "rb");
	if (!hfile) {
		frontend_log_cb(RETRO_LOG_ERROR, "FRONTEND" ,"Error opening core file=%s\n", core_path);
		abort();
	}

	fseeko(hfile, 0, SEEK_END);
	long core_size = ftell(hfile);
	fseeko(hfile, 0, SEEK_SET);

	fread(core_buffer, 1, core_size, hfile);
	fclose(hfile);

	struct frontend_functions_t frontend_funcs = {
		.printf = printf,
		.frontend_log_cb = frontend_log_cb,
		._exit = _exit,
		.abort = abort,
		.malloc = malloc,
		.memset = memset,
		.free = free,
		.calloc = calloc,
		.realloc = realloc,
		.stat = stat,
		.fstat = fstat,
		.kill = kill,
		.getpid = getpid,
		.gettimeofday = gettimeofday,
		.get_time_ms = get_time_ms,
		.open = open,
		.close = close,
		.write = write,
		.read = read,
		.isatty = isatty,
		.lseek = lseek,
		.unlink = unlink,
		.opendir = opendir,
		.closedir = closedir,
		.readdir = readdir 
	};

	core_entry_t core_entry = core_buffer;
	core_header = *core_entry(&frontend_funcs);

	if ((core_header.magic == CORE_API_MAGIC) && (core_header.version == CORE_API_VERSION)) {
		core_api = core_header.core_exports;
		frontend_log_cb(RETRO_LOG_INFO, "CORE_LOADER" ,"Load success\n");
		return true;
	}
	frontend_log_cb(RETRO_LOG_INFO, "CORE_LOADER" ,"Load failed: core is either too old or for another system\n");
	return false;
}
