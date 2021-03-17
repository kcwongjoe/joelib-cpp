#include <thread_utils.h>

namespace Utils
{
#pragma region waitingForFinish

    /**
     * @brief A Waiting loop which wait for "stopWaiting" to true
     * 
     * @code{.cpp}
     * // Mutex Flag
     * std::atomic<bool> mutex = false;
     * 
     * // Create another thread to set mutex as true after 2 second.
     * std::thread t([&mutex]()
     *     {
     *         std::this_thread::sleep_for(std::chrono::seconds(2));
     *         mutex = true;
     *     });
     * t.detach();
     * 
     * // Wait
     * Utils::waitingForFinish(&mutex);
     * @endcode
     *
     * @param[in] stopWaiting Loop will be stopped when this vairable is true.
     * @param[in] delayms (Option) delay time in ms for each iteration .Default as 50ms
     * @param[in] timeout (Option) Time out in ms. Loop will be stopped when time out.

     * @return Return true if success. False will be returned if timeout.
     * @date 2021-03-17
     */
    bool waitingForFinish(std::atomic<bool>* stopWaiting, int delayms, int timeout)
    {
        return waitingForFinish(stopWaiting, [](std::atomic<bool>* dummy) {}, delayms, timeout);
    }

    /**
     * @brief A Waiting loop which wait for the internal flag as true
     *
     * @code{.cpp}
     * // Wait
     * Utils::waitingForFinish([](std::atomic<bool>* mutex)
     *     {
     *         std::this_thread::sleep_for(std::chrono::seconds(2));
     *         *mutex = true;
     *     }
     * );
     * @endcode
     * 
     * @param[in] func void(std::atomic<bool>* stopWaiting). Function which will be run in each iteration. Use "*stopWaiting = true;" in the funciton to stop the loop.
     * @param[in] delayms (Option) delay time in ms for each iteration .Default as 50ms
     * @param[in] timeout (Option) Time out in ms. Loop will be stopped when time out.

     * @return Return true if success. False will be returned if timeout.
     * @date 2021-03-17
     */
    bool waitingForFinish(std::function<void(std::atomic<bool>*)> func, int delayms, int timeout)
    {
        return waitingForFinish(NULL, func, delayms, timeout);
    }

    /**
     * @brief A Waiting loop which wait for "stopWaiting" to true
     *
     * @code{.cpp}
     * // Mutex Flag
     * std::atomic<bool> mutex = false;
     * 
     * // Wait for 2 second
     * Utils::waitingForFinish(&mutex,
     *     [&mutex]()
     *     {
     *         std::this_thread::sleep_for(std::chrono::seconds(2));
     *         mutex = true;
     *     }
     * );
     * @endcode
     * 
     * @param[in] stopWaiting Loop will be stopped when this vairable is true.
     * @param[in] func void(). Function which will be run in each iteration
     * @param[in] delayms (Option) delay time in ms for each iteration .Default as 50ms
     * @param[in] timeout (Option) Time out in ms. Loop will be stopped when time out.

     * @return Return true if success. False will be returned if timeout.
     * @date 2021-03-17
     */
    bool waitingForFinish(std::atomic<bool>* stopWaiting, std::function<void()> func, int delayms, int timeout)
    {
        return waitingForFinish(stopWaiting, [&func](std::atomic<bool>* dummy) {func(); }, delayms, timeout);
    }

	/**
	 * @brief A Waiting loop which wait for "stopWaiting" to true
     * 
     * @code{.cpp}
     * // Mutex Flag
     * std::atomic<bool> mutex = false;
     * 
     * // Wait for 2 second
     * Utils::waitingForFinish(&mutex,
     *     [](std::atomic<bool>* mutex)
     *     {
     *         std::this_thread::sleep_for(std::chrono::seconds(2));
     *         *mutex = true;
     *     }
     * );
     * @endcode
	 *
	 * @param[in] stopWaiting Loop will be stopped when this vairable is true.
     * @param[in] func void(std::atomic<bool>* stopWaiting). Function which will be run in each iteration. Use "*stopWaiting = true;" in the funciton to stop the loop.
	 * @param[in] delayms (Option) delay time in ms for each iteration .Default as 50ms
	 * @param[in] timeout (Option) Time out in ms. Loop will be stopped when time out.

	 * @return Return true if success. False will be returned if timeout.
     * @date 2021-03-17
	 */
	bool waitingForFinish(std::atomic<bool>* stopWaiting, std::function<void(std::atomic<bool>*)> func, int delayms, int timeout)
    {
        int passms = 0;

        // Initalize stopWaiting
        bool requireRelease = false;
        if (!stopWaiting)
        {
            stopWaiting = new std::atomic<bool>(false);
            requireRelease = true;
        }
        
        while (!stopWaiting->load() && passms <= timeout)
        {
            // Wait
            std::this_thread::sleep_for(std::chrono::milliseconds(delayms));
            passms += delayms;

            // Process
            func(stopWaiting);
        }

        // Release
        if (requireRelease) delete stopWaiting;

        // Return
        if (passms > timeout)
        {
            return false;
        }
        else
        {
            return true;
        }
    }

#pragma endregion waitingForFinish
}