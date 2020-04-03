idf_component_register(SRCS "api.cpp"
                       INCLUDE_DIRS "include"
                       PRIV_REQUIRES nvs_flash esp_http_server esp_netif)