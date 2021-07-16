//-------------------------------------------------------
// An object that abstracts the state of Grbl_Esp32
//-------------------------------------------------------

#include "gStatus.h"
#include <WiFi.h>
#include "Grbl_MinUI.h"  // for TEST_STANDALONE_UI

#if !TEST_STANDALONE_UI
	#include <Config.h>
	#include <Grbl.h>
	#include <SDCard.h>
	#include <Report.h>
	#include <Machine/MachineConfig.h>
#endif


#define DEBUG_WIFI  0


#define SD_CARD_CHECK_TIME  1000        // ms

gStatus g_status;


const char *uiMainModeName(ui_main_mode_t mode)
{
    switch (mode)
    {
        case MAIN_MODE_NONE   : return "NONE";
        case MAIN_MODE_IDLE   : return "IDLE";
        case MAIN_MODE_BUSY   : return "BUSY";
        case MAIN_MODE_ALARM  : return "ALARM";
    }
    return "UNKNOWN_MODE";
}


const char *uiJobStateName(ui_job_state_t state)
{
	switch (state)
	{
		case JOB_STATE_NONE		: return "NONE";
		case JOB_STATE_STARTED	: return "STARTED";
		case JOB_STATE_COMPLETE : return "COMPLETE";
		case JOB_STATE_ABORTED  : return "ABORTED";
		case JOB_STATE_ERROR	: return "ERROR";
	}
	return "UNKNOWN_JOB";
}


const char *uiSystemStateName(grbl_State_t state)
{
    switch (state)
    {
        case grbl_State_t::Idle       : return "Idle";
        case grbl_State_t::Alarm      : return "Alarm";
        case grbl_State_t::CheckMode  : return "CheckMode";
        case grbl_State_t::Homing     : return "Homing";
        case grbl_State_t::Cycle      : return "Cycle";
        case grbl_State_t::Hold       : return "Hold";
        case grbl_State_t::Jog        : return "Jog";
        case grbl_State_t::SafetyDoor : return "SafetyDoor";
        case grbl_State_t::Sleep      : return "Sleep";
    }
    return "UNKNOWN_STATE";
}


const char *uiSDCardStateName(grbl_SDState_t state)
	//  grbl BusyPrinting is same as Busy
{
    switch (state)
    {
        case grbl_SDState_t::Idle          : return "Idle";
        case grbl_SDState_t::NotPresent    : return "NotPresent";
        case grbl_SDState_t::Busy  		   : return "Busy";
        case grbl_SDState_t::BusyUploading : return "BusyUploading";
        case grbl_SDState_t::BusyParsing   : return "BusyParsing";
    }
    return "UNKNOWN_SD_STATE";
}


#if DEBUG_WIFI
    static const char *wifiEventName(WiFiEvent_t event)
    {
        switch (event)
        {
            case SYSTEM_EVENT_WIFI_READY	        : return "WIFI_READY";
            case SYSTEM_EVENT_SCAN_DONE	            : return "SCAN_DONE";
            case SYSTEM_EVENT_STA_START	            : return "STA_START";
            case SYSTEM_EVENT_STA_STOP	            : return "STA_STOP";
            case SYSTEM_EVENT_STA_CONNECTED	        : return "STA_CONNECTED";
            case SYSTEM_EVENT_STA_DISCONNECTED	    : return "STA_DISCONNECTED";
            case SYSTEM_EVENT_STA_AUTHMODE_CHANGE	: return "STA_AUTHMODE_CHANGE";
            case SYSTEM_EVENT_STA_GOT_IP	        : return "STA_GOT_IP";
            case SYSTEM_EVENT_STA_LOST_IP	        : return "STA_LOST_IP";
            case SYSTEM_EVENT_STA_WPS_ER_SUCCESS	: return "STA_WPS_ER_SUCCESS";
            case SYSTEM_EVENT_STA_WPS_ER_FAILED	    : return "STA_WPS_ER_FAILED";
            case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT	: return "STA_WPS_ER_TIMEOUT";
            case SYSTEM_EVENT_STA_WPS_ER_PIN	    : return "STA_WPS_ER_PIN";
            case SYSTEM_EVENT_AP_START	            : return "AP_START";
            case SYSTEM_EVENT_AP_STOP	            : return "AP_STOP";
            case SYSTEM_EVENT_AP_STACONNECTED	    : return "AP_STACONNECTED";
            case SYSTEM_EVENT_AP_STADISCONNECTED	: return "AP_STADISCONNECTED";
            case SYSTEM_EVENT_AP_STAIPASSIGNED	    : return "AP_STAIPASSIGNED";
            case SYSTEM_EVENT_AP_PROBEREQRECVED	    : return "AP_PROBEREQRECVED";
            case SYSTEM_EVENT_GOT_IP6	            : return "GOT_IP6";
            case SYSTEM_EVENT_ETH_START	            : return "ETH_START";
            case SYSTEM_EVENT_ETH_STOP	            : return "ETH_STOP";
            case SYSTEM_EVENT_ETH_CONNECTED         : return "ETH_CONNECTED	ESP32";
            case SYSTEM_EVENT_ETH_DISCONNECTED	    : return "ETH_DISCONNECTED";
            case SYSTEM_EVENT_ETH_GOT_IP	        : return "ETH_GOT_IP";
        }
        return "UNKNOWN_WIFI_EVENT";
    }
