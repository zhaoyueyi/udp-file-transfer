#include <stdio.h>
#include <uv.h>
#include "../config.h"

static void alloc_callback(uv_handle_t* handle,
                           size_t suggested_size,
                           uv_buf_t* buf) {
    static char slab[65536];
    buf->base = slab;
    buf->len = sizeof slab;
}

static void on_recv(uv_udp_t* handle,
                    ssize_t nread,
                    const uv_buf_t* buf,
                    const struct sockaddr* addr,
                    unsigned flags) {
    if (nread < 0) {
    }
    else if (nread == 0) {
    }
    else {
        // todo检查确认包
        // 发送数据
        uv_udp_send_t req;
        uv_fs_t file;
        int fd = uv_fs_open(handle->loop, &file, config.test_file, UV_FS_O_RDONLY, 0, NULL);
        uv_buf_t read_buf = uv_buf_init(malloc(1024), 1024);
        while(1) {
            uv_fs_read(handle->loop, &file, fd, &read_buf, 1, -1, NULL);  // offset-1, 自动递进
            if(file.result <= 0) break;
            uv_buf_t send_buf = uv_buf_init(malloc(file.result), file.result);
            memcpy(send_buf.base, read_buf.base, file.result);
            uv_udp_send(
                &req,
                handle,
                &send_buf,
                1, // buf count
                addr,
                NULL); // send callback
            free(send_buf.base);
        }
        uv_fs_close(handle->loop, &file, fd, NULL);
        free(read_buf.base);
        read_buf = uv_buf_init("", 0);
        uv_udp_send(
                &req,
                handle,
                &read_buf,
                1, // buf count
                addr,
                NULL); // send callback
    }
}

int main() {
    static uv_udp_t server;
    struct sockaddr_in server_addr;
    static uv_loop_t* loop;

    loop = uv_default_loop();

    config_init(loop);

    uv_ip4_addr(config.ip, config.port, &server_addr);
    uv_udp_init(loop, &server);
    uv_udp_bind(&server, (const struct sockaddr *)&server_addr, 0);
    uv_udp_recv_start(&server, alloc_callback, on_recv);
    uv_run(loop, UV_RUN_DEFAULT);

    return 0;
}
