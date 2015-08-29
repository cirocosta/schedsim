#ifndef SCHEDSIM__COMMON_H
#define SCHEDSIM__COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* static size_t SM_PID_MAX = 0; */

#define FREE(__ptr)                                                            \
  do {                                                                         \
    free(__ptr);                                                               \
    __ptr = 0;                                                                 \
  } while (0)

#define FREE_ARR(__ptr, __count)                                               \
  do {                                                                         \
    unsigned __i = 0;                                                          \
    while (__i < __count)                                                      \
      free(__ptr[__i++]);                                                      \
    free(__ptr);                                                               \
    __ptr = 0;                                                                 \
  } while (0)

#define LOG(__msg, ...)                                                        \
  do {                                                                         \
    fprintf(stdout, "\n(LOG)\t");                                              \
    fprintf(stdout, __msg, ##__VA_ARGS__);                                     \
  } while (0)

#define LOGERR(__msg, ...)                                                     \
  do {                                                                         \
    fprintf(stderr, "\n(LOGERR)\t");                                           \
    fprintf(stderr, __msg, ##__VA_ARGS__);                                     \
  } while (0)

#define ASSERT(__cond, __msg, ...)                                             \
  do {                                                                         \
    if (!(__cond)) {                                                           \
      fprintf(stderr, "\n" __BASE_FILE__ ": %2d\n", __LINE__);                 \
      fprintf(stderr, "Assertion `%s` failed\n", #__cond);                     \
      fprintf(stderr, "\t" __msg "\n", ##__VA_ARGS__);                         \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

#define TEST(__test, ...)                                                      \
  do {                                                                         \
    __test();                                                                  \
    LOG("%s: %s:" #__VA_ARGS__ " OK!", __BASE_FILE__, #__test);                \
  } while (0)

#ifndef NDEBUG
#define DASSERT(__cond, __msg, ...) ASSERT(__cond, __msg, #__VA_ARGS__)
#else
#define DASSERT(__cond, __msg, ...)                                            \
  do {                                                                         \
  } while (0)
#endif

// dynamically queries for the maximum
// number of concurrent processes that a linux
// system allows.
#if 0
inline static size_t sm_get_pid_max()
{
  int fd = -1;
  char buf[32] = { 0 };
  char *end = NULL;

  if (SM_PID_MAX)
    return SM_PID_MAX;

  // FIXME provide error checking
  fd = open("/proc/sys/kernel/pid_max", 'r');
  read(fd, buf, 32);
  close(fd);

  SM_PID_MAX = strtol(buf, &end, 10);
  DASSERT((end != buf), "Couldn't convert %s to size_t", buf);

  return SM_PID_MAX;
}
#endif

#endif // ! SCHEDSIM__COMMON_H
