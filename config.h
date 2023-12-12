//
// Created by 98302 on 2023/12/12.
//

#ifndef CONFIG_H
#define CONFIG_H

#include <uv.h>

typedef struct{
    char *ip;
    int port;
    char* test_file;
}t_config;

extern t_config config;
void config_init(uv_loop_t *loop);

#endif //CONFIG_H
