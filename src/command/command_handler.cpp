#include "command_handler.h"

#include "command.h"

static esp_err_t parse_get(httpd_req_t *req, char **obuf)
{
    char *buf = NULL;
    size_t buf_len = 0;

    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        buf = (char *)malloc(buf_len);
        if (!buf) {
            httpd_resp_send_500(req);
            return ESP_FAIL;
        }
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            *obuf = buf;
            return ESP_OK;
        }
        free(buf);
    }
    // 500 is sent only for testing, will be changes later
    httpd_resp_send_500(req);
    return ESP_FAIL;
}

static esp_err_t command_handler(httpd_req_t* req)
{
    char *buf = NULL;
    const char* cmd_option = "command";
    char value[10];

    if (parse_get(req, &buf) != ESP_OK) {
        return ESP_FAIL;
    }

    if (httpd_query_key_value(buf, cmd_option, value, sizeof(value)) != ESP_OK) {
        free(buf);
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }
    free(buf);

    if (cmd_move(value) != 0) {
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    const char* resp = "Command handled successfully";

    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    return httpd_resp_send(req, resp, sizeof(resp));
}

void setupCommandHandler(httpd_handle_t command_httpd)
{
    httpd_uri_t command_uri = {
        .uri = "/command",
        .method = HTTP_GET,
        .handler = command_handler,
        .user_ctx = NULL
#ifdef CONFIG_HTTPD_WS_SUPPORT
        ,
        .is_websocket = true,
        .handle_ws_control_frames = false,
        .supported_subprotocol = NULL
#endif
    };

    httpd_register_uri_handler(command_httpd, &command_uri);

}