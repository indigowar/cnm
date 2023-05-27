#ifndef HPP_CNM_LIB_CONNECTION_SERVER_CONTEXT_HPP
#define HPP_CNM_LIB_CONNECTION_SERVER_CONTEXT_HPP

#include <initializer_list>
#include <vector>

#include "communication/message.hpp"
#include "connection/connection.hpp"
#include "connection/context.hpp"

namespace Cnm::Connection {

// TODO: Implement ServerCtx

class ServerCtx : public Context {
 public:
  ServerCtx(Connection&);

  void sendResponse(Communication::Message);
  void sendResponses(std::initializer_list<Communication::Message>);

  [[nodiscard]] size_t getAvailableResponseAmount() const noexcept;
  [[nodiscard]] size_t getSentResponsesAmount() const noexcept;

  Communication::Message getRequest();
  std::vector<Communication::Message> getRequests();

  [[nodiscard]] size_t getAvailableRequestsAmount() const noexcept;
  [[nodiscard]] size_t getRetrievedRequestsAmount() const noexcept;
};

}  // namespace Cnm::Connection

#endif  // HPP_CNM_LIB_CONNECTION_SERVER_CONTEXT_HPP
