#include "directshow_utils.h"

namespace DirectShowUtils
{
#pragma region Get Error String

	/**
	 * @brief Check the HResult for CoCreateInstance() and convert to error string
	 * @param[in] hr HRESULT
	 * @param[out] errorString The output error string, errorDescription + " : " + HRSTRING
	 * @param[in] errorDescription Error description, default as "Error on CoCreateInstance()"
	 * @return Return true if success
     * @date 2021-03-17
	*/
	bool checkCoCreateInstanceResult(HRESULT hr, std::string* errorString, std::string errorDescription)
	{
		bool result = true;
		if (!SUCCEEDED(hr))
		{
			result = false;

			// Pass error string
			if (errorString != NULL)
			{
				*errorString = errorDescription + ": ";
				if (hr == REGDB_E_CLASSNOTREG)
				{
					*errorString += "REGDB_E_CLASSNOTREG";
				}
				else if (hr == CLASS_E_NOAGGREGATION)
				{
					*errorString += "CLASS_E_NOAGGREGATION";
				}
				else if (hr == E_NOINTERFACE)
				{
					*errorString += "E_NOINTERFACE";
				}
				else if (hr == E_POINTER)
				{
					*errorString += "The ppv parameter is NULL.";
				}
				else
				{
					*errorString += "hr = (" + std::to_string(hr) + ")";
				}
			}
		}

		return result;
	}

	/**
	 * @brief Check the HResult for CreateClassEnumerator() and convert to error string
	 * @param[in] hr HRESULT
	 * @param[out] errorString The output error string, errorDescription + " : " + HRSTRING
	 * @param[in] errorDescription (Option) Error description, default as "Error on CreateClassEnumerator()"
	 * @return Return true if success
     * @date 2021-03-17
	*/
	bool checkCreateClassEnumeratorResult(HRESULT hr, std::string* errorString, std::string errorDescription)
	{
		bool result = true;
		if (!SUCCEEDED(hr))
		{
			result = false;

			// Pass error string
			if (errorString != NULL)
			{
				*errorString = errorDescription + ": ";
				if (hr == E_OUTOFMEMORY)
				{
					*errorString += "There is not enough memory available to create a class enumerator.";
				}
				else if (hr == S_FALSE)
				{
					*errorString += "There is no any camera.";
				}
				else if (hr == E_POINTER)
				{
					*errorString += "NULL pointer argument.";
				}
				else
				{
					*errorString += "hr = (" + std::to_string(hr) + ")";
				}
			}
		}

		return result;
	}

	/**
	 * @brief Check the HResult for ICaptureGraphBuilder2::SetFiltergraph() and convert to error string
	 * @param[in] hr HRESULT
	 * @param[out] errorString The output error string, errorDescription + " : " + HRSTRING
	 * @param[in] errorDescription (Option) Error description, default as "Error on ICaptureGraphBuilder2::SetFiltergraph()"
	 * @return Return true if success
     * @date 2021-03-17
	*/
	bool checkICGB2SetFiltergraphResult(HRESULT hr, std::string* errorString, std::string errorDescription)
	{
		bool result = true;
		if (!SUCCEEDED(hr))
		{
			result = false;

			// Pass error string
			if (errorString != NULL)
			{
				*errorString = errorDescription + ": ";
				if (hr == E_UNEXPECTED)
				{
					*errorString += "Unexpected error.";
				}
				else if (hr == E_POINTER)
				{
					*errorString += "NULL pointer argument.";
				}
				else
				{
					*errorString += "hr = (" + std::to_string(hr) + ")";
				}
			}
		}

		return result;
	}

	/**
	 * @brief Check the HResult for IGraphBuilder::AddFilter() and convert to error string
	 * @param[in] hr HRESULT
	 * @param[out] errorString The output error string, errorDescription + " : " + HRSTRING
	 * @param[in] errorDescription (Option) Error description, default as "Error on IGraphBuilder::AddFilter()"
	 * @return Return true if success
     * @date 2021-03-17
	*/
	bool checkIGBAddFilterResult(HRESULT hr, std::string* errorString, std::string errorDescription)
	{
		bool result = true;
		if (!SUCCEEDED(hr) && hr != VFW_S_DUPLICATE_NAME)
		{
			result = false;

			// Pass error string
			if (errorString != NULL)
			{
				*errorString = errorDescription + ": ";
				if (hr == E_FAIL)
				{
					*errorString += "Failure.";
				}
				else if (hr == E_OUTOFMEMORY)
				{
					*errorString += "Insufficient memory.";
				}
				else if (hr == E_POINTER)
				{
					*errorString += "NULL pointer argument memory.";
				}
				else if (hr == VFW_E_CERTIFICATION_FAILURE)
				{
					*errorString += "Use of this filter is restricted by a software key.";
				}
				else if (hr == VFW_E_DUPLICATE_NAME)
				{
					*errorString += "Failed to add a filter with a duplicate name.";
				}
				else
				{
					*errorString += "hr = (" + std::to_string(hr) +")";
				}
			}
		}

		return result;
	}

