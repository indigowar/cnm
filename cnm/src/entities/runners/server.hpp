#ifndef HPP_CNM_LIB_ENTITIES_RUNNERS_SERVER_HPP
#define HPP_CNM_LIB_ENTITIES_RUNNERS_SERVER_HPP

#include "../../communication/slave_ctx.hpp"
#include "../../utils/thread_pool.hpp"
#include "runner.hpp"

namespace cnm::entities::runners {

class server {
 public:
  class logic {
   public:
    virtual void operator()(server&, communication::slave_ctx& ctx) = 0;
  };

  server(std::string_view, std::unique_ptr<logic>);

  ~server();

  void serve(communication::slave_ctx&&) = noexcept override;

  void terminate();

 private:
  std::uniuqe_ptr<logic> m_handler;

  utils::thread_pool m_pool;
};

// class server;

// class server_logic_handler {
//  public:
//   virtual void operator()(server&, serve_object_mock&&) {}
// };

// class server final : public base_runner {
//  public:
//   server(std::string_view, server_logic_handler, size_t);

//   ~server();

//   void serve(serve_object_mock&&) noexcept override;

//  private:
//   server_logic_handler m_handler;
//   utils::thread_pool m_thread_pool;
// };
}  // namespace cnm::entities::runners

#endif  // HPP_CNM_LIB_ENTITIES_RUNNERS_SERVER_HPP
