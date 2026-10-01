#include <libretro.h>
#include <core_api.h>

extern struct retro_core_t core_api;

void retro_init(void) {
    core_api.retro_init();
}

void retro_deinit(void) {
    core_api.retro_deinit();
}

unsigned retro_api_version(void) {
    return core_api.retro_api_version();
}

void retro_get_system_info(struct retro_system_info *info) {
    core_api.retro_get_system_info(info);
}

void retro_get_system_av_info(struct retro_system_av_info *info) {
    core_api.retro_get_system_av_info(info);
}

void retro_set_environment(retro_environment_t cb) {
    core_api.retro_set_environment(cb);
}

void retro_set_video_refresh(retro_video_refresh_t cb) {
    core_api.retro_set_video_refresh(cb);
}

void retro_set_audio_sample(retro_audio_sample_t cb) {
    core_api.retro_set_audio_sample(cb);
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb) {
    core_api.retro_set_audio_sample_batch(cb);
}

void retro_set_input_poll(retro_input_poll_t cb) {
    core_api.retro_set_input_poll(cb);
}

void retro_set_input_state(retro_input_state_t cb) {
    core_api.retro_set_input_state(cb);
}

void retro_set_controller_port_device(unsigned port, unsigned device) {
    core_api.retro_set_controller_port_device(port, device);
}

void retro_reset(void) {
    core_api.retro_reset();
}

void retro_run(void) {
    core_api.retro_run();
}

size_t retro_serialize_size(void) {
    return core_api.retro_serialize_size();
}

bool retro_serialize(void *data, size_t size) {
    return core_api.retro_serialize(data, size);
}

bool retro_unserialize(const void *data, size_t size) {
    return core_api.retro_unserialize(data, size);
}

void retro_cheat_reset(void) {
    core_api.retro_cheat_reset();
}

void retro_cheat_set(unsigned index, bool enabled, const char *code) {
    core_api.retro_cheat_set(index, enabled, code);
}

bool retro_load_game(const struct retro_game_info *info) {
    return core_api.retro_load_game(info);
}

bool retro_load_game_special(unsigned game_type, const struct retro_game_info *info, size_t num_info) {
    return core_api.retro_load_game_special(game_type, info, num_info);
}

void retro_unload_game(void) {
    core_api.retro_unload_game();
}

unsigned retro_get_region(void) {
    return core_api.retro_get_region();
}

void *retro_get_memory_data(unsigned id) {
    return core_api.retro_get_memory_data(id);
}

size_t retro_get_memory_size(unsigned id) {
    return core_api.retro_get_memory_size(id);
}