#endif



void gStatus::gWifiEvent(uint16_t event)
{
    switch (static_cast<WiFiEvent_t>(event))
    {
        case SYSTEM_EVENT_AP_START                 :    // ESP32 soft-AP start
        case SYSTEM_EVENT_STA_DISCONNECTED         :    // ESP32 station disconnected from AP
        case SYSTEM_EVENT_AP_STADISCONNECTED       :    // a station disconnected from ESP32 soft-AP
        case SYSTEM_EVENT_WIFI_READY               :    // ESP32 WiFi ready
            m_wifi_state = IND_STATE_ENABLED;
            break;

        // case SYSTEM_EVENT_SCAN_DONE                :    // ESP32 finish scanning AP
        case SYSTEM_EVENT_STA_START                :    // ESP32 station start
            m_wifi_state = IND_STATE_ACTIVE;
            break;

        case SYSTEM_EVENT_AP_STACONNECTED          :    // a station connected to ESP32 soft-AP
        case SYSTEM_EVENT_STA_GOT_IP               :    // ESP32 station got IP from connected AP
        case SYSTEM_EVENT_STA_CONNECTED            :    // ESP32 station connected to AP
        case SYSTEM_EVENT_STA_WPS_ER_SUCCESS       :    // ESP32 station wps succeeds in enrollee mode
            m_wifi_state = IND_STATE_READY;
            break;

        case SYSTEM_EVENT_AP_STOP                  :    // ESP32 soft-AP stop
        case SYSTEM_EVENT_STA_STOP                 :    // ESP32 station stop
        case SYSTEM_EVENT_STA_LOST_IP              :    // ESP32 station lost IP and the IP is reset to 0
        case SYSTEM_EVENT_STA_WPS_ER_FAILED        :    // ESP32 station wps fails in enrollee mode
        case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT       :    // ESP32 station wps timeout in enrollee mode
        case SYSTEM_EVENT_STA_WPS_ER_PIN           :    // ESP32 station wps pin code in enrollee mode
            m_wifi_state = IND_STATE_ERROR;
            break;


        // case SYSTEM_EVENT_STA_AUTHMODE_CHANGE      :    // the auth mode of AP connected by ESP32 station changed
        // case SYSTEM_EVENT_AP_STAIPASSIGNED         :    // ESP32 soft-AP assign an IP to a connected station
        // case SYSTEM_EVENT_AP_PROBEREQRECVED        :    // Receive probe request packet in soft-AP interface
        // case SYSTEM_EVENT_GOT_IP6                  :    // ESP32 station or ap or ethernet interface v6IP addr is preferred
        // case SYSTEM_EVENT_ETH_START                :    // ESP32 ethernet start
        // case SYSTEM_EVENT_ETH_STOP                 :    // ESP32 ethernet stop
        // case SYSTEM_EVENT_ETH_CONNECTED            :    // ESP32 ethernet phy link up
        // case SYSTEM_EVENT_ETH_DISCONNECTED         :    // ESP32 ethernet phy link down
        // case SYSTEM_EVENT_ETH_GOT_IP               :    // ESP32 ethernet got IP from connected AP
        // case SYSTEM_EVENT_MAX
    }
}


static void onWiFiEvent(WiFiEvent_t event)
    // We need to put activity indicators in Grbl_Esp32 for IO to the Wifi AP/Station
    // and (for them) perhaps add telnet indicator as well.  Fairly low priority
    // for me at this point, though of interest.
{
    #if DEBUG_WIFI
        debug_serial("onWifiEvent(%d) %s",event,wifiEventName(event));
    #endif
    g_status.gWifiEvent(event);
}


