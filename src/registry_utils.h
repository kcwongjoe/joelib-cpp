#pragma once
#ifndef JW_REGISTRY_UTILS_H
#define JW_REGISTRY_UTILS_H


//************Content************
#include <string>
#include <vector>
#include <atlbase.h>
#include <winreg.h>
#include <tchar.h>

namespace Utils
{

/**
 * @brief The maximum length of the registry key.
 * @date 2021-03-17
 */
#define REGISTRY_MAX_KEY_LENGTH 255

	/**
	 * @brief Look up values under the key and subkey in registry and process.
	 * 
	 * @code{.cpp}
	 * // Print all hardware FriendlyName values
	 * int numOfValues = Utils::processRegistryValue(HKEY_LOCAL_MACHINE,
	 *			"SYSTEM\\CurrentControlSet\\Enum",
	 *			[](std::string valueName, DWORD dataType, unsigned char* data, int dataLen)
	 *			{
	 *				if (valueName.compare("FriendlyName") == 0 && dataType == REG_SZ)
	 *				{
	 *					// Found and convert to string, add to result
	 *					std::string dataString(reinterpret_cast<char*>(data), dataLen);
	 *					std::cout << dataString;
	 *				}
	 *			}
	 * );
	 * @endcode
	 * 
	 * @tparam ValueProcess void(std::string valueName, DWORD dataType, unsigned char[] data,DWORD sizeOfData)
	 * 
	 * @param rootKey The root key. @c HKEY_CLASSES_ROOT, @c HKEY_CURRENT_USER, @c HKEY_LOCAL_MACHINE, @c HKEY_USERS, @c HKEY_CURRENT_CONFIG, @c HKEY_CURRENT_USER_LOCAL_SETTINGS, @c HKEY_PERFORMANCE_DATA, @c HKEY_PERFORMANCE_NLSTEXT or @c HKEY_PERFORMANCE_TEXT.
	 * @param searchKey The key to be search. e.g. @c "HARDWARE\\DEVICEMAP\\SERIALCOMM"
	 * @param func The function to be processed on value.
	 * @param searchSubKey [Option] Default as true. If true,all of subkey under the key will also be processed. Set it as false if you only want to process the current key.
	 * @return int Return the number of value to be processed.
     * @date 2021-03-17
	 */
	template <typename ValueProcess> 
	static int processRegistryValue(HKEY rootKey, std::string searchKey, ValueProcess func, bool searchSubKey = true)
	{
		// Initilaize values
		int result = 0;

		// Create registry key
		HKEY hKey;
		if (RegOpenKeyEx(rootKey, searchKey.c_str(), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
		{
			// Get information of subkey
			DWORD numOfSubKey = 0;
			DWORD maxSubKeySize;
			DWORD numOfValue;
			DWORD maxValueNameSize = 0;
			DWORD maxValueSize = 0;
			DWORD retCode = RegQueryInfoKey(hKey, nullptr, nullptr, nullptr, &numOfSubKey, &maxSubKeySize, nullptr, &numOfValue, &maxValueNameSize, &maxValueSize, nullptr,	nullptr);

			// Get value in current key
			if (numOfValue > 0)
			{
				// Look up all value
#pragma warning(suppress : 26451)
				std::unique_ptr<char[]> valueNameBuffer(new char[maxValueNameSize + 1]);
				std::unique_ptr<unsigned char[]> dataByteBuffer(new unsigned char[maxValueSize]);
				for (int i = 0; i < (int)numOfValue; i++)
				{
					//TCHAR valueName[REGISTRY_MAX_VALUE_NAME_LENGTH];
					DWORD dataType;
					DWORD valueNameBufferLen = maxValueNameSize + 1;
					DWORD sizeOfDataByteBuffer = maxValueSize;

					// Get the subkey
					if (RegEnumValue(hKey, i, valueNameBuffer.get(), &valueNameBufferLen, NULL, &dataType, dataByteBuffer.get(), &sizeOfDataByteBuffer) == ERROR_SUCCESS)
					{
						// Convert value name to string
						std::string valueName = std::string(valueNameBuffer.get(), (int)valueNameBufferLen);

						// Process
						try
						{
							func(valueName, dataType, dataByteBuffer.get(), sizeOfDataByteBuffer);
						}
						catch (...)
						{
							// Release buffer
							valueNameBuffer.release();
							dataByteBuffer.release();

							// Close
							RegCloseKey(hKey);

							// Rethrow
							throw;
						}

						// Update counter
						result++;				
					}
				}
			}

			// Search value in subkey
			if (searchSubKey && numOfSubKey > 0)
			{
				// Look up all subkey
				for (int i = 0; i < (int)numOfSubKey; i++)
				{
					TCHAR subKey[REGISTRY_MAX_KEY_LENGTH];
					DWORD subKeyBufferLen = sizeof(subKey);

					// Get the subkey
					if (RegEnumKeyEx(hKey, i, subKey, &subKeyBufferLen, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
					{
						// Search under the subkey
						try
						{
							result += processRegistryValue(rootKey, searchKey + "\\" + subKey, func, searchSubKey);
						}
						catch (...)
						{
							// Close
							RegCloseKey(hKey);

							//Rethrow
							throw;
						}
						
					}
				}
			}			
		}

		// Close
		RegCloseKey(hKey);

		return result;
	}
}

//*******************************

#endif