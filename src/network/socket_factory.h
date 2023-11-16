//
// Created by tqcq on 2023/11/16.
//

#ifndef ULIB_SRC_NETWORK_SOCKET_FACTORY_H_
#define ULIB_SRC_NETWORK_SOCKET_FACTORY_H_

#include "socket.h"

namespace tqcq {

class SocketFactory {
 public:
  virtual ~SocketFactory() = default;
  virtual Socket* CreateSocket(int family, int type) = 0;
};

} // namespace tqcq

#endif //ULIB_SRC_NETWORK_SOCKET_FACTORY_H_
