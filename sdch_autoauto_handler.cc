// Copyright (c) 2015 Yandex LLC. All rights reserved.
// Author: Vasily Chekalkin <bacek@yandex-team.ru>

#include "sdch_autoauto_handler.h"

#include "sdch_dictionary.h"
#include "sdch_main_config.h"
#include "sdch_request_context.h"

namespace sdch {

AutoautoHandler::AutoautoHandler(RequestContext* ctx, Handler* next)
    : Handler(next), ctx_(ctx) {
  // TODO
}

AutoautoHandler::~AutoautoHandler() {}

bool AutoautoHandler::init(RequestContext* ctx) {
  return true;
}

ssize_t AutoautoHandler::on_data(const char* buf, size_t len) {
  ssize_t res = 0;

  blob_.insert(blob_.end(), buf, buf + len);

  if (next_)
    res = next_->on_data(buf, len);
  return res;
}

int AutoautoHandler::on_finish() {
  if (blob_.empty()) {
    ngx_log_error(NGX_LOG_ERR,
                  ctx_->request->connection->log,
                  0,
                  "storing quasidict: no blob");
  } else {
    Dictionary dict;
    dict.init(blob_.data(), blob_.data() + blob_.size(), true);
    auto client_id = dict.client_id();
    auto* main = MainConfig::get(ctx_->request);
    if (main->storage.store(client_id,
                            Storage::Value(time(nullptr), std::move(dict)))) {
      ngx_log_error(NGX_LOG_DEBUG,
                    ctx_->request->connection->log,
                    0,
                    "storing quasidict %s (%d)",
                    client_id.c_str(),
                    blob_.size());
    } else {
      ngx_log_error(NGX_LOG_ERR,
                    ctx_->request->connection->log,
                    0,
                    "failed storing quasidict %s (%d)",
                    client_id.c_str(),
                    blob_.size());
    }
  }

  return next_->on_finish();
}

}  // namespace sdch
