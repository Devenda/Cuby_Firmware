
#include <sys/param.h>

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_http_server.h"

static const char *TAG="server";

static esp_err_t root_get_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "Responding to root_get");

    /* Send HTML file header */
    // httpd_resp_sendstr_chunk(req, "<!DOCTYPE html><html><body>");

    /* Get handle to embedded file upload script */
    extern const unsigned char cuby_start[] asm("_binary_cuby_html_start");
    extern const unsigned char cuby_end[]   asm("_binary_cuby_html_end");
    const size_t cuby_size = (cuby_end - cuby_start);

    /* Add file upload form and script which on execution sends a POST request to /upload */
    httpd_resp_send_chunk(req, (const char *)cuby_start, cuby_size);

    /* Send empty chunk to signal HTTP response completion */
    httpd_resp_sendstr_chunk(req, NULL);
    return ESP_OK;
}

static esp_err_t js_get_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "Responding to js_get");

    /* Send HTML file header */
    // httpd_resp_sendstr_chunk(req, "<!DOCTYPE html><html><body>");

    /* Get handle to embedded file upload script */
    extern const unsigned char js_start[] asm("_binary_bundle_js_start");
    extern const unsigned char js_end[]   asm("_binary_bundle_js_end");
    const size_t js_size = (js_end - js_start);

    /* Add file upload form and script which on execution sends a POST request to /upload */
    httpd_resp_send_chunk(req, (const char *)js_start, js_size);

    /* Send empty chunk to signal HTTP response completion */
    httpd_resp_sendstr_chunk(req, NULL);
    return ESP_OK;
}

esp_err_t server_start(void)
{ 
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    ESP_LOGI(TAG, "Starting HTTP Server");
    if (httpd_start(&server, &config) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start server!");
        return ESP_FAIL;
    }

    /* URI handler for website root */
    httpd_uri_t root_get_uri = {
        .uri       = "/",  // Match all URIs of type /path/to/file
        .method    = HTTP_GET,
        .handler   = root_get_handler,
        .user_ctx  = NULL
    };
    httpd_register_uri_handler(server, &root_get_uri);

    /* URI handler for website root */
    httpd_uri_t js_get_uri = {
        .uri       = "/bundle.js",  // Match all URIs of type /path/to/file
        .method    = HTTP_GET,
        .handler   = js_get_handler,
        .user_ctx  = NULL
    };
    httpd_register_uri_handler(server, &js_get_uri);

    return ESP_OK;
}