	/**
	 * @brief Check the HResult for ICaptureGraphBuilder2::FindInterface() and convert to error string
	 * @param[in] hr HRESULT
	 * @param[out] errorString The output error string, errorDescription + " : " + HRSTRING
	 * @param[in] errorDescription (Option) Error description, default as "Error on ICaptureGraphBuilder2::FindInterface()"
	 * @return Return true if success
     * @date 2021-03-17
	*/
	bool checkICGB2FindInterfaceResult(HRESULT hr, std::string* errorString, std::string errorDescription)
	{
		bool result = true;
		if (!SUCCEEDED(hr))
		{
			result = false;

			// Pass error string
			if (errorString != NULL)
			{
				*errorString = errorDescription + ": ";
				if (hr == E_FAIL)
				{
					*errorString += "Failure.";
				}
				else if (hr == E_NOINTERFACE)
				{
					*errorString += "No such interface supported.";
				}
				else if (hr == E_POINTER)
				{
					*errorString += "NULL pointer argument memory.";
				}
				else
				{
					*errorString += "hr = (" + std::to_string(hr) + ")";
				}
			}
		}

		return result;
	}

	/**
	 * @brief Check the HResult for ICaptureGraphBuilder2::RenderStream() and convert to error string
	 * @param[in] hr HRESULT
	 * @param[out] errorString The output error string, errorDescription + " : " + HRSTRING
	 * @param[in] errorDescription (Option) Error description, default as "Error on ICaptureGraphBuilder2::RenderStream()"
	 * @return Return true if success
     * @date 2021-03-17
	*/
	bool checkICGB2RenderStreamResult(HRESULT hr, std::string* errorString, std::string errorDescription)
	{
		bool result = true;
		if (!SUCCEEDED(hr) && hr != VFW_S_DUPLICATE_NAME)
		{
			result = false;

			// Pass error string
			if (errorString != NULL)
			{
				*errorString = errorDescription + ": ";
				if (hr == E_FAIL)
				{
					*errorString += "Failure.";
				}
				else if (hr == VFW_S_NOPREVIEWPIN)
				{
					*errorString += "Preview was rendered through the Smart Tee Filter.";
				}
				else if (hr == E_INVALIDARG)
				{
					*errorString += "Invalid argument.";
				}
				else if (hr == E_POINTER)
				{
					*errorString += "NULL pointer argument.";
				}
				else if (hr == VFW_E_NOT_IN_GRAPH)
				{
					*errorString += "A filter is not in the filter graph. Check whether call AddFilter() and connect your graph to the Capture Graph Builder by SetFiltergraph().";
				}
				else
				{
					*errorString += "hr = (" + std::to_string(hr) + ")";
				}
			}
		}

		return result;
	}

	/**
	 * @brief Check the HResult for IAMStreamConfig::GetFormat() and convert to error string
	 * @param[in] hr HRESULT
	 * @param[out] errorString The output error string, errorDescription + " : " + HRSTRING
	 * @param[in] errorDescription (Option) Error description, default as "Error on IAMStreamConfig::GetFormat()"
	 * @return Return true if success
     * @date 2021-03-17
	*/
	bool checkIIAMSCGetFormatResult(HRESULT hr, std::string* errorString, std::string errorDescription)
	{
		bool result = true;
		if (!SUCCEEDED(hr))
		{
			result = false;

			// Pass error string
			if (errorString != NULL)
			{
				*errorString = errorDescription + ": ";
				if (hr == E_FAIL)
				{
					*errorString += "Failure.";
				}
				else if (hr == E_OUTOFMEMORY)
				{
					*errorString += "Insufficient memory.";
				}
				else if (hr == E_POINTER)
				{
					*errorString += "NULL pointer argument.";
				}
				else if (hr == VFW_E_NOT_CONNECTED)
				{
					*errorString += "The input pin is not connected.";
				}
				else
				{
					*errorString += "hr = (" + std::to_string(hr) + ")";
				}
			}
		}

		return result;
	}

