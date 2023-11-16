//
// Created by tqcq on 2023/11/16.
//

#ifndef ULIB_SRC_NETWORK_SOCKET_SERVER_H_
#define ULIB_SRC_NETWORK_SOCKET_SERVER_H_

#include "network/socket_factory.h"
#include "units/time_delta.h"
#include "synchronization/event.h"

namespace tqcq {

class Thread;

class SocketServer : public SocketFactory {
 public:
  static const TimeDelta kForever; // = Event::kForever;

  static std::unique_ptr<SocketServer> CreateDefault();

  virtual void SetMessageQueue(Thread* queue) {}

  virtual bool Wait(TimeDelta max_wait_duration, bool process_io) = 0;

  virtual void WakeUp() = 0;
};

} // namespace tqcq

#endif //ULIB_SRC_NETWORK_SOCKET_SERVER_H_
