#ifndef HASH_CALC_H
#define HASH_CALC_H

#include <array>
#include <atomic>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <sstream>
#include <thread>
#include <vector>

#include <future>
#include "mbedtls/crypto/include/mbedtls/sha256.h"
#include "utils.h"

namespace fs = std::filesystem;

namespace HashCalc {
constexpr uint64_t one_megabyte = 1048576;
constexpr uint64_t max_buffer_size = one_megabyte * 64;
}  // namespace HashCalc
//! Implementation of multi-thread SHA256 calculation
class CHashCalc {
 public:
  CHashCalc(const std::string& in_path,
            const std::string& out_path,
            uint64_t size = HashCalc::one_megabyte);

  ~CHashCalc() = default;

  CHashCalc(const CHashCalc&) = delete;

  CHashCalc& operator=(CHashCalc const&) = delete;

  CHashCalc(CHashCalc&&) = delete;

  CHashCalc& operator=(CHashCalc&&) = delete;

  void run();

 private:
  template <typename T>
  void throw_exception(T e) {
    m_thread_manager.stop();
    throw e;
  }

  void generate_hashes(uint32_t thread_id);

 private:
  fs::path m_in_file_path;
  fs::path m_out_file_path;
  uint64_t m_in_size;
  uint64_t m_block_size;
  std::atomic_size_t m_tasks_size;
  CThreadManager m_thread_manager;
  CTaskManager m_task_manager;
  CTimer m_timer;
  std::vector<char> m_buffer;
  CLockVec<std::pair<std::string, uint64_t>> m_hash_results;
};

#endif  // HASH_CALC_H
