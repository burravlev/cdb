#include "../lib/server.h"
#include "../lib/db.h"

int main(void) {
    Server server = new_server(8080);
    db_init();
    server_start(&server, read_cmd);
    db_close();
    return 0;
}