	/**
	 * @brief Check the HResult for IAMStreamConfig::SetFormat() and convert to error string
	 * @param[in] hr HRESULT
	 * @param[out] errorString The output error string, errorDescription + " : " + HRSTRING
	 * @param[in] errorDescription (Option) Error description, default as "Error on IAMStreamConfig::SetFormat()"
	 * @return Return true if success
     * @date 2021-03-17
	*/
	bool checkIIAMSCSetFormatResult(HRESULT hr, std::string* errorString, std::string errorDescription)
	{
		bool result = true;
		if (!SUCCEEDED(hr))
		{
			result = false;

			// Pass error string
			if (errorString != NULL)
			{
				*errorString = errorDescription + ": ";
				if (hr == E_FAIL)
				{
					*errorString += "Failure.";
				}
				else if (hr == E_OUTOFMEMORY)
				{
					*errorString += "Insufficient memory.";
				}
				else if (hr == E_POINTER)
				{
					*errorString += "NULL pointer argument.";
				}
				else if (hr == VFW_E_INVALIDMEDIATYPE)
				{
					*errorString += "This media type is not valid.";
				}
				else if (hr == VFW_E_NOT_CONNECTED)
				{
					*errorString += "The input pin is not connected.";
				}
				else if (hr == VFW_E_NOT_STOPPED)
				{
					*errorString += "Cannot set the type; the filter is not stopped.";
				}
				else if (hr == VFW_E_WRONG_STATE)
				{
					*errorString += "Cannot set the type; the filter is not stopped.";
				}
				else
				{
					*errorString += "hr = (" + std::to_string(hr) + ")";
				}
			}
		}

		return result;
	}

	/**
	 * @brief Check the HResult for IAMStreamConfig::GetNumberOfCapabilities() and convert to error string
	 * @param[in] hr HRESULT
	 * @param[out] errorString The output error string, errorDescription + " : " + HRSTRING
	 * @param[in] errorDescription (Option) Error description, default as "Error on IAMStreamConfig::GetNumberOfCapabilities()"
	 * @return Return true if success
     * @date 2021-03-17
	*/
	bool checkIAMSCGetNumberOfCapabilitiesResult(HRESULT hr, std::string* errorString, std::string errorDescription)
	{
		bool result = true;
		if (!SUCCEEDED(hr))
		{
			result = false;

			// Pass error string
			if (errorString != NULL)
			{
				*errorString = errorDescription + ": ";
				if (hr == E_POINTER)
				{
					*errorString += "NULL pointer argument.";
				}
				else if (hr == VFW_E_NOT_CONNECTED)
				{
					*errorString += "The input pin is not connected.";
				}
				else
				{
					*errorString += "hr = (" + std::to_string(hr) + ")";
				}
			}
		}

		return result;
	}

	/**
	 * @brief Check the HResult for IUnknown::QueryInterface and convert to error string
	 * @param[in] hr HRESULT
	 * @param[out] errorString The output error string, errorDescription + " : " + HRSTRING
	 * @param[in] errorDescription (Option) Error description, default as "Error on IUnknown::QueryInterface"
	 * @return Return true if success
     * @date 2021-03-17
	*/
	bool checkQueryInterfaceResult(HRESULT hr, std::string* errorString, std::string errorDescription)
	{
		bool result = true;
		if (!SUCCEEDED(hr))
		{
			result = false;

			// Pass error string
			if (errorString != NULL)
			{
				*errorString = errorDescription + ": ";
				if (hr == E_NOINTERFACE)
				{
					*errorString += "Interface is not supported.";
				}
				else if (hr == E_POINTER)
				{
					*errorString += "NULL pointer argument.";
				}
				else
				{
					*errorString += "hr = (" + std::to_string(hr) + ")";
				}
			}
		}

		return result;
	}

#pragma endregion Get Error String

	/**
	 * @brief Deletes the format block in an AM_MEDIA_TYPE. e.g. AM_MEDIA_TYPE mt; freeMediaType(mt);
	 * @param amMediaType AM_MEDIA_TYPE
     * @date 2021-03-17
	*/
	void freeMediaType(AM_MEDIA_TYPE& amMediaType)
	{
		if (amMediaType.cbFormat != 0)
		{
			CoTaskMemFree((PVOID)amMediaType.pbFormat);
			amMediaType.cbFormat = 0;
			amMediaType.pbFormat = NULL;
		}
		if (amMediaType.pUnk != NULL)
		{
			// pUnk should not be used.
			amMediaType.pUnk->Release();
			amMediaType.pUnk = NULL;
		}
	}

