#include <stdio.h>

#include "error.hpp"
#include "ipc_file_based.hpp"
#include "ipc_pipe_based.hpp"
#include "ipc_message_queue_based.hpp"
#include "ipc_socket_based.hpp"

#include "ipc_interface.hpp"

IpcInterface::~IpcInterface() { }

IpcInterface* IpcInterface::Create(Type type) {
  switch(type) {
    case kIpcFileBased:
      return FileBasedIpc::Create();
    case kIpcPipeBased:
      return PipeBasedIpc::Create();
    case kIpcMessageQueueBased:
      return MessageQueueBasedIpc::Create();
    case kIpcSocketBased:
      return SocketBasedIpc::Create();
  }
  error::ReportError(error::ERROR_IPC_TYPE);
  return NULL;
}

