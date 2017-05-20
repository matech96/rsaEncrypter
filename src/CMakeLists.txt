
set(SOURCE_FILES main.cpp BigInt.h Prime.cpp Prime.h RSAEncrypter.cpp RSAEncrypter.h)

#add_library(core ${core_SRCS})
#add_executable(exe main.cpp)
#target_link_libraries(exe core)


add_executable(sshEncrypter ${SOURCE_FILES})