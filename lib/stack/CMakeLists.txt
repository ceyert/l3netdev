add_library(stack_lib SHARED 
${STACK_SRC}/arp.cc
${STACK_SRC}/ethernet.cc
${STACK_SRC}/icmp.cc
${STACK_SRC}/ip.cc
${STACK_SRC}/route.cc
${STACK_SRC}/tap.cc
${STACK_SRC}/utils/utils.cc
)

target_include_directories(stack_lib PUBLIC ${STACK_DIR})

