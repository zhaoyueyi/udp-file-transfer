//
// Created by 98302 on 2023/12/12.
//

#include "config.h"
#include <cjson/cJSON.h>

t_config config;

void get_info(cJSON *root) {
    cJSON *item = cJSON_GetObjectItem(root, "ip");
    config.ip = strdup(item->valuestring);
    item = cJSON_GetObjectItem(root, "port");
    config.port = item->valueint;
    item = cJSON_GetObjectItem(root, "test-file");
    config.test_file = strdup(item->valuestring);
    cJSON_Delete(root);
}

void config_init(uv_loop_t *loop) {
    uv_fs_t file;
    const int fd = uv_fs_open(loop, &file, "./config.json", O_RDONLY, 0, NULL);
    const uv_buf_t buf = uv_buf_init(malloc(1024), 1024);
    uv_fs_read(loop, &file, fd, &buf, 1, -1, NULL);
    buf.base[file.result] = '\0';
    uv_fs_close(loop, &file, fd, NULL);
    cJSON *root = cJSON_Parse(buf.base);
    free(buf.base);

    get_info(root);
}

