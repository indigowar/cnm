#include "office_equipment_logic.hpp"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <iostream>

namespace Cnm {

void ScannerLogic::init() {}

void ScannerLogic::execute(ServerCtx &&ctx) {
  auto future_request = ctx->acceptRequest();
  future_request.wait();
  auto request = future_request.get();

  if (request.isErr()) {
    spdlog::warn("Got an error, while serving: %s", request.unwrapErr());
    ctx->abort();
    return;
  }

  auto body = request.unwrap();

  std::vector<Message> targets = body.containsOneMessage()
                                     ? std::vector<Message>{body.getMessage()}
                                     : body.getMessageList();

  std::vector<Message> result{};

  std::for_each(targets.begin(), targets.end(), [&result, this](const auto &i) {
    if (std::filesystem::exists(i)) {
      result.emplace_back(readFile(i));
    } else {
      result.emplace_back("file does not exist");
    }
  });

  ctx->sendResponse(MessageBatch(std::move(result)));
}

std::string ScannerLogic::readFile(const std::string &name) {
  std::ifstream stream;
  try {
    stream = std::ifstream(name);
    std::string content = std::string((std::istreambuf_iterator<char>(stream)),
                                      std::istreambuf_iterator<char>());
    stream.close();
    return content;
  } catch (const std::exception &e) {
    stream.close();
    return "failed to read file";
  }
}

void PrinterLogic::init() {}

void PrinterLogic::execute(ServerCtx &&ctx) {
  auto future_request = ctx->acceptRequest();
  future_request.wait();
  auto request = future_request.get();

  if (request.isErr()) {
    spdlog::warn("Got an error, while serving: %s", request.unwrapErr());
    ctx->abort();
    return;
  }

  auto body = request.unwrap();

  if (body.containsOneMessage()) {
    ctx->sendResponse(MessageBatch("invalid request"));
    return;
  }

  auto data = body.getMessageList();

  if (data.size() != 2) {
    ctx->sendResponse(MessageBatch("invalid request"));
    return;
  }

  auto filename = data.at(0);
  auto content = data.at(1);

  if (std::filesystem::exists(filename)) {
    ctx->sendResponse(MessageBatch("already exists"));
    return;
  }

  std::ofstream stream(filename);

  try {
    stream << content;
    stream.close();
  } catch (const std::exception &e) {
    stream.close();
    spdlog::warn("Got an error while Print::execute: %s", e.what());
    ctx->sendResponse(MessageBatch("failed to execute"));
    return;
  }

  ctx->sendResponse(MessageBatch(Message(filename)));
}

}  // namespace Cnm