#ifndef HPP_CNM_MACHINES_LOGIC_SERVER_HPP
#define HPP_CNM_MACHINES_LOGIC_SERVER_HPP

#include "machines/server.hpp"

namespace Cnm::Machines::Logic {

Server::Logic server_ftp();

Server::Logic http_server();

Server::Logic task_passer();

}  // namespace Cnm::Machines::Logic

#endif  // HPP_CNM_MACHINES_LOGIC_SERVER_HPP
