#ifndef HPP_CNM_LIB_CONNECTION_REQUEST_CONTEXT_HPP
#define HPP_CNM_LIB_CONNECTION_REQUEST_CONTEXT_HPP

#include <initializer_list>
#include <vector>

#include "communication/channels/channel.hpp"
#include "communication/message.hpp"
#include "connection/connection.hpp"
#include "connection/context.hpp"

namespace Cnm::Connection {

class RequesterCtx : public Context {
 public:
  RequesterCtx(Connection& con, int id) : Context(con), id_{id} {}

  ~RequesterCtx();

  void sendRequest(Communication::Message request) {
    if (!isEstablished()) {
      waitForEstablished();
    }
    *(connection_.getRequestChannel()) << request;
  }

  void sendRequests(
      std::initializer_list<Communication::Message> request_bundle) {
    if (!isEstablished()) {
      waitForEstablished();
    }

    auto chan{connection_.getRequestChannel()};

    for (auto& item : request_bundle) *chan << item;
  }

  [[nodiscard]] size_t getAvailableRequestAmount() {
    if (isEstablished() || isServing()) {
      auto chan{connection_.getRequestChannel()};
      return chan->getLimit() - chan->getAmountOfContained();
    }
    return 0;
  }

  [[nodiscard]] size_t getSentRequestsAmount() {
    if (isEstablished() || isServing()) {
      return connection_.getRequestChannel()->getAmountOfContained();
    }
    return 0;
  }

  Communication::Message getResponse() {
    if (!connection_.isServing()) {
      waitForServing();
    }
    auto chan = connection_.getResponseChannel();

    Communication::Message msg;
    *chan >> msg;

    return msg;
  }

  std::vector<Communication::Message> getResponses() {
    if (!connection_.isServing()) {
      waitForServing();
    }

    auto chan{connection_.getResponseChannel()};

    std::vector<Communication::Message> result(chan->getAmountOfContained());
    for (size_t i{}; i != chan->getAmountOfContained(); i++) {
      Communication::Message msg;
      *chan >> msg;
      result.emplace_back(msg);
    }

    return result;
  }

  [[nodiscard]] size_t getAvailableResponsesAmount() const noexcept {
    return connection_.getResponseChannel()->getAmountOfContained();
  }

  void stopRequesting() { connection_.stopRequesting(); }

 private:
  const int id_;
};

}  // namespace Cnm::Connection

#endif  // HPP_CNM_LIB_CONNECTION_REQUEST_CONTEXT_HPP