	/**
	 * @brief Delete the pointer of AM_MEDIA_TYPE, include the format block. Use freeMediaType() for object. e.g. AM_MEDIA_TYPE* mt; deleteMediaType(&mt);
	 * @param amMediaType AM_MEDIA_TYPE to be deleted
     * @date 2021-03-17
	*/
	void deleteMediaType(AM_MEDIA_TYPE** amMediaType)
	{
		if (*amMediaType != NULL)
		{
			freeMediaType(**amMediaType);
			CoTaskMemFree(*amMediaType);
			*amMediaType = NULL;
		}
	}

	/**
	 * @brief Destroy graph. Reference from opencv::cap_dshow.cpp
	 * @param iGraphBuilder 
     * @date 2021-03-17
	*/
	void destroyGraph(IGraphBuilder* iGraphBuilder) {
	
		if (iGraphBuilder)
		{
			HRESULT hr = 0;

			int i = 0;
			while (hr == NOERROR)
			{
				IEnumFilters* pEnum = 0;
				ULONG cFetched = 0;

				// We must get the enumerator again every time because removing a filter from the graph
				// invalidates the enumerator. We always get only the first filter from each enumerator.
				hr = iGraphBuilder->EnumFilters(&pEnum);
				if (FAILED(hr)) { return; }

				IBaseFilter* pFilter = NULL;
				if (pEnum->Next(1, &pFilter, &cFetched) == S_OK)
				{
					FILTER_INFO FilterInfo;
					memset(&FilterInfo, 0, sizeof(FilterInfo));
					hr = pFilter->QueryFilterInfo(&FilterInfo);
					FilterInfo.pGraph->Release();

					int count = 0;
					char buffer[255];
					memset(buffer, 0, 255 * sizeof(char));

					while (FilterInfo.achName[count] != 0x00)
					{
						buffer[count] = (char)FilterInfo.achName[count];
						count++;
					}

					// Remove filter
					hr = iGraphBuilder->RemoveFilter(pFilter);
					if (FAILED(hr)) { return; }

					pFilter->Release();
					pFilter = NULL;
				}
				pEnum->Release();
				pEnum = NULL;

				if (cFetched == 0)
					break;
				i++;
			}
		}
	}

	/**
	 * @brief Take apart the graph from the capture device downstream to the null renderer
	 * @param iGraphBuilder 
	 * @param iBaseFilter 
     * @date 2021-03-17
	*/
	void nukeDownStream(IGraphBuilder* iGraphBuilder, IBaseFilter* iBaseFilter) {

		if (iGraphBuilder && iBaseFilter)
		{

			IPin* pP, * pTo;
			ULONG u;
			IEnumPins* pins = NULL;
			PIN_INFO pininfo;
			HRESULT hr = iBaseFilter->EnumPins(&pins);
			if (hr != S_OK || !pins)
				return;
			pins->Reset();
			while (hr == NOERROR)
			{
				hr = pins->Next(1, &pP, &u);
				if (hr == S_OK && pP)
				{
					pP->ConnectedTo(&pTo);
					if (pTo)
					{
						hr = pTo->QueryPinInfo(&pininfo);
						if (hr == NOERROR)
						{
							if (pininfo.dir == PINDIR_INPUT)
							{
								nukeDownStream(iGraphBuilder, pininfo.pFilter);
								iGraphBuilder->Disconnect(pTo);
								iGraphBuilder->Disconnect(pP);
								iGraphBuilder->RemoveFilter(pininfo.pFilter);
							}
							pininfo.pFilter->Release();
							pininfo.pFilter = NULL;
						}
						pTo->Release();
					}
					pP->Release();
				}
			}
			pins->Release();
		}
	}

#pragma region COM Library

	/**
	 * @brief Initialize COM library
	 *
	 * @return Return the status of initialized.
     * @date 2021-03-17
	*/
	bool initCOMLib()
	{
		HRESULT hr = NO_ERROR;

		if (!s_isInitializedCOMLib)
		{
			hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
		}
	
		return SUCCEEDED(hr);

	}

	/**
	 * @brief Uninitialize COM library
     * @date 2021-03-17
	*/
	void uninitCOMLib()
	{
		CoUninitialize();
		s_isInitializedCOMLib = false;
	}

#pragma endregion COM Library

	/**
	 * @brief Casting GUID to string
	 * @param guid GUID
	 * @return Return GUID string
     * @date 2021-03-17
	*/
	std::string to_string(GUID guid)
	{
		std::string result;

		// Get string
		LPOLESTR guidString;
		HRESULT hr = StringFromCLSID(guid, &guidString);
		if (SUCCEEDED(hr))
		{
			USES_CONVERSION;
			result = OLE2CA(guidString);
		}

		return result;
	}
}