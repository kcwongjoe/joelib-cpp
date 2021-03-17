#pragma once
#ifndef DIRECTSHOW_UTILS_H
#define DIRECTSHOW_UTILS_H

//************Content************
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <iostream>

#include <windows.h>
#include <ds_guid.h>
#include <atlconv.h>

// Direct show is native code, complier it in unmanaged
#pragma managed(push, off)
#include <dshow.h>
#pragma managed(pop)
#pragma comment(lib, "strmiids")



namespace DirectShowUtils
{
    // ******Get error string******
    bool checkCoCreateInstanceResult(HRESULT hr, std::string* errorString = NULL, std::string errorDescription = "Error on CoCreateInstance()");
    bool checkCreateClassEnumeratorResult(HRESULT hr, std::string* errorString = NULL, std::string errorDescription = "Error on CreateClassEnumerator()");
    bool checkICGB2SetFiltergraphResult(HRESULT hr, std::string* errorString = NULL, std::string errorDescription = "Error on ICaptureGraphBuilder2::SetFiltergraph()");
    bool checkIGBAddFilterResult(HRESULT hr, std::string* errorString = NULL, std::string errorDescription = "Error on IGraphBuilder::AddFilter()");
    bool checkICGB2FindInterfaceResult(HRESULT hr, std::string* errorString = NULL, std::string errorDescription = "Error on ICaptureGraphBuilder2::FindInterface()");
    bool checkICGB2RenderStreamResult(HRESULT hr, std::string* errorString = NULL, std::string errorDescription = "Error on ICaptureGraphBuilder2::RenderStream()");
    bool checkIIAMSCGetFormatResult(HRESULT hr, std::string* errorString = NULL, std::string errorDescription = "Error on IAMStreamConfig::GetFormat()");
    bool checkIIAMSCSetFormatResult(HRESULT hr, std::string* errorString = NULL, std::string errorDescription = "Error on IAMStreamConfig::SetFormat()");
	bool checkIAMSCGetNumberOfCapabilitiesResult(HRESULT hr, std::string* errorString = NULL, std::string errorDescription = "Error on IAMStreamConfig::GetNumberOfCapabilities()");
	bool checkQueryInterfaceResult(HRESULT hr, std::string* errorString = NULL, std::string errorDescription = "Error on IUnknown::QueryInterface()");

    // ******Release******
    void freeMediaType(AM_MEDIA_TYPE& amMediaType);
	void deleteMediaType(AM_MEDIA_TYPE** amMediaType);
	void destroyGraph(IGraphBuilder* iGraphBuilder);
	void nukeDownStream(IGraphBuilder* iGraphBuilder, IBaseFilter* iBaseFilter);

	/**
	 * @brief Safe release COM interface pointers. e.g. IBaseFilter* iBaseFilter; SafeRelease(&iBaseFilter);
	 * @tparam T COM interface pointers
	 * @param ppT Pointer of COM interface pointers
     * @date 2021-03-17
	*/
	template <class T>
	void SafeRelease(T** ppT)
	{
		if (*ppT != NULL)
		{
			(*ppT)->Release();
			*ppT = NULL;
		}
	}

	// ******COM library******

	/**
	 * @brief A static flag to record whether COMLib be initialized.
	 */
	static bool s_isInitializedCOMLib = false;
	bool initCOMLib();
	void uninitCOMLib();

	// ******Other******

	std::string to_string(GUID guid);

