//
// Created by tqcq on 2023/11/16.
//

#ifndef ULIB_SRC_NETWORK_SOCKET_H_
#define ULIB_SRC_NETWORK_SOCKET_H_

#include <errno.h>
#include <cstdint>
#include <sys/types.h>

#include "network/socket_address.h"
#include "3party/sigslot/sigslot.h"

namespace tqcq {

inline bool IsBlockingError(int e) {
  return (e == EWOULDBLOCK) || (e == EAGAIN) || (e == EINPROGRESS);
}

class Socket {
 public:
  virtual ~Socket() = default;
  Socket(const Socket &) = delete;
  Socket &operator=(const Socket &) = delete;

  virtual SocketAddress GetLocalAddress() const = 0;
  virtual SocketAddress GetRemoteAddress() const = 0;

  virtual int Bind(const SocketAddress &addr) = 0;
  virtual int Connect(const SocketAddress &addr) = 0;
  virtual int Send(const void *pv, size_t cb) = 0;
  virtual int SendTo(const void *pv, size_t cb, const SocketAddress &addr) = 0;
  virtual int Recv(void *pv, size_t cb, int64_t *timestamp) = 0;
  virtual int RecvFrom(void *pv,
                       size_t cb,
                       SocketAddress *paddr,
                       int64_t *timestamp) = 0;
  virtual int Listen(int backlog) = 0;
  virtual Socket *Accept(SocketAddress *paddr) = 0;
  virtual int Close() = 0;
  virtual int GetError() const = 0;
  virtual void SetError(int error) = 0;
  inline bool IsBlocking() const { return IsBlockingError(GetError()); }

  enum ConnState { CS_CLOSED, CS_CONNECTING, CS_CONNECTED };
  virtual ConnState GetState() const = 0;

  sigslot::signal1<Socket *, sigslot::multi_threaded_local> SignalReadEvent;
  sigslot::signal1<Socket *, sigslot::multi_threaded_local> SignalWriteEvent;
  sigslot::signal1<Socket *> SignalConnectEvent;
  sigslot::signal2<Socket *, int> SignalCloseEvent;
};

} // namespace tqcq

#endif //ULIB_SRC_NETWORK_SOCKET_H_
