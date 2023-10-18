#include "office_equipment_logic.hpp"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>

namespace Cnm {

ScannerOfficeEquipmentLogic::ScannerOfficeEquipmentLogic(
    std::filesystem::path base_dir)
    : base_dir{std::move(base_dir)} {}

void ScannerOfficeEquipmentLogic::execute(Cnm::ServerCtx &&ctx) {
  auto request = ctx->acceptAndGetRequest();
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
  ctx->respond(MessageBatch(handleFile(task)));
}

void ScannerOfficeEquipmentLogic::serveComplexRequest(ServerCtx &&ctx,
                                                      MessageBatch &&task) {
  auto files = task.getMessageList();
  std::vector<std::string> results;
  std::transform(files.begin(), files.end(), std::back_inserter(results),
                 [this](const auto &i) { return handleFile(i); });
  ctx->respond(MessageBatch(std::move(results)));
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
  auto request = ctx->acceptAndGetRequest();
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
  ctx->respond(MessageBatch(std::move(results)));
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