void gStatus::initWifiEventHandler()
	// register the wifiEvent handler
{
	WiFi.onEvent(onWiFiEvent);
}



void gStatus::updateStatus()
{
	#if TEST_STANDALONE_UI
		return;
	#else

		// SYSTEM STATE
		// wait until a known state before polling Grbl_Esp32

		m_sys_state = static_cast<grbl_State_t>(sys.state);
		if (!m_started && m_sys_state != grbl_State_t::Sleep)
		{
			m_started = true;
			delay(100);
				// so my serial doesnt come out in the middle of
				// a line from Grbl_Esp32 ... lines should be atomic

			debug_serial("gStatus started ..");
		}
		if (!m_started)
			return;

		// SDCARD STATE - once per second, if idle or not_present,
		// call get_state(true) to detect removal/insertion

		uint32_t now = millis();
		SDCard *sdCard = config->_sdCard;
		if (sdCard)
		{
			SDCard::State sd_state = sdCard->get_state(false);
				// from Grbl_Esp32/SDCard.h

			m_sd_card_timer = now;
			if (sd_state == SDCard::State::NotPresent ||
				sd_state == SDCard::State::Idle)
			{
				if (now > m_sd_card_timer + SD_CARD_CHECK_TIME)
				{
					m_sd_card_timer = now;

					#if 0
						if (sd_state == SDCard::State::NotPresent ||
							sd_state == SDCard::State::Idle)
							sd_state = sdCard->get_state(true);
					#endif

					#if 0
						SD.end();
						SD.begin(GPIO_NUM_21);   // V_SDCARD_CS);
						uint8_t cardType = SD.cardType();
						if (cardType == CARD_NONE)
						{
							sd_state == sdCard->set_state(SDCard::State::NotPresent);
						}
						else
						{
							sd_state == sdCard->set_state(SDCard::State::Idle);
						}
					#endif
				}
			}
			m_sdcard_state = static_cast<grbl_SDState_t>(sd_state);
			static grbl_SDState_t m_last_sdcard_state = grbl_SDState_t::NotPresent;
			if (m_last_sdcard_state != m_sdcard_state)
			{
				debug_serial("SD Changed from %s to %s",
					uiSDCardStateName(m_last_sdcard_state),
					uiSDCardStateName(m_sdcard_state));
					m_last_sdcard_state = m_sdcard_state;
			}

		}

		// APPLICATION MODE

		if (m_sys_state == grbl_State_t::Alarm)
			m_main_mode = MAIN_MODE_ALARM;
		else if (m_sdcard_state == grbl_SDState_t::Busy)
			m_main_mode = MAIN_MODE_ALARM;
		else
			m_main_mode = MAIN_MODE_IDLE;



		// POSITIONS

		float *pos = system_get_mpos();
		for (int i=0; i<UI_NUM_AXES; i++)
		{
			m_sys_pos[i] = sys_position[i];
				// from Grbl_Esp32/System.h
			m_machine_pos[i] = pos[i];
				// from Grbl_Esp32/System.h
			m_work_pos[i] = pos[i];
				// converted below
		}
		mpos_to_wpos(m_work_pos);
			// DEPENDS on Yaml number of axes agreeing with our constant!!!!
			// or BAD THINGS will happen


		// JOB STATE

		if (m_main_mode == MAIN_MODE_BUSY &&
			m_job_state == JOB_STATE_NONE)
		{
			m_job_state = JOB_STATE_STARTED;
			debug_serial("JOB_STATE_STARTED");
			m_active_filename = sdCard->filename();
		}
		if (m_sys_state == grbl_State_t::Hold)
			m_job_state = JOB_STATE_PAUSED;

		// hmmm ...

		switch (gc_state.modal.program_flow)
		{
			case ProgramFlow::Running:
				m_job_state = JOB_STATE_STARTED;
				m_file_pct = sdCard->report_perc_complete();
				break;
			case ProgramFlow::Paused:
				m_job_state = JOB_STATE_PAUSED;
				m_file_pct = sdCard->report_perc_complete();
				break;
			case ProgramFlow::OptionalStop:
				m_job_state = JOB_STATE_ABORTED;
				m_file_pct = sdCard->report_perc_complete();
				break;
			case ProgramFlow::CompletedM2:
			case ProgramFlow::CompletedM30:
				m_job_state = JOB_STATE_COMPLETE;
				m_file_pct = 100.00;
				break;
		}

		if (m_sdcard_state == grbl_SDState_t::Busy)
		{
			m_file_pct = sdCard->report_perc_complete();
		}
	#endif

}   // gStatus::updateStatus()
