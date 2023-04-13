#ifndef HPP_CNM_LIB_ENTITIES_RUNNERS_SERVER_HPP
#define HPP_CNM_LIB_ENTITIES_RUNNERS_SERVER_HPP

#include "entities/runners/runner.hpp"

namespace cnm::entities::runners {

class server : public base_runner {

  /**
   * TODO: implement the server class
   *
   * This class is general class for all servers.
   * In TR was specified 3 of them:
   * - video
   * - audio
   * - file-transfer
   *
   * On the level of this implementation what they send doesn't matter at all.
   * The server should be able to serve multiple requests at the same moment.
   * This can be achieved using thread pool or, if you will understand how to do it, using std::async.
   *
   * As you can see the server is inherited from base_runner, it means you have to implement:
   *
   * serve(som&&):
   * This method does the main purpose of this class,
   * the argument is some kind of connection(but currently unimplemented),
   * generally speaking right now it doesn't matter what inside this object,
   * because you will probably just pass it to the running implementation of logic.
   *
   * on_termination():
   * this method should end all running task you have right now.
   * It's like syscall KILL or power-button on your PC,
   * the implementation of this is completely on you, because it depends on choice
   * you should make before(where you specify, how you handle multiple requests).
   *
   *
   * Constructor of this class should get as the parameter data for base class and
   * the logical implementation. The implementation should be some kind of a function
   * that gets the serve_object_mock&& as an argument.
   *
   * If stick to the simplest solution, you can define implementation in those ways:
   * - an abstract class with specific method that will be called.
   * - the functor, that probably gonna look like std::function<void(som&&, <additional args>)>.
   *
   * I highly recommend you to store undone task in queue, that is located in
   * utils/thread_safe_queue.hpp. It doesn't require a sync mechanism because it
   * has a mutex within itself.
   *
   *
   * Notes for thread pool solution:
   *
   * First things first, here you can find what is thread pool:
   * https://en.wikipedia.org/wiki/Thread_pool
   *
   * If you choose this approach you need take a really good thinking about
   * how you are going to decide which thread will execute the task.
   * I recommend to use Round-Robin to simplify it(see here: https://en.wikipedia.org/wiki/Round-robin_scheduling
   *
   * You probably will need somehow to give a signal to your workers,
   * that they should stop(when terminate or destructor called).
   *
   * Keep amount of mutexes/latches/others sync mechanism to minimum as possible,
   * so there's smaller change to make a dead-lock situation between pool's workers.
   *
   * Be aware of race conditions(https://en.wikipedia.org/wiki/Race_condition)
   * because they lead to undefined behavior.
   *
   *
   * Notes for std::async approach:
   *
   * This approach doesn't include managing the threads by yourself.
   *
   * Here you should really thing about how to send messages to the running tasks.
   * (about termination f.e.)
   *
   * In some way of thinking, this approach is simple, but it heavily relies on
   * C++ 20 with it's std::async, std::future and so on.
   *
   * std::future is a simplest way to get info about end of the task.
   *
   * You probably will need an atomic counter to ensure that thread doesn't creates
   * more workers, then it have by it's settings.
   *
   */

};
}

#endif  // HPP_CNM_LIB_ENTITIES_RUNNERS_SERVER_HPP
