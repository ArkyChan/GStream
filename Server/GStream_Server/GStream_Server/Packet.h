#ifndef PAKCETS_H
#define PACKETS_H

struct pk_ping {
		
};
struct pk_handshake_in {
	uint8_t ip[4];
	uint8_t tcpPort;
	uint8_t udpPort;
};
struct pk_handshake_out {
	uint8_t tcpPort;
	uint8_t udpPort;
};
struct pk_mousepos {
	uint32_t x;
	uint32_t y;
};
struct pk_keypress {
	uint8_t key_count;
	uint8_t* keys;
};
struct pk_hello {
	uint32_t message_length;
	char* msg;
};
struct pk_error {
	uint32_t error_code;
	uint32_t message_length;
	char* msg;
};
struct pk_frame {
	uint32_t img_length;
	uint32_t sound_length;
	uint8_t* img_data;
	uint16_t* sound_data;
};
struct pk_login_in {

};
struct pk_login_out {
	uint32_t session_id;
};
struct pk_cl_setup_in {
	uint32_t session_id;
};
struct game_ {
	char name[55];
};
struct pk_cl_setup_out {
	uint32_t screen_w;
	uint32_t screen_h;
	uint8_t bit_depth;
	uint32_t sample_rate;
	uint8_t channels;
	uint8_t game_count;
	game_* games;
};
struct pk_stats_in {
	uint32_t session_id;
};
struct pk_stats_out {
	uint32_t link_speed;
	uint8_t ping;
	uint8_t stats[2];
};
struct pk_game_start {
	uint8_t game_id;
};
struct pk_cmd {
	uint32_t session_id;
	uint8_t cmd_code;
};
#endif