	/**
	 * @brief A decorator to extract IMoniker and IPropertyBag. This can be use to retreve the camera information by setting clsid as CLSID_VideoInputDeviceCategory.
	 *
	 * @tparam Func void(IMoniker*, IPropertyBag*)
	 * @param[in] clsid Category of IEnumMoniker
	 * @param[in] func Lambda function to process IMoniker and IPropertyBag
	 * @param[out] errorString (Option) Error String. Default as NULL
	 * @return bool Return true if success.
     * @date 2021-03-17
	*/
	template <typename Func>
	bool iPropertyDecorator(GUID clsid, Func func, std::string* errorString = NULL)
	{
		bool result = true;

		// Create the System Device Enumerator
		ICreateDevEnum* iCreateDevEnum = NULL;
		if (result)
		{
			HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&iCreateDevEnum));
			result = checkCoCreateInstanceResult(hr, errorString, "Error on creating the ICreateDevEnum");
		}

		// Obtain a class enumerator for the video input category.
		IEnumMoniker* iEnumMoniker = NULL;
		if (result)
		{
			HRESULT hr = iCreateDevEnum->CreateClassEnumerator(clsid, &iEnumMoniker, 0);
			result = checkCreateClassEnumeratorResult(hr, errorString, "Error on obtaining the IEnumMoniker");
		}

		// Enumerate the monikers
		if (result)
		{
			IPropertyBag* propertyBag = NULL;
			IMoniker* moniker = NULL;

			try
			{
				while (iEnumMoniker->Next(1, &moniker, NULL) == S_OK)
				{
					// Get a pointer to the IPropertyBag interface
					HRESULT hr = moniker->BindToStorage(0, 0, IID_PPV_ARGS(&propertyBag));
					if (FAILED(hr))
					{
						SafeRelease(&moniker);
						continue;
					}

					// Run the function
					func(moniker, propertyBag);

					// Release
					SafeRelease(&propertyBag);
					SafeRelease(&moniker);
				}
			}
			catch (...)
			{
				// Release
				SafeRelease(&propertyBag);
				SafeRelease(&moniker);
				SafeRelease(&iEnumMoniker);
				SafeRelease(&iCreateDevEnum);

				// Rethrow
				throw;
			}

			// Release
			SafeRelease(&iEnumMoniker);
			SafeRelease(&iCreateDevEnum);

		}



		return result;
	}

	/**
	 * @brief A decorator to extract IPin and PIN_INFO from IMoniker. Use iPropertyDecorator() and amMediaTypeDecorator() can retrieve the video format from cameras.
	 * @tparam Func void(IPin*, PIN_INFO*)
	 * @param[in] iMoniker IMoniker
	 * @param[in] func Lambda function to process IPin and PIN_INFO
	 * @param[out] errorString (Option) Error String. Default as NULL
	 * @return Return true if success.
     * @date 2021-03-17
	*/
	template <typename Func>
	bool iPinDecorator(IMoniker* iMoniker, Func func, std::string* errorString = NULL)
	{
		if (!iMoniker) return false;

		bool result = true;

		// Get IBaseFilter
		IBaseFilter* iBaseFilter;
		if (result)
		{
			HRESULT hr = iMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void**)&iBaseFilter);
			if (FAILED(hr))
			{
				result = false;
			}
		}

		// Get IEnumPins
		IEnumPins* iEnumPin = NULL;
		if (result)
		{
			HRESULT hr = iBaseFilter->EnumPins(&iEnumPin);
			if (FAILED(hr))
			{
				SafeRelease(&iEnumPin);
				result = false;
			}
		}

		if (result)
		{
			// Start at beginning
			iEnumPin->Reset();

			ULONG pinFetched = 0;
			IPin* iPin = NULL;
			try
			{
				while (SUCCEEDED(iEnumPin->Next(1, &iPin, &pinFetched)) && pinFetched) {

					// Get PIN_INFO
					PIN_INFO pinInfo;
					if (FAILED(iPin->QueryPinInfo(&pinInfo))) {
						continue;
					}

					// Run
					func(iPin, &pinInfo);

					// Release
					SafeRelease(&iPin);
				}
			}
			catch (...)
			{
				// Release
				SafeRelease(&iPin);
				SafeRelease(&iEnumPin);
				SafeRelease(&iBaseFilter);

				// Rethrow
				throw;
			}

			// Release
			SafeRelease(&iEnumPin);
			SafeRelease(&iBaseFilter);
		}

		return result;
	}

	/**
	 * @brief A decorator to process AM_MEDIA_TYPE from IPin.
	 * @tparam Func void(AM_MEDIA_TYPE*)
	 * @param[in] iPin IPin
	 * @param[in] func Lambda function to process AM_MEDIA_TYPE
	 * @param[out] errorString (Option) Error String. Default as NULL.
	 * @param[in] releaseAMMediaType (Option) Set it as false if you release AM_MEDIA_TYPE in func(). Default as true which will release AM_MEDIA_TYPE after func() run.
	 * @return Return true if success.
     * @date 2021-03-17
	*/
	template <typename Func>
	bool amMediaTypeDecorator(IPin* iPin, Func func, std::string* errorString = NULL, bool releaseAMMediaType = true)
	{
		if (!iPin) return false;

		bool result = true;

		// Get IEnumMediaTypes
		IEnumMediaTypes* iEnumMediaType = NULL;
		if (FAILED(iPin->EnumMediaTypes(&iEnumMediaType)))
		{
			result = false;
		}

		if (result)
		{
			AM_MEDIA_TYPE* amMediaType = NULL;
			iEnumMediaType->Reset();

			ULONG amMediaTypeFetched = 0;
			try {
				while (SUCCEEDED(iEnumMediaType->Next(1, &amMediaType, &amMediaTypeFetched)) && amMediaTypeFetched) {

					// Run
					func(amMediaType);

					//Release
					if (releaseAMMediaType) deleteMediaType(&amMediaType);
				}
			}
			catch (...)
			{
				// Release
				if (releaseAMMediaType) deleteMediaType(&amMediaType);
				SafeRelease(&iEnumMediaType);

				// Rethrow
				throw;
			}

			// Release
			if (releaseAMMediaType) deleteMediaType(&amMediaType);
			SafeRelease(&iEnumMediaType);
		}

		return result;
	}

	/**
	 * @brief A decorator to process AM_MediaType
	 * @tparam func void(AM_MEDIA_TYPE*)
	 * @param[in] streamConfig StreamConfig to get the AM_MediaType
	 * @param[in] func Lambda funciton for processing AM_MediaType
	 * @param[out] errorString (Option) Error String. Default as NULL.
	 * @return bool Return true if success.
     * @date 2021-03-17
	*/
	template <typename MediaTypeFunc>
	bool amMediaTypeDecorator(IAMStreamConfig* streamConfig, MediaTypeFunc func, std::string* errorString = NULL)
	{
		bool result = true;
		HRESULT hr = NO_ERROR;

		AM_MEDIA_TYPE* amMediaType;
		if (result)
		{
			hr = streamConfig->GetFormat(&amMediaType);
			result = DirectShowUtils::checkIIAMSCGetFormatResult(hr, errorString, "Error on getting media type");
		}

		//Run the function
		if (result)
		{
			try
			{
				func(amMediaType);
			}
			catch (...)
			{
				//Delete media type
				DirectShowUtils::deleteMediaType(&amMediaType);
				throw;
			}

			//Delete media type
			DirectShowUtils::deleteMediaType(&amMediaType);
		}	

		return result;
	}

	/**
	 * @brief A decorator to process IAMVideoProcAmp
	 * @tparam func void(IAMVideoProcAmp*)
	 * @param[in] videoInputFilter Video Input Filter to get the IAMVideoProcAmp
	 * @param[in] func Lambda funciton for processing IAMVideoProcAmp
	 * @param[out] errorString  (Option) Error String. Default as NULL.
	 * @return bool Return true if success.
     * @date 2021-03-17
	*/
	template <typename amVideoProcAmpFunc>
	bool amVideoProcAmpDecorator(IBaseFilter* videoInputFilter, amVideoProcAmpFunc func, std::string* errorString = NULL)
	{
		bool success;
		HRESULT hr = NO_ERROR;

		IAMVideoProcAmp* amVideoProcAmp = NULL;
		hr = videoInputFilter->QueryInterface(IID_IAMVideoProcAmp, (void**)&amVideoProcAmp);
		success = DirectShowUtils::checkQueryInterfaceResult(hr, errorString, "Error on getting IAMVideoProcAmp");

		//Run the function
		if (success)
		{
			try
			{
				func(amVideoProcAmp);
			}
			catch (...)
			{
				// Release
				SafeRelease(&amVideoProcAmp);				

				// Rethrow
				throw;
			}

			// Release
			SafeRelease(&amVideoProcAmp);

			return true;
		}
		else
		{
			return false;
		}
	}

	/**
	 * @brief A decorator to process IAMCameraControl
	 * @tparam func void(IAMCameraControl*)
	 * @param[in] videoInputFilter Video Input Filter to get the IAMCameraControl
	 * @param[in] func Lambda funciton for processing IAMCameraControl
	 * @param[out] errorString  (Option) Error String. Default as NULL.
	 * @return bool Return true if success.
     * @date 2021-03-17
	*/
	template <typename CameraControlFunc>
	bool amCameraControlDecorator(IBaseFilter* videoInputFilter, CameraControlFunc func, std::string* errorString = NULL)
	{
		bool success;
		HRESULT hr = NO_ERROR;

		IAMCameraControl* amCameraControl = NULL;
		hr = videoInputFilter->QueryInterface(IID_IAMCameraControl, (void**)&amCameraControl);
		success = DirectShowUtils::checkQueryInterfaceResult(hr, errorString, "Error on getting camera control");

		//Run the function
		if (success)
		{
			try
			{
				func(amCameraControl);
			}
			catch (...)
			{
				// Release
				SafeRelease(&amCameraControl);

				// Rethrow
				throw;
			}

			// Release
			SafeRelease(&amCameraControl);

			return true;
		}
		else
		{
			return false;
		}
	}
}


//*******************************

#endif