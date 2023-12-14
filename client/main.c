#include <stdbool.h>
#include <stdio.h>
#include <uv.h>
#include "../config.h"

bool first = true;
FILE *fd;

static void alloc_callback(uv_handle_t* handle,
                           size_t suggested_size,
                           uv_buf_t* buf) {
    static char slab[65536];
    buf->base = slab;
    buf->len = sizeof slab;
}

void on_recv(uv_udp_t* handle,
             ssize_t nread,
             const uv_buf_t* buf,
             const struct sockaddr* addr,
             unsigned flags) {
    if (nread < 0) {
    }
    else if (nread == 0) {
        fclose(fd);
        uv_udp_recv_stop(handle);
        first = true;
    }
    else {
        if(first) {
            first = false;
            char file_name[strlen(config.test_file)+6];
            strcpy(file_name, "recv_");
            strcat(file_name, config.test_file);
            printf("save as %s", file_name);
            fd = fopen(file_name, "wb");
        }
        fwrite(buf->base, sizeof(char), nread, fd);
    }
}

int main() {
    static uv_loop_t* loop;
    static uv_udp_t client;
    static struct sockaddr_in server_addr, client_addr;
    /// 创建事件循环，保持运行
    /// 事件驱动编程
    /// 事件：文件准备写入、socket准备读取、计时器超时
    loop = uv_default_loop(); // 分配内存
    config_init(loop);
    // uv_loop_init(loop);  // 初始化loop
    uv_udp_init(loop, &client);
    uv_ip4_addr(config.ip, config.port, &server_addr); // server ip port
    uv_udp_connect(&client, (const struct sockaddr *)&server_addr);

    uv_ip4_addr("0.0.0.0", 0, &client_addr); // client ip port
    uv_udp_bind(&client, (const struct sockaddr *)&client_addr, UV_UDP_REUSEADDR); // bind地址
    uv_udp_recv_start(&client, alloc_callback, on_recv);

    // send
    {
        char data[12] = "request";
        uv_udp_send_t req;
        uv_buf_t buf = uv_buf_init((char *)data, strlen(data));
        uv_udp_send(
            &req,
            &client,
            &buf,
            1, // buf count
            NULL,
            NULL); // send callback
    }
    uv_run(loop, UV_RUN_DEFAULT);
    return 0;
}
