#include <spdlog/spdlog.h>

#include <cnm/machine/office_equipment/office_equipment.hpp>
#include <cnm/machine/personal_computer/personal_computer.hpp>
#include <cnm/machine/server/server.hpp>
#include <cnm/topology/star/star.hpp>
#include <iostream>
#include <utility>

using namespace std::chrono_literals;

class MockSL : public Cnm::ServerLogic {
 public:
  explicit MockSL(size_t id, Cnm::ServerLogic&& l) : id{id}, logic{l} {}

  void execute(std::unique_ptr<Cnm::Communicator>& c,
               Cnm::ServerCtx&& ctx) override {
    spdlog::warn("MockSL::execute(%d) - begin", id);
    logic.execute(c, std::move(ctx));
    spdlog::warn("MockSL::execute(%d) - end", id);
  }

 private:
  size_t id;
  Cnm::ServerLogic logic;
};

class DirectSL : public Cnm::ServerLogic {
 public:
  void execute(std::unique_ptr<Cnm::Communicator>& c,
               Cnm::ServerCtx&& ctx) override {
    auto request_future = ctx->acceptRequest();
    if (!request_future.valid()) {
      spdlog::warn("INVALID FUTURE");
      return;
    }
    request_future.wait();
    auto request = request_future.get();
    if (request.isErr()) {
      //      spdlog::warn("Failed to read request: %s", request.unwrapErr());
      std::cerr << "Failed to read request:" << request.unwrapErr()
                << std::endl;
      ctx->abort();
      return;
    }
    auto body = request.unwrap();
    auto msg = body.containsOneMessage() ? body.getMessage()
                                         : body.getMessageList()[0];
    auto result = reverse(msg);
    ctx->sendResponse(Cnm::MessageBatch(Cnm::Message(result)));
  }

  static std::string reverse(const std::string& s) {
    std::string r{};
    std::copy(s.rbegin(), s.rend(), std::back_inserter(r));
    return r;
  }
};

class DirectPCL : public Cnm::PersonalComputerLogic {
 public:
  explicit DirectPCL(std::string address) : target_addr{std::move(address)} {}

  void execute(std::unique_ptr<Cnm::Communicator>& communicator) override {
    spdlog::warn("DIRECT_PCL_STARTED");
    auto connection_result = communicator->makeConnection(target_addr);
    if (connection_result.isErr()) {
      spdlog::warn("Tried to connect to %s, failed because of: %s", target_addr,
                   connection_result.unwrapErr());
      return;
    }
    auto ctx = connection_result.unwrap();
    auto accept_result = ctx->waitUntilAccepted();
    if (accept_result.isErr()) {
      spdlog::warn("Failed to be accepted for %s, failed because of: %s",
                   target_addr, accept_result.unwrapErr());
      ctx->abort();
      return;
    }
    auto req = messages.at(i);
    i++;
    if (i == messages.size()) {
      i = 0;
    }
    ctx->sendRequest(Cnm::MessageBatch(std::move(req)));
    auto response = ctx->waitAndGetResponse();
    if (response.isErr()) {
      spdlog::warn("Failed to get response: %s", response.unwrapErr());
      return;
    }
    spdlog::info("Got response: %s", response.unwrap().getMessage());
    //    std::cout << "Got response: " << response.unwrap().getMessage()
    //              << std::endl;
  }

 private:
  std::string target_addr;
  size_t i{};
  std::vector<std::string> messages{"hi", "hello, world", "bye", "how are you",
                                    "welcome to the world"};
};

