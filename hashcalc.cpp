#include "hashcalc.h"
//!
//! \param in_path Incoming file path
//! \param out_path Output file path (SHA256 hashes for every block)
//! \param size Size of block
CHashCalc::CHashCalc(const std::string& in_path, const std::string& out_path, uint64_t size)
    : m_in_file_path(in_path),
      m_out_file_path(out_path),
      m_in_size(fs::file_size(m_in_file_path)),
      m_block_size(size),
      m_tasks_size(0),
      m_thread_manager(get_threads_count()) {
  // Throw exception if block size is bigger than max_size
  if (m_block_size > m_buffer.max_size()) {
    throw_exception(std::invalid_argument("Block size is too big " + std::to_string(m_block_size)));
  }
  uint64_t allocate_size;
  if (m_in_size <= m_block_size) {
    allocate_size = m_block_size;
  } else {
    const auto m_expected_size = (HashCalc::max_buffer_size / m_block_size);
    allocate_size = (m_expected_size)*m_block_size;
  }
  try {
    m_buffer = std::vector<char>(allocate_size);
  } catch (std::bad_alloc&) {
    throw_exception(std::invalid_argument("Can't allocate " + std::to_string(m_block_size)));
  }
  auto f = std::bind(&CHashCalc::generate_hashes, this, std::placeholders::_1);
  m_thread_manager.run(f);
}
//! Starts the calculation process, can throw exceptions runtime_error and invalid_argument. <br>
//! Generation of tasks takes place in the main thread, after this tasks processed in the number of
//! threads equal to the number of cores
void CHashCalc::run() {
  std::ifstream fs(m_in_file_path);
  if (!fs.is_open()) {
    throw_exception(
        std::runtime_error("Fatal error, couldn't open file " + m_in_file_path.string()));
  }
  // Throw exception if file is empty
  if (fs.peek() == std::ifstream::traits_type::eof()) {
    throw_exception(std::runtime_error("Fatal error, file is empty " + m_in_file_path.string()));
  }
  // Measure execution time
  m_timer.start();
  // Read until end, this function takes part (64mb) of file and breaks buffer into tasks, then it
  // waits for ready flag
  auto task_id = 0ull;
  auto all = 0ull;
  std::vector<std::tuple<uint64_t, uint64_t, uint64_t>> tasks;
  tasks.reserve(256);
  while (!fs.eof()) {
    auto buffer_ptr = 0ull;
    auto read = uint64_t(fs.read(m_buffer.data(), m_buffer.size()).gcount());
    if (!read) {
      break;
    }
    // Generate tasks
    for (; buffer_ptr < read; buffer_ptr += m_block_size) {
      if (all + m_block_size > m_in_size) {
        m_block_size = m_in_size - all;
      }
      tasks.emplace_back(std::make_tuple(buffer_ptr, m_block_size, ++task_id));
      all += m_block_size;
    }
    // Increase final tasks size
    m_tasks_size.fetch_add(tasks.size());
    m_task_manager.add_tasks(tasks);
    tasks.clear();
    m_thread_manager.wait_work();
  }
  // Stop other threads
  m_thread_manager.stop();
  // Open file with truncation
  std::ofstream out_file(m_out_file_path, std::ios::trunc);
  // If file successfully opened then run sort and write results to file
  if (!out_file.is_open()) {
    throw_exception(
        std::runtime_error("Fatal error, couldn't open output file " + m_out_file_path.string()));
  }
  // Sort results using index
  m_hash_results.sort(sort_by_index);
  // Write results to file
  for (auto& i : m_hash_results) {
    out_file << i.first << std::endl;
  }
  // Stop measurement
  m_timer.stop();
  std::cout << "Hashing completed (includes read file && write "
               "results) - "
            << m_timer.get_micro() << " us" << std::endl;
}
//! This method is launched from the class CTaskManager
//! \param thread_id - unique id for the thread
void CHashCalc::generate_hashes(uint32_t thread_id) {
  while (true) {
    auto task = m_task_manager.get_task();
    if (task.has_value()) {
      auto task_v = task.value();
      // Calculate SHA256 and insert result
      m_hash_results.emplace_back(
          std::make_pair(calc_sha256(m_buffer.data() + std::get<0>(task_v), std::get<1>(task_v)),
                         std::get<2>(task_v)));
      // Report on the completion of the task
      m_task_manager.task_done();
    }
    // Check if hashes ready
    if (m_tasks_size.load() != 0 && m_task_manager.done() == m_tasks_size.load()) {
      m_thread_manager.work_done();
    }
    // Stop it!
    if (m_thread_manager.is_stopping()) {
      break;
    }
  }
  m_thread_manager.report_exit(thread_id);
}
