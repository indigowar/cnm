#include "logics.hpp"

#include <spdlog/spdlog.h>

#include "communication/message.hpp"
#include "connection/server_ctx.hpp"

namespace Cnm::Machines::Logic {

using server_ctx = std::unique_ptr<Connection::ServerCtx>;

namespace Server {

void FtpServer::operator()(const HostInfo& info, server_ctx&& ctx) {
  try {
    auto requests = readAllRequests(ctx);

    if (requests.size() == 0) {
      ctx->abort();
      return;
    }

    if (requests.size() == 1) {
      return sendFileFromRequest(std::move(ctx), info, requests.at(0));
    }

    handleMutlipleRequests(std::move(ctx), info, requests);

  } catch (const std::exception& err) {
    spdlog::warn(err.what());
  }
}

void FtpServer::sendFileFromRequest(server_ctx&& ctx, const HostInfo& info,
                                    const Communication::Message& request) {
  if (!isRequestValid(request) || !isRequestToTheRightService(request, info) ||
      isRequestHasBinaryData(request)) {
    auto response = buildBadRequestResponse(info, "Bad Request");
    ctx->sendResponse(response);
  }
}

}  // namespace Server

namespace OfficeEquipment {}

namespace PersonalComputer {}

}  // namespace Cnm::Machines::Logic
