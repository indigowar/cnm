#include "office_equipment_logic.hpp"

#include <spdlog/spdlog.h>

#include <chrono>
#include <filesystem>
#include <fstream>

// namespace Cnm {
//
// void ScannerLogic::init() {}
//
// void ScannerLogic::execute(ServerCtx &&ctx) {
//   auto future_request = ctx->acceptRequest();
//   future_request.wait();
//   auto request = future_request.get();
//
//   if (request.isErr()) {
//     spdlog::warn("Got an error, while serving: %s", request.unwrapErr());
//     ctx->abort();
//     return;
//   }
//
//   auto body = request.unwrap();
//
//   std::vector<Message> targets = body.containsOneMessage()
//                                      ?
//                                      std::vector<Message>{body.getMessage()}
//                                      : body.getMessageList();
//
//   std::vector<Message> result{};
//
//   std::for_each(targets.begin(), targets.end(), [&result, this](const auto
//   &i) {
//     if (std::filesystem::exists(i)) {
//       result.emplace_back(readFile(i));
//     } else {
//       result.emplace_back("file does not exist");
//     }
//   });
//
//   ctx->sendResponse(MessageBatch(std::move(result)));
// }
//
// std::string ScannerLogic::readFile(const std::string &name) {
//   std::ifstream stream;
//   try {
//     stream = std::ifstream(name);
//     std::string content =
//     std::string((std::istreambuf_iterator<char>(stream)),
//                                       std::istreambuf_iterator<char>());
//     stream.close();
//     return content;
//   } catch (const std::exception &e) {
//     stream.close();
//     return "failed to read file";
//   }
// }
//
// void PrinterLogic::init() {}
//
// void PrinterLogic::execute(ServerCtx &&ctx) {
//   auto future_request = ctx->acceptRequest();
//   future_request.wait();
//   auto request = future_request.get();
//
//   if (request.isErr()) {
//     spdlog::warn("Got an error, while serving: %s", request.unwrapErr());
//     ctx->abort();
//     return;
//   }
//
//   auto body = request.unwrap();
//
//   if (body.containsOneMessage()) {
//     ctx->sendResponse(MessageBatch("invalid request"));
//     return;
//   }
//
//   auto data = body.getMessageList();
//
//   if (data.size() != 2) {
//     ctx->sendResponse(MessageBatch("invalid request"));
//     return;
//   }
//
//   auto filename = data.at(0);
//   auto content = data.at(1);
//
//   if (std::filesystem::exists(filename)) {
//     ctx->sendResponse(MessageBatch("already exists"));
//     return;
//   }
//
//   std::ofstream stream(filename);
//
//   try {
//     stream << content;
//     stream.close();
//   } catch (const std::exception &e) {
//     stream.close();
//     spdlog::warn("Got an error while Print::execute: %s", e.what());
//     ctx->sendResponse(MessageBatch("failed to execute"));
//     return;
//   }
//
//   ctx->sendResponse(MessageBatch(Message(filename)));
// }
//
// }  // namespace Cnm

namespace Cnm {

ScannerOfficeEquipmentLogic::ScannerOfficeEquipmentLogic(
    std::filesystem::path base_dir)
    : base_dir{std::move(base_dir)} {}

void ScannerOfficeEquipmentLogic::execute(Cnm::ServerCtx &&ctx) {
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
    serveSimpleRequest(std::move(ctx), body.getMessage());
  } else {
    serveComplexRequest(std::move(ctx), std::move(body));
  }
}

result_t<std::string> ScannerOfficeEquipmentLogic::readFromFile(
    const std::filesystem::path &path) {
  try {
    std::ifstream stream(path);
    if (!stream) {
      return result_t<std::string>::Err("File does not exist");
    }
    std::string content((std::istreambuf_iterator<char>(stream)),
                        std::istreambuf_iterator<char>());
    return result_t<std::string>::Ok(std::move(content));
  } catch (const std::bad_alloc &e) {
    spdlog::warn("ScannerOfficeEquipmentLogic::readFromFile: got bad_alloc");
    return result_t<std::string>::Err("File is too big");
  }
}

void ScannerOfficeEquipmentLogic::serveSimpleRequest(ServerCtx &&ctx,
                                                     Message &&task) {
  ctx->sendResponse(MessageBatch(handleFile(task)));
}

void ScannerOfficeEquipmentLogic::serveComplexRequest(ServerCtx &&ctx,
                                                      MessageBatch &&task) {
  auto files = task.getMessageList();
  std::vector<std::string> results;
  std::transform(files.begin(), files.end(), std::back_inserter(results),
                 [this](const auto &i) { return handleFile(i); });
  ctx->sendResponse(MessageBatch(std::move(results)));
}

std::string ScannerOfficeEquipmentLogic::handleFile(
    const std::string &filename) {
  const auto path = base_dir / filename;
  auto result = readFromFile(path);
  if (result.isErr()) {
    return std::string{result.unwrapErr()};
  }
  return result.unwrap();
}

PrinterOfficeEquipmentLogic::PrinterOfficeEquipmentLogic(
    std::filesystem::path base_dir)
    : base_dir{std::move(base_dir)} {}

void PrinterOfficeEquipmentLogic::execute(Cnm::ServerCtx &&ctx) {
  auto future_request = ctx->acceptRequest();
  future_request.wait();
  auto request = future_request.get();
  if (request.isErr()) {
    spdlog::warn("Got an error, while serving: %s", request.unwrapErr());
    ctx->abort();
    return;
  }
  auto body = request.unwrap();
  std::vector<std::string> results{};
  auto tasks = body.getMessageList();
  for (size_t i{}; i < tasks.size(); i += 2) {
    auto name = tasks.at(i);
    auto content = tasks.at(i + 1);
    auto result = writeIntoFile(base_dir / name, content);

    if (result.isErr()) {
      results.emplace_back("Got an error");
    } else {
      results.emplace_back("Success");
    }
  }
  ctx->sendResponse(MessageBatch(std::move(results)));
}

result_t<bool> PrinterOfficeEquipmentLogic::writeIntoFile(
    const std::filesystem::path &path, const std::string &content) {
  try {
    std::ofstream stream(path, std::ios::out | std::ios::trunc);
    stream << content;
    stream.close();
  } catch (const std::exception &e) {
    return result_t<bool>::Err(e.what());
  }
  return result_t<bool>::Ok(true);
}

}  // namespace Cnm
