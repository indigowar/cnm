#include <spdlog/spdlog.h>

#include <cnm/machine/office_equipment/office_equipment.hpp>
#include <cnm/machine/personal_computer/personal_computer.hpp>
#include <cnm/machine/server/server.hpp>
#include <cnm/topology/star/star.hpp>

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

static std::map<std::string, std::string> books1 = {
    {"Brave",
     "Being brave means knowing that when you fail, you don't fail forever."},
    {"Wonder", "You can't blend in when you were born to stand out."},
    {"Hope", "Hope is the thing with feathers that perches in the soul."},
    {"Beloved",
     "Freeing yourself was one thing, claiming ownership of that freed self "
     "was another."},
    {"Invisible",
     "The only way to deal with this life was to find a way to survive it."},
    {"Dare", "The only way to overcome fear is to face it head-on."},
    {"Love", "Love is life. And if you miss love, you miss life."},
    {"Wild",
     "What if I forgave myself? I thought. What if I forgave myself even "
     "though I'd done something I shouldn't have?"},
    {"Sapiens",
     "The real difference between us and chimpanzees is the mysterious glue "
     "that enables millions of humans to cooperate effectively."},
    {"Mindset",
     "The view you adopt for yourself profoundly affects the way you lead your "
     " life."},
};

static std::map<std::string, std::string> books2 = {
    {"Dream",
     "A dream doesn't become reality through magic; it takes sweat, "
     "determination, and hard work."},
    {"Joy",
     "Find a place inside where there's joy, and the joy will burn out the "
     "pain."},
    {"Fearless",
     "I learned that courage was not the absence of fear, but the triumph over "
     " it."},
    {"Passion",
     "Passion is energy. Feel the power that comes from focusing on what "
     "excites you."},
    {"Empathy",
     "Empathy is about finding echoes of another person in yourself."},
    {"Reflect", "In the silence of reflection, one finds the answers."},
    {"Rise",
     "Sometimes you have to kind of die inside in order to rise from your own "
     "ashes."},
    {"Illuminate",
     "There is no darkness so dense, so menacing, or so difficult that it "
     "cannot be overcome by light."},
    {"Balance",
     "Happiness is not a matter of intensity but of balance, order, rhythm, "
     "and harmony."},
    {"Explore",
     "The world is full of magic things, patiently waiting for our senses to "
     "grow sharper."},
};

int main(int argc, char** argv) {
  //  spdlog::warn("Before star");
  //  Cnm::Star::Star star;
  //
  //  spdlog::warn("Made star, starting creating the machines");
  //
  //  auto server = std::make_unique<Cnm::Server>(
  //      MockSL(1, Cnm::FileServerLogic(books1)), 2,
  //      Cnm::HostInfo("Server A", "122.33.140.522"), nullptr);
  //
  //  star.addMachine(std::move(server),
  //                  Cnm::HostInfo("Server A", "122.33.140.522"));
  //
  //  server = std::make_unique<Cnm::Server>(
  //      MockSL(2, Cnm::FileServerLogic(books2)), 2,
  //      Cnm::HostInfo("Server B", "22.133.160.522"), nullptr);
  //
  //  star.addMachine(std::move(server),
  //                  Cnm::HostInfo("Server B", "22.133.160.522"));
  //
  //  auto comp = std::make_unique<Cnm::PersonalComputer>(
  //      MockPCL(3, Cnm::RandomPersonalComputerLogic()),
  //      Cnm::HostInfo("Max's PC", "66.63.33.31"), nullptr);
  //
  //  star.addMachine(std::move(comp), Cnm::HostInfo("Max's PC",
  //  "66.63.33.31"));
  //
  //  auto oe = std::make_unique<Cnm::OfficeEquipment>(
  //      Cnm::PrinterLogic(), Cnm::HostInfo("Printer A", "33.132.231.140"),
  //      nullptr);
  //
  //  star.addMachine(std::move(oe), Cnm::HostInfo("Printer A",
  //  "33.132.231.140"));
  //
  //  spdlog::warn("Before star.start()");
  //
  //  star.start();
  //  std::this_thread::sleep_for(10s);
  //  spdlog::warn("gonna freeze");
  //  star.freeze();
  //  std::this_thread::sleep_for(2s);
  //  spdlog::warn("gonna invoke");
  //  star.invoke();
  //  std::this_thread::sleep_for(5s);
  //  spdlog::warn("gonna stop");
  //  star.stop();

  //  auto comp = std::make_unique<Cnm::PersonalComputer>(
  //      MockPCL(3, Cnm::RandomPersonalComputerLogic()),
  //      Cnm::HostInfo("Max's PC", "66.63.33.31"), nullptr);

  auto comp = std::make_unique<Cnm::OfficeEquipment>(
      Cnm::ScannerOfficeEquipmentLogic("./"),
      Cnm::HostInfo("Printer A", "33.132.231.140"), nullptr);

  spdlog::warn("start()");
  comp->start();
  std::this_thread::sleep_for(5s);
  spdlog::warn("freeze()");
  comp->freeze();
  std::this_thread::sleep_for(5s);
  spdlog::warn("invoke()");
  comp->invoke();
  std::this_thread::sleep_for(5s);
  spdlog::warn("stop()");
  comp->stop();
  spdlog::warn("End.");
  return 0;
}