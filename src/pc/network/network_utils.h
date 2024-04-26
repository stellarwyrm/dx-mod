#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <stdbool.h>
#include "network.h"

u8 network_global_index_from_local(u8 localIndex);
u8 network_local_index_from_global(u8 globalIndex);

bool network_is_server(void);
bool network_is_moderator(void);

u8* network_get_player_text_color(u8 localIndex);
const char* network_get_player_text_color_string(u8 localIndex);

bool network_check_singleplayer_pause(void);

#endif