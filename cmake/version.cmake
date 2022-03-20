# v = 3.5.4-6-gc83ad26-dirty
# bv = 3.5.4
# bv = 3.5. + f"{int('4') + 1}"

function(write_version VERSION)
    file(WRITE ${CMAKE_BINARY_DIR}/version ${VERSION})
endfunction()

function(read_version OUT_VAR)
    if(EXISTS ${CMAKE_BINARY_DIR}/version)
        file(STRINGS ${CMAKE_BINARY_DIR}/version CONTENT)
        list(GET CONTENT 0 VERSION)

        set(${OUT_VAR} ${VERSION} PARENT_SCOPE)
    endif()
endfunction()

function(get_semantic_version OUT_VAR)
    # produces something like 3.5.4-6-gc83ad26-dirty, which is of the form
    # MAJOR . MINOR . PATCH [- <number of revisions from HEAD> - <commit hash> - <workspace status>]
    execute_process(COMMAND git describe --dirty --abbrev
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE V
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    # if there's a hyphen, it means we're at (some tag) + changes, so first
    # extract the tag
    if(V MATCHES "^(.+)-.+$")
        # (it will be in match group 1 here)
        set(TAG ${CMAKE_MATCH_1})
        # next extract the PATCH number
        string(REGEX MATCH "^[0-9]+\\.[0-9]+\\.([0-9]+)$" PATCH "${TAG}")
        # and increment it
        math(EXPR PATCH "${PATCH} + 1")
        # finally reconstruct the tag with the new patch number
        string(REGEX REPLACE "^([0-9]+\\.[0-9]+\\.)[0-9]+$" "\\1.${PATCH}" TAG_INCREMENT "${TAG}")

        # Azure Pipelines PR builder will set these values
        if(DEFINED ENV{SYSTEM_PULLREQUEST_PULLREQUESTNUMBER})
            set(SEMPRE "pr$ENV{SYSTEM_PULLREQUEST_PULLREQUESTNUMBER}")
            set(BUILD $ENV{BUILD_BUILDID})
        else()
            # we're either at a commit hash or we have some local changes
            if(V MATCHES "^.+-dirty$")
                set(SEMPRE dirty)
            else()
                set(SEMPRE commit)
            endif()

            # generate a build number
            execute_process(COMMAND git rev-parse --short HEAD
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    OUTPUT_VARIABLE BUILD
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
            execute_process(COMMAND git rev-list "${TAG}..HEAD" --count
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    OUTPUT_VARIABLE NUMBER_SINCE
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

            set(BUILD "${NUMBER_SINCE}.${BUILD}")
        endif()

        # construct the final semver string
        set(${OUT_VAR} "${TAG_INCREMENT}-${SEMPRE}.${BUILD}" PARENT_SCOPE)
    else()
        # if there wasn't a hyphen that means we're clean and at a tag so just
        # return the tag
        set(${OUT_VAR} ${V} PARENT_SCOPE)
    endif()
endfunction()

function(update_version)
    get_semantic_version(SEMVER)
    read_version(VERSION_CACHE)

    if(NOT ${SEMVER} STREQUAL ${VERSION_CACHE} OR NOT EXISTS ${CMAKE_SOURCE_DIR}/include/api.h)
        write_version(${SEMVER})

        # extract the tag components
        string(REGEX REPLACE "^([0-9]+)\\.([0-9]+)\\([0-9]+).*" "\\1;\\2;\\3" TAG "${SEMVER}")
        list(GET TAG 0 MAJOR_VERSION)
        list(GET TAG 1 MINOR_VERSION)
        list(GET TAG 2 PATCH_VERSION)
        set(VERSION_STRING ${SEMVER})

        # configure api.h version macros
        configure_file(${CMAKE_SOURCE_DIR}/include/api.h.in ${CMAKE_SOURCE_DIR}/include.api.h @ONLY)
    endif()
endfunction()

function(setup_version_updater)
    add_custom_target(always_check_version
            COMMAND ${CMAKE_COMMAND} -DRUN_VERSION_CHECK -P ${CMAKE_MODULE_PATH}/version.cmake
            BYPRODUCTS ${CMAKE_SOURCE_DIR}/include/api.h)

    update_version()
endfunction()

if(DEFINED RUN_VERSION_CHECK)
    update_version()
endif()