#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/unistd.h>

#include <libretro.h>
#include <core_api.h>
#include <argent_loader.h>

extern void frontend_log_cb(enum retro_log_level level, const char *tag, const char *fmt, ...);
extern uint32_t get_time_ms(void);

#define CORE_TYPE_LIBRETRO 0x0001
#define CORE_TYPE_SDL12 0x0002

#define CORE_VER_MAJOR 1
#define CORE_VER_MINOR 0

void *core_buffer = (void*)CORE_LOAD_ADDR;
struct retro_core_t core_api;

static bool check_argent_file(mars_header_t *mars_hdr) {
	// 1. Validate Magic Signature
    if (memcmp(mars_hdr->magic, "MARS", 4) != 0) {
        frontend_log_cb(RETRO_LOG_ERROR, "CORE_LOADER", "Invalid MARS magic signature\n");
        return false;
    }

    // 2. Validate Major Version
    if (mars_hdr->version_major != CORE_VER_MAJOR) {
        frontend_log_cb(RETRO_LOG_ERROR, "CORE_LOADER", "Incompatible core version: got %d.%d, expected %d.x\n", mars_hdr->version_major, mars_hdr->version_minor, CORE_VER_MAJOR);
        return false;
    }

    // 3. Validate Core Type
    if (mars_hdr->core_type != CORE_TYPE_LIBRETRO) {
        frontend_log_cb(RETRO_LOG_ERROR, "CORE_LOADER", "Unsupported core type: 0x%04X\n", mars_hdr->core_type);
        return false;
    }

	return true;
}

bool load_core(const char *core_path) {
    memset(core_buffer, 0, CORE_LOAD_SIZE);  // Clear the 16 MB core section
	FILE *hfile = fopen(core_path, "rb");
	if (!hfile) {
		frontend_log_cb(RETRO_LOG_ERROR, "FRONTEND" ,"Error opening core file=%s\n", core_path);
		return false;
	}

	fseeko(hfile, 0, SEEK_END);
	off_t core_size = ftello(hfile);
	fseeko(hfile, 0, SEEK_SET);

	mars_header_t mars_hdr;
    if (fread(&mars_hdr, 1, sizeof(mars_header_t), hfile) != sizeof(mars_header_t)) {
        frontend_log_cb(RETRO_LOG_ERROR, "CORE_LOADER", "Failed to read core header\n");
        fclose(hfile);
        return false;
    }

    if (!check_argent_file(&mars_hdr)) {
        fclose(hfile);
        return false;
    }
	frontend_log_cb(RETRO_LOG_INFO, "CORE_LOADER", "MARS v%d.%d (Type: 0x%04X) parsed\n", 
                    mars_hdr.version_major, mars_hdr.version_minor, mars_hdr.core_type);

	fseeko(hfile, 0, SEEK_SET);
	size_t bytes_read = fread(core_buffer, 1, core_size, hfile);
	if (bytes_read != (size_t)core_size) {
    	frontend_log_cb(RETRO_LOG_ERROR, "CORE_LOADER", "Failed to read full core file\n");
        fclose(hfile);
        return false;
    }
	fclose(hfile);

	struct frontend_functions_t frontend_funcs = {
//      Logging      //
		.printf = printf,
		.frontend_log_cb = frontend_log_cb,
//      System Calls    //
		._exit = _exit,
		.abort = abort,
		.kill = kill,
		.getpid = getpid,
//      Memory Management     //
		.malloc = malloc,
		.memset = memset,
		.free = free,
		.calloc = calloc,
		.realloc = realloc,
//      Time Functions      //
		.get_time_ms = get_time_ms,
		.gettimeofday = gettimeofday,
		.times = times,
//      File System     //
		.stat = stat,
		.fstat = fstat,
		.access = access,
		.mkdir = mkdir,
		.rmdir = rmdir,
		.rename = rename,
		.chdir = chdir,
		.getcwd = getcwd,
		.unlink = unlink,
//      I/O Operations      //
		.open = open,
		.close = close,
		.write = write,
		.read = read,
		.isatty = isatty,
		.lseek = lseek,
//      Dirent      //
		.opendir = opendir,
		.closedir = closedir,
		.readdir = readdir 
	};

	core_entry_t core_entry = (core_entry_t)(uintptr_t)mars_hdr.entry_point;
	core_api = *core_entry(&frontend_funcs);

	frontend_log_cb(RETRO_LOG_INFO, "CORE_LOADER" ,"Load success\n");
	return true;
}