// static std::map<std::string, std::string> books1 = {
//     {"Brave",
//      "Being brave means knowing that when you fail, you don't fail
//      forever."},
//     {"Wonder", "You can't blend in when you were born to stand out."},
//     {"Hope", "Hope is the thing with feathers that perches in the soul."},
//     {"Beloved",
//      "Freeing yourself was one thing, claiming ownership of that freed self "
//      "was another."},
//     {"Invisible",
//      "The only way to deal with this life was to find a way to survive it."},
//     {"Dare", "The only way to overcome fear is to face it head-on."},
//     {"Love", "Love is life. And if you miss love, you miss life."},
//     {"Wild",
//      "What if I forgave myself? I thought. What if I forgave myself even "
//      "though I'd done something I shouldn't have?"},
//     {"Sapiens",
//      "The real difference between us and chimpanzees is the mysterious glue "
//      "that enables millions of humans to cooperate effectively."},
//     {"Mindset",
//      "The view you adopt for yourself profoundly affects the way you lead
//      your " " life."},
// };
//
// static std::map<std::string, std::string> books2 = {
//     {"Dream",
//      "A dream doesn't become reality through magic; it takes sweat, "
//      "determination, and hard work."},
//     {"Joy",
//      "Find a place inside where there's joy, and the joy will burn out the "
//      "pain."},
//     {"Fearless",
//      "I learned that courage was not the absence of fear, but the triumph
//      over " " it."},
//     {"Passion",
//      "Passion is energy. Feel the power that comes from focusing on what "
//      "excites you."},
//     {"Empathy",
//      "Empathy is about finding echoes of another person in yourself."},
//     {"Reflect", "In the silence of reflection, one finds the answers."},
//     {"Rise",
//      "Sometimes you have to kind of die inside in order to rise from your own
//      " "ashes."},
//     {"Illuminate",
//      "There is no darkness so dense, so menacing, or so difficult that it "
//      "cannot be overcome by light."},
//     {"Balance",
//      "Happiness is not a matter of intensity but of balance, order, rhythm, "
//      "and harmony."},
//     {"Explore",
//      "The world is full of magic things, patiently waiting for our senses to
//      " "grow sharper."},
// };
// int main(int argc, char** argv) {
//   spdlog::warn("Before star");
//   Cnm::Star::Star star;
//
//   spdlog::warn("Made star, starting creating the machines");
//
//   auto server = std::make_unique<Cnm::Server>(
//       MockSL(1, Cnm::FileServerLogic(books1)), 2,
//       Cnm::HostInfo("Server A", "122.33.140.522"), nullptr);
//
//   star.addMachine(std::move(server),
//                   Cnm::HostInfo("Server A", "122.33.140.522"));
//
//   server = std::make_unique<Cnm::Server>(
//       MockSL(2, Cnm::FileServerLogic(books2)), 2,
//       Cnm::HostInfo("Server B", "22.133.160.522"), nullptr);
//
//   star.addMachine(std::move(server),
//                   Cnm::HostInfo("Server B", "22.133.160.522"));
//
//   auto comp = std::make_unique<Cnm::PersonalComputer>(
//       MockPCL(3, Cnm::RandomPersonalComputerLogic()),
//       Cnm::HostInfo("Max's PC", "66.63.33.31"), nullptr);
//
//   star.addMachine(std::move(comp), Cnm::HostInfo("Max's PC",
//   "66.63.33.31"));
//
//   auto oe = std::make_unique<Cnm::OfficeEquipment>(
//       Cnm::PrinterLogic(), Cnm::HostInfo("Printer A", "33.132.231.140"),
//       nullptr);
//
//   star.addMachine(std::move(oe), Cnm::HostInfo("Printer A",
//   "33.132.231.140"));
//
//   spdlog::warn("Before star.start()");
//
//   star.start();
//   std::this_thread::sleep_for(10s);
//   spdlog::warn("gonna freeze");
//   star.freeze();
//   std::this_thread::sleep_for(2s);
//   spdlog::warn("gonna invoke");
//   star.invoke();
//   std::this_thread::sleep_for(5s);
//   spdlog::warn("gonna stop");
//   star.stop();

//  auto comp = std::make_unique<Cnm::PersonalComputer>(
//      MockPCL(3, Cnm::RandomPersonalComputerLogic()),
//      Cnm::HostInfo("Max's PC", "66.63.33.31"), nullptr);

//  auto comp = std::make_unique<Cnm::OfficeEquipment>(
//      Cnm::ScannerOfficeEquipmentLogic("./"),
//      Cnm::HostInfo("Printer A", "33.132.231.140"), nullptr);
//
//  spdlog::warn("start()");
//  comp->start();
//  std::this_thread::sleep_for(5s);
//  spdlog::warn("freeze()");
//  comp->freeze();
//  std::this_thread::sleep_for(5s);
//  spdlog::warn("invoke()");
//  comp->invoke();
//  std::this_thread::sleep_for(5s);
//  spdlog::warn("stop()");
//  comp->stop();
//  spdlog::warn("End.");

int main() {
  Cnm::Star::Star star;

  auto svr_addr = Cnm::HostInfo("Direct Server", "12.43.55.100");
  auto svr = std::make_unique<Cnm::Server>(std::make_unique<DirectSL>(), 5,
                                           svr_addr, nullptr);
  auto usr_addr = Cnm::HostInfo("Direct PC", "54.123.43.254");
  auto usr = std::make_unique<Cnm::PersonalComputer>(
      std::make_unique<DirectPCL>(svr_addr.getAddress()), usr_addr, nullptr);

  star.addMachine(std::move(svr), svr_addr);
  star.addMachine(std::move(usr), usr_addr);

  spdlog::info("star");
  star.start();

  spdlog::warn("Enter for stop:");
  std::cin.get();
  star.stop();

  return 0;
}