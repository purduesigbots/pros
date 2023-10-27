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

std::vector<std::string> list_files(const char* path, char* buffer, int32_t len) {
	std::vector<std::string> files = {};
	// Call the C function
	int32_t success = usd_list_files_raw(path, buffer, len);
	// Check if call successful, if PROS_ERR return vector containing PROS_ERR
	if (success == PROS_ERR) {
		// push_back PROS_ERR to files vector
		files.push_back(std::to_string(success));
		return files;
	}
	// Parse buffer given call successful, split by '/n'
	// Store char * buffer as std::string in str
	// Store delimiter in std::string, '\n'
	std::string str(buffer);
	std::string delimiter = "\n";

	// position to keep track of position in str
	// file_name used to store each file name
	size_t position = 0;
	std::string file_name;

	// Loop until delimiter '\n' can not be found anymore
	while ((position = str.find(delimiter)) != std::string::npos) {
		// file_name is the string from the beginning of str to the first '\n', excluding '\n'
		file_name = str.substr(0, position);
		// Add token to files vector
		files.push_back(file_name);
		// Erase file_name from str, would be pos + 1 if we wanted to include '\n'
		str.erase(0, position + 1);
	}

	// return vector of file names
	return files;
}

}  // namespace usd
}  // namespace pros
