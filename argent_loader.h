#ifndef ARGENT_LOADER_H__
#define ARGENT_LOADER_H__

#define CORE_LOAD_ADDR 0x87000000
#define CORE_LOAD_SIZE 0x01000000
#define CORES_DIRECTORY "/media/mmcblk0p2/system/Phobos/cores"
#define MAXPATH 512

extern struct retro_core_t core_api;
bool load_core(const char *file_path);

#endif // ARGENT_LOADER_H__
