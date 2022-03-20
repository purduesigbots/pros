#[=======================================================================[.rst:
FindV5rts
-------

Finds the VEX V5 SDK.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``V5rts::V5rts``
  The VEX V5 SDK

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``V5rts_FOUND``
  True if the system has the Foo library.
``V5rts_INCLUDE_DIRS``
  Include directories needed to use Foo.
``V5rts_LIBRARIES``
  Libraries needed to link to Foo.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``V5rts_INCLUDE_DIR``
  The directory containing ``foo.h``.
``V5rts_LIBRARY``
  The path to the Foo library.

#]=======================================================================]

set(V5rts_ROOT ${CMAKE_SOURCE_DIR}/firmware/libv5rts/sdk/vexv5)

find_path(V5rts_INCLUDE_DIR NAMES v5_api.h PATH_SUFFIXES include REQUIRED)
find_library(V5rts_LIBRARY NAMES libv5rts.a REQUIRED)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(V5rts
        REQUIRED_VARS
            V5rts_LIBRARY
            V5rts_INCLUDE_DIR
        REASON_FAILURE_MESSAGE "did you forget to clone the submodule?")

if(V5rts_FOUND)
    set(V5rts_LIBRARIES ${V5rts_LIBRARY})
    set(V5rts_INCLUDE_DIRS ${V5rts_INCLUDE_DIR})

    if(NOT TARGET V5rts::V5rts)
        add_library(V5rts::V5rts UNKNOWN IMPORTED)
        set_target_properties(V5rts::V5rts PROPERTIES
                IMPORTED_LOCATION "${V5rts_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${V5rts_INCLUDE_DIR}")
    endif()

    mark_as_advanced(V5rts_INCLUDE_DIR V5rts_LIBRARY)
endif()