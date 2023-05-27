#ifndef HPP_CNM_LIB_CONNECTION_REQUEST_CONTEXT_HPP
#define HPP_CNM_LIB_CONNECTION_REQUEST_CONTEXT_HPP

#include <initializer_list>
#include <vector>

#include "communication/message.hpp"
#include "connection/connection.hpp"
#include "connection/context.hpp"

namespace Cnm::Connection {

// TODO: Implement RequesterCtx class.

class RequesterCtx : public Context {
 public:
  RequesterCtx(Connection&);

  void sendRequest(Communication::Message);
  void sendRequests(std::initializer_list<Communication::Message>);

  [[nodiscard]] size_t getAvailableRequestAmount() const noexcept;
  [[nodiscard]] size_t getSentRequestsAmount() const noexcept;

  Communication::Message getResponse();
  std::vector<Communication::Message> getResponses();

  [[nodiscard]] size_t getAvailableResponsesAmount() const noexcept;
};

}  // namespace Cnm::Connection

#endif  // HPP_CNM_LIB_CONNECTION_REQUEST_CONTEXT_HPP
