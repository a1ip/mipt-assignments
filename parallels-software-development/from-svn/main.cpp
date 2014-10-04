#include <algorithm>

#include "file_storage.hpp"
#include "logger.hpp"
#include "mapped_file.hpp"
#include "merge_sort.hpp"
#include "csv_reader.hpp"
#include "csv_writer.hpp"

int main(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
    return EXIT_FAILURE;
  }

  Timer timer;

  bool rv = Logger::GetInstance().Initialize("log.txt");
  if (rv == false) {
    fprintf(stderr, "Unable to initialize logger.\n");
    return EXIT_FAILURE;
  }

  Logger::GetInstance().SetLevel(Logger::LEVEL_INFO);

  Logger::GetInstance().Log(Logger::LEVEL_INFO, "Logger has been initialized.");

  CSVReader reader;
  rv = reader.Open(argv[1]);
  if (rv == false) {
    fprintf(stderr, "Unable to open csv file '%s'.\n", argv[1]);
    return EXIT_FAILURE;
  }

  Logger::GetInstance().Log(Logger::LEVEL_INFO, "File '%s' has been opened for reading.", argv[1]);

  FileStorage input_storage;
  rv = input_storage.Open("merge_sort_input.dat");
  if (rv == false) {
    fprintf(stderr, "Unable to open input storage.");
    return EXIT_FAILURE;
  }

  Logger::GetInstance().Log(Logger::LEVEL_INFO, "Storage for input data has been opened.");

  bool has_next;
  while (true) {
    rv = reader.HasNext(&has_next);
    if (rv == false) {
      fprintf(stderr, "Unable to read from '%s'.\n", argv[1]);
      return EXIT_FAILURE;
    }
    if (!has_next) {
      break;
    }
    int32_t current = reader.ReadNext();
    rv = input_storage.PushBack(current);
    if (rv == false) {
      fprintf(stderr, "Unable to write to input storage.");
      return EXIT_FAILURE;
    }
  }

  rv = reader.Close();
  if (rv == false) {
    fprintf(stderr, "Unable to close csv file '%s'.\n", argv[1]);
    return EXIT_FAILURE;
  }

  Logger::GetInstance().Log(Logger::LEVEL_INFO, "File '%s' has been closed.", argv[1]);

  size_t numbers_count = input_storage.GetSize();

  FileStorage output_storage;
  rv = output_storage.Open("merge_sort_output.dat", numbers_count);
  if (rv == false) {
    fprintf(stderr, "Unable to open output storage.");
    return EXIT_FAILURE;
  }

  Logger::GetInstance().Log(Logger::LEVEL_INFO, "Storage for output data has been opened.");

  rv = MergeSort(&input_storage, &output_storage);
  if (rv == false) {
    fprintf(stderr, "Unable to perform merge sort.");
    return EXIT_FAILURE;
  }

  Logger::GetInstance().Log(Logger::LEVEL_INFO, "Integers has been sorted.");

  rv = input_storage.Close();
  if (rv == false) {
    fprintf(stderr, "Unable to close input storage.");
    return EXIT_FAILURE;
  }

  Logger::GetInstance().Log(Logger::LEVEL_INFO, "Storage for input data has been closed.");

  CSVWriter writer;
  rv = writer.Open(argv[2]);
  if (rv == false) {
    fprintf(stderr, "Unable to open csv file '%s'.\n", argv[1]);
    return EXIT_FAILURE;
  }

  Logger::GetInstance().Log(Logger::LEVEL_INFO, "File '%s' has been opened for writing.", argv[2]);

  for (size_t i = 0; i < numbers_count; i++) {
    int32_t current;
    rv = output_storage.Get(i, &current);
    if (rv == false) {
      fprintf(stderr, "Unable to read from output storage.");
      return EXIT_FAILURE;
    }
    rv = writer.WriteNext(current);
    if (rv == false) {
      fprintf(stderr, "Unable to write to '%s'.\n", argv[2]);
      return EXIT_FAILURE;
    }
  }

  rv = output_storage.Close();
  if (rv == false) {
    fprintf(stderr, "Unable to close output storage.");
    return EXIT_FAILURE;
  }

  Logger::GetInstance().Log(Logger::LEVEL_INFO, "Storage for output data has been closed.");

  rv = writer.Close();
  if (rv == false) {
    fprintf(stderr, "Unable to close csv file '%s'.\n", argv[1]);
    return EXIT_FAILURE;
  }

  Logger::GetInstance().Log(Logger::LEVEL_INFO, "File '%s' has been closed.", argv[2]);

  Logger::GetInstance().Log(Logger::LEVEL_INFO, "Finalizing logger...");

  rv = Logger::GetInstance().Finalize();
  if (rv == false) {
    fprintf(stderr, "Unable to finalize logger.\n");
    return EXIT_FAILURE;
  }

  rv = RemoveFile("merge_sort_input.dat");
  if (rv == false) {
    fprintf(stderr, "Unable to remove input storage file.\n");
    return EXIT_FAILURE;
  }

  rv = RemoveFile("merge_sort_output.dat");
  if (rv == false) {
    fprintf(stderr, "Unable to remove output storage file.\n");
    return EXIT_FAILURE;
  }

  printf("Time elapsed: %.3lf ms\n", timer.Time());

  return EXIT_SUCCESS;
}
