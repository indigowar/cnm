#ifndef HPP_CNM_CORE_MACHINE_SERVER_SERVER_LOGIC_HPP
#define HPP_CNM_CORE_MACHINE_SERVER_SERVER_LOGIC_HPP

#include <map>
#include <string>
#include <utility>

#include "cnm/connection/server_ctx.hpp"
#include "cnm/machine/communicator.hpp"

namespace Cnm {

class ServerLogic {
 public:
  virtual ~ServerLogic() = default;

  void init(HostInfo hi) {
    host_info = std::move(hi);
    onInit();
  }

  virtual void reset() {}

  virtual void execute(std::unique_ptr<Communicator>&, ServerCtx&&) {}

 protected:
  virtual void onInit() {}

  HostInfo host_info;
};

class FileServerLogic final : public ServerLogic {
 public:
  explicit FileServerLogic(std::map<std::string, std::string>);

  void execute(std::unique_ptr<Communicator>&, ServerCtx&&) override;

 private:
  void onInit() override;

  static result_t<MessageBatch> readRequest(ServerCtx& ctx);

  void retrieveFile(ServerCtx&& ctx, const Message& file_name);

  void retrieveFiles(ServerCtx&& ctx, const MessageBatch&);

  std::map<std::string, std::string> file_storage;
  std::map<std::string, std::string> default_files;

  std::mutex mutex;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_MACHINE_SERVER_SERVER_LOGIC_HPP
