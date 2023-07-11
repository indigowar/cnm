#ifndef HPP_CNM_LIB_MACHINES_LOGIC_HPP
#define HPP_CNM_LIB_MACHINES_LOGIC_HPP

#include <X11/extensions/randr.h>

#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "communication/message.hpp"
#include "connection/server_ctx.hpp"
#include "machines/machine.hpp"
#include "machines/server.hpp"

namespace Cnm::Machines::Logic {

struct Base {
  bool isRequestValid(const Communication::Message&);
  bool isRequestToTheRightService(const Communication::Message&,
                                  const HostInfo&);
  bool isRequestHasBinaryData(const Communication::Message&);
  bool isRequestHasTextData(const Communication::Message&);

  Communication::Message buildBadRequestResponse(const HostInfo&, std::string);
  Communication::Message buildNotFoundResponse(const HostInfo&, std::string);
};

namespace Server {

class FtpServer : public Base {
 public:
  struct File final {
    std::string name;
    std::vector<std::byte> data;
  };

  explicit FtpServer(std::vector<File> files) : files_{std::move(files)} {}

  void operator()(const HostInfo&,
                  std::unique_ptr<Connection::ServerCtx>&& ctx);

  void addFile(File);
  void removeFile(std::string);

 private:
  std::vector<Communication::Message> readAllRequests(
      std::unique_ptr<Connection::ServerCtx>&);

  void sendFileFromRequest(std::unique_ptr<Connection::ServerCtx>&&,
                           const HostInfo& host,
                           const Communication::Message& request);

  void handleMutlipleRequests(
      std::unique_ptr<Connection::ServerCtx>&&, const HostInfo& host,
      const std::vector<Communication::Message>& requests);

  std::vector<File> files_;
  std::shared_mutex mutex_;
};

class MessagingServer : public Base {};

}  // namespace Server

namespace OfficeEquipment {

class Printer : public Base {};

}  // namespace OfficeEquipment

namespace PersonalComputer {

class GeneralWorker {};

class UserRuledPC {};

}  // namespace PersonalComputer

}  // namespace Cnm::Machines::Logic

#endif  // HPP_CNM_LIB_MACHINES_LOGIC_HPP
