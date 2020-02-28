idf_build_get_property(project_dir PROJECT_DIR)
idf_component_register(SRCS "ota.cpp"
                       INCLUDE_DIRS "include"
                       PRIV_REQUIRES esp_http_client esp_https_ota app_update nvs_flash
                       EMBED_TXTFILES ${project_dir}/server_certs/ca_cert.pem)