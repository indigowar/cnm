#include "server_logic.hpp"

#include <spdlog/spdlog.h>

namespace Cnm {

FileServerLogic::FileServerLogic(std::map<std::string, std::string> df)
    : default_files(std::move(df)), file_storage() {
  file_storage.insert(default_files.begin(), default_files.end());
}

void FileServerLogic::onInit() {
  file_storage.clear();
  file_storage.insert(default_files.begin(), default_files.end());
}

void FileServerLogic::execute(std::unique_ptr<Communicator>& com,
                              ServerCtx&& ctx) {
  auto request = readRequest(ctx);
  if (request.isErr()) {
    spdlog::warn("Got an error, while reading the request: %s",
                 request.unwrapErr());
    ctx->abort();
    return;
  }
  auto body = request.unwrap();

  if (body.containsOneMessage()) {
    retrieveFile(std::move(ctx), body.getMessage());
  } else {
    retrieveFiles(std::move(ctx), body);
  }
}

result_t<MessageBatch> FileServerLogic::readRequest(ServerCtx& ctx) {
  auto future = ctx->acceptRequest();
  future.wait();
  return future.get();
}

void FileServerLogic::retrieveFile(ServerCtx&& ctx, const Message& file_name) {
  std::unique_lock lock(mutex);
  if (!file_storage.contains(file_name)) {
    ctx->sendResponse(MessageBatch(Message("file is not found")));
    return;
  }
  auto file = file_storage.at(file_name);
  ctx->sendResponse(MessageBatch(Message(file)));
}

void FileServerLogic::retrieveFiles(ServerCtx&& ctx,
                                    const MessageBatch& request) {
  std::unique_lock lock(mutex);
  std::vector<Message> files{};

  auto file_names = request.getMessageList();

  std::for_each(file_names.begin(), file_names.end(),
                [this, &files](const auto& i) {
                  if (!file_storage.contains(i)) {
                    files.emplace_back("not found");
                  } else {
                    files.emplace_back(file_storage.at(i));
                  }
                });

  ctx->sendResponse(MessageBatch(std::move(files)));
}

}  // namespace Cnm