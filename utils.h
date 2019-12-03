#ifndef SIGNATURE_UTILS_H
#define SIGNATURE_UTILS_H

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#else

#include <unistd.h>

#endif

//! Digest length for SHA256
constexpr uint64_t sha256_digest_length = 32;
//! Lambda for sorting
const auto sort_by_index = [](const std::pair<std::string, uint64_t>& a,
                              const std::pair<std::string, uint64_t>& b) {
  return (a.second < b.second);
};
//! Returns the number of cores,
inline uint32_t get_threads_count() {
  uint32_t core_count = std::thread::hardware_concurrency();
  if (!core_count) {
#ifdef _WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    core_count = sysinfo.dwNumberOfProcessors;
#elif __linux__
    core_count = uint32_t(sysconf(_SC_NPROCESSORS_ONLN));
#endif
  }
  return core_count;
}
//! This class provides execution time measurement.
class CTimer {
 public:
  CTimer() : m_is_running(false) {}

  ~CTimer() = default;

  CTimer(const CTimer&) = delete;

  CTimer& operator=(CTimer const&) = delete;

  CTimer(CTimer&&) = delete;

  CTimer& operator=(CTimer&&) = delete;

  inline void start() {
    m_is_running.store(true, std::memory_order_relaxed);
    m_t1 = std::chrono::high_resolution_clock::now();
  }

  inline CTimer& stop() {
    m_t2 = std::chrono::high_resolution_clock::now();
    m_is_running.store(false, std::memory_order_relaxed);
    return *this;
  }

  [[nodiscard]] inline bool is_running() const {
    return m_is_running.load(std::memory_order_relaxed);
  }

  [[nodiscard]] inline uint64_t get_nano() const {
    return uint64_t(std::chrono::duration_cast<std::chrono::nanoseconds>(m_t2 - m_t1).count());
  }

  [[nodiscard]] inline uint64_t get_micro() const {
    return uint64_t(std::chrono::duration_cast<std::chrono::microseconds>(m_t2 - m_t1).count());
  }

  [[nodiscard]] inline uint64_t get_milli() const {
    return uint64_t(std::chrono::duration_cast<std::chrono::milliseconds>(m_t2 - m_t1).count());
  }

 private:
  std::chrono::high_resolution_clock::time_point m_t1;
  std::chrono::high_resolution_clock::time_point m_t2;
  std::atomic_bool m_is_running;
};
//! Calculates SHA256 using mbedtls
static std::string calc_sha256(const char* ptr, const size_t block_size) {
  unsigned char hash[sha256_digest_length] = {};
  mbedtls_sha256_context ctx;
  mbedtls_sha256_init(&ctx);
  mbedtls_sha256_starts(&ctx, 0);
  mbedtls_sha256_update(&ctx, reinterpret_cast<const unsigned char*>(ptr), block_size);
  mbedtls_sha256_finish(&ctx, hash);

  std::ostringstream os;
  os << std::hex << std::setfill('0');

  for (auto i : hash) {
    os << std::setw(2) << static_cast<unsigned int>(i);
  }
  return os.str();
}
//! Implementation of the vector for use in a multi-threaded environment
template <typename T>
class CLockVec {
 public:
  CLockVec() = default;

  CLockVec(const CLockVec&) = delete;

  CLockVec& operator=(CLockVec const&) = delete;

  CLockVec(CLockVec&&) = delete;

  CLockVec& operator=(CLockVec&&) = delete;

  T& operator[](std::size_t index) {
    std::scoped_lock<std::mutex> lock(m_mutex);
    return m_vec[index];
  }

  std::size_t size() {
    std::scoped_lock<std::mutex> lock(m_mutex);
    return m_vec.size();
  }

  void clear() {
    std::scoped_lock<std::mutex> lock(m_mutex);
    m_vec.clear();
  }

  void push_back(T& element) {
    std::scoped_lock<std::mutex> lock(m_mutex);
    m_vec.push_back(element);
  }

  void emplace_back(T&& element) {
    std::scoped_lock<std::mutex> lock(m_mutex);
    m_vec.emplace_back(element);
  }

  bool empty() {
    std::scoped_lock<std::mutex> lock(m_mutex);
    return m_vec.empty();
  }

  void reserve(std::size_t n) {
    std::scoped_lock<std::mutex> lock(m_mutex);
    m_vec.reserve(n);
  }

