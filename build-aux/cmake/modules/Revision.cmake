# - Generate a cmake file containing repository information

if(NOT COMMAND REVISION)

  # Compute project-date, project-id and project-revision using the underlying
  # SCM.
  function(REVISION project-date project-id project-revision)

    # If we are in a Git repository.
    execute_process(COMMAND git rev-list --max-count=1 HEAD
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      TIMEOUT 10s
      RESULT_VARIABLE is-git-repo
      OUTPUT_QUIET ERROR_QUIET)
    if(is-git-repo EQUAL 0)
      git_revision(prj-date prj-id prj-revision)
    endif(is-git-repo EQUAL 0)

    set(${project-date} ${prj-date} PARENT_SCOPE)
    set(${project-id} ${prj-id} PARENT_SCOPE)
    set(${project-revision} ${prj-revision} PARENT_SCOPE)

  endfunction(REVISION)

  # Compute project-date, project-id and project-revision from the underlying
  # git repository.
  function(GIT_REVISION project-date project-id project-revision)

    function(GIT_REV_LIST format outvar)
      execute_process(COMMAND
	git rev-list --max-count=1 "--pretty=format:${format}" HEAD
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	TIMEOUT 10s
	RESULT_VARIABLE is-git-repo
	OUTPUT_VARIABLE out
	ERROR_VARIABLE err
	OUTPUT_STRIP_TRAILING_WHITESPACE)
      string(REGEX REPLACE "commit [0-9a-fA-F]*\n" "" out ${out})
      set(${outvar} ${out} PARENT_SCOPE)
      if(NOT is-git-repo EQUAL 0)
	message(FATAL_ERROR "git_rev_list: failed with format '${format}'
because '${err}'")
      endif(NOT is-git-repo EQUAL 0)
    endfunction(GIT_REV_LIST)

    git_rev_list("%ai" pkg-date)
    git_rev_list("%h (%ai %ae: %s)" pkg-id)
    git_rev_list("%h" pkg-rev)

    set(${project-date} ${pkg-date} PARENT_SCOPE)
    set(${project-id} ${pkg-id} PARENT_SCOPE)
    set(${project-revision} ${pkg-rev} PARENT_SCOPE)

  endfunction(GIT_REVISION)

endif(NOT COMMAND REVISION)
