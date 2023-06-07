#ifndef HPP_CNM_LIB_CONNECTION_SERVER_CONTEXT_HPP
#define HPP_CNM_LIB_CONNECTION_SERVER_CONTEXT_HPP

#include <initializer_list>
#include <vector>

#include "communication/message.hpp"
#include "connection/connection.hpp"
#include "connection/context.hpp"
#include "connection/exceptions.hpp"

namespace Cnm::Connection {

class ServerCtx : public Context {
 public:
  ServerCtx(Connection& con, int id) : Context(con), id_{id} {}

  void sendResponse(Communication::Message response) {
    if (!isServing()) {
      waitForServing();
    }

    *connection_.getResponseChannel() << response;
  }

  void sendResponses(
      std::initializer_list<Communication::Message> response_bundle) {
    if (!isServing()) {
      waitForServing();
    }

    auto chan{connection_.getResponseChannel()};

    for (auto& item : response_bundle) *chan << item;
  }

  void stopServing() { connection_.stopServing(); }

  [[nodiscard]] size_t getAvailableResponseAmount() const noexcept;
  [[nodiscard]] size_t getSentResponsesAmount() const noexcept;

  Communication::Message getRequest();
  std::vector<Communication::Message> getRequests();

  [[nodiscard]] size_t getAvailableRequestsAmount() const noexcept;
  [[nodiscard]] size_t getRetrievedRequestsAmount() const noexcept;

 private:
  const int id_;
};

}  // namespace Cnm::Connection

#endif  // HPP_CNM_LIB_CONNECTION_SERVER_CONTEXT_HPP
