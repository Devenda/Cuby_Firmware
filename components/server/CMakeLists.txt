set(WEB_SRC_DIR "${CMAKE_BINARY_DIR}/../frontend")

if(EXISTS ${WEB_SRC_DIR})
    idf_component_register(SRCS "server.cpp"
                           INCLUDE_DIRS "include"
                           PRIV_REQUIRES esp_http_server
                           EMBED_FILES "${WEB_SRC_DIR}/bundle.js" "${WEB_SRC_DIR}/cuby.html")
else()
    message(FATAL_ERROR "${WEB_SRC_DIR} doesn't exit...")
endif()