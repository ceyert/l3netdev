
set(APP_SRC ${PROJECT_SOURCE_DIR}/app/src)

add_executable(${APP_NAME}
${APP_SRC}/main.cc
)

target_include_directories(${APP_NAME} PUBLIC ${STACK_DIR} PUBLIC ${NETDEV_DIR} PUBLIC ${PROCCESSOR_DIR})

target_link_directories(${APP_NAME} PUBLIC ${STACK_DIR} PUBLIC ${NETDEV_DIR} PUBLIC ${PROCCESSOR_DIR})

target_link_libraries(${APP_NAME} PUBLIC stack_lib PUBLIC netdev_obj PUBLIC processor_obj)