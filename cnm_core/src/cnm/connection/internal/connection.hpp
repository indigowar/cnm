#ifndef HPP_CNM_CORE_CONNECTION_INTERNAL_CONNECTION_HPP
#define HPP_CNM_CORE_CONNECTION_INTERNAL_CONNECTION_HPP

#include <memory>
#include <mutex>

namespace Cnm::Connections {

class ConnectionNode;

// Connection is a base class for connection, it is used for nodes to
// interact with connection itself.
class Connection {
 public:
  virtual ~Connection() = default;

  // abort() - kill the connection and stop all interaction between it's nodes.
  virtual void abort() = 0;

  // returns true, if client can send requests.
  virtual bool isRequesting() const noexcept = 0;

  // returns true, if server can send responses.
  virtual bool isServing() const noexcept = 0;

  // lock() - lock the connection to ensure that the action will not be
  // interrupted.
  virtual std::unique_lock<std::mutex> lock() = 0;

  // getSpeed() - retrieve the speed of the network( how many ms required to
  // pass one Message from one node to another).
  virtual size_t getSpeed() const noexcept = 0;
};

}  // namespace Cnm::Connections

#endif  // HPP_CNM_CORE_CONNECTION_INTERNAL_CONNECTION_HPP
