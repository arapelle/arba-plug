function(resolve_external_git_project project_name repo_url branch)
  if(NOT EXISTS "${GIT_EXECUTABLE}")
      find_package(Git REQUIRED)
  endif()
  set(cmtk_dir "${CMAKE_CURRENT_LIST_DIR}/${project_name}/")
  message(STATUS "Resolve ${project_name} ${branch}")
  if(IS_DIRECTORY "${cmtk_dir}/.git")
      execute_process(COMMAND ${GIT_EXECUTABLE} checkout ${branch} WORKING_DIRECTORY "${cmtk_dir}")
  else()
      execute_process(COMMAND ${GIT_EXECUTABLE} clone "${repo_url}" --branch ${branch} "${cmtk_dir}")
  endif()
endfunction()

add_subdirectory(cmake)
