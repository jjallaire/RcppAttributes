#
# CMakeLists.txt
#
# Copyright (C) 2009-11 by RStudio, Inc.
#
# This program is licensed to you under the terms of version 3 of the
# GNU Affero General Public License. This program is distributed WITHOUT
# ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THOSE OF NON-INFRINGEMENT,
# MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. Please refer to the
# AGPL (http://www.gnu.org/licenses/agpl-3.0.txt) for more details.
#
#

cmake_minimum_required(VERSION 2.8)
project(RcppAttributes)

# include files
file(GLOB_RECURSE HEADER_FILES "*.h*")

set(SOURCE_FILES
  Main.cpp
)

add_executable(attribs ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(attribs
    ${CMAKE_DL_LIBS}
)
