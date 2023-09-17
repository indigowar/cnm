#ifndef HPP_CNM_CORE_MACHINE_SERVER_SERVER_LOGIC_HPP
#define HPP_CNM_CORE_MACHINE_SERVER_SERVER_LOGIC_HPP

#include <map>
#include <string>

#include "cnm/connection/server_ctx.hpp"

namespace Cnm {

class ServerLogic {
 public:
  virtual ~ServerLogic() = default;

  virtual void init() {}

  virtual void execute(ServerCtx&&) {}
};

class FileServerLogic final : public ServerLogic {
 public:
  explicit FileServerLogic(std::map<std::string, std::string>);

  void init() override;

  void execute(ServerCtx&&) override;

 private:
  static result_t<MessageBatch> readRequest(ServerCtx& ctx);

  void retrieveFile(ServerCtx&& ctx, const Message& file_name);

  void retrieveFiles(ServerCtx&& ctx, const MessageBatch&);

  std::map<std::string, std::string> file_storage;
  std::map<std::string, std::string> default_files;

  std::mutex mutex;
};

}  // namespace Cnm

#endif  // HPP_CNM_CORE_MACHINE_SERVER_SERVER_LOGIC_HPP
