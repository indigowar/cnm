// #include "ring.hpp"
//
// #include <memory>
// #include <mutex>
//
// #include "cnm/machine/host_info.hpp"
// #include "cnm/machine/machine.hpp"
// #include "cnm/utils/result.hpp"
//
// namespace Cnm {
//
// void Ring::start() {
//   // TODO: implement Ring::start().
// }
//
// void Ring::stop() {
//   // TODO: implement Ring::stop().
// }
//
// void Ring::invoke() {
//   // TODO: implement Ring::invoke().
// }
//
// void Ring::freeze() {
//   // TODO: implement Ring::freeze().
// }
//
//// result_t<HostInfo> Ring::addMachine(std::unique_ptr<Machine>&& machine) {
////   auto lock = makeLock();
////
////   auto info = generateHostInfo(machine->getType());
////
////   auto new_node = std::make_shared<RingNode>(info, machine);
////   nodes.emplace(new_node->getHostInfo().getAddress(), new_node);
////
////   return result_t<HostInfo>::Ok(new_node->getHostInfo());
//// }
////
//// result_t<HostInfo> Ring::addMachine(std::unique_ptr<Machine>&& machine,
////                                     HostInfo info) {
////   auto lock = makeLock();
////
////   if (nodes.contains(info.getAddress())) {
////     return result_t<HostInfo>::Err("address is already in use.");
////   }
////
////   auto new_node = std::make_shared<RingNode>(info, machine);
////   nodes.emplace(new_node->getHostInfo().getAddress(), new_node);
////
////   return result_t<HostInfo>::Ok(new_node->getHostInfo());
//// }
////
//// result_t<HostInfo> Ring::addMachineWithName(std::unique_ptr<Machine>&&
//// machine,
////                                             std::string_view name) {
////   auto lock = makeLock();
////
////   auto info = generateHostInfo(name);
////
////   auto new_node = std::make_shared<RingNode>(info, machine);
////   nodes.emplace(new_node->getHostInfo().getAddress(), new_node);
////
////   return result_t<HostInfo>::Ok(new_node->getHostInfo());
//// }
////
//// result_t<HostInfo> Ring::addMachineWithAddress(
////     std::unique_ptr<Machine>&& machine, std::string_view address) {
////   auto lock = makeLock();
////
////   if (nodes.contains(address)) {
////     return result_t<HostInfo>::Err("address is already in use.");
////   }
////
////   HostInfo info(machine->getType(), address);
////
////   auto new_node = std::make_shared<RingNode>(info, machine);
////   nodes.emplace(new_node->getHostInfo().getAddress(), new_node);
////
////   return result_t<HostInfo>::Ok(new_node->getHostInfo());
//// }
////
//// result_t<bool> Ring::deleteMachine(HostInfo info) {
////   auto lock = makeLock();
////   // TODO: implemenet Ring::deleteMachine
////
////   if (!nodes.contains(info.getAddress())) {
////     return result_t<bool>::Err("there is no machine with given info.");
////   }
////
////   return result_t<bool>::Ok(true);
//// }
////
//// std::unique_lock<std::mutex> Ring::makeLock() const noexcept {
////   return std::unique_lock(mutex);
//// }
////
//// HostInfo Ring::generateHostInfo(std::string_view name) {
////   HostInfo info;
////   do {
////     info = HostInfo::generate(name);
////   } while (!nodes.contains(info.getAddress()));
////   return info;
//// }
//
//}  // namespace Cnm
