#include "server_handlers.h"
#include "server_protocol.h"   // send_all_line, split_at 등

#include <algorithm>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <thread>

using namespace std;

static vector<int> snapshot_client_fds(ServerContext& ctx) {
    vector<int> fds;
    lock_guard<mutex> lk(ctx.mtx);
    fds.reserve(ctx.clients.size());
    for (auto* c : ctx.clients) fds.push_back(c->fd);
    return fds;
}

static ClientInfo* find_by_id(ServerContext& ctx, const string& id) {
    lock_guard<mutex> lk(ctx.mtx);
    auto it = ctx.id2cli.find(id);
    return (it == ctx.id2cli.end()) ? nullptr : it->second;
}

void handle_echo(ServerContext& ctx, ClientInfo* self,
                 const vector<string>& tok,
                 const string& fullLine)
{
    if (tok.size() < 2) { (void)send_all_line(self->fd, "ERR@FORMAT"); return; }
    const string& who = tok[1];

    if (who == "ALL") {
        auto fds = snapshot_client_fds(ctx);
        for (int fd : fds) (void)send_all_line(fd, fullLine);
    } else if (who == "ME") {
        (void)send_all_line(self->fd, fullLine);
    } else {
        (void)send_all_line(self->fd, "ERR@UNKNOWN@ECHO_TARGET");
    }
}
