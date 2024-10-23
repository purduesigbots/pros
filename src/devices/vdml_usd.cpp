/**
 * \file devices/usd.cpp
 *
 * Contains functions for interacting with the SD card.
 *
 * \copyright Copyright (c) 2017-2023, Purdue University ACM SIGBots.
 * All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "kapi.h"

namespace pros {
namespace usd {
using namespace pros::c;

std::int32_t is_installed(void) {
	return usd_is_installed();
}

std::int32_t list_files_raw(const char* path, char* buffer, int32_t len) {
	return usd_list_files_raw(path, buffer, len);
}

std::vector<std::string> list_files(const char* path) {
	std::vector<std::string> files = {};
	// malloc buffer to store file names
	size_t buffer_size = 10000;
	char *buffer = (char *) malloc(buffer_size);
	if (buffer == NULL) {
		// try again smaller buffer to see if that works
		buffer_size = 500;
		buffer = (char *) malloc(buffer_size);
		if (buffer == NULL) {
			// if still fails, return vector containing error state
			// set errno to ENOMEM
			errno = ENOMEM;
			files.push_back("ERROR");
			files.push_back("not enough memory to get file names");
			return files;
		}
	}

	// Check path user passed in
	std::string_view path_sv(path);
	constexpr std::string_view usd_prefix {"usd"};
	const size_t usd_prefix_idx = path_sv.find(usd_prefix);
	if (usd_prefix_idx == 0 || usd_prefix_idx == 1) {
		// Deal with when user prepends path with usd
		// as either "usd/..." or "/usd/..."
		path_sv.remove_prefix(usd_prefix.length() + usd_prefix_idx);
	}

	// set path to path_sv.data()
	path = path_sv.data();

	// Call the C function
	int32_t success = usd_list_files_raw(path, buffer, buffer_size);
	// Check if call successful, if error return vector containing error state
	if (success == PROS_ERR) {
		files.push_back("ERROR");
		// Check errno to see which error state occurred
		// push back error state to files vector as std::string
		if (errno == EINVAL || errno == ENOENT) {
			// errors related to path not found
			files.push_back("path not found");
		} else {
			// other error stats related to file io
			files.push_back("file i/o error");
		}
		return files;
	}

	// Parse buffer given call successful, split by '/n'
	std::string_view str(buffer);

	// delimter_pos is the position of the delimiter '\n'
	// index of which character to start substr from
	// file_name used to store each file name
	size_t delimiter_pos = 0;
	size_t index = 0;
	std::string file_name;

	// Loop until delimiter '\n' can not be found anymore
	while ((delimiter_pos = str.find('\n', index)) != std::string::npos) {
		// file_name is the string from the beginning of str to the first '\n', excluding '\n'
		file_name = std::string(str.data() + index, delimiter_pos - index);

		files.push_back(file_name);
		// Increment index to start substr from
		index = delimiter_pos + 1;

		// If index is greater than or equal to str length, break
		if (index >= str.length()) {
			break;
		}
	}

	// Free buffer
	free(buffer);

	// Return vector of file names
	return files;
}

}  // namespace usd
}  // namespace pros
