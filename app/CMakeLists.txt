#if(GNOMON_ENABLE_WIDGETS)
#add_subdirectory(gnomon)
#endif(GNOMON_ENABLE_WIDGETS)

if(GNOMON_ENABLE_GUI)
  add_subdirectory(gnomon)
endif()
add_subdirectory(gnomonPipelineRunner)
