#pragma once

namespace HTTP{


class HTTPResponse{
    struct start_line{
        char version[9] = "HTTP/1.1";
        char status_code[4];
    };
    struct header{
        char server[7] = "Custom";
        char date[30] = "Sun, 7 Jun 2026 08:12:31 GMT";
        char content_length[10];
        char content_type[10];
        char cache_control[9] = "no-store";
        char* file_path;
    };

    HTTPResponse(char[4], char[10], char[10], char*);
    char* render();
};

}