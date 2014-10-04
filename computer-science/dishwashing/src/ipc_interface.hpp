#ifndef _IPC_INTERFACE_HPP
#define _IPC_INTERFACE_HPP

#include <string>

class IpcInterface {
 public:
  // Types of the implementation.
  enum Type {
    kIpcFileBased,
    kIpcPipeBased,
    kIpcMessageQueueBased,
    kIpcSharedMemoryBased,
    kIpcSocketBased,  
  };

  // Creates an ipc using the specified implementation.
  // Must be called in the parent process.
  // Returns the pointer to the created ipc on success.
  // Returns NULL on error.
  static IpcInterface* Create(Type type);

  virtual ~IpcInterface();

  // Must be called in the parent, producer's and consumer's processes.
  // Returns true on success, returns false on error.
  virtual bool Close() = 0;

  // Must be called in the producer's process.
  // Returns true on success, returns false on error.
  virtual bool InitializeProducer() = 0;

  // Must be called in the consumer's process.
  // Returns true on success, returns false on error.
  virtual bool InitializeConsumer() = 0;

  // Returns true on success, returns false on error.
  virtual bool SendMessage(const std::string& message) = 0;

  // Blocks if there are no messages until one appears.
  // Returns true on success, returns false on error.
  virtual bool ReceiveMessage(std::string* message) = 0;
};

#endif /*_IPC_INTERFACE_HPP*/

