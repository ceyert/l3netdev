
add_library(processor_obj OBJECT 
${PROCCESSOR_SRC}/packet_processor.cc
)

target_include_directories(processor_obj PUBLIC ${PROCCESSOR_DIR} PUBLIC ${STACK_DIR})