  std::size_t capacity() {
    std::scoped_lock<std::mutex> lock(m_mutex);
    return m_vec.capacity();
  }

  template <typename FT>
  void sort(FT func) {
    std::scoped_lock<std::mutex> lock(m_mutex);
    std::sort(m_vec.begin(), m_vec.end(), func);
  }

  typename std::vector<T>::iterator begin() { return m_vec.begin(); }

  typename std::vector<T>::iterator end() { return m_vec.end(); }

  typename std::vector<T>::const_iterator cbegin() const { return m_vec.begin(); }

  typename std::vector<T>::const_iterator cend() const { return m_vec.end(); }

 private:
  std::vector<T> m_vec;
  std::mutex m_mutex;
};
//! Provides job management methods
class CTaskManager {
 public:
  CTaskManager() : m_task_index(0), m_tasks_done(0) {}

  ~CTaskManager() = default;

  CTaskManager(const CTaskManager&) = delete;

  CTaskManager& operator=(CTaskManager const&) = delete;

  CTaskManager(CTaskManager&&) = delete;

  CTaskManager& operator=(CTaskManager&&) = delete;

  std::optional<std::tuple<uint64_t, uint64_t, uint64_t>> get_task() {
    std::unique_lock lock(m_mutex);
    if (m_tasks.empty() || m_task_index >= m_tasks.size()) {
      return std::nullopt;
    }
    return m_tasks[m_task_index++];
  }

  void add_tasks(std::vector<std::tuple<uint64_t, uint64_t, uint64_t>> tasks) {
    std::unique_lock lock(m_mutex);
    m_task_index = 0;
    m_tasks = std::move(tasks);
  }

  bool empty() {
    std::unique_lock lock(m_mutex);
    return m_tasks.empty();
  }

  void task_done() {
    std::unique_lock lock(m_mutex);
    m_tasks_done++;
  }

  std::size_t done() {
    std::unique_lock lock(m_mutex);
    return m_tasks_done;
  }

 private:
  std::mutex m_mutex;
  std::size_t m_task_index;
  std::vector<std::tuple<uint64_t, uint64_t, uint64_t>> m_tasks;
  std::size_t m_tasks_done;
};
//! Provides simple thread pool management methods
class CThreadManager {
 public:
  explicit CThreadManager(uint32_t thread_count)
      : m_thread_count(thread_count),
        m_exit_counter(0),
        m_should_stop(false),
        m_should_wake(false),
        m_ready(false),
        m_threads_started(0) {}

  ~CThreadManager() = default;

  CThreadManager(const CThreadManager&) = delete;

  CThreadManager& operator=(CThreadManager const&) = delete;

  CThreadManager(CThreadManager&&) = delete;

  CThreadManager& operator=(CThreadManager&&) = delete;

  template <typename F>
  void run(F f) {
    for (uint32_t i = 0; i < m_thread_count; i++) {
      try {
        std::thread(f, i).detach();
        m_threads_started++;
      } catch (...) {
      }
    }
  }

  [[nodiscard]] inline bool is_stopping() const {
    return m_should_stop.load(std::memory_order_relaxed);
  }

  void stop() {
    // If no threads were started
    if (!m_threads_started) {
      return;
    }
    m_should_stop.store(true, std::memory_order_relaxed);
    while (m_exit_counter.load(std::memory_order_relaxed) != m_threads_started) {
      m_sleep_cv.notify_all();
    }
  }

  void wait_work() {
    m_ready.store(false, std::memory_order_relaxed);
    m_sleep_cv.notify_all();
    while (!m_ready.load(std::memory_order_relaxed))
      ;
  }

  void work_done() {
    m_ready.store(true, std::memory_order_relaxed);
    std::unique_lock<std::mutex> lock(m_mutex);
    m_sleep_cv.wait(lock, [this] {
      return !m_ready.load(std::memory_order_relaxed) ||
             m_should_stop.load(std::memory_order_relaxed);
    });
  }

  void report_exit(uint32_t id) { m_exit_counter.fetch_add(1, std::memory_order_relaxed); }

 private:
  std::mutex m_mutex;
  std::condition_variable m_sleep_cv;
  uint32_t m_thread_count;
  std::atomic_uint32_t m_exit_counter;
  std::atomic_bool m_should_stop;
  std::atomic_bool m_should_wake;
  std::atomic_bool m_ready;
  std::uint32_t m_threads_started;
};

#endif  // SIGNATURE_UTILS_H
