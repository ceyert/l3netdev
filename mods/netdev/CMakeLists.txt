add_library(netdev_obj OBJECT 
${NETDEV_SRC}/netdev.cc
)

target_include_directories(netdev_obj PUBLIC ${NETDEV_DIR} PUBLIC ${STACK_DIR})

