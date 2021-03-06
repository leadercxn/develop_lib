/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 *
 * @defgroup app_scheduler Scheduler
 * @{
 * @ingroup app_common
 *
 * @brief The scheduler is used for transferring execution from the interrupt context to the main
 *        context.
 *
 * @details See @ref seq_diagrams_sched for sequence diagrams illustrating the flow of events
 *          when using the Scheduler.
 *
 * @section app_scheduler_req Requirements:
 *
 * @subsection main_context_logic Logic in main context:
 *
 *   - Define an event handler for each type of event expected.
 *   - Initialize the scheduler by calling the APP_SCHED_INIT() macro before entering the
 *     application main loop.
 *   - Call app_sched_execute() from the main loop each time the application wakes up because of an
 *     event (typically when sd_app_evt_wait() returns).
 *
 * @subsection int_context_logic Logic in interrupt context:
 *
 *   - In the interrupt handler, call app_sched_event_put()
 *     with the appropriate data and event handler. This will insert an event into the
 *     scheduler's queue. The app_sched_execute() function will pull this event and call its
 *     handler in the main context.
 *
 * @if (PERIPHERAL)
 * For an example usage of the scheduler, see the implementations of
 * @ref ble_sdk_app_hids_mouse and @ref ble_sdk_app_hids_keyboard.
 * @endif
 *
 * @image html scheduler_working.jpg The high level design of the scheduler
 */

#ifndef APP_SCHEDULER_H__
#define APP_SCHEDULER_H__

#include <stdint.h>
#include "util.h"


#define APP_SCHED_EVENT_HEADER_SIZE 8       /**< Size of app_scheduler.event_header_t (only for use inside APP_SCHED_BUF_SIZE()). */

/**@brief Compute number of bytes required to hold the scheduler buffer.
 *
 * @param[in] EVENT_SIZE   Maximum size of events to be passed through the scheduler.
 * @param[in] QUEUE_SIZE   Number of entries in scheduler queue (i.e. the maximum number of events
 *                         that can be scheduled for execution).
 *
 * @return    Required scheduler buffer size (in bytes).
 */
#define APP_SCHED_BUF_SIZE(EVENT_SIZE, QUEUE_SIZE)                                                 \
            (((EVENT_SIZE) + APP_SCHED_EVENT_HEADER_SIZE) * ((QUEUE_SIZE) + 1))

/**@brief Scheduler event handler type. */
typedef void (*app_sched_event_handler_t)(void * p_event_data);

/**@brief Structure for holding a scheduled event header. */
typedef struct
{
    app_sched_event_handler_t handler;          /**< Pointer to event handler to receive the event. */
    uint16_t                  event_data_size;  /**< Size of event data. */
} event_header_t;

typedef struct
{
    event_header_t * m_queue_event_headers;  /**< Array for holding the queue event headers. */
    uint8_t        * m_queue_event_data;     /**< Array for holding the queue event data. */
    volatile uint8_t m_queue_start_index;    /**< Index of queue entry at the start of the queue. */
    volatile uint8_t m_queue_end_index;      /**< Index of queue entry at the end of the queue. */
    uint16_t         m_queue_event_size;     /**< Maximum event size in queue. */
    uint16_t         m_queue_size;           /**< Number of queue entries. */

    uint16_t m_max_queue_utilization;        /**< Maximum observed queue utilization. */
} app_scheduler_t;

/**@brief Macro for initializing the event scheduler.
 *
 * @details It will also handle dimensioning and allocation of the memory buffer required by the
 *          scheduler, making sure the buffer is correctly aligned.
 *
 * @param[in] EVENT_SIZE   Maximum size of events to be passed through the scheduler.
 * @param[in] QUEUE_SIZE   Number of entries in scheduler queue (i.e. the maximum number of events
 *                         that can be scheduled for execution).
 *
 * @note Since this macro allocates a buffer, it must only be called once (it is OK to call it
 *       several times as long as it is from the same location, e.g. to do a reinitialization).
 */
#define APP_SCHED_INIT(P_SCHED, EVENT_SIZE, QUEUE_SIZE)                                            \
    do                                                                                             \
    {                                                                                              \
        static uint32_t APP_SCHED_BUF[CEIL_DIV(APP_SCHED_BUF_SIZE((EVENT_SIZE), (QUEUE_SIZE)),     \
                                               sizeof(uint32_t))];                                 \
        uint32_t ERR_CODE = app_sched_init(P_SCHED, (EVENT_SIZE), (QUEUE_SIZE), APP_SCHED_BUF);    \
    } while (0)


int app_sched_init(app_scheduler_t * p_sched, uint16_t max_event_size, uint16_t queue_size, void * p_evt_buffer);


void app_sched_execute(app_scheduler_t * p_sched);


int app_sched_event_put(app_scheduler_t         * p_sched,
                             void *                    p_event_data,
                             uint16_t                  event_size,
                             app_sched_event_handler_t handler);









/**@brief Function for getting the maximum observed queue utilization.
 *
 * Function for tuning the module and determining QUEUE_SIZE value and thus module RAM usage.
 *
 * @return Maximum number of events in queue observed so far.
 */
uint16_t app_sched_queue_utilization_get(app_scheduler_t * p_sched);

/**@brief A function to pause the scheduler.
 *
 * @details When the scheduler is paused events are not pulled from the scheduler queue for
 *          processing. The function can be called multiple times. To unblock the scheduler the
 *          function @ref app_sched_resume has to be called the same number of times.
 */
void app_sched_pause(app_scheduler_t * p_sched);

/**@brief A function to resume a scheduler.
 *
 * @details To unblock the scheduler this function has to be called the same number of times as
 *          @ref app_sched_pause function.
 */
void app_sched_resume(app_scheduler_t * p_sched);




#endif // APP_SCHEDULER_H__
