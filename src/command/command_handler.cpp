#include "command_handler.h"

#include "command.h"

static esp_err_t parse_get(httpd_req_t *req, char **obuf)
{
    char *buf = NULL;
    size_t buf_len = httpd_req_get_url_query_len(req) + 1;

    if (buf_len > 1) {
        buf = (char *)malloc(buf_len);
        if (!buf) {
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to retrieve query from request");
            return ESP_FAIL;
        }
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            *obuf = buf;
            return ESP_OK;
        }
        free(buf);
    } else if (buf_len == 1) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Empty query received, cannot resolve command");
        return ESP_FAIL;
    }

    httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to retrieve query from request");
    return ESP_FAIL;
}

static esp_err_t command_handler(httpd_req_t* req)
{
    char *query = NULL;
    const char* cmd_option = "action";
    char value[10] = {0};

    if (parse_get(req, &query) != ESP_OK) {
        return ESP_FAIL;
    }

    esp_err_t query_parse_result = httpd_query_key_value(query, cmd_option, value, sizeof(value));
    if (query_parse_result == ESP_ERR_NOT_FOUND) {
        free(query);
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "No action option received, cannot resolve command");
        return ESP_FAIL;
    } else if (query_parse_result != ESP_OK) {
        free(query);
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to parse query");
        return ESP_FAIL;
    }

    if (cmd_move(value) != 0) {
        free(query);
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid action received");
        return ESP_FAIL;
    }
    free(query);

    const char resp[] = "Command handled successfully";

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